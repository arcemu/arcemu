/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

const char* GetDifficultyString(uint8 difficulty)
{
	switch(difficulty)
	{
		case MODE_NORMAL:
			return "normal";
		case MODE_HEROIC:
			return "heroic";
		default:
			return "unknown";
	}
}

const char* GetRaidDifficultyString(uint8 diff)
{
	switch(diff)
	{
		case MODE_NORMAL_10MEN:
			return "normal 10men";
		case MODE_NORMAL_25MEN:
			return "normal 25men";
		case MODE_HEROIC_10MEN:
			return "heroic 10men";
		case MODE_HEROIC_25MEN:
			return "heroic 25men";
		default:
			return "unknown";
	}
}

const char* GetMapTypeString(uint8 type)
{
	switch(type)
	{
		case INSTANCE_NULL:
			return "Continent";
		case INSTANCE_RAID:
			return "Raid";
		case INSTANCE_NONRAID:
			return "Non-Raid";
		case INSTANCE_BATTLEGROUND:
			return "PvP";
		case INSTANCE_MULTIMODE:
			return "MultiMode";
		default:
			return "Unknown";
	}
}

bool ChatHandler::HandleResetAllInstancesCommand(const char* args, WorldSession* m_session)
{

	Player* plr;
	if(strlen(args) == 0)
		plr = getSelectedChar(m_session, true);
	else
		plr = objmgr.GetPlayer(args, false);;

	if(!plr)
	{
		RedSystemMessage(m_session, "Player not found");
		return true;
	}

	SystemMessage(m_session, "Trying to reset all instances of player %s...", plr->GetName());
	sInstanceMgr.ResetSavedInstances(plr);
	SystemMessage(m_session, "...done");

	sGMLog.writefromsession(m_session, "used reset all instances command on %s,", plr->GetName());
	return true;
}

bool ChatHandler::HandleResetInstanceCommand(const char* args, WorldSession* m_session)
{

	uint32 instanceId;
	int argc = 1;
	char* playername = NULL;
	char* guidString = (char*)args;

	// Parse arguments
	char* space = (char*)strchr(args, ' ');
	if(space)
	{
		*space = '\0';
		playername = space + 1;
		argc = 2;
	}

	instanceId = atoi(guidString);
	if(!instanceId)
	{
		RedSystemMessage(m_session, "You must specify an instance id.");
		return true;
	}


	Player* plr;

	if(argc == 1)
		plr = getSelectedChar(m_session, true);
	else
		plr = objmgr.GetPlayer((const char*)playername, false);;

	if(!plr)
	{
		RedSystemMessage(m_session, "Player not found");
		return true;
	}

	Instance* instance = sInstanceMgr.GetInstanceByIds(NUM_MAPS, instanceId);
	if(instance == NULL)
	{
		RedSystemMessage(m_session, "There's no instance with id %u.", instanceId);
		return true;
	}

	if(IS_PERSISTENT_INSTANCE(instance))
	{
		if(m_session->CanUseCommand('z'))
		{
			bool foundSomething = false;
			plr->getPlayerInfo()->savedInstanceIdsLock.Acquire();
			for(uint32 difficulty = 0; difficulty < NUM_INSTANCE_MODES; difficulty++)
			{
				PlayerInstanceMap::iterator itr = plr->getPlayerInfo()->savedInstanceIds[difficulty].find(instance->m_mapId);
				if(itr == plr->getPlayerInfo()->savedInstanceIds[difficulty].end() || (*itr).second != instance->m_instanceId)
					continue;
				plr->SetPersistentInstanceId(instance->m_mapId, difficulty, 0);
				SystemMessage(m_session, "Instance with id %u (%s) is persistent and will only be revoked from player.", instanceId, GetDifficultyString(static_cast<uint8>(difficulty)));
				foundSomething = true;
			}
			plr->getPlayerInfo()->savedInstanceIdsLock.Release();
			if(!foundSomething)
				RedSystemMessage(m_session, "Player is not assigned to persistent instance with id %u.", instanceId);
			return true;
		}
		else
		{
			RedSystemMessage(m_session, "Instance with id %u is persistent and can only be removed from player by admins.", instanceId);
			return true;
		}
	}

	if(instance->m_mapMgr && instance->m_mapMgr->HasPlayers())
	{
		RedSystemMessage(m_session, "Failed to reset non-persistent instance with id %u, due to player still inside.", instanceId);
		return true;
	}

	if(instance->m_creatorGroup)
	{
		Group* group = plr->GetGroup();
		if(group == NULL || instance->m_creatorGroup != group->GetID())
		{
			RedSystemMessage(m_session, "Player %s is not a member of the group assigned to the non-persistent instance with id %u.", plr->GetName(), instanceId);
			return true;
		}
	}
	else if(instance->m_creatorGuid == 0 || instance->m_creatorGuid != plr->GetLowGUID())
	{
		RedSystemMessage(m_session, "Player %s is not assigned to instance with id %u.", plr->GetName(), instanceId);
		return true;
	}

	// tell player the instance was reset
	WorldPacket data(SMSG_INSTANCE_RESET, 4);
	data << instance->m_mapId;
	plr->GetSession()->SendPacket(&data);
	// shut down instance
	sInstanceMgr.DeleteBattlegroundInstance(instance->m_mapId, instance->m_instanceId);
	instance->m_mapMgr->InstanceShutdown();
//	RedSystemMessage(m_session, "Resetting single non-persistent instances is not available yet.");
	sGMLog.writefromsession(m_session, "used reset instance command on %s, instance %u,", plr->GetName(), instanceId);
	return true;
}

bool ChatHandler::HandleShutdownInstanceCommand(const char* args, WorldSession* m_session)
{
	uint32 instanceId = (args ? atoi(args) : 0);
	if(instanceId == 0)
		return false;

	Instance* instance = sInstanceMgr.GetInstanceByIds(NUM_MAPS, instanceId);
	if(instance == NULL)
	{
		RedSystemMessage(m_session, "There's no instance with id %u.", instanceId);
		return true;
	}

	if(instance->m_mapMgr == NULL)
	{
		RedSystemMessage(m_session, "Instance with id %u already shut down.", instanceId);
		return true;
	}

	SystemMessage(m_session, "Attempting to shutdown instance with id %u...", instanceId);

	sInstanceMgr.SafeDeleteInstance(instance->m_mapMgr);

	instance = NULL;

	SystemMessage(m_session, "...done");

	sGMLog.writefromsession(m_session, "used shutdown instance command on instance %u,", instanceId);

	return true;
}

//bool ChatHandler::HandleDeleteInstanceCommand(const char* args, WorldSession *m_session)
//{
//	RedSystemMessage(m_session, "Command not implemented yet.");
//	return true;
//	//sGMLog.writefromsession(m_session, "used delete instance command on instance %u,", 0);
//}

bool ChatHandler::HandleGetInstanceInfoCommand(const char* args, WorldSession* m_session)
{
	Player* plr = m_session->GetPlayer();
	if(plr == NULL)
		return false;

	bool userInput = true;
	uint32 instanceId = (args ? atoi(args) : 0);
	if(instanceId == 0)
	{
		userInput = false;
		instanceId = plr->GetInstanceID();
		if(instanceId == 0)
			return false;
	}

	Instance* instance = sInstanceMgr.GetInstanceByIds(NUM_MAPS, instanceId);
	if(instance == NULL)
	{
		if(userInput)
		{
			RedSystemMessage(m_session, "Instance with id %u not found.", instanceId);
			return true;
		}
		return false;
	}

	std::stringstream ss;
	ss << "Instance ID: " << MSG_COLOR_CYAN << instance->m_instanceId << "|r (" << MSG_COLOR_CYAN;
	if(instance->m_mapInfo == NULL)
		ss << instance->m_mapId;
	else
		ss << instance->m_mapInfo->name;
	ss << "|r)\n";
	ss << "Persistent: " << MSG_COLOR_CYAN << (instance->m_persistent ? "Yes" : "No") << "|r\n";
	if(instance->m_mapInfo != NULL)
	{
		ss << "Type: " << MSG_COLOR_CYAN << GetMapTypeString(static_cast<uint8>(instance->m_mapInfo->type)) << "|r";

		if(instance->m_mapInfo->type == INSTANCE_MULTIMODE)
		{
			ss << " (" << MSG_COLOR_CYAN << GetDifficultyString(static_cast<uint8>(instance->m_difficulty)) << "|r)";
		}

		if(instance->m_mapInfo->type == INSTANCE_RAID)
		{
			ss << " (" << MSG_COLOR_CYAN << GetRaidDifficultyString(static_cast<uint8>(instance->m_difficulty)) << "|r)";
		}

		ss << "\n";
	}
	ss << "Created: " << MSG_COLOR_CYAN << ConvertTimeStampToDataTime((uint32)instance->m_creation) << "|r\n";
	if(instance->m_expiration != 0)
		ss << "Expires: " << MSG_COLOR_CYAN << ConvertTimeStampToDataTime((uint32)instance->m_expiration) << "|r\n";

	if(instance->m_mapMgr == NULL)
	{
		ss << "Status: " << MSG_COLOR_LIGHTRED << "Shut Down|r\n";
	}
	else if(!instance->m_mapMgr->HasPlayers())
	{
		ss << "Status: " << MSG_COLOR_LIGHTRED << "Idle|r";
		if(instance->m_mapMgr->InactiveMoveTime && instance->m_mapMgr->GetMapInfo()->type != INSTANCE_NULL)
			ss << " (" << MSG_COLOR_CYAN << "Shutdown in " << MSG_COLOR_LIGHTRED << (((long)instance->m_mapMgr->InactiveMoveTime - UNIXTIME) / 60) << MSG_COLOR_CYAN << " minutes|r)";
		ss << "\n";
	}
	else
	{
		ss << "Status: " << MSG_COLOR_GREEN << "In use|r (" << MSG_COLOR_GREEN << (uint32)instance->m_mapMgr->GetPlayerCount() << MSG_COLOR_CYAN << " players inside|r)\n";

	}
	SendMultilineMessage(m_session, ss.str().c_str());

	return true;
}

bool ChatHandler::HandleCreateInstanceCommand(const char* args, WorldSession* m_session)
{
	Player* plr = getSelectedChar(m_session, true);
	float x, y, z;
	uint32 mapid;
	if(sscanf(args, "%u %f %f %f", (unsigned int*)&mapid, &x, &y, &z) != 4)
		return false;

	if(!plr)
	{
		plr = m_session->GetPlayer();
		SystemMessage(m_session, "Auto-targeting self.");
	}
	if(!plr) return false;

	/* Create Map Manager */
	MapMgr* mgr = sInstanceMgr.CreateInstance(INSTANCE_NONRAID, mapid);
	if(mgr == NULL)
	{
		sLog.Error("CreateInstanceGMCommand", "CreateInstance() call failed for map %u", mapid);
		return false;      // Shouldn't happen
	}
	Log.Notice("CreateInstanceGMCommand", "GM created instance for map %u", mapid);

	LocationVector vec(x, y, z);
	m_session->GetPlayer()->SafeTeleport(mgr, vec);
	return true;

}

bool ChatHandler::HandleExitInstanceCommand(const char* args, WorldSession* m_session)
{
	BlueSystemMessage(m_session, "Attempting to exit from instance...");
	bool result = m_session->GetPlayer()->ExitInstance();
	if(!result)
	{
		RedSystemMessage(m_session, "Entry points not found.");
		return true;
	}
	else
	{
		GreenSystemMessage(m_session, "Removal successful.");
		return true;
	}
}

bool ChatHandler::HandleShowInstancesCommand(const char* args, WorldSession* m_session)
{
	Player* plr = getSelectedChar(m_session, true);
	if(!plr)
		return true;

	uint32 count = 0;
	std::stringstream ss;
	ss << "Show persistent instances of " << MSG_COLOR_CYAN << plr->GetName() << "|r\n";
	plr->getPlayerInfo()->savedInstanceIdsLock.Acquire();
	for(uint32 difficulty = 0; difficulty < NUM_INSTANCE_MODES; difficulty++)
	{
		for(PlayerInstanceMap::iterator itr = plr->getPlayerInfo()->savedInstanceIds[difficulty].begin(); itr != plr->getPlayerInfo()->savedInstanceIds[difficulty].end(); ++itr)
		{
			count++;
			ss << " - " << MSG_COLOR_CYAN << (*itr).second << "|r";
			MapInfo* mapInfo = WorldMapInfoStorage.LookupEntry((*itr).first);
			if(mapInfo != NULL)
				ss << " (" << MSG_COLOR_CYAN << mapInfo->name << "|r)";
			Instance* pInstance = sInstanceMgr.GetInstanceByIds((*itr).first, (*itr).second);
			if(pInstance == NULL)
				ss << " - " << MSG_COLOR_RED << "Expired!|r";
			else
			{
				ss << " [" << GetMapTypeString(static_cast<uint8>(pInstance->m_mapInfo->type)) << "]";
				if(pInstance->m_mapInfo->type == INSTANCE_MULTIMODE)
				{
					ss << " [" << GetDifficultyString(static_cast<uint8>(pInstance->m_difficulty)) << "]";
				}
				ss << " - ";
				if(pInstance->m_mapMgr == NULL)
					ss << MSG_COLOR_LIGHTRED << "Shut Down|r";
				else
				{
					if(!pInstance->m_mapMgr->HasPlayers())
						ss << MSG_COLOR_LIGHTRED << "Idle|r";
					else
						ss << MSG_COLOR_GREEN << "In use|r";
				}
			}
			ss << "\n";
		}
	}
	plr->getPlayerInfo()->savedInstanceIdsLock.Release();

	if(count == 0)
		ss << "Player is not assigned to any persistent instances.\n";
	else
		ss << "Player is assigned to " << MSG_COLOR_CYAN << count << "|r persistent instances.\n";

	SendMultilineMessage(m_session, ss.str().c_str());
	sGMLog.writefromsession(m_session, "used show instances command on %s,", plr->GetName());
	return true;
}
