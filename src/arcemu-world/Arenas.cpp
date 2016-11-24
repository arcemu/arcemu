/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
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

#define ARENA_PREPARATION 32727

#define GREEN_TEAM 0
#define GOLD_TEAM 1

Arena::Arena(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for(i = 0; i < 2; i++)
	{
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}

	m_pvpData.clear();
	m_resurrectMap.clear();

	m_started = false;
	m_playerCountPerTeam = players_per_side;
	m_buffs[0] = m_buffs[1] = NULL;
	m_playersCount[0] = m_playersCount[1] = 0;
	m_teams[0] = m_teams[1] = NULL;
	switch(t)
	{
		case BATTLEGROUND_ARENA_5V5:
			m_arenateamtype = 2;
			break;

		case BATTLEGROUND_ARENA_3V3:
			m_arenateamtype = 1;
			break;

		case BATTLEGROUND_ARENA_2V2:
			m_arenateamtype = 0;
			break;

		default:
			m_arenateamtype = 0;
			break;
	}
	rated_match = false;


	switch( m_mapMgr->GetMapId() ){
		case 559:
			m_zoneid = 3698;
			break;

		case 562:
			m_zoneid = 3702;
			break;

		case 572:
			m_zoneid = 3968;
			break;

		case 617:
			m_zoneid = 4378;
			break;

		case 618:
			m_zoneid = 4408;
			break;

	}
}

Arena::~Arena()
{
	int i;

	for(i = 0; i < 2; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && m_buffs[i]->IsInWorld() == false)
			delete m_buffs[i];
	}


	for(set<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		if((*itr) != NULL)
		{
			if(!(*itr)->IsInWorld())
				delete(*itr);
		}
	}

}

void Arena::OnAddPlayer(Player* plr)
{
	if(plr == NULL)
		return;

	plr->m_deathVision = true;

	// remove all buffs (exclude talents, include flasks)
	for(uint32 x = MAX_REMOVABLE_AURAS_START; x < MAX_REMOVABLE_AURAS_END; x++)
	{
		if(plr->m_auras[x])
		{
			if(plr->m_auras[x] && !plr->m_auras[x]->GetSpellProto()->DurationIndex && plr->m_auras[x]->GetSpellProto()->AttributesExC & CAN_PERSIST_AND_CASTED_WHILE_DEAD)
				continue;
			else
			{
				plr->m_auras[x]->Remove();
			}
		}
	}
	// On arena start all conjured items are removed
	plr->GetItemInterface()->RemoveAllConjured();
	// On arena start remove all temp enchants
	plr->RemoveTempEnchantsOnArena();

	// Before the arena starts all your cooldowns are reset
	if(!m_started  && plr->IsInWorld())
		plr->ResetAllCooldowns();

	// if( plr->m_isGmInvisible == false )
	// Make sure the player isn't a GM an isn't invisible (monitoring?)
	if(!plr->m_isGmInvisible)
	{
		if(!m_started  && plr->IsInWorld())
			plr->CastSpell(plr, ARENA_PREPARATION, true);

		m_playersCount[plr->GetTeam()]++;
		UpdatePlayerCounts();
	}
	// If they're still queued for the arena, remove them from the queue
	if(plr->m_bgIsQueued)
		plr->m_bgIsQueued = false;

	/* Add the green/gold team flag */
	Aura* aura = sSpellFactoryMgr.NewAura(dbcSpell.LookupEntry((plr->GetTeamInitial()) ? 35775 - plr->m_bgTeam : 32725 - plr->m_bgTeam), -1, plr, plr, true);
	plr->AddAura(aura);

	/* Set FFA PvP Flag */
	plr->SetFFAPvPFlag();

	m_playersAlive.insert(plr->GetLowGUID());
}

void Arena::OnRemovePlayer(Player* plr)
{
	/* remove arena readiness buff */
	plr->m_deathVision = false;

	// All auras are removed on exit
	// plr->RemoveAura(ARENA_PREPARATION);
	plr->RemoveAllAuras();

	// Player has left arena, call HookOnPlayerDeath as if he died
	HookOnPlayerDeath(plr);

	plr->RemoveAura(plr->GetTeamInitial() ? 35775 - plr->m_bgTeam : 32725 - plr->m_bgTeam);
	plr->RemoveFFAPvPFlag();

	// Reset all their cooldowns and restore their HP/Mana/Energy to max
	plr->ResetAllCooldowns();
	plr->FullHPMP();
}

void Arena::HookOnPlayerKill(Player* plr, Player* pVictim)
{
#ifdef ANTI_CHEAT
	if(!m_started)
	{
		plr->KillPlayer(); //cheater.
		return;
	}
#endif
	if(pVictim->IsPlayer())
	{
		plr->m_bgScore.KillingBlows++;
	}
}

void Arena::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
}

void Arena::HookOnPlayerDeath(Player* plr)
{
	ARCEMU_ASSERT(plr != NULL);

	if(plr->m_isGmInvisible == true) return;

	if(m_playersAlive.find(plr->GetLowGUID()) != m_playersAlive.end())
	{
		m_playersCount[plr->GetTeam()]--;
		UpdatePlayerCounts();
		m_playersAlive.erase(plr->GetLowGUID());
	}
}

void Arena::OnCreate()
{
	/* push gates into world */
	for(set<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
		(*itr)->PushToWorld(m_mapMgr);

	/*Conflict added in commit https://github.com/arcemu/arcemu/commit/26e5bc46a8d47c89287405a78aa2b3cc9cd71e26#diff-f518273db965706d6a2ad12fe79de129R410
	GameObject * obj;
	switch (m_mapMgr->GetMapId())
	{
		// ruins of lordaeron 
	case 572: {
				  obj = SpawnGameObject(185917, 572, 1278.647705f, 1730.556641f, 31.605574f, 1.684245f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.746058f);
				  obj->SetParentRotation(3, 0.665881f);
				  m_gates.insert(obj);
				  obj = SpawnGameObject(185918, 572, 1293.560791f, 1601.937988f, 31.605574f, -1.457349f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, -0.665881f);
				  obj->SetParentRotation(3, 0.746058f);
				  m_gates.insert(obj);
	}break;
		// blades edge arena 
	case 562: {
				  obj = SpawnGameObject(183972, 562, 6177.707520f, 227.348145f, 3.604374f, -2.260201f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.90445f);
				  obj->SetParentRotation(3, -0.426569f);
				  obj->PushToWorld(m_mapMgr);
				  obj = SpawnGameObject(183973, 562, 6189.546387f, 241.709854f, 3.101481f, 0.881392f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.426569f);
				  obj->SetParentRotation(3, 0.904455f);
				  m_gates.insert(obj);
				  obj = SpawnGameObject(183970, 562, 6299.115723f, 296.549438f, 3.308032f, 0.881392f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.426569f);
				  obj->SetParentRotation(3, 0.904455f);
				  obj->PushToWorld(m_mapMgr);
				  obj = SpawnGameObject(183971, 562, 6287.276855f, 282.187714f, 3.810925f, -2.260201f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.904455f);
				  obj->SetParentRotation(3, -0.426569f);
				  m_gates.insert(obj);
	}break;
		// nagrand arena 
	case 559: {
				  obj = SpawnGameObject(183979, 559, 4090.064453f, 2858.437744f, 10.236313f, 0.492805f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.243916f);
				  obj->SetParentRotation(3, 0.969796f);
				  obj->PushToWorld(m_mapMgr);
				  obj = SpawnGameObject(183980, 559, 4081.178955f, 2874.970459f, 12.391714f, 0.492805f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.243916f);
				  obj->SetParentRotation(3, 0.969796f);
				  m_gates.insert(obj);
				  obj = SpawnGameObject(183977, 559, 4023.709473f, 2981.776611f, 10.701169f, -2.648788f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.969796f);
				  obj->SetParentRotation(3, -0.243916f);
				  obj->PushToWorld(m_mapMgr);
				  obj = SpawnGameObject(183978, 559, 4031.854248f, 2966.833496f, 12.646200f, -2.648788f, 32, 1375, 1.0f);
				  obj->SetState(GAMEOBJECT_STATE_CLOSED);
				  obj->SetParentRotation(2, 0.969796f);
				  obj->SetParentRotation(3, -0.243916f);
				  m_gates.insert(obj);
	}break;
	}
	// push gates into world 
	for (set<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
		(*itr)->PushToWorld(m_mapMgr);
	SetWorldState(0x08D4, 0x0000);
	SetWorldState(0x08D8, 0x0000);
	SetWorldState(0x08D7, 0x0000);
	SetWorldState(0x08D6, 0x0000);
	SetWorldState(0x08D5, 0x0000);
	SetWorldState(0x08D3, 0x0000);
	SetWorldState(0x0C0D, 0x017B);
	// Show players count
	switch (m_mapMgr->GetMapId())
	{
		// ruins of lordaeron 
	case 572:
		SetWorldState(3002, 1);
		break;
		// blades edge arena 
	case 562:
		SetWorldState(2547, 1);
		break;
		// nagrand arena
	case 559:
		SetWorldState(2577, 1);
		break;
	}*/
}

void Arena::HookOnShadowSight()
{
}

void Arena::OnStart()
{
	int i;

	/* remove arena readiness buff */
	for(i = 0; i < 2; ++i)
	{
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
		{
			Player* plr = *itr;
			plr->RemoveAura(ARENA_PREPARATION);
			m_players2[i].insert(plr->GetLowGUID());

			/* update arena team stats */
			if(rated_match && plr->m_arenaTeams[m_arenateamtype] != NULL)
			{
				m_teams[i] = plr->m_arenaTeams[m_arenateamtype];
				ArenaTeamMember* tp = m_teams[i]->GetMember(plr->getPlayerInfo());
				if(tp != NULL)
				{
					tp->Played_ThisWeek++;
					tp->Played_ThisSeason++;
				}
			}
		}
	}

	for(i = 0; i < 2; i++)
	{
		if(m_teams[i] == NULL) continue;

		m_teams[i]->m_stat_gamesplayedseason++;
		m_teams[i]->m_stat_gamesplayedweek++;
		m_teams[i]->SaveToDB();
	}

	/* open gates */
	for(set<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetFlags(64);
		(*itr)->SetState(GAMEOBJECT_STATE_CLOSED);
	}

	m_started = true;

	/* Incase all players left */
	UpdatePlayerCounts();

	// WHEEEE
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	sEventMgr.RemoveEvents(this, EVENT_ARENA_SHADOW_SIGHT);
	sEventMgr.AddEvent(TO< CBattleground* >(this), &CBattleground::HookOnShadowSight, EVENT_ARENA_SHADOW_SIGHT, 90000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
}

void Arena::UpdatePlayerCounts()
{
	if(m_ended)
		return;
	
	SetWorldState(WORLDSTATE_ARENA__GREEN_PLAYER_COUNT, m_playersCount[0]);
	SetWorldState(WORLDSTATE_ARENA__GOLD_PLAYER_COUNT, m_playersCount[1]);

	if(!m_started)
		return;

//	return;

	if(m_playersCount[1] == 0)
		m_winningteam = 0;
	else if(m_playersCount[0] == 0)
		m_winningteam = 1;
	else
		return;

	Finish();
}

uint32 Arena::CalcDeltaRating(uint32 oldRating, uint32 opponentRating, bool outcome)
{
	// ---- Elo Rating System ----
	// Expected Chance to Win for Team A vs Team B
	//                     1
	// -------------------------------------------
	//                   (PB - PA)/400
	//              1 + 10

	double power = (int)(opponentRating - oldRating) / 400.0f;
	double divisor = pow(((double)(10.0)), power);
	divisor += 1.0;

	double winChance = 1.0 / divisor;

	// New Rating Calculation via Elo
	// New Rating = Old Rating + K * (outcome - Expected Win Chance)
	// outcome = 1 for a win and 0 for a loss (0.5 for a draw ... but we cant have that)
	// K is the maximum possible change
	// Through investigation, K was estimated to be 32 (same as chess)
	double multiplier = (outcome ? 1.0 : 0.0) - winChance;
	return long2int32(32.0 * multiplier);
}

uint32 Arena::GetTeamFaction( uint32 team )
{
	std::set< Player* >::iterator itr = m_players[ team ].begin();
	Player *p = *itr;
	return p->GetTeam();
}

void Arena::Finish()
{
	m_ended = true;

	/* update arena team stats */
	if(rated_match)
	{
		m_deltaRating[0] = m_deltaRating[1] = 0;
		for(uint32 i = 0; i < 2; ++i)
		{
			uint32 j = i ? 0 : 1; // opposing side
			bool outcome;

			if(m_teams[i] == NULL || m_teams[j] == NULL)
				continue;

			outcome = (i == m_winningteam);
			if(outcome)
			{
				m_teams[i]->m_stat_gameswonseason++;
				m_teams[i]->m_stat_gameswonweek++;
			}

			m_deltaRating[i] = CalcDeltaRating(m_teams[i]->m_stat_rating, m_teams[j]->m_stat_rating, outcome);
			m_teams[i]->m_stat_rating += m_deltaRating[i];
			if(static_cast<int32>(m_teams[i]->m_stat_rating) < 0) m_teams[i]->m_stat_rating = 0;

			for(set<uint32>::iterator itr = m_players2[i].begin(); itr != m_players2[i].end(); ++itr)
			{
				PlayerInfo* info = objmgr.GetPlayerInfo(*itr);
				if(info)
				{
					ArenaTeamMember* tp = m_teams[i]->GetMember(info);

					if(tp != NULL)
					{
						tp->PersonalRating += CalcDeltaRating(tp->PersonalRating, m_teams[j]->m_stat_rating, outcome);
						if(static_cast<int32>(tp->PersonalRating) < 0)
							tp->PersonalRating = 0;

						if(outcome)
						{
							++(tp->Won_ThisWeek);
							++(tp->Won_ThisSeason);
						}
					}
				}
			}

			m_teams[i]->SaveToDB();
		}
	}

	objmgr.UpdateArenaTeamRankings();

	m_nextPvPUpdateTime = 0;
	UpdatePvPData();
	PlaySoundToAll(m_winningteam ? SOUND_ALLIANCEWINS : SOUND_HORDEWINS);

	sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
	sEventMgr.AddEvent(TO< CBattleground* >(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	for(int i = 0; i < 2; i++)
	{
		bool victorious = (i == m_winningteam);
		set<Player*>::iterator itr = m_players[i].begin();
		for(; itr != m_players[i].end(); itr++)
		{
			Player* plr = (Player*)(*itr);
			if(plr != NULL)
			{
				sHookInterface.OnArenaFinish(plr, plr->m_arenaTeams[m_arenateamtype], victorious, rated_match);
				plr->ResetAllCooldowns();
			}
		}
	}
}

LocationVector Arena::GetStartingCoords(uint32 Team)
{
	return LocationVector(0, 0, 0, 0);
}

bool Arena::HookHandleRepop(Player* plr)
{
	return false;
}

void Arena::HookOnAreaTrigger(Player* plr, uint32 id)
{
	int32 buffslot = -1;

	ARCEMU_ASSERT(plr != NULL);

	switch(id)
	{
		case 4536:
		case 4538:
		case 4696:
			buffslot = 0;
			break;
		case 4537:
		case 4539:
		case 4697:
			buffslot = 1;
			break;
	}

	if(buffslot >= 0)
	{
		if(m_buffs[buffslot] != NULL && m_buffs[buffslot]->IsInWorld())
		{
			/* apply the buff */
			SpellEntry* sp = dbcSpell.LookupEntryForced(m_buffs[buffslot]->GetInfo()->raw.sound3);
			ARCEMU_ASSERT(sp != NULL);

			Spell* s = sSpellFactoryMgr.NewSpell(plr, sp, true, 0);
			SpellCastTargets targets(plr->GetGUID());
			s->prepare(&targets);

			/* despawn the gameobject (not delete!) */
			m_buffs[buffslot]->Despawn(0, 30*1000 /*BUFF_RESPAWN_TIME*/);
		}
	}
}
void Arena::HookGenerateLoot(Player* plr, Object* pCorpse)    // Not Used
{
}

void Arena::HookOnUnitKill(Player* plr, Unit* pVictim)
{
}

void Arena::HookOnFlagDrop(Player* plr)
{
}
