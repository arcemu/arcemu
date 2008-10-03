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

// Class Map
// Holder for all instances of each mapmgr, handles transferring
// players between, and template holding.

#include "StdAfx.h"

Map::Map(uint32 mapid, MapInfo * inf)
{
	memset(spawns,0,sizeof(CellSpawns*) * _sizeX);

	_mapInfo = inf;
	_mapId = mapid;

	//new stuff Load Spawns
	LoadSpawns(false);

	// Setup terrain
	_terrain = new TerrainMgr(sWorld.MapPath, _mapId, !(inf->type==INSTANCE_NULL));
	if(!_terrain->LoadTerrainHeader())
	{
		delete _terrain;
		_terrain = NULL;
	}

	// get our name
	me = dbcMap.LookupEntry(_mapId);
	if(_mapInfo)
		name = _mapInfo->name;
	else
		name = "Unknown";
}

Map::~Map()
{
	Log.Notice("Map", "~Map %u", this->_mapId);
	delete _terrain;

	for(uint32 x=0;x<_sizeX;x++)
	{
		if(spawns[x])
		{
			for(uint32 y=0;y<_sizeY;y++)
			{
				if(spawns[x][y])
				{	
					CellSpawns * sp=spawns[x][y];
					for(CreatureSpawnList::iterator i = sp->CreatureSpawns.begin();i!=sp->CreatureSpawns.end();i++)
						delete (*i);
					for(GOSpawnList::iterator it = sp->GOSpawns.begin();it!=sp->GOSpawns.end();it++)
						delete (*it);

					delete sp;
					spawns[x][y]=NULL;
				}
			}
			delete [] spawns[x];
		}
	}

	for(CreatureSpawnList::iterator i = staticSpawns.CreatureSpawns.begin(); i != staticSpawns.CreatureSpawns.end(); ++i)
		delete *i;
	for(GOSpawnList::iterator i = staticSpawns.GOSpawns.begin(); i != staticSpawns.GOSpawns.end(); ++i)
		delete *i;
}

bool first_table_warning = true;
bool CheckResultLengthCreatures(QueryResult * res)
{
	if( res->GetFieldCount() != 20 )
	{
		if( first_table_warning )
		{
			first_table_warning = false;
			Log.LargeErrorMessage(LARGERRORMESSAGE_WARNING, "One of your creature_spawns table has the wrong column count.",
				"arcemu has skipped loading this table in order to avoid crashing.",
				"Please correct this, if you do not no spawns will show.", NULL);
		}

		return false;
	}
	else
		return true;
}

bool first_table_warningg = true;
bool CheckResultLengthGameObject(QueryResult * res)
{
	if( res->GetFieldCount() != 16 )
	{
		if( first_table_warningg )
		{
			first_table_warningg = false;
			Log.LargeErrorMessage(LARGERRORMESSAGE_WARNING, "One of your gameobject_spawns table has the wrong column count.",
				"arcemu has skipped loading this table in order to avoid crashing.",
				"Please correct this, if you do not no spawns will show.", NULL);
		}

		return false;
	}
	else
		return true;
}

void Map::LoadSpawns(bool reload)
{
	//uint32 st=getMSTime();
	CreatureSpawnCount = 0;
	if ( reload )//perform cleanup
		for ( uint32 x = 0; x < _sizeX; x++ )
			for ( uint32 y = 0; y < _sizeY; y++ )
				if ( spawns[x][y])
				{	
					CellSpawns * sp = spawns[x][y];
					for ( CreatureSpawnList::iterator i = sp->CreatureSpawns.begin(); i != sp->CreatureSpawns.end(); i++ )
						delete (*i);
					for ( GOSpawnList::iterator it = sp->GOSpawns.begin(); it != sp->GOSpawns.end(); it++ )
						delete (*it);

					delete sp;
					spawns[x][y] = NULL;
				}

	QueryResult * result;
	set<string>::iterator tableiterator;
	for(tableiterator=ExtraMapCreatureTables.begin(); tableiterator!=ExtraMapCreatureTables.end();++tableiterator)
	{
		result = WorldDatabase.Query("SELECT * FROM %s WHERE Map = %u",(*tableiterator).c_str(),this->_mapId);
		if(result)
		{
			if(CheckResultLengthCreatures( result) )
			{
				do{
					Field * fields = result->Fetch();
					CreatureSpawn * cspawn = new CreatureSpawn;
					cspawn->id = fields[0].GetUInt32();
					cspawn->form = FormationMgr::getSingleton().GetFormation(cspawn->id);
					cspawn->entry = fields[1].GetUInt32();
					cspawn->x = fields[3].GetFloat();
					cspawn->y = fields[4].GetFloat();
					cspawn->z = fields[5].GetFloat();
					cspawn->o = fields[6].GetFloat();
					/*uint32 cellx=float2int32(((_maxX-cspawn->x)/_cellSize));
					uint32 celly=float2int32(((_maxY-cspawn->y)/_cellSize));*/
					uint32 cellx=CellHandler<MapMgr>::GetPosX(cspawn->x);
					uint32 celly=CellHandler<MapMgr>::GetPosY(cspawn->y);
					if(spawns[cellx]==NULL)
					{
						spawns[cellx]=new CellSpawns*[_sizeY];
						memset(spawns[cellx],0,sizeof(CellSpawns*)*_sizeY);
					}

					if(!spawns[cellx][celly])
						spawns[cellx][celly]=new CellSpawns;
					cspawn->movetype = fields[7].GetUInt8();
					cspawn->displayid = fields[8].GetUInt32();
					cspawn->factionid = fields[9].GetUInt32();
					cspawn->flags = fields[10].GetUInt32();
					cspawn->bytes0 = fields[11].GetUInt32();
					cspawn->bytes1 = fields[12].GetUInt32();
					cspawn->bytes2 = fields[13].GetUInt32();
					cspawn->emote_state = fields[14].GetUInt32();
					//cspawn->respawnNpcLink = fields[15].GetUInt32();
					cspawn->channel_spell = fields[16].GetUInt16();
					cspawn->channel_target_go = fields[17].GetUInt32();
					cspawn->channel_target_creature = fields[18].GetUInt32();
					cspawn->stand_state = fields[19].GetUInt16();
					spawns[cellx][celly]->CreatureSpawns.push_back(cspawn);
					++CreatureSpawnCount;
				}while(result->NextRow());
			}

			delete result;
		}
	}

	result = WorldDatabase.Query("SELECT * FROM creature_staticspawns WHERE Map = %u",this->_mapId);
	if(result)
	{
		if( CheckResultLengthCreatures(result) )
		{
			do{
				Field * fields = result->Fetch();
				CreatureSpawn * cspawn = new CreatureSpawn;
				cspawn->id = fields[0].GetUInt32();
				cspawn->form = FormationMgr::getSingleton().GetFormation(cspawn->id);
				cspawn->entry = fields[1].GetUInt32();
				cspawn->x = fields[3].GetFloat();
				cspawn->y = fields[4].GetFloat();
				cspawn->z = fields[5].GetFloat();
				cspawn->o = fields[6].GetFloat();
				cspawn->movetype = fields[7].GetUInt8();
				cspawn->displayid = fields[8].GetUInt32();
				cspawn->factionid = fields[9].GetUInt32();
				cspawn->flags = fields[10].GetUInt32();
				cspawn->bytes0 = fields[11].GetUInt32();
				cspawn->bytes1 = fields[12].GetUInt32();
				cspawn->bytes2 = fields[13].GetUInt32();
				cspawn->emote_state = fields[14].GetUInt32();
				//cspawn->respawnNpcLink = fields[15].GetUInt32();
				cspawn->channel_spell=0;
				cspawn->channel_target_creature=0;
				cspawn->channel_target_go=0;
				cspawn->stand_state = fields[19].GetUInt16();
				staticSpawns.CreatureSpawns.push_back(cspawn);
				++CreatureSpawnCount;
			}while(result->NextRow());
		}

		delete result;
	}

	GameObjectSpawnCount = 0;
	result = WorldDatabase.Query("SELECT * FROM gameobject_staticspawns WHERE Map = %u",this->_mapId);
	if(result)
	{
		if( CheckResultLengthGameObject(result) )
		{
			do{
				Field * fields = result->Fetch();
				GOSpawn * gspawn = new GOSpawn;
				gspawn->entry = fields[1].GetUInt32();
				gspawn->id = fields[0].GetUInt32();
				gspawn->x=fields[3].GetFloat();
				gspawn->y=fields[4].GetFloat();
				gspawn->z=fields[5].GetFloat();
				gspawn->facing=fields[6].GetFloat();
				gspawn->o =fields[7].GetFloat();
				gspawn->o1=fields[8].GetFloat();
				gspawn->o2=fields[9].GetFloat();
				gspawn->o3=fields[10].GetFloat();
				gspawn->state=fields[11].GetUInt32();
				gspawn->flags=fields[12].GetUInt32();
				gspawn->faction=fields[13].GetUInt32();
				gspawn->scale = fields[14].GetFloat();
				//gspawn->stateNpcLink = fields[15].GetUInt32();
				staticSpawns.GOSpawns.push_back(gspawn);
				++GameObjectSpawnCount;
			}while(result->NextRow());
		}

		delete result;
	}

	for(tableiterator=ExtraMapGameObjectTables.begin(); tableiterator!=ExtraMapGameObjectTables.end();++tableiterator)
	{
		result = WorldDatabase.Query("SELECT * FROM %s WHERE map = %u",(*tableiterator).c_str(),this->_mapId);
		if(result)
		{
			if( CheckResultLengthGameObject(result) )
				{
				do{
					Field * fields = result->Fetch();
					GOSpawn * gspawn = new GOSpawn;
					gspawn->entry = fields[1].GetUInt32();
					gspawn->id = fields[0].GetUInt32();
					gspawn->x=fields[3].GetFloat();
					gspawn->y=fields[4].GetFloat();
					gspawn->z=fields[5].GetFloat();
					gspawn->facing=fields[6].GetFloat();
					gspawn->o =fields[7].GetFloat();
					gspawn->o1=fields[8].GetFloat();
					gspawn->o2=fields[9].GetFloat();
					gspawn->o3=fields[10].GetFloat();
					gspawn->state=fields[11].GetUInt32();
					gspawn->flags=fields[12].GetUInt32();
					gspawn->faction=fields[13].GetUInt32();
					gspawn->scale = fields[14].GetFloat();
					//gspawn->stateNpcLink = fields[15].GetUInt32();

					//uint32 cellx=float2int32(((_maxX-gspawn->x)/_cellSize));
					//uint32 celly=float2int32(((_maxY-gspawn->y)/_cellSize));
					uint32 cellx=CellHandler<MapMgr>::GetPosX(gspawn->x);
					uint32 celly=CellHandler<MapMgr>::GetPosY(gspawn->y);
					if(spawns[cellx]==NULL)
					{
						spawns[cellx]=new CellSpawns*[_sizeY];
						memset(spawns[cellx],0,sizeof(CellSpawns*)*_sizeY);
					}

					if(!spawns[cellx][celly])
						spawns[cellx][celly]=new CellSpawns;

					spawns[cellx][celly]->GOSpawns.push_back(gspawn);
					++GameObjectSpawnCount;
				}while(result->NextRow());
			}

			delete result;
		}
	}

	Log.Notice("Map", "%u creatures / %u gameobjects on map %u cached.", CreatureSpawnCount, GameObjectSpawnCount, _mapId);
}
