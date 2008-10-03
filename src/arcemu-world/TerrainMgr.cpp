/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
#include <fcntl.h>
#include "TerrainMgr.h"

TerrainMgr::TerrainMgr(string MapPath, uint32 MapId, bool Instanced) : mapPath(MapPath), mapId(MapId), Instance(Instanced)
{
#ifndef USE_MEMORY_MAPPING_FOR_MAPS
	FileDescriptor = NULL;
	CellInformation = NULL;
#endif
}

TerrainMgr::~TerrainMgr()
{
#ifndef USE_MEMORY_MAPPING_FOR_MAPS
	if(FileDescriptor)
	{
		// Free up our file pointer.
		mutex.Acquire();
		fclose(FileDescriptor);
		mutex.Release();
		FileDescriptor = NULL;
	}

	// Big memory cleanup, whee.
	if(CellInformation)
	{
		for(uint32 x = 0; x < _sizeX; ++x)
		{
			for(uint32 y = 0; y < _sizeY; ++y)
			{
				if(CellInformation[x][y] != 0)
					delete CellInformation[x][y];
			}
			delete [] CellInformation[x];
		}
		delete [] CellInformation;
		CellInformation = NULL;
	}
#else

	mutex.Acquire();

	// Big memory cleanup, whee.
	for(uint32 x = 0; x < _sizeX; ++x)
	{
		delete [] CellInformation[x];
	}
	delete [] CellInformation;

#ifdef WIN32
	UnmapViewOfFile(m_Memory);
	CloseHandle(hMap);
	CloseHandle(hMappedFile);
#else
#error moo
#endif
	mutex.Release();
#endif
}

bool TerrainMgr::LoadTerrainHeader()
{
	// Create the path
	char File[200];

	snprintf(File, 200, "%s/Map_%u.bin", mapPath.c_str(), (unsigned int)mapId);

#ifndef USE_MEMORY_MAPPING_FOR_MAPS

	FileDescriptor = fopen(File, "rb");

	if(FileDescriptor == 0)
	{
		Log.Error("TerrainMgr", "Map load failed for %s. Missing file?", File);
		return false;
	}

	/* check file size */
	fseek(FileDescriptor, 0, SEEK_END);
	if(ftell(FileDescriptor) == 1048576)
	{
		/* file with no data */
		fclose(FileDescriptor);
		FileDescriptor=NULL;
		return false;
	}

	// Read in the header.
	fseek(FileDescriptor,0,SEEK_SET);
	size_t dread = fread(CellOffsets, 1, TERRAIN_HEADER_SIZE, FileDescriptor);
	if(dread != TERRAIN_HEADER_SIZE)
	{
		fclose(FileDescriptor);
		FileDescriptor=NULL;
		return false;
	}

	// Allocate both storage arrays.
	CellInformation = new CellTerrainInformation**[_sizeX];
	for(uint32 x = 0; x < _sizeX; ++x)
	{
		CellInformation[x] = new CellTerrainInformation*[_sizeY];
		for(uint32 y = 0; y < _sizeY; ++y)
		{
			// Clear the pointer.
			CellInformation[x][y] = 0;
		}
	}

#ifdef USING_BIG_ENDIAN
	uint32 x,y;
	for(x=0;x<512;++x) {
		for(y=0;y<512;++y) {
			CellOffsets[x][y] = swap32(CellOffsets[x][y]);
		}
	}
#endif

	return true;

#else

#ifdef WIN32
	
	DWORD sizehigh;

	hMappedFile = CreateFile(File, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	if(hMappedFile == INVALID_HANDLE_VALUE)
		return false;

	hMap = CreateFileMapping(hMappedFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(hMap==INVALID_HANDLE_VALUE)
	{
		CloseHandle(hMappedFile);
		return false;
	}

	mFileSize = GetFileSize(hMappedFile, &sizehigh);

	ASSERT(ReadFile(hMappedFile, CellOffsets, TERRAIN_HEADER_SIZE, &sizehigh, NULL));
	ASSERT(sizehigh==TERRAIN_HEADER_SIZE);

	SetFilePointer(hMappedFile, 0, NULL, FILE_BEGIN);
	m_Memory = (uint8*)MapViewOfFile(hMap, FILE_MAP_READ, 0, TERRAIN_HEADER_SIZE, mFileSize-TERRAIN_HEADER_SIZE);
	if(m_Memory==NULL)
	{
		CloseHandle(hMap);
		CloseHandle(hMappedFile);
	}

	// Allocate both storage arrays.
	CellInformation = new CellTerrainInformation**[_sizeX];
	for(uint32 x = 0; x < _sizeX; ++x)
	{
		CellInformation[x] = new CellTerrainInformation*[_sizeY];
		for(uint32 y = 0; y < _sizeY; ++y)
		{
			// Set pointer
			if(CellOffsets[x][y] != 0)
				CellInformation[x][y] = (CellTerrainInformation*)m_Memory+CellOffsets[x][y]-TERRAIN_HEADER_SIZE;
			else
				CellInformation[x][y] = 0;
		}
	}

#else
#error unimplemented in *nix
#endif
	return true;
#endif
}

bool TerrainMgr::LoadCellInformation(uint32 x, uint32 y)
{
#ifdef USE_MEMORY_MAPPING_FOR_MAPS
	if(CellOffsets[x][y]==0)
		return false;
	else
		return true;
#else
	if(!FileDescriptor)
		return false;

	// Make sure that we're not already loaded.
	assert(CellInformation[x][y] == 0);

	// Find our offset in our cached header.
	uint32 Offset = CellOffsets[x][y];

	// If our offset = 0, it means we don't have cell information for 
	// these coords.
	if(Offset == 0)
		return false;

	// Lock the mutex to prevent double reading.
	mutex.Acquire();

	// Check that we haven't been loaded by another thread.
	if(CellInformation[x][y] != 0)
	{
		mutex.Release();
		return true;
	}
	
	// Seek to our specified offset.
	if(fseek(FileDescriptor, Offset, SEEK_SET) == 0)
	{
		// Allocate the cell information.
		CellInformation[x][y] = new CellTerrainInformation;

		// Read from our file into this newly created struct.
		fread(CellInformation[x][y], sizeof(CellTerrainInformation), 1, FileDescriptor);

#ifdef USING_BIG_ENDIAN
		uint32 i,j;
		/* Swap all the data */

		for(i = 0; j < 2; ++j) {
			for(j = 0; j < 2; ++j) {
				CellInformation[x][y]->AreaID[i][j] = swap16(CellInformation[x][y]->AreaID[i][j]);
				CellInformation[x][y]->LiquidLevel[i][j] = swapfloat(CellInformation[x][y]->LiquidLevel[i][j]);
			}
		}

		for(i = 0; i < 32; ++j) {
			for(j = 0; j < 32; ++j) {
				CellInformation[x][y]->Z[i][j] = swapfloat(CellInformation[x][y]->Z[i][j]);
			}
		}
#endif
	}
	// Release the mutex.
	mutex.Release();

	// If we don't equal 0, it means the load was successful.
	if(CellInformation[x][y] != 0)
		return true;
	else
		return false;
#endif
}

bool TerrainMgr::UnloadCellInformation(uint32 x, uint32 y)
{
#ifdef USE_MEMORY_MAPPING_FOR_MAPS
	return true;
#else
	uint32 Start = getMSTime();

	assert(!Instance);
	// Find our information pointer.
	CellTerrainInformation * ptr = CellInformation[x][y];
	assert(ptr != 0);

	// Set the spot to unloaded (null).
	CellInformation[x][y] = 0;

	// Free the memory.
	delete ptr;

	sLog.outDetail("Unloaded cell information for cell [%u][%u] in %ums.", x, y, getMSTime() - Start);
	// Success
	return true;
#endif
}

uint8 TerrainMgr::GetWaterType(float x, float y)
{
	if(!AreCoordinatesValid(x, y))
		return 0;

	// Convert the co-ordinates to cells.
	uint32 CellX = ConvertGlobalXCoordinate(x);
	uint32 CellY = ConvertGlobalYCoordinate(y);

	if(!CellInformationLoaded(CellX, CellY))
		return 0;

	// Convert the co-ordinates to cell's internal
	// system.
	float IntX = ConvertInternalXCoordinate(x, CellX);
	float IntY = ConvertInternalYCoordinate(y, CellY);

	// Find the offset in the 2d array.
	return GetCellInformation(CellX, CellY)->LiquidType[ConvertTo2dArray(IntX)][ConvertTo2dArray(IntY)];
}

float TerrainMgr::GetWaterHeight(float x, float y)
{
	if(!AreCoordinatesValid(x, y))
		return 0.0f;

	// Convert the co-ordinates to cells.
	uint32 CellX = ConvertGlobalXCoordinate(x);
	uint32 CellY = ConvertGlobalYCoordinate(y);

	if(!CellInformationLoaded(CellX, CellY))
		return 0.0f;

	// Convert the co-ordinates to cell's internal
	// system.
	float IntX = ConvertInternalXCoordinate(x, CellX);
	float IntY = ConvertInternalYCoordinate(y, CellY);

	// Find the offset in the 2d array.
	return GetCellInformation(CellX, CellY)->LiquidLevel[ConvertTo2dArray(IntX)][ConvertTo2dArray(IntY)];
}

uint8 TerrainMgr::GetWalkableState(float x, float y)
{
	// This has to be implemented.
	return 1;
}

uint16 TerrainMgr::GetAreaID(float x, float y)
{
	if(!AreCoordinatesValid(x, y))
		return 0;

	// Convert the co-ordinates to cells.
	uint32 CellX = ConvertGlobalXCoordinate(x);
	uint32 CellY = ConvertGlobalYCoordinate(y);

	if(!CellInformationLoaded(CellX, CellY) && !LoadCellInformation(CellX, CellY))
		return 0;

	// Convert the co-ordinates to cell's internal
	// system.
	float IntX = ConvertInternalXCoordinate(x, CellX);
	float IntY = ConvertInternalYCoordinate(y, CellY);

	// Find the offset in the 2d array.
	return GetCellInformation(CellX, CellY)->AreaID[ConvertTo2dArray(IntX)][ConvertTo2dArray(IntY)];
}

float TerrainMgr::GetLandHeight(float x, float y)
{
	if(!AreCoordinatesValid(x, y))
		return 0.0f;

	// Convert the co-ordinates to cells.
	uint32 CellX = ConvertGlobalXCoordinate(x);
	uint32 CellY = ConvertGlobalYCoordinate(y);

	if(!CellInformationLoaded(CellX, CellY) && !LoadCellInformation(CellX, CellY))
		return 0.0f;

	// Convert the co-ordinates to cell's internal
	// system.
	float IntX = ConvertInternalXCoordinate(x, CellX);
	float IntY = ConvertInternalYCoordinate(y, CellY);

	// Calculate x index.
	float TempFloat = IntX * (MAP_RESOLUTION / CellsPerTile / _cellSize);
	uint32 XOffset = FL2UINT(TempFloat);
	if((TempFloat - (XOffset * _cellSize)) >= 0.5f)
		++XOffset;

	// Calculate y index.
	TempFloat = IntY * (MAP_RESOLUTION / CellsPerTile / _cellSize);
	uint32 YOffset = FL2UINT(TempFloat);
	if((TempFloat - (YOffset * _cellSize)) >= 0.5f)
		++YOffset;

	//ignore precision in case it would lead to bad values 
	if( XOffset > 31 )
		XOffset = 31;
	if( YOffset > 31 )
		YOffset = 31;

	// Return our cached information.
	return GetCellInformation(CellX, CellY)->Z[XOffset][YOffset];
}

void TerrainMgr::CellGoneActive(uint32 x, uint32 y)
{
	// Load cell information if it's not already loaded.
	if(!CellInformationLoaded(x, y))
		LoadCellInformation(x, y);
}

void TerrainMgr::CellGoneIdle(uint32 x, uint32 y)
{
	// If we're not an instance, unload our cell info.
	if(!Instance && CellInformationLoaded(x, y) && sWorld.UnloadMapFiles)
		UnloadCellInformation(x, y);
}
