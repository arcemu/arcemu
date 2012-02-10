/*
* ArcEmu MMORPG Server
* Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
* Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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
typedef CBattleground* (*CreateBattlegroundFunc)(MapMgr* mgr, uint32 iid, uint32 group, uint32 type);

const static uint32 BGMapIds[ BATTLEGROUND_NUM_TYPES ] =
{
	0,	// 0
	30,	// AV
	489,	// WSG
	529,	// AB
	0,	// 2v2
	0,	// 3v3
	0,	// 5v5
	566,	// EOTS
	0,
	607,	// SOTA
	0,
	0,//11
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	628,
	0,
	0
};

const static CreateBattlegroundFunc BGCFuncs[BATTLEGROUND_NUM_TYPES] =
{
	NULL,				// 0
	&AlteracValley::Create,		// AV
	&WarsongGulch::Create,		// WSG
	&ArathiBasin::Create,		// AB
	NULL,				// 2v2
	NULL,				// 3v3
	NULL,				// 5v5
	&EyeOfTheStorm::Create,		// EotS
	NULL,
	&StrandOfTheAncient::Create,	// SOTA
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&IsleOfConquest::Create, //IOC
	NULL,
	NULL
};

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


	if(bgtype >= BATTLEGROUND_NUM_TYPES || !bgtype)
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
	SendBattlefieldStatus(m_session->GetPlayer(), BGSTATUS_INQUEUE, bgtype, instance, 0, BGMapIds[bgtype], 0);
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


CBattleground::CBattleground(MapMgr* mgr, uint32 id, uint32 levelgroup, uint32 type) : m_mapMgr(mgr), m_id(id), m_type(type), m_levelGroup(levelgroup)
{
	m_nextPvPUpdateTime = 0;
	m_countdownStage = 0;
	m_ended = false;
	m_started = false;
	m_winningteam = 0;
	m_startTime = (uint32)UNIXTIME;
	m_lastResurrect = (uint32)UNIXTIME;
	m_invisGMs = 0;
	m_zoneid = 0;
	sEventMgr.AddEvent(this, &CBattleground::EventResurrectPlayers, EVENT_BATTLEGROUND_QUEUE_UPDATE, 30000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	/* create raid groups */
	for(uint32 i = 0; i < 2; ++i)
	{
		m_groups[i] = new Group(true);
		m_groups[i]->m_disbandOnNoMembers = false;
		m_groups[i]->ExpandToRaid();
	}
	m_honorPerKill = HonorHandler::CalculateHonorPointsForKill(m_levelGroup * 10, m_levelGroup * 10);
}

CBattleground::~CBattleground()
{
	sEventMgr.RemoveEvents(this);
	for(uint32 i = 0; i < 2; ++i)
	{
		PlayerInfo* inf;
		for(uint32 j = 0; j < m_groups[i]->GetSubGroupCount(); ++j)
		{
			for(GroupMembersSet::iterator itr = m_groups[i]->GetSubGroup(j)->GetGroupMembersBegin(); itr != m_groups[i]->GetSubGroup(j)->GetGroupMembersEnd();)
			{
				inf = (*itr);
				++itr;
				m_groups[i]->RemovePlayer(inf);
			}
		}
		delete m_groups[i];
	}

	m_resurrectMap.clear();
	m_players[0].clear();
	m_players[1].clear();
}

void CBattleground::UpdatePvPData()
{
	if(IS_ARENA(m_type))
	{
		if(!m_ended)
		{
			return;
		}
	}

	if(UNIXTIME >= m_nextPvPUpdateTime)
	{
		m_mainLock.Acquire();
		WorldPacket data(10 * (m_players[0].size() + m_players[1].size()) + 50);
		BuildPvPUpdateDataPacket(&data);
		DistributePacketToAll(&data);
		m_mainLock.Release();

		m_nextPvPUpdateTime = UNIXTIME + 2;
	}
}

void CBattleground::BuildPvPUpdateDataPacket(WorldPacket* data)
{
	ARCEMU_ASSERT(data != NULL);

	data->Initialize(MSG_PVP_LOG_DATA);
	data->reserve(10 * (m_players[0].size() + m_players[1].size()) + 50);

	BGScore* bs;
	if(IS_ARENA(m_type))
	{
		if(!m_ended)
		{
			return;
		}

		*data << uint8(1);
		//In 3.1 this should be the uint32(negative rating), uint32(positive rating), uint32(0)[<-this is the new field in 3.1], and a name if available / which is a null-terminated string, and we send an uint8(0), so we provide a zero length name string /
		if(!Rated())
		{
			*data << uint32(0) << uint32(0) << uint32(0) << uint8(0);
			*data << uint32(0) << uint32(0) << uint32(0) << uint8(0);
		}
		else
		{
			/* Grab some arena teams */
			ArenaTeam** teams = TO< Arena* >(this)->GetTeams();

			if(teams[0])
			{
				*data << uint32(0) << uint32(3000 + m_deltaRating[0]) << uint32(0) << uint8(0);
			}
			else
			{
				*data << uint32(0) << uint32(0) << uint32(0) << uint8(0);
			}

			if(teams[1])
			{
				*data << uint32(0) << uint32(3000 + m_deltaRating[1]) << uint32(0) << uint8(0);
			}
			else
			{
				*data << uint32(0) << uint32(0) << uint32(0) << uint8(0);
			}
		}

		*data << uint8(1);
		*data << uint8(m_winningteam);

		*data << uint32((m_players[0].size() + m_players[1].size()) - m_invisGMs);
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				if((*itr)->m_isGmInvisible)continue;
				*data << (*itr)->GetGUID();
				bs = &(*itr)->m_bgScore;
				*data << bs->KillingBlows;

				*data << uint8((*itr)->m_bgTeam);

				*data << bs->DamageDone;
				*data << bs->HealingDone;
				*data << uint32(0);
			}
		}
	}
	else
	{
		*data << uint8(0);
		if(m_ended)
		{
			*data << uint8(1);
			*data << uint8(m_winningteam ? 0 : 1);
		}
		else
			*data << uint8(0);      // If the game has ended - this will be 1

		*data << uint32((m_players[0].size() + m_players[1].size()) - m_invisGMs);

		uint32 FieldCount = GetFieldCount(GetType());
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				ARCEMU_ASSERT(*itr != NULL);
				if((*itr)->m_isGmInvisible)
					continue;
				*data << (*itr)->GetGUID();
				bs = &(*itr)->m_bgScore;

				*data << bs->KillingBlows;
				*data << bs->HonorableKills;
				*data << bs->Deaths;
				*data << bs->BonusHonor;
				*data << bs->DamageDone;
				*data << bs->HealingDone;

				*data << FieldCount;
				for(uint32 x = 0; x < FieldCount; ++x)
					*data << bs->MiscData[x];
			}
		}
	}

}
void CBattleground::AddPlayer(Player* plr, uint32 team)
{
	m_mainLock.Acquire();

	/* This is called when the player is added, not when they port. So, they're essentially still queued, but not inside the bg yet */
	m_pendPlayers[team].insert(plr->GetLowGUID());

	/* Send a packet telling them that they can enter */
	plr->m_pendingBattleground = this;
	BattlegroundManager.SendBattlefieldStatus(plr, BGSTATUS_READY, m_type, m_id, 80000, m_mapMgr->GetMapId(), Rated());        // You will be removed from the queue in 2 minutes.

	/* Add an event to remove them in 1 minute 20 seconds time. */
	sEventMgr.AddEvent(plr, &Player::RemoveFromBattlegroundQueue, EVENT_BATTLEGROUND_QUEUE_UPDATE, 80000, 1, 0);

	m_mainLock.Release();
}

void CBattleground::RemovePendingPlayer(Player* plr)
{
	m_mainLock.Acquire();

	m_pendPlayers[plr->m_bgTeam].erase(plr->GetLowGUID());

	/* send a null bg update (so they don't join) */
	BattlegroundManager.SendBattlefieldStatus(plr, BGSTATUS_NOFLAGS, 0, 0, 0, 0, 0);
	plr->m_pendingBattleground = NULL;
	plr->m_bgTeam = plr->GetTeam();

	m_mainLock.Release();
}

void CBattleground::OnPlayerPushed(Player* plr)
{
	if(plr->GetGroup() && !Rated())
		plr->GetGroup()->RemovePlayer(plr->getPlayerInfo());

	plr->ProcessPendingUpdates();

	if(plr->GetGroup() == NULL)
	{
		if(plr->m_isGmInvisible == false)    //do not join invisible gm's into bg groups.
			m_groups[plr->m_bgTeam]->AddMember(plr->getPlayerInfo());
	}
}

void CBattleground::PortPlayer(Player* plr, bool skip_teleport /* = false*/)
{
	m_mainLock.Acquire();
	if(m_ended)
	{
		sChatHandler.SystemMessage(plr->GetSession(), plr->GetSession()->LocalizedWorldSrv(53));
		BattlegroundManager.SendBattlefieldStatus(plr, BGSTATUS_NOFLAGS, 0, 0, 0, 0, 0);
		plr->m_pendingBattleground = NULL;
		m_mainLock.Release();
		return;
	}

	m_pendPlayers[plr->m_bgTeam].erase(plr->GetLowGUID());
	if(m_players[plr->m_bgTeam].find(plr) != m_players[plr->m_bgTeam].end())
	{
		m_mainLock.Release();
		return;
	}

	plr->FullHPMP();
	plr->SetTeam(plr->m_bgTeam);
	if(plr->m_isGmInvisible == false)
	{
		//Do not let everyone know an invisible gm has joined.
		WorldPacket data(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
		data << plr->GetGUID();
		DistributePacketToTeam(&data, plr->m_bgTeam);
	}
	else
	{
		m_invisGMs++;
	}
	m_players[plr->m_bgTeam].insert(plr);

	/* remove from any auto queue remove events */
	sEventMgr.RemoveEvents(plr, EVENT_BATTLEGROUND_QUEUE_UPDATE);

	if(!skip_teleport)
	{
		if(plr->IsInWorld())
			plr->RemoveFromWorld();
	}

	plr->m_pendingBattleground = NULL;
	plr->m_bg = this;

	if(!plr->IsPvPFlagged())
		plr->SetPvPFlag();

	plr->RemoveAurasByInterruptFlag(AURA_INTERRUPT_ON_PVP_ENTER);

	/* Reset the score */
	memset(&plr->m_bgScore, 0, sizeof(BGScore));

	/* update pvp data */
	UpdatePvPData();

	/* add the player to the group */
	if(plr->GetGroup() && !Rated())
	{
		// remove them from their group
		plr->GetGroup()->RemovePlayer(plr->getPlayerInfo());
	}

	if(!m_countdownStage)
	{
		m_countdownStage = 1;
		sEventMgr.AddEvent(this, &CBattleground::EventCountdown, EVENT_BATTLEGROUND_COUNTDOWN, 30000, 0, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 10000);
	}

	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);

	if(!skip_teleport)
	{
		/* This is where we actually teleport the player to the battleground. */
		plr->SafeTeleport(m_mapMgr, GetStartingCoords(plr->m_bgTeam));
		BattlegroundManager.SendBattlefieldStatus(plr, BGSTATUS_TIME, m_type, m_id, (uint32)UNIXTIME - m_startTime, m_mapMgr->GetMapId(), Rated());     // Elapsed time is the last argument
	}
	else
	{
		/* If we are not ported, call this immediatelly, otherwise its called after teleportation in Player::OnPushToWorld */
		OnAddPlayer(plr);
	}

	m_mainLock.Release();
}

CBattleground* CBattlegroundManager::CreateInstance(uint32 Type, uint32 LevelGroup)
{
	CreateBattlegroundFunc cfunc = BGCFuncs[Type];
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
		static const uint32 arena_map_ids[3] = { 559, 562, 572 };
		uint32 mapid = arena_map_ids[RandomUInt(2)];
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
		bg = new Arena(mgr, iid, LevelGroup, Type, players_per_side);
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
	mgr = sInstanceMgr.CreateBattlegroundInstance(BGMapIds[Type]);
	if(mgr == NULL)
	{
		sLog.Error("BattlegroundManager", "CreateInstance() call failed for map %u, type %u, level group %u", BGMapIds[Type], Type, LevelGroup);
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

GameObject* CBattleground::SpawnGameObject(uint32 entry, uint32 MapId , float x, float y, float z, float o, uint32 flags, uint32 faction, float scale)
{
	GameObject* go = m_mapMgr->CreateGameObject(entry);

	Arcemu::Util::ArcemuAssert( go != NULL );

	go->CreateFromProto(entry, MapId, x, y, z, o);

	go->SetFaction(faction);
	go->SetScale(scale);
	go->SetUInt32Value(GAMEOBJECT_FLAGS, flags);
	go->SetPosition(x, y, z, o);
	go->SetInstanceID(m_mapMgr->GetInstanceID());

	return go;
}

GameObject* CBattleground::SpawnGameObject( uint32 entry, LocationVector &v, uint32 flags, uint32 faction, float scale ){
	return SpawnGameObject( entry, m_mapMgr->GetMapId(), v.x, v.y, v.z, v.o, flags, faction, scale );
}

Creature* CBattleground::SpawnCreature(uint32 entry, float x, float y, float z, float o, uint32 faction )
{
	CreatureProto* cp = CreatureProtoStorage.LookupEntry(entry);
	Creature* c = m_mapMgr->CreateCreature(entry);

	Arcemu::Util::ArcemuAssert( c != NULL );

	c->Load(cp, x, y, z, o);
	
	if( faction != 0 )
		c->SetFaction( faction );

	c->PushToWorld(m_mapMgr);
	return c;
}

Creature* CBattleground::SpawnCreature( uint32 entry, LocationVector &v, uint32 faction ){
	return SpawnCreature( entry, v.x, v.y, v.z, v.o, faction );
}

void CBattleground::AddHonorToTeam( uint32 team, uint32 amount ){
	m_mainLock.Acquire();
	for( std::set< Player* >::iterator itr = m_players[ team ].begin(); itr != m_players[ team ].end(); ++itr ){
		Player *p = *itr;
		HonorHandler::AddHonorPointsToPlayer( p, amount );
	}
	m_mainLock.Release();
}

void CBattleground::CastSpellOnTeam( uint32 team, uint32 spell ){
	m_mainLock.Acquire();
	for( std::set< Player* >::iterator itr = m_players[ team ].begin(); itr != m_players[ team ].end(); ++itr ){
		Player *p = *itr;
		p->CastSpell( p, spell, false );
	}
	m_mainLock.Release();
}

void CBattleground::RemoveAuraFromTeam( uint32 team, uint32 aura ){
	m_mainLock.Acquire();
	for( std::set< Player* >::iterator itr = m_players[ team ].begin(); itr != m_players[ team ].end(); ++itr ){
		Player *p = *itr;
		p->RemoveAura( aura );
	}
	m_mainLock.Release();
}

void CBattleground::SendChatMessage(uint32 Type, uint64 Guid, const char* Format, ...)
{
	char msg[500];
	va_list ap;
	va_start(ap, Format);
	vsnprintf(msg, 500, Format, ap);
	va_end(ap);
	WorldPacket* data = sChatHandler.FillMessageData(Type, 0, msg, Guid, 0);
	DistributePacketToAll(data);
	delete data;
}

void CBattleground::DistributePacketToAll(WorldPacket* packet)
{
	m_mainLock.Acquire();
	for(int i = 0; i < 2; ++i)
	{
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			if((*itr) && (*itr)->GetSession())
				(*itr)->GetSession()->SendPacket(packet);
	}
	m_mainLock.Release();
}

void CBattleground::DistributePacketToTeam(WorldPacket* packet, uint32 Team)
{
	m_mainLock.Acquire();
	for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		if((*itr) && (*itr)->GetSession())
			(*itr)->GetSession()->SendPacket(packet);
	m_mainLock.Release();
}

void CBattleground::PlaySoundToAll(uint32 Sound)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << Sound;
	DistributePacketToAll(&data);
}

void CBattleground::PlaySoundToTeam(uint32 Team, uint32 Sound)
{
	WorldPacket data(SMSG_PLAY_SOUND, 4);
	data << Sound;
	DistributePacketToTeam(&data, Team);
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

void CBattleground::RemovePlayer(Player* plr, bool logout)
{
	m_mainLock.Acquire();

	WorldPacket data(SMSG_BATTLEGROUND_PLAYER_LEFT, 30);
	data << plr->GetGUID();
	if(plr->m_isGmInvisible == false)
	{
		//Don't show invisible gm's leaving the game.
		DistributePacketToAll(&data);
	}
	else
	{
		RemoveInvisGM();
	}

	// Call subclassed virtual method
	OnRemovePlayer(plr);

	// Clean-up
	plr->m_bg = NULL;
	plr->FullHPMP();
	m_players[plr->m_bgTeam].erase(plr);
	memset(&plr->m_bgScore, 0, sizeof(BGScore));

	/* are we in the group? */
	if(plr->GetGroup() == m_groups[plr->m_bgTeam])
		plr->GetGroup()->RemovePlayer(plr->getPlayerInfo());

	// reset team
	plr->ResetTeam();

	/* revive the player if he is dead */
	if(!plr->isAlive())
	{
		plr->SetHealth(plr->GetMaxHealth());
		plr->ResurrectPlayer();
	}

	/* remove buffs */
	plr->RemoveAura(32727); // Arena preparation
	plr->RemoveAura(44521); // BG preparation
	plr->RemoveAura(44535);
	plr->RemoveAura(21074);

	plr->Unroot();

	/* teleport out */
	if(!logout)
	{
		if(!m_ended && !plr->GetSession()->HasGMPermissions())
			plr->CastSpell(plr, BG_DESERTER, true);

		if(!IS_INSTANCE(plr->m_bgEntryPointMap))
		{
			LocationVector vec(plr->m_bgEntryPointX, plr->m_bgEntryPointY, plr->m_bgEntryPointZ, plr->m_bgEntryPointO);
			plr->SafeTeleport(plr->m_bgEntryPointMap, plr->m_bgEntryPointInstance, vec);
		}
		else
		{
			LocationVector vec(plr->GetBindPositionX(), plr->GetBindPositionY(), plr->GetBindPositionZ());
			plr->SafeTeleport(plr->GetBindMapId(), 0, vec);
		}

		BattlegroundManager.SendBattlefieldStatus(plr, BGSTATUS_NOFLAGS, 0, 0, 0, 0, 0);
	}

	if(/*!m_ended && */m_players[0].size() == 0 && m_players[1].size() == 0)
	{
		/* create an inactive event */
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);                  // 10mins
		//sEventMgr.AddEvent(this, &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 600000, 1,0); //this is BS..appears to be		the cause if the battleground crashes.
		this->Close();
	}

	plr->m_bgTeam = plr->GetTeam();

	m_mainLock.Release();
}

void CBattleground::SendPVPData(Player* plr)
{
	m_mainLock.Acquire();

	WorldPacket data(10 * (m_players[0].size() + m_players[1].size()) + 50);
	BuildPvPUpdateDataPacket(&data);
	plr->GetSession()->SendPacket(&data);

	m_mainLock.Release();
}

void CBattleground::EventCreate()
{
	OnCreate();
}

int32 CBattleground::event_GetInstanceID()
{
	return m_mapMgr->GetInstanceID();
}

void CBattleground::EventCountdown()
{
	if(m_countdownStage == 1)
	{
		m_countdownStage = 2;

		m_mainLock.Acquire();
		for(int i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				if((*itr) && (*itr)->GetSession())
				{
					(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(46), (*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
				}
		}
		m_mainLock.Release();

		// SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "One minute until the battle for %s begins!", GetName() );
	}
	else if(m_countdownStage == 2)
	{
		m_countdownStage = 3;

		m_mainLock.Acquire();
		for(int i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				if((*itr) && (*itr)->GetSession())
				{
					(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(47), (*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
				}
		}
		m_mainLock.Release();

		//SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Thirty seconds until the battle for %s begins!", GetName() );
	}
	else if(m_countdownStage == 3)
	{
		m_countdownStage = 4;

		m_mainLock.Acquire();
		for(int i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				if((*itr) && (*itr)->GetSession())
				{
					(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(48), (*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
				}
		}
		m_mainLock.Release();

		//SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Fifteen seconds until the battle for %s begins!", GetName() );
		sEventMgr.ModifyEventTime(this, EVENT_BATTLEGROUND_COUNTDOWN, 150);
		sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 15000);
	}
	else
	{
		m_mainLock.Acquire();
		for(int i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
				if((*itr) && (*itr)->GetSession())
				{
					(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(49), (*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
				}
		}
		m_mainLock.Release();
		//SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The battle for %s has begun!", GetName() );
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_COUNTDOWN);
		Start();
	}
}

void CBattleground::Start()
{
	OnStart();
}

void CBattleground::SetWorldState(uint32 Index, uint32 Value)
{
	if( m_zoneid == 0 )
		return;

	m_mapMgr->GetWorldStatesHandler().SetWorldStateForZone( m_zoneid, 0, Index, Value );
}

void CBattleground::Close()
{
	/* remove all players from the battleground */
	m_mainLock.Acquire();
	m_ended = true;
	for(uint32 i = 0; i < 2; ++i)
	{
		set<Player*>::iterator itr;
		set<uint32>::iterator it2;
		uint32 guid;
		Player* plr;
		for(itr = m_players[i].begin(); itr != m_players[i].end();)
		{
			plr = *itr;
			++itr;
			RemovePlayer(plr, false);
		}

		for(it2 = m_pendPlayers[i].begin(); it2 != m_pendPlayers[i].end();)
		{
			guid = *it2;
			++it2;
			plr = objmgr.GetPlayer(guid);

			if(plr)
				RemovePendingPlayer(plr);
			else
				m_pendPlayers[i].erase(guid);
		}
	}

	/* call the virtual on close for cleanup etc */
	OnClose();

	/* shut down the map thread. this will delete the battleground from the current context. */
	m_mapMgr->SetThreadState(THREADSTATE_TERMINATE);

	m_mainLock.Release();
}

Creature* CBattleground::SpawnSpiritGuide(float x, float y, float z, float o, uint32 horde)
{
	if(horde > 1)
		horde = 1;

	CreatureInfo* pInfo = CreatureNameStorage.LookupEntry(13116 + horde);
	if(pInfo == NULL)
	{
		return NULL;
	}

	Creature* pCreature = m_mapMgr->CreateCreature(pInfo->Id);

	pCreature->Create(pInfo->Name, m_mapMgr->GetMapId(), x, y, z, o);

	pCreature->SetEntry(13116 + horde);
	pCreature->SetScale(1.0f);

	pCreature->SetMaxHealth(10000);
	pCreature->SetMaxPower(POWER_TYPE_MANA, 4868);
	pCreature->SetMaxPower(POWER_TYPE_FOCUS, 200);
	pCreature->SetMaxPower(POWER_TYPE_HAPPINESS, 2000000);

	pCreature->SetHealth(100000);
	pCreature->SetPower(POWER_TYPE_MANA, 4868);
	pCreature->SetPower(POWER_TYPE_FOCUS, 200);
	pCreature->SetPower(POWER_TYPE_HAPPINESS, 2000000);

	pCreature->setLevel(60);
	pCreature->SetFaction(84 - horde);

	pCreature->setRace(0);
	pCreature->setClass(2);
	pCreature->setGender(1);
	pCreature->SetPowerType(0);

	pCreature->SetEquippedItem(MELEE, 22802);

	pCreature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLUS_MOB | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_UNKNOWN_10 | UNIT_FLAG_PVP); // 4928

	pCreature->SetBaseAttackTime(MELEE, 2000);
	pCreature->SetBaseAttackTime(OFFHAND, 2000);
	pCreature->SetBoundingRadius(0.208f);
	pCreature->SetCombatReach(1.5f);

	pCreature->SetDisplayId(13337 + horde);
	pCreature->SetNativeDisplayId(13337 + horde);

	pCreature->SetChannelSpellId(22011);
	pCreature->SetCastSpeedMod(1.0f);

	pCreature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITGUIDE);
	pCreature->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x10 << 8));

	pCreature->DisableAI();

	pCreature->SetCreatureInfo(pInfo);
	pCreature->SetCreatureProto(CreatureProtoStorage.LookupEntry(pInfo->Id));

	pCreature->PushToWorld(m_mapMgr);
	return pCreature;
}

Creature* CBattleground::SpawnSpiritGuide( LocationVector &v, uint32 faction ){
	return SpawnSpiritGuide( v.x, v.y, v.z, v.o, faction );
}

void CBattleground::QueuePlayerForResurrect(Player* plr, Creature* spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
		itr->second.insert(plr->GetLowGUID());
	plr->m_areaSpiritHealer_guid = spirit_healer->GetGUID();
	m_mainLock.Release();
}

void CBattleground::RemovePlayerFromResurrect(Player* plr, Creature* spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
		itr->second.erase(plr->GetLowGUID());
	plr->m_areaSpiritHealer_guid = 0;
	m_mainLock.Release();
}

void CBattleground::AddSpiritGuide(Creature* pCreature)
{
	m_mainLock.Acquire();
	map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.find(pCreature);
	if(itr == m_resurrectMap.end())
	{
		set<uint32> ti;
		m_resurrectMap.insert(make_pair(pCreature, ti));
	}
	m_mainLock.Release();
}

void CBattleground::RemoveSpiritGuide(Creature* pCreature)
{
	m_mainLock.Acquire();
	m_resurrectMap.erase(pCreature);
	m_mainLock.Release();
}

void CBattleground::EventResurrectPlayers()
{
	m_mainLock.Acquire();
	Player* plr;
	set<uint32>::iterator itr;
	map<Creature*, set<uint32> >::iterator i;
	WorldPacket data(50);
	for(i = m_resurrectMap.begin(); i != m_resurrectMap.end(); ++i)
	{
		for(itr = i->second.begin(); itr != i->second.end(); ++itr)
		{
			plr = m_mapMgr->GetPlayer(*itr);
			if(plr && plr->IsDead())
			{
				data.Initialize(SMSG_SPELL_START);
				data << plr->GetNewGUID() << plr->GetNewGUID() << uint32(RESURRECT_SPELL) << uint8(0) << uint16(0) << uint32(0) << uint16(2) << plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				data.Initialize(SMSG_SPELL_GO);
				data << plr->GetNewGUID() << plr->GetNewGUID() << uint32(RESURRECT_SPELL) << uint8(0) << uint8(1) << uint8(1) << plr->GetGUID() << uint8(0) << uint16(2)
				     << plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				plr->ResurrectPlayer();
				plr->SetHealth(plr->GetMaxHealth());
				plr->SetPower(POWER_TYPE_MANA, plr->GetMaxPower(POWER_TYPE_MANA));
				plr->SetPower(POWER_TYPE_ENERGY, plr->GetMaxPower(POWER_TYPE_ENERGY));
				plr->CastSpell(plr, BG_REVIVE_PREPARATION, true);
			}
		}
		i->second.clear();
	}
	m_lastResurrect = (uint32)UNIXTIME;
	m_mainLock.Release();
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

bool CBattleground::CanPlayerJoin(Player* plr, uint32 type)
{
	return HasFreeSlots(plr->m_bgTeam, type) && (GetLevelGrouping(plr->getLevel()) == GetLevelGroup()) && (!plr->HasAura(BG_DESERTER));
}

void CBattleground::QueueAtNearestSpiritGuide(Player* plr, Creature* old)
{
	float dd;
	float dist = 999999.0f;
	Creature* cl = NULL;
	set<uint32> *closest = NULL;
	m_lock.Acquire();
	map<Creature*, set<uint32> >::iterator itr = m_resurrectMap.begin();
	for(; itr != m_resurrectMap.end(); ++itr)
	{
		if(itr->first == old)
			continue;

		dd = plr->GetDistance2dSq(itr->first) < dist;
		if(dd < dist)
		{
			cl = itr->first;
			closest = &itr->second;
			dist = dd;
		}
	}

	if(closest != NULL)
	{
		closest->insert(plr->GetLowGUID());
		plr->m_areaSpiritHealer_guid = cl->GetGUID();
		plr->CastSpell(plr, 2584, true);
	}

	m_lock.Release();
}

uint32 CBattleground::GetFreeSlots(uint32 t, uint32 type)
{
	uint32 maxPlayers = BattlegroundManager.GetMaximumPlayers(type);

	m_mainLock.Acquire();
	size_t s = maxPlayers - m_players[t].size() - m_pendPlayers[t].size();
	m_mainLock.Release();
	return (uint32)s;
}

bool CBattleground::HasFreeSlots(uint32 Team, uint32 type)
{
	bool res;
	uint32 maxPlayers = BattlegroundManager.GetMaximumPlayers(type);

	m_mainLock.Acquire();
	if(IS_ARENA(type))
	{
		res = ((uint32)m_players[Team].size() + m_pendPlayers[Team].size() < maxPlayers);
	}
	else
	{
		uint32 size[2];
		size[0] = uint32(m_players[0].size() + m_pendPlayers[0].size());
		size[1] = uint32(m_players[1].size() + m_pendPlayers[1].size());
		res = (size[Team] < maxPlayers) && (((int)size[Team] - (int)size[1 - Team]) <= 0);
	}
	m_mainLock.Release();
	return res;
}


