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

/////////////////////////////////////////////////
//  Normal User Chat Commands
//

#include "StdAfx.h"
#include <git_version.h>

bool ChatHandler::ShowHelpForCommand(WorldSession* m_session, ChatCommand* table, const char* cmd)
{
	for(uint32 i = 0; table[i].Name != NULL; i++)
	{
		if(!hasStringAbbr(table[i].Name, cmd))
			continue;

		if(m_session->CanUseCommand(table[i].CommandGroup))
			continue;

		if(table[i].ChildCommands != NULL)
		{
			cmd = strtok(NULL, " ");
			if(cmd && ShowHelpForCommand(m_session, table[i].ChildCommands, cmd))
				return true;
		}

		if(table[i].Help == "")
		{
			SystemMessage(m_session, "There is no help for that command");
			return true;
		}

		SendMultilineMessage(m_session, table[i].Help.c_str());

		return true;
	}

	return false;
}

bool ChatHandler::HandleHelpCommand(const char* args, WorldSession* m_session)
{
//	ChatCommand *table = getCommandTable();
	WorldPacket data;

	if(!*args)
		return false;

	char* cmd = strtok((char*)args, " ");
	if(!cmd)
		return false;

	if(!ShowHelpForCommand(m_session, CommandTableStorage::getSingleton().Get(), cmd))
	{
		RedSystemMessage(m_session, "Sorry, no help was found for this command, or that command does not exist.");
	}

	return true;
}


bool ChatHandler::HandleCommandsCommand(const char* args, WorldSession* m_session)
{
	ChatCommand* table = CommandTableStorage::getSingleton().Get();
	WorldPacket data;

	std::string output;
	uint32 count = 0;

	output = "Available commands: \n\n";

	for(uint32 i = 0; table[i].Name != NULL; i++)
	{
		if(*args && !hasStringAbbr(table[i].Name, (char*)args))
			continue;

		if(table[i].CommandGroup != '0' && !m_session->CanUseCommand(table[i].CommandGroup))
			continue;

		switch(table[i].CommandGroup)
		{
			case 'z':
				{
					output += "|cffff6060";
					output += table[i].Name;
					output += "|r, ";
				}
				break;
			case 'm':
				{
					output += "|cff00ffff";
					output += table[i].Name;
					output += ", ";
				}
				break;
			case 'c':
				{
					output += "|cff00ff00";
					output += table[i].Name;
					output += "|r, ";
				}
				break;
			default:
				{
					output += "|cff00ccff";
					output += table[i].Name;
					output += "|r, ";
				}
				break;
		}

		count++;
		if(count == 5)  // 5 per line
		{
			output += "\n";
			count = 0;
		}
	}
	if(count)
		output += "\n";


	//FillSystemMessageData(&data, table[i].Name);
	//m_session->SendPacket(&data);
	//}

	SendMultilineMessage(m_session, output.c_str());

	return true;
}

bool ChatHandler::HandleStartCommand(const char* args, WorldSession* m_session)
{
	std::string race;
	Player* m_plyr = getSelectedChar(m_session, false);
	if(m_plyr == NULL)
		return false;

	uint8 raceid = 0;
	uint8 classid = 0;

	if(strlen(args) > 0)
	{
		race = args;
		if(race == "human")
			raceid = 1;
		else if(race == "orc")
			raceid = 2;
		else if(race == "dwarf")
			raceid = 3;
		else if(race == "nightelf" || race == "ne")
			raceid = 4;
		else if(race == "undead")
			raceid = 5;
		else if(race == "tauren")
			raceid = 6;
		else if(race == "gnome")
			raceid = 7;
		else if(race == "troll")
			raceid = 8;
		else if(race == "bloodelf" || race == "be")
			raceid = 10;
		else if(race == "draenei")
			raceid = 11;
		else if(race == "deathknight" || race == "dk")
			classid = 6;
		else
		{
			RedSystemMessage(m_session, "Invalid start location! Valid locations are: human, dwarf, gnome, nightelf, draenei, orc, troll, tauren, undead, bloodelf, deathknight");
			return true;
		}
	}
	else
	{
		raceid = m_plyr->getRace();
		classid = m_plyr->getClass();
		race = "his";
	}
	// find the first matching one
	PlayerCreateInfo* info = NULL;
	for(uint8 i = 1; i <= 11; i++)
	{
		info = objmgr.GetPlayerCreateInfo((raceid ? raceid : i), (classid ? classid : i));
		if(info != NULL)
			break;
	}

	if(info == NULL)
	{
		RedSystemMessage(m_session, "Internal error: Could not find create info for race %u and class %u.", raceid, classid);
		return false;
	}

	GreenSystemMessage(m_session, "Teleporting %s to %s starting location.", m_plyr->GetName(), race.c_str());

	m_plyr->SafeTeleport(info->mapId, 0, LocationVector(info->positionX, info->positionY, info->positionZ));
	return true;
}


bool ChatHandler::HandleInfoCommand(const char* args, WorldSession* m_session)
{
	WorldPacket data;


	//uint32 clientsNum = (uint32)sWorld.GetSessionCount();

	int gm = 0;
	int count = 0;
	int avg = 0;
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
	{
		if(itr->second->GetSession())
		{
			count++;
			avg += itr->second->GetSession()->GetLatency();
			if(itr->second->GetSession()->GetPermissionCount())
				gm++;
		}
	}
	objmgr._playerslock.ReleaseReadLock();
	GreenSystemMessage(m_session, "Server Revision: |r%sArcEmu %s/%s-%s-%s %s(www.arcemu.org)", MSG_COLOR_WHITE,
		BUILD_HASH_STR, CONFIG, PLATFORM_TEXT, ARCH, MSG_COLOR_LIGHTBLUE);
	GreenSystemMessage(m_session, "Server Uptime: |r%s", sWorld.GetUptimeString().c_str());
	GreenSystemMessage(m_session, "Current Players: |r%d (%d GMs) (%u Peak)", count, gm, sWorld.PeakSessionCount);
	GreenSystemMessage(m_session, "Active Thread Count: |r%u", ThreadPool.GetActiveThreadCount());
	GreenSystemMessage(m_session, "Free Thread Count: |r%u", ThreadPool.GetFreeThreadCount());
	GreenSystemMessage(m_session, "Average Latency: |r%.3fms", ((float)avg / (float)count));
	GreenSystemMessage(m_session, "CPU Usage: %3.2f %%", sWorld.GetCPUUsage());
	GreenSystemMessage(m_session, "RAM Usage: %4.2f MB", sWorld.GetRAMUsage());
	GreenSystemMessage(m_session, "SQL Query Cache Size (World): |r%u queries delayed", WorldDatabase.GetQueueSize());
	GreenSystemMessage(m_session, "SQL Query Cache Size (Character): |r%u queries delayed", CharacterDatabase.GetQueueSize());

	return true;
}

bool ChatHandler::HandleNetworkStatusCommand(const char* args, WorldSession* m_session)
{
	//sSocketMgr.ShowStatus();
	return true;
}

bool ChatHandler::HandleNYICommand(const char* args, WorldSession* m_session)
{
	RedSystemMessage(m_session, "Not yet implemented.");
	return true;
}

bool ChatHandler::HandleDismountCommand(const char* args, WorldSession* m_session)
{
	Unit* m_target = NULL;

	Player* p_target = getSelectedChar(m_session, false);

	if(p_target)
		m_target = p_target;
	else
	{
		Creature* m_crt = getSelectedCreature(m_session, false);
		if(m_crt)
			m_target = m_crt;
	}

	if(!m_target)
	{
		RedSystemMessage(m_session, "No target found.");
		return true;
	}

	if(m_target->GetMount() == 0)
	{
		RedSystemMessage(m_session, "Target is not mounted.");
		return true;
	}

	if(p_target != NULL)
		p_target->Dismount();

	m_target->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID , 0);
	//m_target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);

	BlueSystemMessage(m_session, "Now unmounted.");
	return true;
}


bool ChatHandler::HandleSaveCommand(const char* args, WorldSession* m_session)
{
	Player* p_target = getSelectedChar(m_session, false);
	if(p_target == NULL)
		return false;

	if(p_target->m_nextSave < 300000)  //5min out of 10 left so 5 min since last save
	{
		p_target->SaveToDB(false);
		GreenSystemMessage(m_session, "Player %s saved to DB", p_target->GetName());
	}
	else
	{
		RedSystemMessage(m_session, "You can only save once every 5 minutes.");
	}
	return true;
}

bool ChatHandler::HandleGMListCommand(const char* args, WorldSession* m_session)
{
	WorldPacket data;
	bool first = true;

	bool isGM = m_session->GetPermissionCount() != 0;

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
	{
		if(itr->second->GetSession()->GetPermissionCount())
		{
			if(isGM || !sWorld.gamemaster_listOnlyActiveGMs || (sWorld.gamemaster_listOnlyActiveGMs && itr->second->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM)))
			{
				if(first)
					GreenSystemMessage(m_session, "There are following active GMs on this server:");

				if(sWorld.gamemaster_hidePermissions && !isGM)
					SystemMessage(m_session, " - %s", itr->second->GetName());
				else
				{
					if(sWorld.gamemaster_listOnlyActiveGMs && !itr->second->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
						SystemMessage(m_session, "|cff888888 - %s [%s]|r", itr->second->GetName(), itr->second->GetSession()->GetPermissions());
					else
						SystemMessage(m_session, " - %s [%s]", itr->second->GetName(), itr->second->GetSession()->GetPermissions());
				}

				first = false;
			}
		}
	}
	objmgr._playerslock.ReleaseReadLock();

	if(first)
		SystemMessage(m_session, "There are no GMs currently logged in on this server.");

	return true;
}

bool ChatHandler::HandleGMStatusCommand(const char* args, WorldSession* m_session)
{
	if(m_session->GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
		BlueSystemMessage(m_session, "GM Flag: On");
	else
		BlueSystemMessage(m_session, "GM Flag: Off");

	if(m_session->GetPlayer()->m_isGmInvisible)
		BlueSystemMessage(m_session, "GM Invis: On");
	else
		BlueSystemMessage(m_session, "GM Invis: Off");

	return true;
}

bool ChatHandler::HandleRangeCheckCommand(const char* args , WorldSession* m_session)
{
	WorldPacket data;
	uint64 guid = m_session->GetPlayer()->GetSelection();
	m_session->SystemMessage("=== RANGE CHECK ===");
	if(guid == 0)
	{
		m_session->SystemMessage("No selection.");
		return true;
	}

	Unit* unit = m_session->GetPlayer()->GetMapMgr()->GetUnit(guid);
	if(!unit)
	{
		m_session->SystemMessage("Invalid selection.");
		return true;
	}
	float DistSq = unit->GetDistanceSq(m_session->GetPlayer());
	m_session->SystemMessage("GetDistanceSq  :   %u" , float2int32(DistSq));
	LocationVector locvec(m_session->GetPlayer()->GetPositionX() , m_session->GetPlayer()->GetPositionY() , m_session->GetPlayer()->GetPositionZ());
	float DistReal = unit->CalcDistance(locvec);
	m_session->SystemMessage("CalcDistance   :   %u" , float2int32(DistReal));
	float Dist2DSq = unit->GetDistance2dSq(m_session->GetPlayer());
	m_session->SystemMessage("GetDistance2dSq:   %u" , float2int32(Dist2DSq));
	return true;
}

bool ChatHandler::HandleGmLogCommentCommand(const char* args , WorldSession* m_session)
{
	if(!args || !strlen(args)) return false;
	BlueSystemMessage(m_session, "Added Logcomment: %s", args);
	sGMLog.writefromsession(m_session, "Logcomment: %s", args);
	return true;
}

bool ChatHandler::HandleRatingsCommand(const char* args , WorldSession* m_session)
{
	m_session->SystemMessage("Ratings!!!");
	Player* m_plyr = getSelectedChar(m_session, false);

	if(m_plyr == NULL)
		return false;

	for(uint32 i = 0; i < 24; i++)
	{
		m_plyr->ModUnsigned32Value(PLAYER_FIELD_COMBAT_RATING_1 + i, i);
	}
	m_plyr->UpdateStats();
	return true;
}

float CalculateDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	float dz = z1 - z2;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

bool ChatHandler::HandleSimpleDistanceCommand(const char* args , WorldSession* m_session)
{
	float toX, toY, toZ;
	if(sscanf(args, "%f %f %f", &toX, &toY, &toZ) != 3)
		return false;

	if(toX >= _maxX || toX <= _minX || toY <= _minY || toY >= _maxY)
		return false;

	float distance = CalculateDistance(
	                     m_session->GetPlayer()->GetPositionX(),
	                     m_session->GetPlayer()->GetPositionY(),
	                     m_session->GetPlayer()->GetPositionZ(),
	                     toX, toY, toZ);

	m_session->SystemMessage("Your distance to location (%f, %f, %f) is %0.2f meters.",
	                         toX, toY, toZ, distance);

	return true;
}

bool ChatHandler::HandleSendFailed(const char* args , WorldSession* m_session)
{
	Player* plr = getSelectedChar(m_session, true);
	if(plr == NULL)
		return false;

	uint32 fail = atol(args);
	if(SPELL_CANCAST_OK < fail)
	{
		RedSystemMessage(m_session, "Argument %u is out of range!", fail);
		return false;
	}
	plr->SendCastResult(1, (uint8)fail, 0, 0);
	return true;
}

bool ChatHandler::HandlePlayMovie(const char* args, WorldSession* m_session)
{
	Player* plr = getSelectedChar(m_session, true);
	if(plr == NULL)
		return false;

	uint32 movie = atol(args);

	plr->SendTriggerMovie(movie);

	SystemMessage(m_session, "Movie started.");
	return true;
}

bool ChatHandler::HandleAuraUpdateAdd(const char* args, WorldSession* m_session)
{
	if(!args)
		return false;

	uint32 SpellID = 0;
	int Flags = 0;
	int StackCount = 0;
	if(sscanf(args, "%u 0x%X %i", &SpellID, &Flags, &StackCount) != 3 && sscanf(args, "%u %u %i", &SpellID, &Flags, &StackCount) != 3)
		return false;

	Player* Pl = m_session->GetPlayer();
	if(Aura* AuraPtr = Pl->FindAura(SpellID))
	{
		uint8 VisualSlot = AuraPtr->m_visualSlot;
		Pl->SendAuraUpdate(AuraPtr->m_auraSlot, false);
		SystemMessage(m_session, "SMSG_AURA_UPDATE (update): VisualSlot %u - SpellID %u - Flags %i (0x%04X) - StackCount %i", VisualSlot, SpellID, Flags, Flags, StackCount);
	}
	else
	{
		SpellEntry* Sp = dbcSpell.LookupEntryForced(SpellID);
		if(!Sp)
		{
			SystemMessage(m_session, "SpellID %u is invalid.", SpellID);
			return true;
		}
		Spell* SpellPtr = sSpellFactoryMgr.NewSpell(Pl, Sp, false, NULL);
		AuraPtr = sSpellFactoryMgr.NewAura(Sp, SpellPtr->GetDuration(), Pl, Pl);
		Pl->AddAura(AuraPtr); // Serves purpose to just add the aura to our auraslots
		SystemMessage(m_session, "SMSG_AURA_UPDATE (add): VisualSlot %u - SpellID %u - Flags %i (0x%04X) - StackCount %i", AuraPtr->m_visualSlot, SpellID, Flags, Flags, StackCount);
		delete SpellPtr;
	}
	return true;
}

bool ChatHandler::HandleAuraUpdateRemove(const char* args, WorldSession* m_session)
{
	if(!args)
		return false;

	char* pArgs = strtok((char*)args, " ");
	if(!pArgs)
		return false;
	uint8 VisualSlot = (uint8)atoi(pArgs);
	Player* Pl = m_session->GetPlayer();
	Aura* AuraPtr = Pl->FindAura(Pl->m_auravisuals[VisualSlot]);
	if(!AuraPtr)
	{
		SystemMessage(m_session, "No auraid found in slot %u", VisualSlot);
		return true;
	}
	SystemMessage(m_session, "SMSG_AURA_UPDATE (remove): VisualSlot %u - SpellID 0", VisualSlot);
	AuraPtr->Remove();
	return true;
}

bool ChatHandler::HandlePhaseCommand(const char* args , WorldSession* m_session)
{
	Player* p_target = getSelectedChar(m_session, false);
	if(p_target == NULL)
		return false;

	if(strlen(args) < 1)
	{
		SystemMessage(m_session, "%s phase:%s%u", MSG_COLOR_GREEN, MSG_COLOR_LIGHTBLUE, p_target->GetPhase());
		return true;
	}

	uint32 i = atoi(args);
	p_target->Phase(PHASE_SET, i);

	if(p_target->GetSession())
	{
		WorldPacket data(SMSG_SET_PHASE_SHIFT, 4);
		data << i;
		p_target->GetSession()->SendPacket(&data);
	}

	return true;
}