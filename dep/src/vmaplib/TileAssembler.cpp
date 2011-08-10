/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "WorldModel.h"
#include "TileAssembler.h"
#include "MapTree.h"
#include "BIH.h"
#include "VMapDefinitions.h"

#include <set>
#include <iomanip>
#include <sstream>
#include <iomanip>

template<> struct BoundsTrait<VMAP::ModelSpawn*>
{
	static void getBounds(const VMAP::ModelSpawn* const & obj, G3D::AABox & out) { out = obj->getBounds(); }
};

namespace VMAP
{
	bool readChunk(FILE* rf, char* dest, const char* compare, G3D::uint32 len)
	{
		if(fread(dest, sizeof(char), len, rf) != len) return false;
		return memcmp(dest, compare, len) == 0;
	}

	G3D::Vector3 ModelPosition::transform(const G3D::Vector3 & pIn) const
	{
		G3D::Vector3 out = pIn * iScale;
		out = iRotation * out;
		return(out);
	}

	//=================================================================

	TileAssembler::TileAssembler(const std::string & pSrcDirName, const std::string & pDestDirName)
	{
		iCurrentUniqueNameId = 0;
		iFilterMethod = NULL;
		iSrcDir = pSrcDirName;
		iDestDir = pDestDirName;
		//mkdir(iDestDir);
		//init();
	}

	TileAssembler::~TileAssembler()
	{
		//delete iCoordModelMapping;
	}

	bool TileAssembler::convertWorld2()
	{
		std::set<std::string> spawnedModelFiles;
		bool success = readMapSpawns();
		if(!success)
			return false;

		// export Map data
		for(MapData::iterator map_iter = mapData.begin(); map_iter != mapData.end() && success; ++map_iter)
		{
			// build global map tree
			std::vector<ModelSpawn*> mapSpawns;
			UniqueEntryMap::iterator entry;
			printf("Calculating model bounds for map %u...\n", map_iter->first);
			for(entry = map_iter->second->UniqueEntries.begin(); entry != map_iter->second->UniqueEntries.end(); ++entry)
			{
				// M2 models don't have a bound set in WDT/ADT placement data, i still think they're not used for LoS at all on retail
				if(entry->second.flags & MOD_M2)
				{
					if(!calculateTransformedBound(entry->second))
						break;
				}
				else if(entry->second.flags & MOD_WORLDSPAWN)  // WMO maps and terrain maps use different origin, so we need to adapt :/
				{
					// TODO: remove extractor hack and uncomment below line:
					//entry->second.iPos += G3D::Vector3(533.33333f*32, 533.33333f*32, 0.f);
					entry->second.iBound = entry->second.iBound + G3D::Vector3(533.33333f * 32, 533.33333f * 32, 0.f);
				}
				mapSpawns.push_back(&(entry->second));
				spawnedModelFiles.insert(entry->second.name);
			}

			printf("Creating map tree...\n");
			BIH pTree;
			pTree.build(mapSpawns, BoundsTrait<ModelSpawn*>::getBounds);

			// ===> possibly move this code to StaticMapTree class
			std::map<G3D::uint32, G3D::uint32> modelNodeIdx;
			for(G3D::uint32 i = 0; i < mapSpawns.size(); ++i)
				modelNodeIdx.insert( std::pair<G3D::uint32, G3D::uint32>(mapSpawns[i]->ID, i));

			// write map tree file
			std::stringstream mapfilename;
			mapfilename << iDestDir << "/" << std::setfill('0') << std::setw(3) << map_iter->first << ".vmtree";
			FILE* mapfile = fopen(mapfilename.str().c_str(), "wb");
			if(!mapfile)
			{
				success = false;
				printf("Cannot open %s\n", mapfilename.str().c_str());
				break;
			}

			//general info
			if(success && fwrite(VMAP_MAGIC, 1, 8, mapfile) != 8) success = false;
			G3D::uint32 globalTileID = StaticMapTree::packTileID(65, 65);
			std::pair<TileMap::iterator, TileMap::iterator> globalRange = map_iter->second->TileEntries.equal_range(globalTileID);
			char isTiled = globalRange.first == globalRange.second; // only maps without terrain (tiles) have global WMO
			if(success && fwrite(&isTiled, sizeof(char), 1, mapfile) != 1) success = false;
			// Nodes
			if(success && fwrite("NODE", 4, 1, mapfile) != 1) success = false;
			if(success) success = pTree.writeToFile(mapfile);
			// global map spawns (WDT), if any (most instances)
			if(success && fwrite("GOBJ", 4, 1, mapfile) != 1) success = false;

			for(TileMap::iterator glob = globalRange.first; glob != globalRange.second && success; ++glob)
			{
				success = ModelSpawn::writeToFile(mapfile, map_iter->second->UniqueEntries[glob->second]);
			}

			fclose(mapfile);

			// <====

			// write map tile files, similar to ADT files, only with extra BSP tree node info
			TileMap & tileEntries = map_iter->second->TileEntries;
			TileMap::iterator tile;
			for(tile = tileEntries.begin(); tile != tileEntries.end(); ++tile)
			{
				const ModelSpawn & spawn = map_iter->second->UniqueEntries[tile->second];
				if(spawn.flags & MOD_WORLDSPAWN)  // WDT spawn, saved as tile 65/65 currently...
					continue;
				G3D::uint32 nSpawns = tileEntries.count(tile->first);
				std::stringstream tilefilename;
				tilefilename.fill('0');
				tilefilename << iDestDir << "/" << std::setw(3) << map_iter->first << "_";
				G3D::uint32 x, y;
				StaticMapTree::unpackTileID(tile->first, x, y);
				tilefilename << std::setw(2) << x << "_" << std::setw(2) << y << ".vmtile";
				FILE* tilefile = fopen(tilefilename.str().c_str(), "wb");
				// file header
				if(success && fwrite(VMAP_MAGIC, 1, 8, tilefile) != 8) success = false;
				// write number of tile spawns
				if(success && fwrite(&nSpawns, sizeof(G3D::uint32), 1, tilefile) != 1) success = false;
				// write tile spawns
				for(G3D::uint32 s = 0; s < nSpawns; ++s)
				{
					if(s)
						++tile;
					const ModelSpawn & spawn2 = map_iter->second->UniqueEntries[tile->second];
					success = success && ModelSpawn::writeToFile(tilefile, spawn2);
					// MapTree nodes to update when loading tile:
					std::map<G3D::uint32, G3D::uint32>::iterator nIdx = modelNodeIdx.find(spawn2.ID);
					if(success && fwrite(&nIdx->second, sizeof(G3D::uint32), 1, tilefile) != 1) success = false;
				}
				fclose(tilefile);
			}
			// break; //test, extract only first map; TODO: remvoe this line
		}

		// export objects
		std::cout << "\nConverting Model Files" << std::endl;
		for(std::set<std::string>::iterator mfile = spawnedModelFiles.begin(); mfile != spawnedModelFiles.end(); ++mfile)
		{
			std::cout << "Converting " << *mfile << std::endl;
			if(!convertRawFile(*mfile))
			{
				std::cout << "error converting " << *mfile << std::endl;
				success = false;
				break;
			}
		}

		//cleanup:
		for(MapData::iterator map_iter = mapData.begin(); map_iter != mapData.end(); ++map_iter)
		{
			delete map_iter->second;
		}
		return success;
	}

	bool TileAssembler::readMapSpawns()
	{
		std::string fname = iSrcDir + "/dir_bin";
		FILE* dirf = fopen(fname.c_str(), "rb");
		if(!dirf)
		{
			printf("Could not read dir_bin file!\n");
			return false;
		}
		printf("Read coordinate mapping...\n");
		G3D::uint32 mapID, tileX, tileY, check = 0;
		G3D::Vector3 v1, v2;
		ModelSpawn spawn;
		while(!feof(dirf))
		{
			check = 0;
			// read mapID, tileX, tileY, Flags, adtID, ID, Pos, Rot, Scale, Bound_lo, Bound_hi, name
			check += fread(&mapID, sizeof(G3D::uint32), 1, dirf);
			if(check == 0)  // EoF...
				break;
			check += fread(&tileX, sizeof(G3D::uint32), 1, dirf);
			check += fread(&tileY, sizeof(G3D::uint32), 1, dirf);
			if(!ModelSpawn::readFromFile(dirf, spawn))
				break;

			MapSpawns* current;
			MapData::iterator map_iter = mapData.find(mapID);
			if(map_iter == mapData.end())
			{
				printf("spawning Map %d\n", mapID);
				mapData[mapID] = current = new MapSpawns();
			}
			else current = (*map_iter).second;
			current->UniqueEntries.insert(std::pair<G3D::uint32, ModelSpawn>(spawn.ID, spawn));
			current->TileEntries.insert(std::pair<G3D::uint32, G3D::uint32>(StaticMapTree::packTileID(tileX, tileY), spawn.ID));
		}
		bool success = (ferror(dirf) == 0);
		fclose(dirf);
		return success;
	}

	bool TileAssembler::calculateTransformedBound(ModelSpawn & spawn)
	{
		std::string modelFilename = iSrcDir + "/" + spawn.name;
		ModelPosition modelPosition;
		modelPosition.iDir = spawn.iRot;
		modelPosition.iScale = spawn.iScale;
		modelPosition.init();

		FILE* rf = fopen(modelFilename.c_str(), "rb");
		if(!rf)
		{
			printf("ERROR: Can't open model file: %s\n", modelFilename.c_str());
			return false;
		}

		G3D::AABox modelBound;
		bool boundEmpty = true;
		char ident[8];

		int readOperation = 1;

		// temporary use defines to simplify read/check code (close file and return at fail)
#define READ_OR_RETURN(V,S) if(fread((V), (S), 1, rf) != 1) { \
                                        fclose(rf); printf("readfail, op = %i\n", readOperation); return(false); }readOperation++;
#define CMP_OR_RETURN(V,S)  if(strcmp((V),(S)) != 0)        { \
                                        fclose(rf); printf("cmpfail, %s!=%s\n", V, S);return(false); }

		READ_OR_RETURN(&ident, 8);
		CMP_OR_RETURN(ident, RAW_VMAP_MAGIC);

		// we have to read one int. This is needed during the export and we have to skip it here
		G3D::uint32 tempNVectors;
		READ_OR_RETURN(&tempNVectors, sizeof(tempNVectors));

		G3D::uint32 groups, wmoRootId;
		char blockId[5];
		blockId[4] = 0;
		int blocksize;
		float* vectorarray = 0;

		READ_OR_RETURN(&groups, sizeof(G3D::uint32));
		READ_OR_RETURN(&wmoRootId, sizeof(G3D::uint32));
		if(groups != 1) printf("Warning: '%s' does not seem to be a M2 model!\n", modelFilename.c_str());

		for(G3D::uint32 g = 0; g < groups; ++g) // should be only one for M2 files...
		{
			fseek(rf, 3 * sizeof(G3D::uint32) + 6 * sizeof(float), SEEK_CUR);

			READ_OR_RETURN(&blockId, 4);
			CMP_OR_RETURN(blockId, "GRP ");
			READ_OR_RETURN(&blocksize, sizeof(int));
			fseek(rf, blocksize, SEEK_CUR);

			// ---- indexes
			READ_OR_RETURN(&blockId, 4);
			CMP_OR_RETURN(blockId, "INDX");
			READ_OR_RETURN(&blocksize, sizeof(int));
			fseek(rf, blocksize, SEEK_CUR);

			// ---- vectors
			READ_OR_RETURN(&blockId, 4);
			CMP_OR_RETURN(blockId, "VERT");
			READ_OR_RETURN(&blocksize, sizeof(int));
			G3D::uint32 nvectors;
			READ_OR_RETURN(&nvectors, sizeof(G3D::uint32));

			if(nvectors > 0)
			{
				vectorarray = new float[nvectors * 3];
				READ_OR_RETURN(vectorarray, nvectors * sizeof(float) * 3);
			}
			else
			{
				std::cout << "error: model '" << spawn.name << "' has no geometry!" << std::endl;
				return false;
			}

			for(G3D::uint32 i = 0, indexNo = 0; indexNo < nvectors; indexNo++, i += 3)
			{
				G3D::Vector3 v = G3D::Vector3(vectorarray[i + 0], vectorarray[i + 1], vectorarray[i + 2]);
				v = modelPosition.transform(v);

				if(boundEmpty)
					modelBound = G3D::AABox(v, v), boundEmpty = false;
				else
					modelBound.merge(v);
			}
			delete[] vectorarray;
			// drop of temporary use defines
#undef READ_OR_RETURN
#undef CMP_OR_RETURN
		}
		spawn.iBound = modelBound + spawn.iPos;
		spawn.flags |= MOD_HAS_BOUND;
		fclose(rf);
		return true;
	}

	struct WMOLiquidHeader
	{
		int xverts, yverts, xtiles, ytiles;
		float pos_x;
		float pos_y;
		float pos_z;
		short type;
	};
	//=================================================================
	bool TileAssembler::convertRawFile(const std::string & pModelFilename)
	{
		bool success = true;
		std::string filename = iSrcDir;
		if(filename.length() > 0)
			filename.append("/");
		filename.append(pModelFilename);
		FILE* rf = fopen(filename.c_str(), "rb");

		if(!rf)
		{
			printf("ERROR: Can't open model file in form: %s", pModelFilename.c_str());
			printf("...                          or form: %s", filename.c_str());
			return false;
		}

		char ident[8];

		int readOperation = 1;

		// temporary use defines to simplify read/check code (close file and return at fail)
#define READ_OR_RETURN(V,S) if(fread((V), (S), 1, rf) != 1) { \
                                        fclose(rf); printf("readfail, op = %i\n", readOperation); return(false); }readOperation++;
#define CMP_OR_RETURN(V,S)  if(strcmp((V),(S)) != 0)        { \
                                        fclose(rf); printf("cmpfail, %s!=%s\n", V, S);return(false); }

		READ_OR_RETURN(&ident, 8);
		CMP_OR_RETURN(ident, RAW_VMAP_MAGIC);

		// we have to read one int. This is needed during the export and we have to skip it here
		G3D::uint32 tempNVectors;
		READ_OR_RETURN(&tempNVectors, sizeof(tempNVectors));

		G3D::uint32 groups;
		G3D::uint32 RootWMOID;
		char blockId[5];
		blockId[4] = 0;
		int blocksize;

		READ_OR_RETURN(&groups, sizeof(G3D::uint32));
		READ_OR_RETURN(&RootWMOID, sizeof(G3D::uint32));

		std::vector<GroupModel> groupsArray;

		for(G3D::uint32 g = 0; g < groups; ++g)
		{
			std::vector<MeshTriangle> triangles;
			std::vector<G3D::Vector3> vertexArray;

			G3D::uint32 mogpflags, GroupWMOID;
			READ_OR_RETURN(&mogpflags, sizeof(G3D::uint32));
			READ_OR_RETURN(&GroupWMOID, sizeof(G3D::uint32));

			float bbox1[3], bbox2[3];
			READ_OR_RETURN(bbox1, sizeof(float) * 3);
			READ_OR_RETURN(bbox2, sizeof(float) * 3);

			G3D::uint32 liquidflags;
			READ_OR_RETURN(&liquidflags, sizeof(G3D::uint32));

			// will this ever be used? what is it good for anyway??
			G3D::uint32 branches;
			READ_OR_RETURN(&blockId, 4);
			CMP_OR_RETURN(blockId, "GRP ");
			READ_OR_RETURN(&blocksize, sizeof(int));
			READ_OR_RETURN(&branches, sizeof(G3D::uint32));
			for(G3D::uint32 b = 0; b < branches; ++b)
			{
				G3D::uint32 indexes;
				// indexes for each branch (not used jet)
				READ_OR_RETURN(&indexes, sizeof(G3D::uint32));
			}

			// ---- indexes
			READ_OR_RETURN(&blockId, 4);
			CMP_OR_RETURN(blockId, "INDX");
			READ_OR_RETURN(&blocksize, sizeof(int));
			G3D::uint32 nindexes;
			READ_OR_RETURN(&nindexes, sizeof(G3D::uint32));
			if(nindexes > 0)
			{
				G3D::uint16* indexarray = new G3D::uint16[nindexes];
				READ_OR_RETURN(indexarray, nindexes * sizeof(G3D::uint16));
				for(G3D::uint32 i = 0; i < nindexes; i += 3)
				{
					triangles.push_back(MeshTriangle(indexarray[i], indexarray[i + 1], indexarray[i + 2]));
				}
				delete[] indexarray;
			}

			// ---- vectors
			READ_OR_RETURN(&blockId, 4);
			CMP_OR_RETURN(blockId, "VERT");
			READ_OR_RETURN(&blocksize, sizeof(int));
			G3D::uint32 nvectors;
			READ_OR_RETURN(&nvectors, sizeof(G3D::uint32));

			if(nvectors > 0)
			{
				float* vectorarray = new float[nvectors * 3];
				READ_OR_RETURN(vectorarray, nvectors * sizeof(float) * 3);
				for(G3D::uint32 i = 0; i < nvectors; ++i)
				{
					vertexArray.push_back(G3D::Vector3(vectorarray + 3 * i));
				}
				delete[] vectorarray;
			}
			// ----- liquid
			WmoLiquid* liquid = 0;
			if(liquidflags & 1)
			{
				WMOLiquidHeader hlq;
				READ_OR_RETURN(&blockId, 4);
				CMP_OR_RETURN(blockId, "LIQU");
				READ_OR_RETURN(&blocksize, sizeof(int));
				READ_OR_RETURN(&hlq, sizeof(WMOLiquidHeader));
				liquid = new WmoLiquid(hlq.xtiles, hlq.ytiles, G3D::Vector3(hlq.pos_x, hlq.pos_y, hlq.pos_z), hlq.type);
				G3D::uint32 size = hlq.xverts * hlq.yverts;
				READ_OR_RETURN(liquid->GetHeightStorage(), size * sizeof(float));
				size = hlq.xtiles * hlq.ytiles;
				READ_OR_RETURN(liquid->GetFlagsStorage(), size);
			}

			groupsArray.push_back(GroupModel(mogpflags, GroupWMOID, G3D::AABox(G3D::Vector3(bbox1), G3D::Vector3(bbox2))));
			groupsArray.back().setMeshData(vertexArray, triangles);
			groupsArray.back().setLiquidData(liquid);

			// drop of temporary use defines
#undef READ_OR_RETURN
#undef CMP_OR_RETURN

		}
		fclose(rf);

		// write WorldModel
		WorldModel model;
		model.setRootWmoID(RootWMOID);
		if(groupsArray.size())
		{
			model.setGroupModels(groupsArray);
			success = model.writeFile(iDestDir + "/" + pModelFilename + ".vmo");
		}

		//std::cout << "readRawFile2: '" << pModelFilename << "' tris: " << nElements << " nodes: " << nNodes << std::endl;
		return success;
	}
}
