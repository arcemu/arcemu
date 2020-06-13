/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2011 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *	This script will add support for an SQL table called gameobject_teleports.
 *	This table can be used to teleport players when they use a game object such
 *  as a door or portal. Any object used in this table should exist in the
 *	gameobject_names table, and be of type 10. Custom portal can use the generic
 *	display id of 6831. Portals also have the Sound2 field set to '1'.
 *
 */

#include "Setup.h"

struct GameobjectTeleport
{
	uint32 mapid;
	float x, y, z, o;
	uint32 req_level;
};
std::map<uint32, GameobjectTeleport*> m_teleStorage;

class CustomTeleport: public GameObjectAIScript // Custom Portals
{
	public:
		CustomTeleport(GameObject* goinstance) : GameObjectAIScript(goinstance)
		{
		}
		~CustomTeleport()
		{
		}

		void OnActivate(Player* pPlayer)
		{
			float x, y, z, orientation;
			uint32 mapid;

			std::map<uint32, GameobjectTeleport*>::iterator itr = m_teleStorage.find(this->_gameobject->GetEntry());
			if(itr != m_teleStorage.end())
			{
				GameobjectTeleport* gt = itr->second;
				uint32 required_level = gt->req_level;

				if((required_level == 0) || (required_level <= pPlayer->getLevel()))
				{
					mapid = gt->mapid;
					x = gt->x;
					y = gt->y;
					z = gt->z;
					orientation = gt->o;

					pPlayer->SafeTeleport(mapid, 0, x, y, z, orientation);
				}
				else if(required_level != 0)
				{
					pPlayer->BroadcastMessage("You must be at least level %ld to use this portal", required_level);
				}
			}
		}
		static GameObjectAIScript* Create(GameObject* GO) { return new CustomTeleport(GO); }
};


//Pay Attention: This feature is disable by default.
void InitializeGameObjectTeleportTable(ScriptMgr* mgr)
{
	QueryResult* result = NULL;
	result = WorldDatabase.Query("SELECT * FROM gameobject_teleports");
	if(result != NULL)
	{
		// Check if the SQL table is setup correctly
		if(result->GetFieldCount() < 7)
		{
			sLog.outError("Error: Custom portals disabled, invalid 'gameobject_teleports' table.");
			delete result;
			return;
		}
		do
		{
			GameobjectTeleport* gt = new GameobjectTeleport;
			Field* fields = result->Fetch();
			uint32 entry = fields[0].GetUInt32();
			gt->mapid = fields[1].GetUInt32();
			gt->x = fields[2].GetFloat();
			gt->y = fields[3].GetFloat();
			gt->z = fields[4].GetFloat();
			gt->o = fields[5].GetFloat();
			gt->req_level = fields[6].GetUInt32();
			m_teleStorage[entry] = gt;
			mgr->register_gameobject_script(entry, &CustomTeleport::Create);
		}
		while(result->NextRow());
		delete result;
	}
}
