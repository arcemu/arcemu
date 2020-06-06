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

