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

initialiseSingleton(CBattlegroundManager);

CBattlegroundManager::CBattlegroundManager()
	: EventableObject()
{
	int i;

	// Yes we will be running from WorldRunnable
	m_holder = sEventMgr.GetEventHolder(WORLD_INSTANCE);

	sEventMgr.AddEvent(this, &CBattlegroundManager::EventQueueUpdate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 15000, 0, 0);

	for(i = 0; i < BATTLEGROUND_NUM_TYPES; i++)
	{
		m_instances[i].clear();
		m_maxBattlegroundId[i] = 0;
	}

}

CBattlegroundManager::~CBattlegroundManager()
{

}

void CBattlegroundManager::RegisterBgFactory( uint32 map, BattlegroundFactoryMethod method ){
	std::map< uint32, BattlegroundFactoryMethod >::iterator itr = bgFactories.find( map );
	if( itr != bgFactories.end() )
		return;

	bgFactories[ map ] = method;

}

void CBattlegroundManager::RegisterArenaFactory( uint32 map, ArenaFactoryMethod method ){
	std::vector< uint32 >::iterator itr =
		std::find( arenaMaps.begin(), arenaMaps.end(), map );
	if( itr != arenaMaps.end() )
		return;
	arenaMaps.push_back( map );
	arenaFactories.push_back( method );
}


void CBattlegroundManager::RegisterMapForBgType( uint32 type, uint32 map ){
	std::map< uint32, uint32 >::iterator itr = bgMaps.find( type );
	if( itr != bgMaps.end() )
		return;

	bgMaps[ type ] = map;

}

void CBattlegroundManager::HandleBattlegroundListPacket(WorldSession* m_session, uint32 BattlegroundType, uint8 from)
{
	WorldPacket data(SMSG_BATTLEFIELD_LIST, 18);

	// Send 0 instead of GUID when using the BG UI instead of Battlemaster
	if(from == 0)
		data << uint64(m_session->GetPlayer()->GetGUID());
	else
		data << uint64(0);

	data << from;
	data << uint32(BattlegroundType);   // typeid
	data << uint8(0);                                      // unk
	data << uint8(0);                                      // unk

	// Rewards
	data << uint8(0);                                      // 3.3.3 hasWin
	data << uint32(0);                                     // 3.3.3 winHonor
	data << uint32(0);                                     // 3.3.3 winArena
	data << uint32(0);                                     // 3.3.3 lossHonor

	uint8 isRandom = 0;
	data << uint8(isRandom);                               // 3.3.3 isRandom

	// Random bgs
	if(isRandom == 1)
	{
		// rewards
		data << uint8(0);                                  // win random
		data << uint32(0);                                 // Reward honor if won
		data << uint32(0);                                 // Reward arena point if won
		data << uint32(0);                                 // Lost honor if lost
	}

	if(IS_ARENA(BattlegroundType))
	{
		data << uint32(0);
		m_session->SendPacket(&data);
		return;
	}

	if(BattlegroundType >= BATTLEGROUND_NUM_TYPES) return;   //VLack: Nasty hackers might try to abuse this packet to crash us...

	uint32 Count = 0;
	size_t pos = data.wpos();

	data << uint32(0);      // Count

	/* Append the battlegrounds */
	m_instanceLock.Acquire();
	for(map<uint32, CBattleground*>::iterator itr = m_instances[BattlegroundType].begin(); itr != m_instances[BattlegroundType].end(); ++itr)
	{
		if(itr->second->CanPlayerJoin(m_session->GetPlayer(), BattlegroundType) && !itr->second->HasEnded())
		{
			data << uint32(itr->first);
			++Count;
		}
	}
	m_instanceLock.Release();

	data.put< uint32 >(pos, Count);

	m_session->SendPacket(&data);
}

void CBattlegroundManager::HandleBattlegroundJoin(WorldSession* m_session, WorldPacket & pck)
{

	uint64 guid;
	uint32 pguid = m_session->GetPlayer()->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(m_session->GetPlayer()->getLevel());
	uint32 bgtype;
	uint32 instance;

	pck >> guid >> bgtype >> instance;

	// Random battlegrounds aren't yet implemented
	if( bgtype == BATTLEGROUND_RANDOM )
		return;

	if( ( bgtype >= BATTLEGROUND_NUM_TYPES ) || ( bgtype == 0 ) ||
		( ( bgMaps.find( bgtype ) == bgMaps.end() ) && bgtype != BATTLEGROUND_RANDOM ) )
	{
		sCheatLog.writefromsession(m_session, "tried to crash the server by joining battleground that does not exist (0)");
		m_session->Disconnect();
		return;      // cheater!
	}

	/* Check the instance id */
	if(instance)
	{
		/* We haven't picked the first instance. This means we've specified an instance to join. */
		m_instanceLock.Acquire();
		map<uint32, CBattleground*>::iterator itr = m_instances[bgtype].find(instance);

		if(itr == m_instances[bgtype].end())
		{
			sChatHandler.SystemMessage(m_session, m_session->LocalizedWorldSrv(51));
			m_instanceLock.Release();
			return;
		}

		m_instanceLock.Release();
	}

	/* Queue him! */
	m_queueLock.Acquire();
	m_queuedPlayers[bgtype][lgroup].push_back(pguid);
	Log.Notice("BattlegroundManager", "Player %u is now in battleground queue for instance %u", m_session->GetPlayer()->GetLowGUID(), (instance + 1));

	/* send the battleground status packet */
	SendBattlefieldStatus(m_session->GetPlayer(), BGSTATUS_INQUEUE, bgtype, instance, 0, bgMaps[bgtype], 0);
	m_session->GetPlayer()->m_bgIsQueued = true;
	m_session->GetPlayer()->m_bgQueueInstanceId = instance;
	m_session->GetPlayer()->m_bgQueueType = bgtype;

	/* Set battleground entry point */
	m_session->GetPlayer()->m_bgEntryPointX = m_session->GetPlayer()->GetPositionX();
	m_session->GetPlayer()->m_bgEntryPointY = m_session->GetPlayer()->GetPositionY();
	m_session->GetPlayer()->m_bgEntryPointZ = m_session->GetPlayer()->GetPositionZ();
	m_session->GetPlayer()->m_bgEntryPointMap = m_session->GetPlayer()->GetMapId();
	m_session->GetPlayer()->m_bgEntryPointInstance = m_session->GetPlayer()->GetInstanceID();

	m_queueLock.Release();

	/* We will get updated next few seconds =) */
}

#define IS_ARENA(x) ( (x) >= BATTLEGROUND_ARENA_2V2 && (x) <= BATTLEGROUND_ARENA_5V5 )

void ErasePlayerFromList(uint32 guid, list<uint32>* l)
{
	for(list<uint32>::iterator itr = l->begin(); itr != l->end(); ++itr)
	{
		if((*itr) == guid)
		{
			l->erase(itr);
			return;
		}
	}
}

uint8 GetBattlegroundCaption(BattleGroundTypes bgType)
{
	switch(bgType)
	{
		case BATTLEGROUND_ALTERAC_VALLEY:
			return 38;
		case BATTLEGROUND_WARSONG_GULCH:
			return 39;
		case BATTLEGROUND_ARATHI_BASIN:
			return 40;
		case BATTLEGROUND_ARENA_2V2:
			return 41;
		case BATTLEGROUND_ARENA_3V3:
			return 42;
		case BATTLEGROUND_ARENA_5V5:
			return 43;
		case BATTLEGROUND_EYE_OF_THE_STORM:
			return 44;
		case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
			return 34;
		default:
			return 45;
	}
}

void CBattlegroundManager::HandleGetBattlegroundQueueCommand(WorldSession* m_session)
{
	std::stringstream ss;

	uint32 i, j;
	Player* plr;
	list<uint32>::iterator it3, it4;

	m_queueLock.Acquire();

	bool foundSomething = false;

	for(i = 0; i < BATTLEGROUND_NUM_TYPES; ++i)
	{
		for(j = 0; j < MAX_LEVEL_GROUP; ++j)
		{
			if(!m_queuedPlayers[i][j].size())
				continue;

			foundSomething = true;

			ss << m_session->LocalizedWorldSrv(GetBattlegroundCaption((BattleGroundTypes)i));

			switch(j)
			{
				case 0:
					ss << " (<10)";
					break;
				case 1:
					ss << " (<20)";
					break;
				case 2:
					ss << " (<30)";
					break;
				case 3:
					ss << " (<40)";
					break;
				case 4:
					ss << " (<50)";
					break;
				case 5:
					ss << " (<60)";
					break;
				case 6:
					ss << " (<70)";
					break;
				case 7:
					ss << " (<80)";
					break;
			}

			ss << ": ";

			ss << (uint32)m_queuedPlayers[i][j].size() << " players queued";

			if(!IS_ARENA(i))
			{
				int ally = 0, horde = 0;

				for(it3 = m_queuedPlayers[i][j].begin(); it3 != m_queuedPlayers[i][j].end();)
				{
					it4 = it3++;
					plr = objmgr.GetPlayer(*it4);

					if(!plr || GetLevelGrouping(plr->getLevel()) != j)
					{
						continue;
					}

					if(plr->IsTeamAlliance())
						ally++;
					else
						horde++;
				}

				ss << " (Alliance: " << ally << " Horde: " << horde;
				if((int)m_queuedPlayers[i][j].size() > (ally + horde))
					ss << " Unknown: " << ((int)m_queuedPlayers[i][j].size() - ally - horde);
				ss << ")";
			}

			m_session->SystemMessage(ss.str().c_str());
			ss.rdbuf()->str("");
		}

		if(IS_ARENA(i))
		{
			if(m_queuedGroups[i].size())
			{
				foundSomething = true;

				ss << m_session->LocalizedWorldSrv(GetBattlegroundCaption((BattleGroundTypes)i)) << " (rated): ";
				ss << (uint32)m_queuedGroups[i].size() << " groups queued";

				m_session->SystemMessage(ss.str().c_str());
				ss.rdbuf()->str("");
			}
		}
	}

	m_queueLock.Release();

	if(!foundSomething)
		m_session->SystemMessage("There's nobody queued.");
}

void CBattlegroundManager::EventQueueUpdate()
{
	this->EventQueueUpdate(false);
}

uint32 CBattlegroundManager::GetArenaGroupQInfo(Group* group, int type, uint32* avgRating)
{
	ArenaTeam* team;
	ArenaTeamMember* atm;
	Player* plr;
	uint32 count = 0;
	uint32 rating = 0;

	if(group == NULL || group->GetLeader() == NULL) return 0;

	plr = group->GetLeader()->m_loggedInPlayer;
	if(plr == NULL) return 0;

	team = plr->m_arenaTeams[type - BATTLEGROUND_ARENA_2V2];
	if(team == NULL) return 0;

	GroupMembersSet::iterator itx;
	for(itx = group->GetSubGroup(0)->GetGroupMembersBegin(); itx != group->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
	{
		plr = (*itx)->m_loggedInPlayer;
		if(plr)
		{
			if(team == plr->m_arenaTeams[type - BATTLEGROUND_ARENA_2V2])
			{
				atm = team->GetMemberByGuid(plr->GetLowGUID());
				if(atm)
				{
					rating += atm->PersonalRating;
					count++;
				}
			}
		}
	}

	*avgRating = count > 0 ? rating / count : 0;

	return team ? team->m_id : 0;
}

void CBattlegroundManager::AddGroupToArena(CBattleground* bg, Group* group, int nteam)
{
	ArenaTeam* team;
	Player* plr;

	if(group == NULL || group->GetLeader() == NULL) return;

	plr = group->GetLeader()->m_loggedInPlayer;
	if(plr == NULL) return;

	team = plr->m_arenaTeams[bg->GetType() - BATTLEGROUND_ARENA_2V2];
	if(team == NULL) return;

	GroupMembersSet::iterator itx;
	for(itx = group->GetSubGroup(0)->GetGroupMembersBegin(); itx != group->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
	{
		plr = (*itx)->m_loggedInPlayer;
		if(plr && team == plr->m_arenaTeams[bg->GetType() - BATTLEGROUND_ARENA_2V2])
		{
			if(bg->HasFreeSlots(nteam, bg->GetType()))
			{
				bg->AddPlayer(plr, nteam);
				plr->SetTeam(nteam);
			}
		}
	}
}

int CBattlegroundManager::CreateArenaType(int type, Group* group1, Group* group2)
{
	Arena* ar = TO< Arena* >(CreateInstance(type, LEVEL_GROUP_70));
	if(ar == NULL)
	{
		sLog.Error("BattlegroundMgr", "%s (%u): Couldn't create Arena Instance", __FILE__, __LINE__);
		m_queueLock.Release();
		m_instanceLock.Release();
		return -1;
	}
	ar->rated_match = true;

	AddGroupToArena(ar, group1, 0);
	AddGroupToArena(ar, group2, 1);

	return 0;
}

void CBattlegroundManager::AddPlayerToBg(CBattleground* bg, deque<uint32> *playerVec, uint32 i, uint32 j)
{
	uint32 plrguid = *playerVec->begin();
	playerVec->pop_front();
	Player* plr = objmgr.GetPlayer(plrguid);
	if(plr)
	{
		if(bg->CanPlayerJoin(plr, bg->GetType()))
		{
			bg->AddPlayer(plr, plr->GetTeam());
			ErasePlayerFromList(plr->GetLowGUID(), &m_queuedPlayers[i][j]);
		}
		else
		{
			// Put again the player in the queue
			playerVec->push_back(plrguid);
		}
	}
	else
	{
		ErasePlayerFromList(plrguid, &m_queuedPlayers[i][j]);
	}
}

void CBattlegroundManager::AddPlayerToBgTeam(CBattleground* bg, deque<uint32> *playerVec, uint32 i, uint32 j, int Team)
{
	if(bg->HasFreeSlots(Team, bg->GetType()))
	{
		uint32 plrguid = *playerVec->begin();
		playerVec->pop_front();
		Player* plr = objmgr.GetPlayer(plrguid);
		if(plr)
		{
			plr->m_bgTeam = Team;
			bg->AddPlayer(plr, Team);
		}
		ErasePlayerFromList(plrguid, &m_queuedPlayers[i][j]);
	}
}

void CBattlegroundManager::EventQueueUpdate(bool forceStart)
{
	deque<uint32> tempPlayerVec[2];
	uint32 i, j, k;
	Player* plr;
	CBattleground* bg;
	list<uint32>::iterator it3, it4;
	map<uint32, CBattleground*>::iterator iitr;
	Arena* arena;
	int32 team;
	uint32 plrguid;
	m_queueLock.Acquire();
	m_instanceLock.Acquire();

	for(i = 0; i < BATTLEGROUND_NUM_TYPES; ++i)
	{
		for(j = 0; j < MAX_LEVEL_GROUP; ++j)
		{
			if(!m_queuedPlayers[i][j].size())
				continue;

			tempPlayerVec[0].clear();
			tempPlayerVec[1].clear();

			for(it3 = m_queuedPlayers[i][j].begin(); it3 != m_queuedPlayers[i][j].end();)
			{
				it4 = it3++;
				plrguid = *it4;
				plr = objmgr.GetPlayer(plrguid);

				if(!plr || GetLevelGrouping(plr->getLevel()) != j)
				{
					m_queuedPlayers[i][j].erase(it4);
					continue;
				}

				// queued to a specific instance id?
				if(plr->m_bgQueueInstanceId != 0)
				{
					iitr = m_instances[i].find(plr->m_bgQueueInstanceId);
					if(iitr == m_instances[i].end())
					{
						// queue no longer valid
						plr->GetSession()->SystemMessage(plr->GetSession()->LocalizedWorldSrv(52), plr->m_bgQueueInstanceId);
						plr->m_bgIsQueued = false;
						plr->m_bgQueueType = 0;
						plr->m_bgQueueInstanceId = 0;
						m_queuedPlayers[i][j].erase(it4);
						continue;
					}

					// can we join?
					bg = iitr->second;
					if(bg->CanPlayerJoin(plr, bg->GetType()))
					{
						bg->AddPlayer(plr, plr->GetTeam());
						m_queuedPlayers[i][j].erase(it4);
					}
				}
				else
				{
					if(IS_ARENA(i))
						tempPlayerVec[0].push_back(plrguid);
					else if(!plr->HasAura(BG_DESERTER))
						tempPlayerVec[plr->GetTeam()].push_back(plrguid);
				}
			}

			// try to join existing instances
			for(iitr = m_instances[i].begin(); iitr != m_instances[i].end(); ++iitr)
			{
				if(iitr->second->HasEnded() || iitr->second->GetLevelGroup() != j)
					continue;

				if(IS_ARENA(i))
				{
					arena = TO< Arena* >(iitr->second);
					if(arena->Rated())
						continue;

					team = arena->GetFreeTeam();
					while(team >= 0 && tempPlayerVec[0].size())
					{
						plrguid = *tempPlayerVec[0].begin();
						tempPlayerVec[0].pop_front();
						plr = objmgr.GetPlayer(plrguid);
						if(plr)
						{
							plr->m_bgTeam = team;
							arena->AddPlayer(plr, team);
							team = arena->GetFreeTeam();
						}
						ErasePlayerFromList(plrguid, &m_queuedPlayers[i][j]);
					}
				}
				else
				{
					bg = iitr->second;
					int size = (int)min(tempPlayerVec[0].size(), tempPlayerVec[1].size());
					for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
						AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
					}

					while(tempPlayerVec[0].size() > 0 && bg->HasFreeSlots(0, bg->GetType()))
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
					}
					while(tempPlayerVec[1].size() > 0 && bg->HasFreeSlots(1, bg->GetType()))
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
					}
				}
			}

			if(IS_ARENA(i))
			{
				// enough players to start a round?
				uint32 minPlayers = BattlegroundManager.GetMinimumPlayers(i);
				if(!forceStart && tempPlayerVec[0].size() < minPlayers)
					continue;

				if(CanCreateInstance(i, j))
				{
					arena = TO< Arena* >(CreateInstance(i, j));
					if(arena == NULL)
					{
						sLog.Error("BattlegroundMgr", "%s (%u): Couldn't create Arena Instance", __FILE__, __LINE__);
						m_queueLock.Release();
						m_instanceLock.Release();
						return;
					}

					team = arena->GetFreeTeam();
					while(!arena->IsFull() && tempPlayerVec[0].size() && team >= 0)
					{
						plrguid = *tempPlayerVec[0].begin();
						tempPlayerVec[0].pop_front();
						plr = objmgr.GetPlayer(plrguid);

						if(plr)
						{
							plr->m_bgTeam = team;
							arena->AddPlayer(plr, team);
							team = arena->GetFreeTeam();
						}

						// remove from the main queue (painful!)
						ErasePlayerFromList(plrguid, &m_queuedPlayers[i][j]);
					}
				}
			}
			else
			{
				uint32 minPlayers = BattlegroundManager.GetMinimumPlayers(i);
				if(forceStart || (tempPlayerVec[0].size() >= minPlayers && tempPlayerVec[1].size() >= minPlayers))
				{
					if(CanCreateInstance(i, j))
					{
						bg = CreateInstance(i, j);
						if(bg == NULL)
						{
							m_queueLock.Release();
							m_instanceLock.Release();
							return;
						}

						// push as many as possible in
						if(forceStart)
						{
							for(k = 0; k < 2; ++k)
							{
								while(tempPlayerVec[k].size() && bg->HasFreeSlots(k, bg->GetType()))
								{
									AddPlayerToBgTeam(bg, &tempPlayerVec[k], i, j, k);
								}
							}
						}
						else
						{
							int size = (int)min(tempPlayerVec[0].size(), tempPlayerVec[1].size());
							for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
							{
								AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
								AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
							}
						}
					}
				}
			}
		}
	}

	/* Handle paired arena team joining */
	Group* group1, *group2;
	uint32 teamids[2] = {0, 0};
	uint32 avgRating[2] = {0, 0};
	uint32 n;
	list<uint32>::iterator itz;
	for(i = BATTLEGROUND_ARENA_2V2; i <= BATTLEGROUND_ARENA_5V5; ++i)
	{
		if(!forceStart && m_queuedGroups[i].size() < 2)      /* got enough to have an arena battle ;P */
		{
			continue;
		}

		for(j = 0; j < (uint32)m_queuedGroups[i].size(); j++)
		{
			group1 = group2 = NULL;
			n =	RandomUInt((uint32)m_queuedGroups[i].size()) - 1;
			for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end() && n > 0; ++itz)
				--n;

			if(itz == m_queuedGroups[i].end())
				itz = m_queuedGroups[i].begin();

			if(itz == m_queuedGroups[i].end())
			{
				sLog.Error("BattlegroundMgr", "Internal error at %s:%u", __FILE__, __LINE__);
				m_queueLock.Release();
				m_instanceLock.Release();
				return;
			}

			group1 = objmgr.GetGroupById(*itz);
			if(group1 == NULL)
			{
				continue;
			}

			if(forceStart && m_queuedGroups[i].size() == 1)
			{
				if(CreateArenaType(i, group1, NULL) == -1) return;
				m_queuedGroups[i].remove(group1->GetID());
				continue;
			}

			teamids[0] = GetArenaGroupQInfo(group1, i, &avgRating[0]);

			list<uint32> possibleGroups;
			for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end(); ++itz)
			{
				group2 = objmgr.GetGroupById(*itz);
				if(group2)
				{
					teamids[1] = GetArenaGroupQInfo(group2, i, &avgRating[1]);
					uint32 delta = abs((int32)avgRating[0] - (int32)avgRating[1]);
					if(teamids[0] != teamids[1] && delta <= sWorld.ArenaQueueDiff)
					{
						possibleGroups.push_back(group2->GetID());
					}
				}
			}

			if(possibleGroups.size() > 0)
			{
				n = RandomUInt((uint32)possibleGroups.size()) - 1;
				for(itz = possibleGroups.begin(); itz != possibleGroups.end() && n > 0; ++itz)
					--n;

				if(itz == possibleGroups.end())
					itz = possibleGroups.begin();

				if(itz == possibleGroups.end())
				{
					sLog.Error("BattlegroundMgr", "Internal error at %s:%u", __FILE__, __LINE__);
					m_queueLock.Release();
					m_instanceLock.Release();
					return;
				}

				group2 = objmgr.GetGroupById(*itz);
				if(group2)
				{
					if(CreateArenaType(i, group1, group2) == -1) return;
					m_queuedGroups[i].remove(group1->GetID());
					m_queuedGroups[i].remove(group2->GetID());
				}
			}
		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();
}

void CBattlegroundManager::RemovePlayerFromQueues(Player* plr)
{
	m_queueLock.Acquire();

	ARCEMU_ASSERT(plr->m_bgQueueType < BATTLEGROUND_NUM_TYPES);

	sEventMgr.RemoveEvents(plr, EVENT_BATTLEGROUND_QUEUE_UPDATE);

	uint32 lgroup = GetLevelGrouping(plr->getLevel());
	list<uint32>::iterator itr;

	itr = m_queuedPlayers[plr->m_bgQueueType][lgroup].begin();
	while(itr != m_queuedPlayers[plr->m_bgQueueType][lgroup].end())
	{
		if((*itr) == plr->GetLowGUID())
		{
			Log.Debug("BattlegroundManager", "Removing player %u from queue instance %u type %u", plr->GetLowGUID(), plr->m_bgQueueInstanceId, plr->m_bgQueueType);
			m_queuedPlayers[plr->m_bgQueueType][lgroup].erase(itr);
			break;
		}

		++itr;
	}

	plr->m_bgIsQueued = false;
	plr->m_bgTeam = plr->GetTeam();
	plr->m_pendingBattleground = NULL;
	SendBattlefieldStatus(plr, BGSTATUS_NOFLAGS, 0, 0, 0, 0, 0);
	m_queueLock.Release();

	Group* group;
	group = plr->GetGroup();
	if(group)  //if da niggas in a group, boot dis bitch ass' group outa da q
	{
		Log.Debug("BattlegroundManager", "Player %u removed whilst in a group. Removing players group %u from queue", plr->GetLowGUID(), group->GetID());
		RemoveGroupFromQueues(group);
	}
}

void CBattlegroundManager::RemoveGroupFromQueues(Group* grp)
{
	m_queueLock.Acquire();
	for(uint32 i = BATTLEGROUND_ARENA_2V2; i < BATTLEGROUND_ARENA_5V5 + 1; ++i)
	{
		for(list<uint32>::iterator itr = m_queuedGroups[i].begin(); itr != m_queuedGroups[i].end();)
		{
			if((*itr) == grp->GetID())
				itr = m_queuedGroups[i].erase(itr);
			else
				++itr;
		}
	}

	for(GroupMembersSet::iterator itr = grp->GetSubGroup(0)->GetGroupMembersBegin(); itr != grp->GetSubGroup(0)->GetGroupMembersEnd(); ++itr)
		if((*itr)->m_loggedInPlayer)
			SendBattlefieldStatus((*itr)->m_loggedInPlayer, BGSTATUS_NOFLAGS, 0, 0, 0, 0, 0);

	m_queueLock.Release();
}


bool CBattlegroundManager::CanCreateInstance(uint32 Type, uint32 LevelGroup)
{
	/*uint32 lc = 0;
	for(map<uint32, CBattleground*>::iterator itr = m_instances[Type].begin(); itr != m_instances[Type].end(); ++itr)
	{
	if(itr->second->GetLevelGroup() == LevelGroup)
	{
	lc++;
	if(lc >= MAXIMUM_BATTLEGROUNDS_PER_LEVEL_GROUP)
	return false;
	}
	}*/

	return true;
}

/* Returns the minimum number of players (Only valid for battlegrounds) */
uint32 CBattlegroundManager::GetMinimumPlayers(uint32 dbcIndex)
{
	switch(dbcIndex)
	{
		case BATTLEGROUND_ALTERAC_VALLEY:
			return sWorld.bgsettings.AV_MIN;
		case BATTLEGROUND_WARSONG_GULCH:
			return sWorld.bgsettings.WSG_MIN;
		case BATTLEGROUND_ARATHI_BASIN:
			return sWorld.bgsettings.AB_MIN;
		case BATTLEGROUND_EYE_OF_THE_STORM:
			return sWorld.bgsettings.EOTS_MIN;
		case BATTLEGROUND_ARENA_2V2:
			return 2;
		case BATTLEGROUND_ARENA_3V3:
			return 3;
		case BATTLEGROUND_ARENA_5V5:
			return 5;
		case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
			return sWorld.bgsettings.SOTA_MIN;
		case BATTLEGROUND_ISLE_OF_CONQUEST:
			return sWorld.bgsettings.IOC_MIN;
		default:
			return 1;
	}
}

/* Returns the maximum number of players (Only valid for battlegrounds) */
uint32 CBattlegroundManager::GetMaximumPlayers(uint32 dbcIndex)
{
	switch(dbcIndex)
	{
		case BATTLEGROUND_ALTERAC_VALLEY:
			return sWorld.bgsettings.AV_MAX;
		case BATTLEGROUND_WARSONG_GULCH:
			return sWorld.bgsettings.WSG_MAX;
		case BATTLEGROUND_ARATHI_BASIN:
			return sWorld.bgsettings.AB_MAX;
		case BATTLEGROUND_EYE_OF_THE_STORM:
			return sWorld.bgsettings.EOTS_MAX;
		case BATTLEGROUND_ARENA_2V2:
			return 2;
		case BATTLEGROUND_ARENA_3V3:
			return 3;
		case BATTLEGROUND_ARENA_5V5:
			return 5;
		case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
			return sWorld.bgsettings.SOTA_MAX;
		case BATTLEGROUND_ISLE_OF_CONQUEST:
			return sWorld.bgsettings.IOC_MAX;
		default:
			return 1;
	}
}


CBattleground* CBattlegroundManager::CreateInstance(uint32 Type, uint32 LevelGroup)
{
	if( bgMaps.find( Type ) == bgMaps.end() ){
		if( !IS_ARENA( Type ) ){
			LOG_ERROR( "BattlegroundManager", "No map Id is registered for Battleground type %u", Type );
			return NULL;
		}
	}

	BattlegroundFactoryMethod cfunc = NULL;

	if( !IS_ARENA( Type ) )
		if( bgFactories.find( bgMaps[ Type ] ) != bgFactories.end() )
			cfunc = bgFactories[ bgMaps[ Type ] ];

	MapMgr* mgr = NULL;
	CBattleground* bg;
	bool isWeekend = false;
	struct tm tm;
	uint32 iid;
	time_t t;
	int n;

	if(IS_ARENA(Type))
	{
		/* arenas follow a different procedure. */
		uint32 arenaMapCount = arenaMaps.size();
		if( arenaMapCount == 0 ){
			LOG_ERROR( "BattlegroundManager", "There are no Arenas registered. Cannot create Arena." );
			return NULL;
		}

		uint32 index = RandomUInt( arenaMapCount - 1 );
		uint32 mapid = arenaMaps[ index ];
		ArenaFactoryMethod arenaFactory = arenaFactories[ index ];
		uint32 players_per_side;

		mgr = sInstanceMgr.CreateBattlegroundInstance(mapid);
		if(mgr == NULL)
		{
			sLog.Error("BattlegroundManager", "Arena CreateInstance() call failed for map %u, type %u, level group %u", mapid, Type, LevelGroup);
			return NULL;      // Shouldn't happen
		}

		switch(Type)
		{
			case BATTLEGROUND_ARENA_2V2:
				players_per_side = 2;
				break;

			case BATTLEGROUND_ARENA_3V3:
				players_per_side = 3;
				break;

			case BATTLEGROUND_ARENA_5V5:
				players_per_side = 5;
				break;
			default:
				players_per_side = 0;
				break;
		}

		iid = ++m_maxBattlegroundId[Type];
		bg = arenaFactory(mgr, iid, LevelGroup, Type, players_per_side);
		mgr->m_battleground = bg;
		Log.Notice("BattlegroundManager", "Created arena battleground type %u for level group %u on map %u.", Type, LevelGroup, mapid);
		sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 1, 1, 0);
		m_instanceLock.Acquire();
		m_instances[Type].insert(make_pair(iid, bg));
		m_instanceLock.Release();
		return bg;
	}

	if(cfunc == NULL)
	{
		sLog.Error("BattlegroundManager", "Could not find CreateBattlegroundFunc pointer for type %u level group %u", Type, LevelGroup);
		return NULL;
	}

	t = time(NULL);
#ifdef WIN32
//	localtime_s(&tm, &t);
	//zack : some luv for vs2k3 compiler
	tm = *localtime(&t);
#else
	localtime_r(&t, &tm);
#endif

	switch(Type)
	{
		case BATTLEGROUND_WARSONG_GULCH:
			n = 0;
			break;
		case BATTLEGROUND_ARATHI_BASIN:
			n = 1;
			break;
		case BATTLEGROUND_EYE_OF_THE_STORM:
			n = 2;
			break;
		case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
			n = 3;
			break;
		default:
			n = 0;
			break;
	}
	if(((tm.tm_yday / 7) % 4) == n)
	{
		/* Set weekend from Thursday night at midnight until Tuesday morning */
		isWeekend = tm.tm_wday >= 5 || tm.tm_wday < 2;
	}

	/* Create Map Manager */
	mgr = sInstanceMgr.CreateBattlegroundInstance(bgMaps[Type]);
	if(mgr == NULL)
	{
		sLog.Error("BattlegroundManager", "CreateInstance() call failed for map %u, type %u, level group %u", bgMaps[Type], Type, LevelGroup);
		return NULL;      // Shouldn't happen
	}

	/* Call the create function */
	iid = ++m_maxBattlegroundId[Type];
	bg = cfunc(mgr, iid, LevelGroup, Type);
	bg->SetIsWeekend(isWeekend);
	mgr->m_battleground = bg;
	sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 1, 1, 0);
	Log.Notice("BattlegroundManager", "Created battleground type %u for level group %u.", Type, LevelGroup);

	m_instanceLock.Acquire();
	m_instances[Type].insert(make_pair(iid, bg));
	m_instanceLock.Release();

	return bg;
}

void CBattlegroundManager::DeleteBattleground(CBattleground* bg)
{
	try
	{
		uint32 i = bg->GetType();
		uint32 j = bg->GetLevelGroup();
		Player* plr;

		m_instanceLock.Acquire();
		m_queueLock.Acquire();
		m_instances[i].erase(bg->GetId());

		/* erase any queued players */
		list<uint32>::iterator itr = m_queuedPlayers[i][j].begin();
		list<uint32>::iterator it2;
		for(; itr != m_queuedPlayers[i][j].end();)
		{
			it2 = itr++;
			plr = objmgr.GetPlayer(*it2);
			if(!plr)
			{
				m_queuedPlayers[i][j].erase(it2);
				continue;
			}

			if(plr && plr->m_bgQueueInstanceId == bg->GetId())
			{
				sChatHandler.SystemMessageToPlr(plr, plr->GetSession()->LocalizedWorldSrv(54), bg->GetId());
				SendBattlefieldStatus(plr, BGSTATUS_NOFLAGS, 0, 0, 0, 0, 0);
				plr->m_bgIsQueued = false;
				m_queuedPlayers[i][j].erase(it2);
			}
		}

		m_queueLock.Release();
		m_instanceLock.Release();

		//LOG_DETAIL("Deleting battleground from queue %u, instance %u", bg->GetType(), bg->GetId());
		delete bg;
	}
	catch(...)  // for Win32 Debug
	{
		LOG_ERROR("Exception: CBattlegroundManager::DeleteBattleground");
		printStackTrace();
		throw;
	}

}



void CBattlegroundManager::SendBattlefieldStatus(Player* plr, BattleGroundStatus Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch)
{
	WorldPacket data(SMSG_BATTLEFIELD_STATUS, 30);
	if(Status == 0)
		data << uint32(0) << uint64(0);
	else
	{
		if(IS_ARENA(Type))
		{
			data << uint32(0);		// Queue Slot 0..2. Only the first slot is used in arcemu!
			switch(Type)
			{
				case BATTLEGROUND_ARENA_2V2:
					data << uint8(2);
					break;

				case BATTLEGROUND_ARENA_3V3:
					data << uint8(3);
					break;

				case BATTLEGROUND_ARENA_5V5:
					data << uint8(5);
					break;
			}
			data << uint8(0xC);
			data << uint32(6);
			data << uint16(0x1F90);
			data << uint8(0);                // 3.3.0
			data << uint8(0);                // 3.3.0
			data << uint32(11);
			data << uint8(RatedMatch);      // 1 = rated match
		}
		else
		{
			data << uint32(0);
			data << uint8(0) << uint8(2);
			data << Type;
			data << uint8(0);                // 3.3.0
			data << uint8(0);                // 3.3.0
			data << uint16(0x1F90);
			data << InstanceID;
			data << uint8(0);
		}

		data << uint32(Status);

		switch(Status)
		{
			case BGSTATUS_INQUEUE:					// Waiting in queue
				data << uint32(60) << uint32(0);	// Time / Elapsed time
				break;
			case BGSTATUS_READY:					// Ready to join!
				data << MapId;
				data << uint64(0);
				data << Time;
				break;
			case BGSTATUS_TIME:
				data << MapId;
				data << uint64(0);
				data << uint32(0);
				data << Time;
				if(IS_ARENA(Type))
					data << uint8(0);
				else
					data << uint8(1);
				break;
			case BGSTATUS_NOFLAGS:
				break;
		}
	}

	plr->GetSession()->SendPacket(&data);
}

void CBattlegroundManager::HandleArenaJoin(WorldSession* m_session, uint32 BattlegroundType, uint8 as_group, uint8 rated_match)
{
	uint32 pguid = m_session->GetPlayer()->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(m_session->GetPlayer()->getLevel());
	if(as_group && m_session->GetPlayer()->GetGroup() == NULL)
		return;

	Group* pGroup = m_session->GetPlayer()->GetGroup();
	if(as_group)
	{
		if(pGroup->GetSubGroupCount() != 1)
		{
			m_session->SystemMessage(m_session->LocalizedWorldSrv(55));
			return;
		}
		if(pGroup->GetLeader() != m_session->GetPlayer()->getPlayerInfo())
		{
			m_session->SystemMessage(m_session->LocalizedWorldSrv(56));
			return;
		}

		GroupMembersSet::iterator itx;
		if(!rated_match)
		{
			/* add all players normally.. bleh ;P */
			pGroup->Lock();
			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if((*itx)->m_loggedInPlayer && !(*itx)->m_loggedInPlayer->m_bgIsQueued && !(*itx)->m_loggedInPlayer->m_bg)
					HandleArenaJoin((*itx)->m_loggedInPlayer->GetSession(), BattlegroundType, 0, 0);
			}
			pGroup->Unlock();
			return;
		}
		else
		{
			/* make sure all players are 70 */
			uint32 maxplayers;
			uint32 type = BattlegroundType - BATTLEGROUND_ARENA_2V2;
			switch(BattlegroundType)
			{
				case BATTLEGROUND_ARENA_3V3:
					maxplayers = 3;
					break;

				case BATTLEGROUND_ARENA_5V5:
					maxplayers = 5;
					break;

				case BATTLEGROUND_ARENA_2V2:
				default:
					maxplayers = 2;
					break;
			}

			if(pGroup->GetLeader()->m_loggedInPlayer && pGroup->GetLeader()->m_loggedInPlayer->m_arenaTeams[type] == NULL)
			{
				m_session->SendNotInArenaTeamPacket(uint8(maxplayers));
				return;
			}

			pGroup->Lock();
			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if(maxplayers == 0)
				{
					m_session->SystemMessage(m_session->LocalizedWorldSrv(58));
					pGroup->Unlock();
					return;
				}

				if((*itx)->lastLevel < PLAYER_ARENA_MIN_LEVEL)
				{
					m_session->SystemMessage(m_session->LocalizedWorldSrv(59));
					pGroup->Unlock();
					return;
				}

				if((*itx)->m_loggedInPlayer)
				{
					if((*itx)->m_loggedInPlayer->m_bg || (*itx)->m_loggedInPlayer->m_bgIsQueued)
					{
						m_session->SystemMessage(m_session->LocalizedWorldSrv(60));
						pGroup->Unlock();
						return;
					};
					if((*itx)->m_loggedInPlayer->m_arenaTeams[type] != pGroup->GetLeader()->m_loggedInPlayer->m_arenaTeams[type])
					{
						m_session->SystemMessage(m_session->LocalizedWorldSrv(61));
						pGroup->Unlock();
						return;
					}

					--maxplayers;
				}
			}
			WorldPacket data(SMSG_GROUP_JOINED_BATTLEGROUND, 4);
			data << uint32(6);      // all arenas

			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if((*itx)->m_loggedInPlayer)
				{
					SendBattlefieldStatus((*itx)->m_loggedInPlayer, BGSTATUS_INQUEUE, BattlegroundType, 0 , 0, 0, 1);
					(*itx)->m_loggedInPlayer->m_bgIsQueued = true;
					(*itx)->m_loggedInPlayer->m_bgQueueInstanceId = 0;
					(*itx)->m_loggedInPlayer->m_bgQueueType = BattlegroundType;
					(*itx)->m_loggedInPlayer->GetSession()->SendPacket(&data);
					(*itx)->m_loggedInPlayer->m_bgEntryPointX = (*itx)->m_loggedInPlayer->GetPositionX();
					(*itx)->m_loggedInPlayer->m_bgEntryPointY = (*itx)->m_loggedInPlayer->GetPositionY();
					(*itx)->m_loggedInPlayer->m_bgEntryPointZ = (*itx)->m_loggedInPlayer->GetPositionZ();
					(*itx)->m_loggedInPlayer->m_bgEntryPointMap = (*itx)->m_loggedInPlayer->GetMapId();
				}
			}

			pGroup->Unlock();

			m_queueLock.Acquire();
			m_queuedGroups[BattlegroundType].push_back(pGroup->GetID());
			m_queueLock.Release();
			Log.Notice("BattlegroundMgr", "Group %u is now in battleground queue for arena type %u", pGroup->GetID(), BattlegroundType);

			/* send the battleground status packet */

			return;
		}
	}


	/* Queue him! */
	m_queueLock.Acquire();
	m_queuedPlayers[BattlegroundType][lgroup].push_back(pguid);
	Log.Notice("BattlegroundMgr", "Player %u is now in battleground queue for {Arena %u}", m_session->GetPlayer()->GetLowGUID(), BattlegroundType);

	/* send the battleground status packet */
	SendBattlefieldStatus(m_session->GetPlayer(), BGSTATUS_INQUEUE, BattlegroundType, 0 , 0, 0, 0);
	m_session->GetPlayer()->m_bgIsQueued = true;
	m_session->GetPlayer()->m_bgQueueInstanceId = 0;
	m_session->GetPlayer()->m_bgQueueType = BattlegroundType;

	/* Set battleground entry point */
	m_session->GetPlayer()->m_bgEntryPointX = m_session->GetPlayer()->GetPositionX();
	m_session->GetPlayer()->m_bgEntryPointY = m_session->GetPlayer()->GetPositionY();
	m_session->GetPlayer()->m_bgEntryPointZ = m_session->GetPlayer()->GetPositionZ();
	m_session->GetPlayer()->m_bgEntryPointMap = m_session->GetPlayer()->GetMapId();
	m_session->GetPlayer()->m_bgEntryPointInstance = m_session->GetPlayer()->GetInstanceID();

	m_queueLock.Release();
}


