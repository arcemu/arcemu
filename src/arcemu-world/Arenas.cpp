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

#define ARENA_PREPARATION 32727
#define GREEN_TEAM 0
#define GOLD_TEAM 1

Arena::Arena(MapMgr * mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for (i=0; i<2; i++) {
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}
	m_worldStates.clear();
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
		m_arenateamtype=2;
		break;

	case BATTLEGROUND_ARENA_3V3:
		m_arenateamtype=1;
		break;
		
	case BATTLEGROUND_ARENA_2V2:
		m_arenateamtype=0;
		break;

	default:
		m_arenateamtype=0;
		break;
	}
	rated_match=false;
}

Arena::~Arena()
{
	for(uint32 i = 0; i < 2; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && m_buffs[i]->IsInWorld()==false)
			delete m_buffs[i];
	}
}

void Arena::OnAddPlayer(Player * plr)
{
	if (plr == NULL) return;

	plr->m_deathVision = true;

	// remove all buffs (exclude talents, include flasks)
	for(uint32 x=MAX_REMOVABLE_AURAS_START;x<MAX_REMOVABLE_AURAS_END;x++)
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
	plr->GetItemInterface()->RemoveAllConjured();
	if( !m_started )
	{
		plr->ResetAllCooldowns();
	}

	if( plr->m_isGmInvisible == false )
	{
		if( !m_started )
			plr->CastSpell(plr, ARENA_PREPARATION, true);
		m_playersCount[plr->GetTeam()]++;
		UpdatePlayerCounts();
	}

	if (plr->m_bgIsQueued)
		plr->m_bgIsQueued = false;

	/* Add the green/gold team flag */
	Aura * aura = AuraPool.PooledNew();
	aura->Init(dbcSpell.LookupEntry((plr->GetTeamInitial()) ? 35775-plr->m_bgTeam : 32725-plr->m_bgTeam), -1, plr, plr);
	plr->AddAura(aura);
	
	/* Set FFA PvP Flag */
	if(!plr->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP))
		plr->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);

	m_playersAlive.insert(plr->GetLowGUID());
}

void Arena::OnRemovePlayer(Player * plr)
{
	/* remove arena readyness buff */
	plr->m_deathVision = false;
	plr->RemoveAura(ARENA_PREPARATION);

	/* plr left arena, call HookOnPlayerDeath as if he died */
	HookOnPlayerDeath(plr);

	plr->RemoveAura(plr->GetTeamInitial() ? 35775-plr->m_bgTeam : 32725-plr->m_bgTeam);
	if(plr->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP))
		plr->RemoveFlag(PLAYER_FLAGS, PLAYER_FLAG_FREE_FOR_ALL_PVP);
	
}

void Arena::HookOnPlayerKill(Player * plr, Unit * pVictim)
{
	if ( pVictim->IsPlayer() )
	{
		plr->m_bgScore.KillingBlows++;
	}
}

void Arena::HookOnHK(Player * plr)
{
	plr->m_bgScore.HonorableKills++;
}

void Arena::HookOnPlayerDeath(Player * plr)
{
	ASSERT(plr != NULL);

	if( plr->m_isGmInvisible == true ) return;

	if (m_playersAlive.find(plr->GetLowGUID()) != m_playersAlive.end())
	{
		m_playersCount[plr->GetTeam()]--;
		UpdatePlayerCounts();
		m_playersAlive.erase(plr->GetLowGUID());
	}
}

void Arena::OnCreate()
{
	GameObject * obj;
	switch(m_mapMgr->GetMapId())
	{
		/* ruins of lordaeron */
	case 572: {
		obj = SpawnGameObject(185917, 572, 1278.647705f, 1730.556641f, 31.605574f, 1.684245f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.746058f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, 0.665881f);
		m_gates.insert(obj);

		obj = SpawnGameObject(185918, 572, 1293.560791f, 1601.937988f, 31.605574f, -1.457349f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, -0.665881f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, 0.746058f);
		m_gates.insert(obj);

			  }break;

		/* blades edge arena */
	case 562: {
		obj = SpawnGameObject(183972, 562, 6177.707520f, 227.348145f, 3.604374f, -2.260201f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.90445f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		obj->PushToWorld(m_mapMgr);

		obj = SpawnGameObject(183973, 562, 6189.546387f, 241.709854f, 3.101481f, 0.881392f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.426569f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, 0.904455f);
		m_gates.insert(obj);

		obj = SpawnGameObject(183970, 562, 6299.115723f, 296.549438f, 3.308032f, 0.881392f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.426569f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, 0.904455f);
		obj->PushToWorld(m_mapMgr);

		obj = SpawnGameObject(183971, 562, 6287.276855f, 282.187714f, 3.810925f, -2.260201f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.904455f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_gates.insert(obj);
			  }break;

		/* nagrand arena */
	case 559: {
		obj = SpawnGameObject(183979, 559, 4090.064453f, 2858.437744f, 10.236313f, 0.492805f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.243916f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, 0.969796f);
		obj->PushToWorld(m_mapMgr);

		obj = SpawnGameObject(183980, 559, 4081.178955f, 2874.970459f, 12.391714f, 0.492805f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.243916f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, 0.969796f);
		m_gates.insert(obj);

		obj = SpawnGameObject(183977, 559, 4023.709473f, 2981.776611f, 10.701169f, -2.648788f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.969796f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.243916f);
		obj->PushToWorld(m_mapMgr);

		obj = SpawnGameObject(183978, 559, 4031.854248f, 2966.833496f, 12.646200f, -2.648788f, 32, 1375, 1.0f);
		obj->SetUInt32Value(GAMEOBJECT_STATE, 1);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.969796f);
		obj->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.243916f);
		m_gates.insert(obj);

			  }break;
	}

	/* push gates into world */
	for(set<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
		(*itr)->PushToWorld(m_mapMgr);

	SetWorldState(0x08D4	,0x0000);
	SetWorldState(0x08D8	,0x0000);
	SetWorldState(0x08D7	,0x0000);
	SetWorldState(0x08D6	,0x0000);
	SetWorldState(0x08D5	,0x0000);
	SetWorldState(0x08D3	,0x0000);
	SetWorldState(0x0C0D	,0x017B);

	// Show players count
	switch(m_mapMgr->GetMapId())
	{
		/* ruins of lordaeron */
	case 572:
		SetWorldState(3002, 1);
		break;

		/* blades edge arena */
	case 562:
		SetWorldState(2547, 1);
		break;

		/* nagrand arena */
	case 559:
		SetWorldState(2577, 1);
		break;
	}
}

void Arena::HookOnShadowSight()
{
	switch(m_mapMgr->GetMapId())
	{
		/* ruins of lordaeron */
	case 572:
		m_buffs[0] = SpawnGameObject(184664, 572, 1328.729268f, 1632.738403f, 34.838585f, 2.611449f, 32, 1375, 1.0f);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		m_buffs[0]->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.904455f);
		m_buffs[0]->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		m_buffs[0]->PushToWorld(m_mapMgr);

		m_buffs[1] = SpawnGameObject(184664, 572, 1243.306763f, 1699.334351f, 34.837566f, 5.713773f, 32, 1375, 1.0f);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		m_buffs[1]->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.90445f);
		m_buffs[1]->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		m_buffs[1]->PushToWorld(m_mapMgr);
		break;

		/* blades edge arena */
	case 562:
		m_buffs[0] = SpawnGameObject(184664, 562, 6249.276855f, 275.187714f, 11.201481f, -2.260201f, 32, 1375, 1.0f);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		m_buffs[0]->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.904455f);
		m_buffs[0]->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		m_buffs[0]->PushToWorld(m_mapMgr);

		m_buffs[1] = SpawnGameObject(184664, 562, 6228.546387f, 249.709854f, 11.201481f, 0.881392f, 32, 1375, 1.0f);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		m_buffs[1]->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.90445f);
		m_buffs[1]->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		m_buffs[1]->PushToWorld(m_mapMgr);
		break;

		/* nagrand arena */
	case 559:
		m_buffs[0] = SpawnGameObject(184664, 559, 4011.113232f, 2896.879980f, 12.523950f, 0.486944f, 32, 1375, 1.0f);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		m_buffs[0]->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.904455f);
		m_buffs[0]->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		m_buffs[0]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		m_buffs[0]->PushToWorld(m_mapMgr);

		m_buffs[1] = SpawnGameObject(184664, 559, 4102.111426f, 2945.843262f, 12.662578f, 3.628544f, 32, 1375, 1.0f);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_STATE, 1);
		m_buffs[1]->SetFloatValue(GAMEOBJECT_ROTATION_02, 0.90445f);
		m_buffs[1]->SetFloatValue(GAMEOBJECT_ROTATION_03, -0.426569f);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_TYPE_ID, 6);
		m_buffs[1]->SetUInt32Value(GAMEOBJECT_ANIMPROGRESS, 100);
		m_buffs[1]->PushToWorld(m_mapMgr);
		break;
	}
}

void Arena::OnStart()
{
	int i;

	/* remove arena readyness buff */
	for(i = 0; i < 2; ++i) {
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr) {
			Player *plr = *itr;
			plr->RemoveAura(ARENA_PREPARATION);
			m_players2[i].insert(plr->GetLowGUID());

			/* update arena team stats */
			if(rated_match && plr->m_arenaTeams[m_arenateamtype] != NULL)
			{
				m_teams[i] = plr->m_arenaTeams[m_arenateamtype];
				ArenaTeamMember * tp = m_teams[i]->GetMember(plr->m_playerInfo);
				if(tp != NULL)
				{
					tp->Played_ThisWeek++;
					tp->Played_ThisSeason++;
				}
			}
		}
	}

	for (i=0; i<2; i++) {
		if (m_teams[i] == NULL) continue;

		m_teams[i]->m_stat_gamesplayedseason++;
		m_teams[i]->m_stat_gamesplayedweek++;
		m_teams[i]->SaveToDB();
	}

	/* open gates */
	for(set<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
		(*itr)->SetUInt32Value(GAMEOBJECT_STATE, 0);
	}

	m_started = true;

	/* Incase all players left */
	UpdatePlayerCounts();

	// WHEEEE
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	sEventMgr.RemoveEvents(this, EVENT_ARENA_SHADOW_SIGHT);
	sEventMgr.AddEvent(((CBattleground*)this), &CBattleground::HookOnShadowSight, EVENT_ARENA_SHADOW_SIGHT, 90000, 1,0);
}

void Arena::UpdatePlayerCounts()
{
	if(m_ended)
		return;

	switch(m_mapMgr->GetMapId())
	{
		/* ruins of lordaeron */
	case 572:
		SetWorldState(3000, m_playersCount[0]);
		SetWorldState(3001, m_playersCount[1]);
		break;

		/* blades edge arena */
	case 562:
		SetWorldState(2544, m_playersCount[0]);
		SetWorldState(2545, m_playersCount[1]);
		break;

		/* nagrand arena */
	case 559:
		SetWorldState(2575, m_playersCount[0]);
		SetWorldState(2576, m_playersCount[1]);
		break;
	}

	if(!m_started)
		return;

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

void Arena::Finish()
{
	m_ended = true;

	/* update arena team stats */
	if(rated_match)
	{
		m_deltaRating[0] = m_deltaRating[1] = 0;
		for (uint32 i = 0; i < 2; ++i) {
			uint32 j = i ? 0 : 1; // opposing side
			bool outcome;

			if (m_teams[i] == NULL || m_teams[j] == NULL) continue;

			outcome = (i == m_winningteam);
			if (outcome) {
				m_teams[i]->m_stat_gameswonseason++;
				m_teams[i]->m_stat_gameswonweek++;
			}

			m_deltaRating[i] = CalcDeltaRating(m_teams[i]->m_stat_rating, m_teams[j]->m_stat_rating, outcome);
			m_teams[i]->m_stat_rating += m_deltaRating[i];
			if (m_teams[i]->m_stat_rating < 0) m_teams[i]->m_stat_rating = 0;

			for(set<uint32>::iterator itr = m_players2[i].begin(); itr != m_players2[i].end(); ++itr) {
				PlayerInfo * info = objmgr.GetPlayerInfo(*itr);
				if (info) {
					ArenaTeamMember * tp = m_teams[i]->GetMember(info);

					if(tp != NULL) {
						tp->PersonalRating += CalcDeltaRating(tp->PersonalRating, m_teams[j]->m_stat_rating, outcome);
						if (tp->PersonalRating < 0) tp->PersonalRating = 0;

						if(outcome) {
							tp->Won_ThisWeek++;
							tp->Won_ThisSeason++;
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
	sEventMgr.AddEvent(((CBattleground*)this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1,0);

	for(int i = 0; i < 2; i++)
	{
		bool victorious = (i == m_winningteam);
		set<Player*>::iterator itr = m_players[i].begin();
		for(; itr != m_players[i].end(); itr++)
		{
			Player * plr = (Player *)(*itr);
			if (plr != NULL)
				sHookInterface.OnArenaFinish(plr, plr->m_arenaTeams[m_arenateamtype], victorious, rated_match);
		}
	}
}

LocationVector Arena::GetStartingCoords(uint32 Team)
{
	// 559, 562, 572
	/*
	A start
	H start
	Repop
	572 1295.322388 1585.953369 31.605387
	572 1277.105103 1743.956177 31.603209
	572 1286.112061 1668.334961 39.289127

	562 6184.806641 236.643463 5.037095
	562 6292.032227 287.570343 5.003577
	562 6241.171875 261.067322 0.891833

	559 4085.861328 2866.750488 12.417445
	559 4027.004883 2976.964844 11.600499
	559 4057.042725 2918.686523 13.051933
	*/
	switch(m_mapMgr->GetMapId())
	{
		/* loraedeon */
	case 572:
		{
			if(Team)
				return LocationVector(1277.105103f, 1743.956177f, 31.603209f);
			else
				return LocationVector(1295.322388f, 1585.953369f, 31.605387f);
		}break;

		/* blades edge arena */
	case 562:
		{
			if(Team)
				return LocationVector(6292.032227f, 287.570343f, 5.003577f);
			else
				return LocationVector(6184.806641f, 236.643463f, 5.037095f);
		}break;

		/* nagrand arena */
	case 559:
		{
			if(Team)
				return LocationVector(4027.004883f, 2976.964844f, 11.600499f);
			else
				return LocationVector(4085.861328f, 2866.750488f, 12.417445f);
		}break;
	}

	return LocationVector(0,0,0,0);
}

bool Arena::HookHandleRepop(Player * plr)
{
	// 559, 562, 572
	/*
	A start
	H start
	Repop
	572 1295.322388 1585.953369 31.605387
	572 1277.105103 1743.956177 31.603209
	572 1286.112061 1668.334961 39.289127

	562 6184.806641 236.643463 5.037095
	562 6292.032227 287.570343 5.003577
	562 6241.171875 261.067322 0.891833

	559 4085.861328 2866.750488 12.417445
	559 4027.004883 2976.964844 11.600499
	559 4057.042725 2918.686523 13.051933
	*/
	LocationVector dest(0,0,0,0);
	switch(m_mapMgr->GetMapId())
	{
		/* loraedeon */
	case 572: {
			dest.ChangeCoords(1286.112061f, 1668.334961f, 39.289127f);
		}break;

		/* blades edge arena */
	case 562: {
			dest.ChangeCoords(6241.171875f, 261.067322f, 0.891833f);
		}break;

		/* nagrand arena */
	case 559: {
			dest.ChangeCoords(4057.042725f, 2918.686523f, 13.051933f);
		}break;
	}

	plr->SafeTeleport(m_mapMgr->GetMapId(), m_mapMgr->GetInstanceID(), dest);
	return true;
}

void Arena::HookOnAreaTrigger(Player * plr, uint32 id)
{
	int32 buffslot = -1;

	ASSERT(plr != NULL);

	switch (id) 
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
			SpellEntry * sp = dbcSpell.LookupEntry(m_buffs[buffslot]->GetInfo()->sound3);
			Spell * s = SpellPool.PooledNew();

			ASSERT(sp != NULL);
			ASSERT(s != NULL);

			s->Init(plr, sp, true, 0);
			SpellCastTargets targets(plr->GetGUID());
			s->prepare(&targets);

			/* despawn the gameobject (not delete!) */
			m_buffs[buffslot]->Despawn(BUFF_RESPAWN_TIME);
		}
	}
}
