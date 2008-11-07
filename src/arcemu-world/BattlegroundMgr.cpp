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

#define ENABLE_AB
#define ENABLE_EOTS

initialiseSingleton(CBattlegroundManager);
typedef CBattleground*(*CreateBattlegroundFunc)(MapMgr* mgr,uint32 iid,uint32 group, uint32 type);

const static uint32 BGMapIds[BATTLEGROUND_NUM_TYPES] = {
	0,      // 0
	30,      // AV
	489,   // WSG
	529,   // AB
	0,      // 2v2
	0,      // 3v3
	0,      // 5v5
	566,   // EOTS
};

const static CreateBattlegroundFunc BGCFuncs[BATTLEGROUND_NUM_TYPES] = {
	NULL,                  // 0
	NULL,                  // AV
	&WarsongGulch::Create,      // WSG
#ifdef ENABLE_AB
	&ArathiBasin::Create,      // AB
#else
	NULL,                  // AB
#endif
	NULL,                  // 2v2
	NULL,                  // 3v3
	NULL,                  // 5v5
#ifdef ENABLE_EOTS
	&EyeOfTheStorm::Create,      // EotS
#else
	NULL,                  // EotS
#endif
};

CBattlegroundManager::CBattlegroundManager() : EventableObject()
{
	int i;
	LoadBGSetFromConfig(); // config
	m_maxBattlegroundId = 0;
	sEventMgr.AddEvent(this, &CBattlegroundManager::EventQueueUpdate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 15000, 0,0);

	for (i=0; i<BATTLEGROUND_NUM_TYPES; i++) {
		m_instances[i].clear();
	}
}

CBattlegroundManager::~CBattlegroundManager()
{

}
void CBattlegroundManager::LoadBGSetFromConfig()
{
	// cebernic: for external controlled
	BGMaximumPlayers[0] = 0;
	BGMaximumPlayers[1] = sWorld.m_bgSet_AV_MAX;
	BGMaximumPlayers[2] = sWorld.m_bgSet_WS_MAX;
	BGMaximumPlayers[3] = sWorld.m_bgSet_AB_MAX;
	BGMaximumPlayers[4] = 4;
	BGMaximumPlayers[5] = 6;
	BGMaximumPlayers[6] = 10;
	BGMaximumPlayers[7] = sWorld.m_bgSet_EOS_MAX;

	BGMinimumPlayers[0] = 0;
	BGMinimumPlayers[1] = sWorld.m_bgSet_AV_MIN;
	BGMinimumPlayers[2] = sWorld.m_bgSet_WS_MIN;
	BGMinimumPlayers[3] = sWorld.m_bgSet_AB_MIN;
	BGMinimumPlayers[4] = 4;
	BGMinimumPlayers[5] = 6;
	BGMinimumPlayers[6] = 10;
	BGMinimumPlayers[7] = sWorld.m_bgSet_EOS_MIN;
	Log.Notice("BattlegroundManager","Min/Max - AV(%d/%d) AB(%d/%d) WS(%d/%d) EOTS(%d/%d).",
		sWorld.m_bgSet_AV_MIN,sWorld.m_bgSet_AV_MAX,
		sWorld.m_bgSet_AB_MIN,sWorld.m_bgSet_AB_MAX,
		sWorld.m_bgSet_WS_MIN,sWorld.m_bgSet_WS_MAX,
		sWorld.m_bgSet_EOS_MIN,sWorld.m_bgSet_EOS_MAX);
}

void CBattlegroundManager::HandleBattlegroundListPacket(WorldSession * m_session, uint32 BattlegroundType)
{
	if(BattlegroundType == BATTLEGROUND_ARENA_2V2 || BattlegroundType == BATTLEGROUND_ARENA_3V3 || BattlegroundType == BATTLEGROUND_ARENA_5V5)
	{
		WorldPacket data(SMSG_BATTLEFIELD_LIST, 17);
		data << m_session->GetPlayer()->GetGUID() << uint32(6) << uint32(0xC) << uint8(0);
		m_session->SendPacket(&data);
		return;
	}

	uint32 Count = 0;
	WorldPacket data(SMSG_BATTLEFIELD_LIST, 200);
	data << m_session->GetPlayer()->GetGUID();
	data << BattlegroundType;
	data << uint8(2);
	data << uint32(0);      // Count

	/* Append the battlegrounds */
	m_instanceLock.Acquire();
	for(map<uint32, CBattleground*>::iterator itr = m_instances[BattlegroundType].begin(); itr != m_instances[BattlegroundType].end(); ++itr)
	{
		if(itr->second->CanPlayerJoin(m_session->GetPlayer(),BattlegroundType) && !itr->second->HasEnded() )
		{
			data << itr->first;
			++Count;
		}
	}
	m_instanceLock.Release();
#ifdef USING_BIG_ENDIAN
	*(uint32*)&data.contents()[13] = swap32(Count);
#else
	*(uint32*)&data.contents()[13] = Count;
#endif
	m_session->SendPacket(&data);
}

void CBattlegroundManager::HandleBattlegroundJoin(WorldSession * m_session, WorldPacket & pck)
{
	uint64 guid;
	uint32 pguid = m_session->GetPlayer()->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(m_session->GetPlayer()->getLevel());
	uint32 bgtype;
	uint32 instance;

	pck >> guid >> bgtype >> instance;

	if ( ! guid )
		return; //crash fix. /script JoinBattlefield(0,1); ;s

	if(bgtype >= BATTLEGROUND_NUM_TYPES || !bgtype)
	{
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
	Log.Success("BattlegroundManager", "Player %u is now in battleground queue for instance %u", m_session->GetPlayer()->GetLowGUID(), instance );

	/* send the battleground status packet */
	SendBattlefieldStatus(m_session->GetPlayer(), 1, bgtype, instance, 0, BGMapIds[bgtype],0);
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
	case BATTLEGROUND_WARSUNG_GULCH:
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
	default:
		return 45;
	}
}

void CBattlegroundManager::HandleGetBattlegroundQueueCommand(WorldSession * m_session)
{
	std::stringstream ss;

	uint32 i,j;
	Player * plr;
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

					if(plr->GetTeam() == 0)
						ally++;
					if(plr->GetTeam() == 1)
						horde++;
				}

				ss << " (Alliance: " << ally << " Horde: " << horde;
				if((int)m_queuedPlayers[i][j].size() > (ally + horde))
					ss << " Unknown: " << ((int)m_queuedPlayers[i][j].size() - ally - horde);
				ss << ")";
			}

			m_session->SystemMessage( ss.str().c_str() );
			ss.rdbuf()->str("");
		}

		if(IS_ARENA(i))
		{
			if(m_queuedGroups[i].size())
			{
				foundSomething = true;

				ss << m_session->LocalizedWorldSrv(GetBattlegroundCaption((BattleGroundTypes)i)) << " (rated): ";
				ss << (uint32)m_queuedGroups[i].size() << " groups queued";

				m_session->SystemMessage( ss.str().c_str() );
				ss.rdbuf()->str("");
			}
		}
	}

	m_queueLock.Release();

	if(!foundSomething)
		m_session->SystemMessage( "There's nobody queued." );
}

void CBattlegroundManager::EventQueueUpdate()
{
	this->EventQueueUpdate(false);
}

uint32 CBattlegroundManager::GetArenaGroupQInfo(Group * group, int type, uint32 *avgRating)
{
	ArenaTeam *team;
	ArenaTeamMember *atm;
	Player *plr;
	uint32 count=0;
	uint32 rating=0;

	if (group == NULL || group->GetLeader() == NULL) return 0;

	plr = group->GetLeader()->m_loggedInPlayer;
	if (plr == NULL) return 0;

	team = plr->m_arenaTeams[type-BATTLEGROUND_ARENA_2V2];
	if (team == NULL) return 0;

	GroupMembersSet::iterator itx;
	for(itx = group->GetSubGroup(0)->GetGroupMembersBegin(); itx != group->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
	{
		plr = (*itx)->m_loggedInPlayer;
		if(plr)
		{
			if (team == plr->m_arenaTeams[type-BATTLEGROUND_ARENA_2V2])
			{
				atm = team->GetMemberByGuid(plr->GetLowGUID());
				if (atm)
				{
					rating+= atm->PersonalRating;
					count++;
				}
			}
		}
	}

	*avgRating = count > 0 ? rating/count : 0;

	return team ? team->m_id : 0;
}

void CBattlegroundManager::AddGroupToArena(CBattleground * bg, Group * group, int nteam)
{
	ArenaTeam *team;
	Player *plr;

	if (group == NULL || group->GetLeader() == NULL) return;

	plr = group->GetLeader()->m_loggedInPlayer;
	if (plr == NULL) return;

	team = plr->m_arenaTeams[bg->GetType()-BATTLEGROUND_ARENA_2V2];
	if (team == NULL) return;

	GroupMembersSet::iterator itx;
	for(itx = group->GetSubGroup(0)->GetGroupMembersBegin(); itx != group->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
	{
		plr = (*itx)->m_loggedInPlayer;
		if(plr && team == plr->m_arenaTeams[bg->GetType()-BATTLEGROUND_ARENA_2V2])
		{
			if( bg->HasFreeSlots(nteam,bg->GetType()) )
			{
				bg->AddPlayer(plr, nteam);
				plr->SetTeam(nteam);
			}
		}
	}
}

int CBattlegroundManager::CreateArenaType(int type, Group * group1, Group * group2)
{
	Arena * ar = ((Arena*)CreateInstance(type, LEVEL_GROUP_70));
	if (ar == NULL)
	{
		Log.Error("BattlegroundMgr", "%s (%u): Couldn't create Arena Instance", __FILE__, __LINE__);
		m_queueLock.Release();
		m_instanceLock.Release();
		return -1;
	}
	ar->rated_match=true;

	AddGroupToArena(ar, group1, 0);
	AddGroupToArena(ar, group2, 1);

	return 0;
}

void CBattlegroundManager::AddPlayerToBg(CBattleground * bg, deque<uint32> *playerVec, uint32 i, uint32 j)
{
	uint32 plrguid = *playerVec->begin();
	playerVec->pop_front();
	Player *plr = objmgr.GetPlayer(plrguid);
	if(plr) {
		if(bg->CanPlayerJoin(plr,bg->GetType()))
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

void CBattlegroundManager::AddPlayerToBgTeam(CBattleground * bg, deque<uint32> *playerVec, uint32 i, uint32 j, int Team)
{
	if (bg->HasFreeSlots(Team,bg->GetType()))
	{
		uint32 plrguid = *playerVec->begin();
		playerVec->pop_front();
		Player *plr = objmgr.GetPlayer(plrguid);
		if(plr)
		{
			plr->m_bgTeam=Team;
			bg->AddPlayer(plr, Team);
		}
		ErasePlayerFromList(plrguid, &m_queuedPlayers[i][j]);
	}
}

void CBattlegroundManager::EventQueueUpdate(bool forceStart)
{
	deque<uint32> tempPlayerVec[2];
	uint32 i,j,k;
	Player * plr;
	CBattleground * bg;
	list<uint32>::iterator it3, it4;
	map<uint32, CBattleground*>::iterator iitr;
	Arena * arena;
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
					if(bg->CanPlayerJoin(plr,bg->GetType()))
					{
						bg->AddPlayer(plr, plr->GetTeam());
						m_queuedPlayers[i][j].erase(it4);
					}
				}
				else
				{
					if(IS_ARENA(i))
						tempPlayerVec[0].push_back(plrguid);
					else if (!plr->HasAura(BG_DESERTER))
						tempPlayerVec[plr->GetTeam()].push_back(plrguid);
				}
			}

			// try to join existing instances
			for(iitr = m_instances[i].begin(); iitr != m_instances[i].end(); ++iitr)
			{
				if( iitr->second->HasEnded() || iitr->second->GetLevelGroup() != j )
					continue;

				if(IS_ARENA(i))
				{
					arena = ((Arena*)iitr->second);
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
							plr->m_bgTeam=team;
							arena->AddPlayer(plr, team);
							team = arena->GetFreeTeam();
						}
						ErasePlayerFromList(plrguid, &m_queuedPlayers[i][j]);
					}
				}
				else
				{
					bg = iitr->second;
					int size = (int)min(tempPlayerVec[0].size(),tempPlayerVec[1].size());
					for(int counter = 0; (counter < size) && (bg->IsFull() == false); counter++)
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
						AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
					}

					while (tempPlayerVec[0].size() > 0 && bg->HasFreeSlots(0, bg->GetType()))
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[0], i, j, 0);
					}
					while (tempPlayerVec[1].size() > 0 && bg->HasFreeSlots(1, bg->GetType()))
					{
						AddPlayerToBgTeam(bg, &tempPlayerVec[1], i, j, 1);
					}
				}
			}

			if(IS_ARENA(i))
			{
				// enough players to start a round?
				if(!forceStart && tempPlayerVec[0].size() < BGMinimumPlayers[i])
					continue;

				if(CanCreateInstance(i,j))
				{
					arena = ((Arena*)CreateInstance(i, j));
					if ( arena == NULL )
					{
						Log.Error("BattlegroundMgr", "%s (%u): Couldn't create Arena Instance", __FILE__, __LINE__);
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
							plr->m_bgTeam=team;
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
				if(forceStart || (tempPlayerVec[0].size() >= BGMinimumPlayers[i] && tempPlayerVec[1].size() >= BGMinimumPlayers[i]))
				{
					if(CanCreateInstance(i,j))
					{
						bg = CreateInstance(i,j);
						if ( bg == NULL )
						{
							m_queueLock.Release();
							m_instanceLock.Release();
 							return;
						}

						// push as many as possible in
						if (forceStart)
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
							int size = (int)min(tempPlayerVec[0].size(),tempPlayerVec[1].size());
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
	Group * group1, *group2;
	uint32 teamids[2] = {0,0};
	uint32 avgRating[2] = {0,0};
	uint32 n;
	list<uint32>::iterator itz;
	for(i = BATTLEGROUND_ARENA_2V2; i <= BATTLEGROUND_ARENA_5V5; ++i)
	{
		if(!forceStart && m_queuedGroups[i].size() < 2)      /* got enough to have an arena battle ;P */
		{
			continue;
		}

		for (j=0; j<(uint32)m_queuedGroups[i].size(); j++)
		{
			group1 = group2 = NULL;
			n =	RandomUInt((uint32)m_queuedGroups[i].size()) - 1;
			for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end() && n>0; ++itz)
				--n;

			if(itz == m_queuedGroups[i].end())
				itz=m_queuedGroups[i].begin();

			if(itz == m_queuedGroups[i].end())
			{
				Log.Error("BattlegroundMgr", "Internal error at %s:%u", __FILE__, __LINE__);
				m_queueLock.Release();
				m_instanceLock.Release();
				return;
			}

			group1 = objmgr.GetGroupById(*itz);
			if (group1 == NULL)
			{
				continue;
			}

			if (forceStart && m_queuedGroups[i].size() == 1)
			{
				if (CreateArenaType(i, group1, NULL) == -1) return;
				m_queuedGroups[i].remove(group1->GetID());
				continue;
			}

			teamids[0] = GetArenaGroupQInfo(group1, i, &avgRating[0]);

			list<uint32> possibleGroups;
			for(itz = m_queuedGroups[i].begin(); itz != m_queuedGroups[i].end(); ++itz)
			{
				group2 = objmgr.GetGroupById(*itz);
				if (group2)
				{
					teamids[1] = GetArenaGroupQInfo(group2, i, &avgRating[1]);
					uint32 delta = abs((int32)avgRating[0] - (int32)avgRating[1]);
					if (teamids[0] != teamids[1] && delta <= sWorld.ArenaQueueDiff)
					{
						possibleGroups.push_back(group2->GetID());
					}
				}
			}

			if (possibleGroups.size() > 0)
			{
				n = RandomUInt((uint32)possibleGroups.size()) - 1;
				for(itz = possibleGroups.begin(); itz != possibleGroups.end() && n>0; ++itz)
					--n;

				if(itz == possibleGroups.end())
					itz=possibleGroups.begin();

				if(itz == possibleGroups.end())
				{
					Log.Error("BattlegroundMgr", "Internal error at %s:%u", __FILE__, __LINE__);
					m_queueLock.Release();
					m_instanceLock.Release();
					return;
				}

				group2 = objmgr.GetGroupById(*itz);
				if (group2)
				{
					if (CreateArenaType(i, group1, group2) == -1) return;
					m_queuedGroups[i].remove(group1->GetID());
					m_queuedGroups[i].remove(group2->GetID());
				}
			}
		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();
}

void CBattlegroundManager::RemovePlayerFromQueues(Player * plr)
{
	m_queueLock.Acquire();

	ASSERT(plr->m_bgQueueType < BATTLEGROUND_NUM_TYPES);
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
	plr->m_bgTeam=plr->GetTeam();
	plr->m_pendingBattleground=0;
	SendBattlefieldStatus(plr,0,0,0,0,0,0);
	m_queueLock.Release();
}

void CBattlegroundManager::RemoveGroupFromQueues(Group * grp)
{
	m_queueLock.Acquire();
	for(uint32 i = BATTLEGROUND_ARENA_2V2; i < BATTLEGROUND_ARENA_5V5+1; ++i)
	{
		for(list<uint32>::iterator itr = m_queuedGroups[i].begin(); itr != m_queuedGroups[i].end(); )
		{
			if((*itr) == grp->GetID())
				itr = m_queuedGroups[i].erase(itr);
			else
				++itr;
		}
	}

	for(GroupMembersSet::iterator itr = grp->GetSubGroup(0)->GetGroupMembersBegin(); itr != grp->GetSubGroup(0)->GetGroupMembersEnd(); ++itr)
		if((*itr)->m_loggedInPlayer)
			SendBattlefieldStatus((*itr)->m_loggedInPlayer, 0, 0, 0, 0, 0, 0);

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

void CBattleground::SendWorldStates(Player * plr)
{
	if(!m_worldStates.size())
		return;

	uint32 bflag = 0;
	uint32 bflag2 = 0;

	switch(m_mapMgr->GetMapId())
	{
	case  489: bflag = 0x0CCD; bflag2 = 0x0CF9; break;
	case  529: bflag = 0x0D1E; break;
	case   30: bflag = 0x0A25; break;
	case  559: bflag = 3698; break;
	case 566: bflag = 0x0eec; bflag2 = 0; break;         // EOTS

	default:      /* arenas */
		bflag  = 0x0E76;
		bflag2 = 0;
		break;
	}

	WorldPacket data(SMSG_INIT_WORLD_STATES, 10 + (m_worldStates.size() * 8));
	data << m_mapMgr->GetMapId();
	data << bflag;
	data << bflag2;
	data << uint16(m_worldStates.size());

	for(map<uint32, uint32>::iterator itr = m_worldStates.begin(); itr != m_worldStates.end(); ++itr)
		data << itr->first << itr->second;
	plr->GetSession()->SendPacket(&data);
}

CBattleground::CBattleground(MapMgr * mgr, uint32 id, uint32 levelgroup, uint32 type) : m_mapMgr(mgr), m_id(id), m_type(type), m_levelGroup(levelgroup)
{
	m_nextPvPUpdateTime = 0;
	m_countdownStage = 0;
	m_ended = false;
	m_started = false;
	m_winningteam = 0;
	m_startTime = (uint32)UNIXTIME;
	m_lastResurrect = (uint32)UNIXTIME;
	m_invisGMs = 0;
	sEventMgr.AddEvent(this, &CBattleground::EventResurrectPlayers, EVENT_BATTLEGROUND_QUEUE_UPDATE, 30000, 0,0);

	/* create raid groups */
	for(uint32 i = 0; i < 2; ++i)
	{
		m_groups[i] = new Group(true);
		m_groups[i]->m_disbandOnNoMembers = false;
		m_groups[i]->ExpandToRaid();
	}
}

CBattleground::~CBattleground()
{
	sEventMgr.RemoveEvents(this);
	for(uint32 i = 0; i < 2; ++i)
	{
		PlayerInfo *inf;
		for(uint32 j = 0; j < m_groups[i]->GetSubGroupCount(); ++j) {
			for(GroupMembersSet::iterator itr = m_groups[i]->GetSubGroup(j)->GetGroupMembersBegin(); itr != m_groups[i]->GetSubGroup(j)->GetGroupMembersEnd();) {
				inf = (*itr);
				++itr;
				m_groups[i]->RemovePlayer(inf);
			}
		}
		delete m_groups[i];
	}
}

void CBattleground::UpdatePvPData()
{
	if(m_type >= BATTLEGROUND_ARENA_2V2 && m_type <= BATTLEGROUND_ARENA_5V5)
	{
		if(!m_ended)
		{
			return;
		}
	}

	if(UNIXTIME >= m_nextPvPUpdateTime)
	{
		m_mainLock.Acquire();
		WorldPacket data(10*(m_players[0].size()+m_players[1].size())+50);
		BuildPvPUpdateDataPacket(&data);
		DistributePacketToAll(&data);
		m_mainLock.Release();

		m_nextPvPUpdateTime = UNIXTIME + 2;
	}
}

void CBattleground::BuildPvPUpdateDataPacket(WorldPacket * data)
{
	data->Initialize(MSG_PVP_LOG_DATA);
	data->reserve(10*(m_players[0].size()+m_players[1].size())+50);

	BGScore * bs;
	if(m_type >= BATTLEGROUND_ARENA_2V2 && m_type <= BATTLEGROUND_ARENA_5V5)
	{
		if(!m_ended)
		{
			return;
		}

		*data << uint8(1);
		if(!Rated())
		{
			*data << uint32(0) << uint32(0) << uint8(0);
			*data << uint32(0) << uint32(0) << uint8(0);
		}
		else
		{
			/* Grab some arena teams */
			ArenaTeam **teams = ((Arena*)this)->GetTeams();

			if(teams[0]) {
				*data << uint32(0) << uint32(3000+m_deltaRating[0]) << uint8(0);
			} else {
				*data << uint32(0) << uint32(0) << uint8(0);
			}

			if(teams[1]) {
				*data << uint32(0) << uint32(3000+m_deltaRating[1]) << uint8(0);
			} else {
				*data << uint32(0) << uint32(0) << uint8(0);
			}
		}

		*data << uint8(1);
		*data << uint8(m_winningteam);

		*data << uint32((m_players[0].size() + m_players[1].size())-m_invisGMs);
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				if( (*itr)->m_isGmInvisible )continue;
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

		*data << uint32((m_players[0].size() + m_players[1].size())-m_invisGMs);
		for(uint32 i = 0; i < 2; ++i)
		{
			for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			{
				if( (*itr)->m_isGmInvisible )continue;
				*data << (*itr)->GetGUID();
				bs = &(*itr)->m_bgScore;

				*data << bs->KillingBlows;
				*data << bs->HonorableKills;
				*data << bs->Deaths;
				*data << bs->BonusHonor;
				*data << bs->DamageDone;
				*data << bs->HealingDone;
				*data << uint32(0x2);
				*data << bs->Misc1;
				*data << bs->Misc2;
			}
		}
	}

}
void CBattleground::AddPlayer(Player * plr, uint32 team)
{
	m_mainLock.Acquire();

	/* This is called when the player is added, not when they port. So, they're essentially still queued, but not inside the bg yet */
	m_pendPlayers[team].insert(plr->GetLowGUID());

	/* Send a packet telling them that they can enter */
	BattlegroundManager.SendBattlefieldStatus(plr, 2, m_type, m_id, 80000, m_mapMgr->GetMapId(),Rated());      // You will be removed from the queue in 2 minutes.

	/* Add an event to remove them in 1 minute 20 seconds time. */
	sEventMgr.AddEvent(plr, &Player::RemoveFromBattlegroundQueue, EVENT_BATTLEGROUND_QUEUE_UPDATE, 80000, 1,0);
	plr->m_pendingBattleground = this;

	m_mainLock.Release();
}

void CBattleground::RemovePendingPlayer(Player * plr)
{
	m_mainLock.Acquire();
	m_pendPlayers[plr->m_bgTeam].erase(plr->GetLowGUID());

	/* send a null bg update (so they don't join) */
	BattlegroundManager.SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);
	plr->m_pendingBattleground =0;
	plr->m_bgTeam=plr->GetTeam();

	m_mainLock.Release();
}

void CBattleground::OnPlayerPushed(Player * plr)
{
	if( plr->GetGroup() && !Rated() )
		plr->GetGroup()->RemovePlayer(plr->m_playerInfo);

	plr->ProcessPendingUpdates();

	if( plr->GetGroup() == NULL )
	{
		if ( plr->m_isGmInvisible == false ) //do not join invisible gm's into bg groups.
			m_groups[plr->m_bgTeam]->AddMember( plr->m_playerInfo );
	}
}

void CBattleground::PortPlayer(Player * plr, bool skip_teleport /* = false*/)
{
	m_mainLock.Acquire();
	if(m_ended)
	{
		sChatHandler.SystemMessage(plr->GetSession(), plr->GetSession()->LocalizedWorldSrv(53) );
		BattlegroundManager.SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);
		plr->m_pendingBattleground = 0;
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
	if ( plr->m_isGmInvisible == false )
	{
		//Do not let everyone know an invisible gm has joined.
		WorldPacket data(SMSG_BATTLEGROUND_PLAYER_JOINED, 8);
		data << plr->GetGUID();
		DistributePacketToTeam(&data,plr->m_bgTeam);
	}
	else
	{
		m_invisGMs++;
	}
	m_players[plr->m_bgTeam].insert(plr);

	/* remove from any auto queue remove events */
	sEventMgr.RemoveEvents(plr, EVENT_BATTLEGROUND_QUEUE_UPDATE);

	if( !skip_teleport )
	{
		if( plr->IsInWorld() )
			plr->RemoveFromWorld();
	}

	plr->m_pendingBattleground = 0;
	plr->m_bg = this;

	if(!plr->IsPvPFlagged())
		plr->SetPvPFlag();

	/* Reset the score */
	memset(&plr->m_bgScore, 0, sizeof(BGScore));

	/* send him the world states */
	SendWorldStates(plr);

	/* update pvp data */
	UpdatePvPData();

	/* add the player to the group */
	if(plr->GetGroup() && !Rated())
	{
		// remove them from their group
		plr->GetGroup()->RemovePlayer( plr->m_playerInfo );
	}

	if(!m_countdownStage)
	{
		m_countdownStage = 1;
		sEventMgr.AddEvent(this, &CBattleground::EventCountdown, EVENT_BATTLEGROUND_COUNTDOWN, 30000, 0,0);
		sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 10000);
	}

	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
	OnAddPlayer(plr);

	if(!skip_teleport)
	{
		/* This is where we actually teleport the player to the battleground. */   
		plr->SafeTeleport(m_mapMgr,GetStartingCoords(plr->m_bgTeam));
		BattlegroundManager.SendBattlefieldStatus(plr, 3, m_type, m_id, (uint32)UNIXTIME - m_startTime, m_mapMgr->GetMapId(),Rated());   // Elapsed time is the last argument
	}

	m_mainLock.Release();
}

CBattleground * CBattlegroundManager::CreateInstance(uint32 Type, uint32 LevelGroup)
{
	CreateBattlegroundFunc cfunc = BGCFuncs[Type];
	MapMgr * mgr = 0;
	CBattleground * bg;
	bool isWeekend = false;
	struct tm tm;
	uint32 iid;
	time_t t;
	int n;

	if(Type == BATTLEGROUND_ARENA_2V2 || Type == BATTLEGROUND_ARENA_3V3 || Type == BATTLEGROUND_ARENA_5V5)
	{
		/* arenas follow a different procedure. */
		static const uint32 arena_map_ids[3] = { 559, 562, 572 };
		uint32 mapid = arena_map_ids[RandomUInt(2)];
		uint32 players_per_side;

		mgr = sInstanceMgr.CreateBattlegroundInstance(mapid);
		if(mgr == NULL)
		{
			Log.Error("BattlegroundManager", "Arena CreateInstance() call failed for map %u, type %u, level group %u", mapid, Type, LevelGroup);
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

		iid = ++m_maxBattlegroundId;
		bg = new Arena(mgr, iid, LevelGroup, Type, players_per_side);
		mgr->m_battleground = bg;
		Log.Success("BattlegroundManager", "Created arena battleground type %u for level group %u on map %u.", Type, LevelGroup, mapid);
		sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 1, 1,0);
		m_instanceLock.Acquire();
		m_instances[Type].insert( make_pair(iid, bg) );
		m_instanceLock.Release();
		return bg;
	}

	if(cfunc == NULL)
	{
		Log.Error("BattlegroundManager", "Could not find CreateBattlegroundFunc pointer for type %u level group %u", Type, LevelGroup);
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

	switch (Type)
	{
		case BATTLEGROUND_WARSUNG_GULCH: n = 0; break;
		case BATTLEGROUND_ARATHI_BASIN: n = 1; break;
		case BATTLEGROUND_EYE_OF_THE_STORM: n = 2; break;
		default: n = 0; break;
	}
	if (((tm.tm_yday / 7) % 3) == n)
	{
		/* Set weekend from thursday night at midnight until tuesday morning */
		isWeekend = tm.tm_wday >= 5 || tm.tm_wday < 2;
	}

	/* Create Map Manager */
	mgr = sInstanceMgr.CreateBattlegroundInstance(BGMapIds[Type]);
	if(mgr == NULL)
	{
		Log.Error("BattlegroundManager", "CreateInstance() call failed for map %u, type %u, level group %u", BGMapIds[Type], Type, LevelGroup);
		return NULL;      // Shouldn't happen
	}

	/* Call the create function */
	iid = ++m_maxBattlegroundId;
	bg = cfunc(mgr, iid, LevelGroup, Type);
	bg->SetIsWeekend(isWeekend);
	mgr->m_battleground = bg;
	sEventMgr.AddEvent(bg, &CBattleground::EventCreate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 1, 1,0);
	Log.Success("BattlegroundManager", "Created battleground type %u for level group %u.", Type, LevelGroup);

	m_instanceLock.Acquire();
	m_instances[Type].insert( make_pair(iid, bg) );
	m_instanceLock.Release();

	return bg;
}

void CBattlegroundManager::DeleteBattleground(CBattleground * bg)
{
	uint32 i = bg->GetType();
	uint32 j = bg->GetLevelGroup();
	Player * plr;

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
			SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);
			plr->m_bgIsQueued = false;
			m_queuedPlayers[i][j].erase(it2);
		}
	}

	m_queueLock.Release();
	m_instanceLock.Release();

}

GameObject * CBattleground::SpawnGameObject(uint32 entry,uint32 MapId , float x, float y, float z, float o, uint32 flags, uint32 faction, float scale)
{
	GameObject *go = m_mapMgr->CreateGameObject(entry);

	go->CreateFromProto(entry, MapId, x, y, z, o);

	go->SetUInt32Value(GAMEOBJECT_FACTION,faction);
	go->_setFaction();
	go->SetFloatValue(OBJECT_FIELD_SCALE_X,scale);   
	go->SetUInt32Value(GAMEOBJECT_FLAGS, flags);
	go->SetFloatValue(GAMEOBJECT_POS_X, x);
	go->SetFloatValue(GAMEOBJECT_POS_Y, y);
	go->SetFloatValue(GAMEOBJECT_POS_Z, z);
	go->SetFloatValue(GAMEOBJECT_FACING, o);
	go->SetInstanceID(m_mapMgr->GetInstanceID());
	go->m_battleground = this;

	return go;
}

void CBattleground::SendChatMessage(uint32 Type, uint64 Guid, const char * Format, ...)
{
	char msg[500];
	va_list ap;
	va_start(ap, Format);
	vsnprintf(msg, 500, Format, ap);
	va_end(ap);
	WorldPacket * data = sChatHandler.FillMessageData(Type, 0, msg, Guid, 0);
	DistributePacketToAll(data);
	delete data;
}

void CBattleground::DistributePacketToAll(WorldPacket * packet)
{
	m_mainLock.Acquire();
	for(int i = 0; i < 2; ++i)
	{
     for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			if( (*itr) && (*itr)->GetSession() )
			  (*itr)->GetSession()->SendPacket(packet);
	}
	m_mainLock.Release();
}

void CBattleground::DistributePacketToTeam(WorldPacket * packet, uint32 Team)
{
	m_mainLock.Acquire();
	for(set<Player*>::iterator itr = m_players[Team].begin(); itr != m_players[Team].end(); ++itr)
		if( (*itr) && (*itr)->GetSession() )
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

void CBattlegroundManager::SendBattlefieldStatus(Player * plr, uint32 Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch)
{
	WorldPacket data(SMSG_BATTLEFIELD_STATUS, 30);
	if(Status == 0)
		data << uint64(0) << uint32(0);
	else
	{
		if(Type >= BATTLEGROUND_ARENA_2V2 && Type <= BATTLEGROUND_ARENA_5V5)
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
			data << uint32(11);
			data << uint8(RatedMatch);      // 1 = rated match
		}
		else
		{
			data << uint32(0);
			data << uint8(0) << uint8(2);
			data << Type;
			data << uint16(0x1F90);
			data << InstanceID;
			data << uint8(0);
		}

		data << Status;

		switch(Status)
		{
		case 1:               // Waiting in queue
			data << uint32(60) << uint32(0);            // Time / Elapsed time
			break;
		case 2:               // Ready to join!
			data << MapId << Time;
			break;
		case 3:
			if(Type >= BATTLEGROUND_ARENA_2V2 && Type <= BATTLEGROUND_ARENA_5V5)
				data << MapId << uint32(0) << Time << uint8(0);
			else
				data << MapId << uint32(0) << Time << uint8(1);
			break;
		}
	}

	plr->GetSession()->SendPacket(&data);
}

void CBattleground::RemovePlayer(Player * plr, bool logout)
{
	WorldPacket data(SMSG_BATTLEGROUND_PLAYER_LEFT, 30);
	data << plr->GetGUID();
	if ( plr->m_isGmInvisible == false )
	{
		//Dont show invisble gm's leaving the game.
		DistributePacketToAll(&data);
	}
	else
	{
		RemoveInvisGM();
	}
	m_mainLock.Acquire();
	m_players[plr->m_bgTeam].erase(plr);

	memset(&plr->m_bgScore, 0, sizeof(BGScore));
	OnRemovePlayer(plr);
	plr->m_bg = NULL;
	plr->FullHPMP();

	/* are we in the group? */
	if(plr->GetGroup() == m_groups[plr->m_bgTeam])
		plr->GetGroup()->RemovePlayer( plr->m_playerInfo );

	// reset team
	plr->ResetTeam();

	/* revive the player if he is dead */
	if(!plr->isAlive())
	{
		plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
		plr->ResurrectPlayer();
	}

	/* remove buffs */
	plr->RemoveAura(32727); // Arena preparation
	plr->RemoveAura(44521); // BG preparation
	plr->RemoveAura(44535);
	plr->RemoveAura(21074);

	/* teleport out */
	if(!logout)
	{
		if(!m_ended)
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

		BattlegroundManager.SendBattlefieldStatus(plr, 0, 0, 0, 0, 0,0);

		/* send some null world states */
		data.Initialize(SMSG_INIT_WORLD_STATES);
		data << uint32(plr->GetMapId()) << uint32(0) << uint32(0);
		plr->GetSession()->SendPacket(&data);
	}

	if(!m_ended && m_players[0].size() == 0 && m_players[1].size() == 0)
	{
		/* create an inactive event */
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);                  // 10mins
		sEventMgr.AddEvent(this, &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 600000, 1,0);
	}

	plr->m_bgTeam=plr->GetTeam();
	m_mainLock.Release();
}

void CBattleground::SendPVPData(Player * plr)
{
	m_mainLock.Acquire();

	WorldPacket data(10*(m_players[0].size()+m_players[1].size())+50);
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
				 if( (*itr) && (*itr)->GetSession() ){
						(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(46),(*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
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
				 if( (*itr) && (*itr)->GetSession() ){
						(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(47),(*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
					}
		}
		m_mainLock.Release();

		//SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Thirty seconds until the battle for %s begins!", GetName() );
	}
	else if(m_countdownStage == 3)
		if(m_countdownStage==1)
		{
			m_countdownStage = 4;

			m_mainLock.Acquire();
			for(int i = 0; i < 2; ++i)
			{
				 for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
					 if( (*itr) && (*itr)->GetSession() ){
							(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(48),(*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
						}
			}
			m_mainLock.Release();

			//SendChatMessage( CHAT_MSG_BG_EVENT_NEUTRAL, 0, "Fifteen seconds until the battle for %s begins!", GetName() );
			sEventMgr.ModifyEventTime(this, EVENT_BATTLEGROUND_COUNTDOWN, 15000);
			sEventMgr.ModifyEventTimeLeft(this, EVENT_BATTLEGROUND_COUNTDOWN, 15000);
		}
		else
		{
			m_mainLock.Acquire();
			for(int i = 0; i < 2; ++i)
			{
				 for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
					 if( (*itr) && (*itr)->GetSession() ){
							(*itr)->GetSession()->SystemMessage((*itr)->GetSession()->LocalizedWorldSrv(49),(*itr)->GetSession()->LocalizedWorldSrv(GetNameID()));
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
	map<uint32, uint32>::iterator itr = m_worldStates.find(Index);
	if(itr == m_worldStates.end())
		m_worldStates.insert( make_pair( Index, Value ) );
	else
		itr->second = Value;

	WorldPacket data(SMSG_UPDATE_WORLD_STATE, 8);
	data << Index << Value;
	DistributePacketToAll(&data);
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
		Player * plr;
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

	/* call the virtual onclose for cleanup etc */
	OnClose();

	/* shut down the map thread. this will delete the battleground from the corrent context. */
	m_mapMgr->SetThreadState(THREADSTATE_TERMINATE);

	m_mainLock.Release();
}

Creature * CBattleground::SpawnSpiritGuide(float x, float y, float z, float o, uint32 horde)
{
	if(horde > 1)
		horde = 1;

	CreatureInfo * pInfo = CreatureNameStorage.LookupEntry(13116 + horde);
	if(pInfo == 0)
	{
		return NULL;
	}

	Creature * pCreature = m_mapMgr->CreateCreature(pInfo->Id);

	pCreature->Create(pInfo->Name, m_mapMgr->GetMapId(), x, y, z, o);

	pCreature->SetInstanceID(m_mapMgr->GetInstanceID());
	pCreature->SetUInt32Value(OBJECT_FIELD_ENTRY, 13116 + horde);
	pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 1.0f);

	pCreature->SetUInt32Value(UNIT_FIELD_HEALTH, 100000);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER1, 4868);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER3, 200);
	pCreature->SetUInt32Value(UNIT_FIELD_POWER5, 2000000);

	pCreature->SetUInt32Value(UNIT_FIELD_MAXHEALTH, 10000);
	pCreature->SetUInt32Value(UNIT_FIELD_MAXPOWER1, 4868);
	pCreature->SetUInt32Value(UNIT_FIELD_MAXPOWER3, 200);
	pCreature->SetUInt32Value(UNIT_FIELD_MAXPOWER5, 2000000);

	pCreature->SetUInt32Value(UNIT_FIELD_LEVEL, 60);
	pCreature->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, 84 - horde);
	pCreature->_setFaction();
	pCreature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0 | (2 << 8) | (1 << 16));

	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, 22802);
	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO, 2 | (0xA << 8) | (2 << 16) | (0x11 << 24));
	pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_INFO_01, 2);

	pCreature->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_PLUS_MOB | UNIT_FLAG_NOT_ATTACKABLE_9 | UNIT_FLAG_UNKNOWN_10 | UNIT_FLAG_PVP); // 4928

	pCreature->SetUInt32Value(UNIT_FIELD_AURA, 22011);
	pCreature->SetUInt32Value(UNIT_FIELD_AURAFLAGS, 9);
	pCreature->SetUInt32Value(UNIT_FIELD_AURALEVELS, 0x3C);
	pCreature->SetUInt32Value(UNIT_FIELD_AURAAPPLICATIONS, 0xFF);

	pCreature->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME, 2000);
	pCreature->SetUInt32Value(UNIT_FIELD_BASEATTACKTIME_01, 2000);
	pCreature->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 0.208f);
	pCreature->SetFloatValue(UNIT_FIELD_COMBATREACH, 1.5f);

	pCreature->SetUInt32Value(UNIT_FIELD_DISPLAYID, 13337 + horde);
	pCreature->SetUInt32Value(UNIT_FIELD_NATIVEDISPLAYID, 13337 + horde);

	pCreature->SetUInt32Value(UNIT_CHANNEL_SPELL, 22011);
	pCreature->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);

	pCreature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITGUIDE);
	pCreature->SetUInt32Value(UNIT_FIELD_BYTES_2, 1 | (0x10 << 8));

	pCreature->DisableAI();
	pCreature->PushToWorld(m_mapMgr);
	return pCreature;
}

void CBattleground::QueuePlayerForResurrect(Player * plr, Creature * spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
		itr->second.insert(plr->GetLowGUID());
	plr->m_areaSpiritHealer_guid=spirit_healer->GetGUID();
	m_mainLock.Release();
}

void CBattleground::RemovePlayerFromResurrect(Player * plr, Creature * spirit_healer)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(spirit_healer);
	if(itr != m_resurrectMap.end())
		itr->second.erase(plr->GetLowGUID());
	plr->m_areaSpiritHealer_guid=0;
	m_mainLock.Release();
}

void CBattleground::AddSpiritGuide(Creature * pCreature)
{
	m_mainLock.Acquire();
	map<Creature*,set<uint32> >::iterator itr = m_resurrectMap.find(pCreature);
	if(itr == m_resurrectMap.end())
	{
		set<uint32> ti;
		m_resurrectMap.insert(make_pair(pCreature,ti));
	}
	m_mainLock.Release();
}

void CBattleground::RemoveSpiritGuide(Creature * pCreature)
{
	m_mainLock.Acquire();
	m_resurrectMap.erase(pCreature);
	m_mainLock.Release();
}

void CBattleground::EventResurrectPlayers()
{
	m_mainLock.Acquire();
	Player * plr;
	set<uint32>::iterator itr;
	map<Creature*,set<uint32> >::iterator i;
	WorldPacket data(50);
	for(i = m_resurrectMap.begin(); i != m_resurrectMap.end(); ++i)
	{
		for(itr = i->second.begin(); itr != i->second.end(); ++itr)
		{
			plr = m_mapMgr->GetPlayer(*itr);
			if(plr && plr->isDead())
			{
				data.Initialize(SMSG_SPELL_START);
				data << plr->GetNewGUID() << plr->GetNewGUID() << uint32(RESURRECT_SPELL) << uint8(0) << uint16(0) << uint32(0) << uint16(2) << plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				data.Initialize(SMSG_SPELL_GO);
				data << plr->GetNewGUID() << plr->GetNewGUID() << uint32(RESURRECT_SPELL) << uint8(0) << uint8(1) << uint8(1) << plr->GetGUID() << uint8(0) << uint16(2)
					<< plr->GetGUID();
				plr->SendMessageToSet(&data, true);

				plr->ResurrectPlayer();
				plr->SetUInt32Value(UNIT_FIELD_HEALTH, plr->GetUInt32Value(UNIT_FIELD_MAXHEALTH));
				plr->SetUInt32Value(UNIT_FIELD_POWER1, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER1));
				plr->SetUInt32Value(UNIT_FIELD_POWER4, plr->GetUInt32Value(UNIT_FIELD_MAXPOWER4));
				plr->CastSpell(plr, BG_REVIVE_PREPARATION, true);
			}
		}
		i->second.clear();
	}
	m_lastResurrect = (uint32)UNIXTIME;
	m_mainLock.Release();
}

void CBattlegroundManager::HandleArenaJoin(WorldSession * m_session, uint32 BattlegroundType, uint8 as_group, uint8 rated_match)
{
	uint32 pguid = m_session->GetPlayer()->GetLowGUID();
	uint32 lgroup = GetLevelGrouping(m_session->GetPlayer()->getLevel());
	if(as_group && m_session->GetPlayer()->GetGroup() == NULL)
		return;

	Group * pGroup = m_session->GetPlayer()->GetGroup();
	if(as_group)
	{
		if(pGroup->GetSubGroupCount() != 1)
		{
			m_session->SystemMessage(m_session->LocalizedWorldSrv(55) );
			return;
		}
		if(pGroup->GetLeader() != m_session->GetPlayer()->m_playerInfo)
		{
			m_session->SystemMessage(m_session->LocalizedWorldSrv(56) );
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
			uint32 type=BattlegroundType-BATTLEGROUND_ARENA_2V2;
			switch(BattlegroundType)
			{
			case BATTLEGROUND_ARENA_3V3:
				maxplayers=3;
				break;

			case BATTLEGROUND_ARENA_5V5:
				maxplayers=5;
				break;

			case BATTLEGROUND_ARENA_2V2:
			default:
				maxplayers=2;
				break;
			}

			if(pGroup->GetLeader()->m_loggedInPlayer && pGroup->GetLeader()->m_loggedInPlayer->m_arenaTeams[type] == NULL)
			{
				m_session->SystemMessage( m_session->LocalizedWorldSrv(57));
				return;
			}

			pGroup->Lock();
			for(itx = pGroup->GetSubGroup(0)->GetGroupMembersBegin(); itx != pGroup->GetSubGroup(0)->GetGroupMembersEnd(); ++itx)
			{
				if(maxplayers==0)
				{
					m_session->SystemMessage(m_session->LocalizedWorldSrv(58));
					pGroup->Unlock();
					return;
				}

				if((*itx)->lastLevel < PLAYER_LEVEL_CAP_70)
				{
					m_session->SystemMessage(m_session->LocalizedWorldSrv(59));
					pGroup->Unlock();
					return;
				}

				if((*itx)->m_loggedInPlayer)
				{
					if((*itx)->m_loggedInPlayer->m_bg || (*itx)->m_loggedInPlayer->m_bg || (*itx)->m_loggedInPlayer->m_bgIsQueued)
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
					SendBattlefieldStatus((*itx)->m_loggedInPlayer, 1, BattlegroundType, 0 , 0, 0,1);
					(*itx)->m_loggedInPlayer->m_bgIsQueued = true;
					(*itx)->m_loggedInPlayer->m_bgQueueInstanceId = 0;
					(*itx)->m_loggedInPlayer->m_bgQueueType = BattlegroundType;
					(*itx)->m_loggedInPlayer->GetSession()->SendPacket(&data);
					(*itx)->m_loggedInPlayer->m_bgEntryPointX=(*itx)->m_loggedInPlayer->GetPositionX();
					(*itx)->m_loggedInPlayer->m_bgEntryPointY=(*itx)->m_loggedInPlayer->GetPositionY();
					(*itx)->m_loggedInPlayer->m_bgEntryPointZ=(*itx)->m_loggedInPlayer->GetPositionZ();
					(*itx)->m_loggedInPlayer->m_bgEntryPointMap=(*itx)->m_loggedInPlayer->GetMapId();
				}
			}

			pGroup->Unlock();

			m_queueLock.Acquire();
			m_queuedGroups[BattlegroundType].push_back(pGroup->GetID());
			m_queueLock.Release();
			Log.Success("BattlegroundMgr", "Group %u is now in battleground queue for arena type %u", pGroup->GetID(), BattlegroundType);

			/* send the battleground status packet */

			return;
		}
	}


	/* Queue him! */
	m_queueLock.Acquire();
	m_queuedPlayers[BattlegroundType][lgroup].push_back(pguid);
	Log.Success("BattlegroundMgr", "Player %u is now in battleground queue for {Arena %u}", m_session->GetPlayer()->GetLowGUID(), BattlegroundType );

	/* send the battleground status packet */
	SendBattlefieldStatus(m_session->GetPlayer(), 1, BattlegroundType, 0 , 0, 0,0);
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

bool CBattleground::CanPlayerJoin(Player * plr, uint32 type)
{
	return HasFreeSlots(plr->m_bgTeam,type)&&(GetLevelGrouping(plr->getLevel())==GetLevelGroup())&&(!plr->HasAura(BG_DESERTER));
}
