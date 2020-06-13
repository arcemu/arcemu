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
#include "WarsongGulch.h"


WarsongGulch::WarsongGulch(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t) : CBattleground(mgr, id, lgroup, t)
{
	int i;

	for(i = 0; i < 2; i++)
	{
		m_players[i].clear();
		m_pendPlayers[i].clear();
	}

	m_pvpData.clear();
	m_resurrectMap.clear();

	m_flagHolders[0] = m_flagHolders[1] = 0;
	m_lgroup = lgroup;

	/* create the buffs */
	for(i = 0; i < 6; ++i)
		SpawnBuff(i);

	/* take note: these are swapped around for performance bonus */
	// warsong flag - horde base
	m_homeFlags[0] = SpawnGameObject(179831, 489, 915.367f, 1433.78f, 346.089f, 3.17301f, 0, 210, 2.5f);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
	m_homeFlags[0]->SetType(GAMEOBJECT_TYPE_FLAGSTAND);
	m_homeFlags[0]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);

	// silverwing flag - alliance base
	m_homeFlags[1] = SpawnGameObject(179830, 489, 1540.29f, 1481.34f, 352.64f, 3.17301f, 0, 1314, 2.5f);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
	m_homeFlags[1]->SetType(GAMEOBJECT_TYPE_FLAGSTAND);
	m_homeFlags[1]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);

	// dropped flags
	m_dropFlags[1] = m_mapMgr->CreateGameObject(179786);
	if(!m_dropFlags[1]->CreateFromProto(179785, 489, 0, 0, 0, 0))
		Log.Warning("WarsongGulch", "Could not create dropped flag 1");

	m_dropFlags[0] = m_mapMgr->CreateGameObject(179786);
	if(!m_dropFlags[0]->CreateFromProto(179786, 489, 0, 0, 0, 0))
		Log.Warning("WarsongGulch", "Could not create dropped flag 0");

	for(i = 0; i < 2; ++i)
	{
		m_dropFlags[i]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
		m_dropFlags[i]->SetScale(2.5f);
	}

	m_scores[0] = m_scores[1] = 0;

	m_zoneid = 3277;

}

WarsongGulch::~WarsongGulch()
{
	// gates are always spawned, so mapmgr will clean them up
	for(uint32 i = 0; i < 6; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && m_buffs[i]->IsInWorld() == false)
			delete m_buffs[i];
	}

	for(uint32 i = 0; i < 2; ++i)
	{
		if(m_dropFlags[i] && m_dropFlags[i]->IsInWorld() == false)
			delete m_dropFlags[i];

		if(m_homeFlags[i] && m_homeFlags[i]->IsInWorld() == false)
			delete m_homeFlags[i];
	}

	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		if((*itr) != NULL)
		{
			if(!(*itr)->IsInWorld())
				delete(*itr);
		}
	}

	m_resurrectMap.clear();

}

void WarsongGulch::HookOnAreaTrigger(Player* plr, uint32 id)
{
	int32 buffslot = -1;
	switch(id)
	{
		case 3686:	  // Speed
			buffslot = 0;
			break;
		case 3687:	  // Speed (Horde)
			buffslot = 1;
			break;
		case 3706:	  // Restoration
			buffslot = 2;
			break;
		case 3708:	  // Restoration (Horde)
			buffslot = 3;
			break;
		case 3707:	  // Berserking
			buffslot = 4;
			break;
		case 3709:	  // Berserking (Horde)
			buffslot = 5;
			break;
	}

	if(buffslot >= 0)
	{
		if(m_buffs[buffslot] != 0 && m_buffs[buffslot]->IsInWorld())
		{
			/* apply the buff */
			SpellEntry* sp = dbcSpell.LookupEntry(m_buffs[buffslot]->GetInfo()->sound3);
			Spell* s = sSpellFactoryMgr.NewSpell(plr, sp, true, 0);
			SpellCastTargets targets(plr->GetGUID());
			s->prepare(&targets);

			/* despawn the gameobject (not delete!) */
			m_buffs[buffslot]->Despawn(0, BUFF_RESPAWN_TIME);
		}
		return;
	}

	if(((id == 3646 && plr->IsTeamAlliance()) || (id == 3647 && plr->IsTeamHorde())) && (plr->m_bgHasFlag && m_flagHolders[plr->GetTeam()] == plr->GetLowGUID()))
	{
		if(m_flagHolders[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE] != 0 || m_dropFlags[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE]->IsInWorld())
		{
			/* can't cap while flag dropped */
			return;
		}
		float distance = plr->IsTeamAlliance() ? plr->CalcDistance(1540.29f, 1481.34f, 352.64f) : plr->CalcDistance(915.367f, 1433.78f, 346.089f);
		if(distance > 50.0f)
		{
			//50 yards from the spawn, gtfo hacker.
			sCheatLog.writefromsession(plr->GetSession(), "Tried to capture the flag in WSG while being more then 50 yards away. (%f yards)", plr->CalcDistance(915.367f, 1433.78f, 346.089f));
			plr->GetSession()->Disconnect();
			return;
		}

		/* remove the bool from the player so the flag doesn't drop */
		m_flagHolders[plr->GetTeam()] = 0;
		plr->m_bgHasFlag = 0;

		/* remove flag aura from player */
		plr->RemoveAura(23333 + (plr->GetTeam() * 2));

		/* capture flag points */
		plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_CAPTURED]++;

		PlaySoundToAll(plr->IsTeamHorde() ? SOUND_HORDE_SCORES : SOUND_ALLIANCE_SCORES);

		if(plr->IsTeamHorde())
			SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "%s captured the Alliance flag!", plr->GetName());
		else
			SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "%s captured the Horde flag!", plr->GetName());

		SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);

		// Remove the Other Flag
		if(m_homeFlags[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE]->IsInWorld())
			m_homeFlags[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE]->RemoveFromWorld(false);

		// Add the Event to respawn the Flags
		sEventMgr.AddEvent(this, &WarsongGulch::EventReturnFlags, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG, 20000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

		/* give each player on that team bonus honor and reputation*/
		uint32 honorToAdd = 2 * m_honorPerKill;
		uint32 repToAdd = m_isWeekend ? 45 : 35;
		uint32 fact = plr->IsTeamHorde() ? 889 : 890; /*Warsong Outriders : Sliverwing Sentinels*/
		for(set<Player*>::iterator itr = m_players[plr->GetTeam()].begin(); itr != m_players[plr->GetTeam()].end(); ++itr)
		{
			(*itr)->m_bgScore.BonusHonor += honorToAdd;
			HonorHandler::AddHonorPointsToPlayer((*itr), honorToAdd);
			plr->ModStanding(fact, repToAdd);
		}

		m_scores[plr->GetTeam()]++;
		if(m_scores[plr->GetTeam()] == 3)
		{
			/* victory! */
			m_ended = true;
			m_winningteam = (uint8)plr->GetTeam();
			m_nextPvPUpdateTime = 0;

			sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
			sEventMgr.AddEvent(TO< CBattleground* >(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

			AddHonorToTeam( m_winningteam, 3 * 185 );

			CastSpellOnTeam( m_winningteam, 69158 );
			CastSpellOnTeam( m_winningteam, 69496 );
			CastSpellOnTeam( m_winningteam, 69497 );
			CastSpellOnTeam( m_winningteam, 69498 );

			if( m_winningteam == TEAM_ALLIANCE )
				AddHonorToTeam( TEAM_HORDE, 1 * 185 );
			else
				AddHonorToTeam( TEAM_ALLIANCE, 1 * 185 );

			m_mainLock.Release();
		}

		/* increment the score world state */
		SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_HORDE_SCORE : WORLDSTATE_WSG_ALLIANCE_SCORE, m_scores[plr->GetTeam()]);

		UpdatePvPData();
	}
}

void WarsongGulch::EventReturnFlags()
{
	for(uint32 x = 0; x < 2; x++)
	{
		if(m_homeFlags[x] != NULL)
			m_homeFlags[x]->PushToWorld(m_mapMgr);
	}
	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Alliance's flag is now placed at her base.");
	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Horde's flag is now placed at her base.");
}

void WarsongGulch::HookOnFlagDrop(Player* plr)
{
	if(!plr->m_bgHasFlag || m_dropFlags[plr->GetTeam()]->IsInWorld())
		return;

	/* drop the flag! */
	m_dropFlags[plr->GetTeam()]->SetPosition(plr->GetPosition());
//	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_POS_X, plr->GetPositionX());
//	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_POS_Y, plr->GetPositionY());
//	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_POS_Z, plr->GetPositionZ());
//	m_dropFlags[plr->GetTeam()]->SetFloatValue(GAMEOBJECT_FACING, plr->GetOrientation());
	m_dropFlags[plr->GetTeam()]->PushToWorld(m_mapMgr);

	m_flagHolders[plr->GetTeam()] = 0;
	plr->m_bgHasFlag = false;
	plr->RemoveAura(23333 + (plr->GetTeam() * 2));

	SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);

	sEventMgr.AddEvent(this, &WarsongGulch::ReturnFlag, plr->GetTeam(), EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + plr->GetTeam(), 5000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

	if(plr->IsTeamHorde())
		SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "The Alliance flag was dropped by %s!", plr->GetName());
	else
		SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The Horde flag was dropped by %s!", plr->GetName());
}

void WarsongGulch::HookFlagDrop(Player* plr, GameObject* obj)
{
	/* picking up a dropped flag */
	if(m_dropFlags[plr->GetTeam()] != obj)
	{
		/* are we returning it? */
		if((obj->GetEntry() == 179785 && plr->IsTeamAlliance()) ||
		        (obj->GetEntry() == 179786 && plr->IsTeamHorde()))
		{
			uint32 x = plr->GetTeam() ? 0 : 1;
			sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + (plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE));

			if(m_dropFlags[x]->IsInWorld())
				m_dropFlags[x]->RemoveFromWorld(false);

			if(m_homeFlags[x]->IsInWorld() == false)
				m_homeFlags[x]->PushToWorld(m_mapMgr);

			plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_RETURNED]++;
			UpdatePvPData();

			if(plr->IsTeamHorde())
				SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The Horde flag was returned to its base by %s!", plr->GetName());
			else
				SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "The Alliance flag was returned to its base by %s!", plr->GetName());

			SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 1);
			PlaySoundToAll(plr->IsTeamHorde() ? SOUND_HORDE_RETURNED : SOUND_ALLIANCE_RETURNED);
		}
		return;
	}

	map<uint32, uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if(itr != plr->m_forcedReactions.end())
	{
		return;
	}

	if(plr->IsTeamAlliance())
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG);
	else
		sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_WSG_AUTO_RETURN_FLAG + 1);

	if(m_dropFlags[plr->GetTeam()]->IsInWorld())
		m_dropFlags[plr->GetTeam()]->RemoveFromWorld(false);

	m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
	plr->m_bgHasFlag = true;

	/* This is *really* strange. Even though the A9 create sent to the client is exactly the same as the first one, if
	 * you spawn and despawn it, then spawn it again it will not show. So we'll assign it a new guid, hopefully that
	 * will work.
	 * - Burlex
	 */
	m_dropFlags[plr->GetTeam()]->SetNewGuid(m_mapMgr->GenerateGameobjectGuid());

	SpellEntry* pSp = dbcSpell.LookupEntry(23333 + (plr->GetTeam() * 2));
	Spell* sp = sSpellFactoryMgr.NewSpell(plr, pSp, true, 0);
	SpellCastTargets targets(plr->GetGUID());
	sp->prepare(&targets);
	SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 2);
	if(plr->IsTeamHorde())
		SendChatMessage(CHAT_MSG_BG_EVENT_HORDE, plr->GetGUID(), "The Alliance's flag has been taken by %s !", plr->GetName());
	else
		SendChatMessage(CHAT_MSG_BG_EVENT_ALLIANCE, plr->GetGUID(), "The Horde's flag has been taken by %s !", plr->GetName());
}

void WarsongGulch::ReturnFlag(uint32 team)
{
	if(m_dropFlags[team]->IsInWorld())
		m_dropFlags[team]->RemoveFromWorld(false);

	if(!m_homeFlags[team]->IsInWorld())
		m_homeFlags[team]->PushToWorld(m_mapMgr);

	if(team)
		SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Alliance flag was returned to its base!");
	else
		SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Horde flag was returned to its base!");
}

void WarsongGulch::HookFlagStand(Player* plr, GameObject* obj)
{
#ifdef ANTI_CHEAT
	if(!m_started)
	{
		Anticheat_Log->writefromsession(plr->GetSession(), "%s tryed to hook the flag in warsong gluch before battleground (ID %u) started.", plr->GetName(), this->m_id);
		SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, plr->GetGUID(), "%s will be removed from the game for cheating.", plr->GetName());
		// Remove player from battleground.
		this->RemovePlayer(plr, false);
		// Kick player from server.
		plr->Kick(6000);
		return;
	}
#endif
	if(m_flagHolders[plr->GetTeam()] || m_homeFlags[plr->GetTeam()] != obj || m_dropFlags[plr->GetTeam()]->IsInWorld())
	{
		// cheater!
		return;
	}

	map<uint32, uint32>::iterator itr = plr->m_forcedReactions.find(1059);
	if(itr != plr->m_forcedReactions.end())
	{
		return;
	}

	SpellEntry* pSp = dbcSpell.LookupEntry(23333 + (plr->GetTeam() * 2));
	Spell* sp = sSpellFactoryMgr.NewSpell(plr, pSp, true, 0);
	SpellCastTargets targets(plr->GetGUID());
	sp->prepare(&targets);

	/* set the flag holder */
	plr->m_bgHasFlag = true;
	m_flagHolders[plr->GetTeam()] = plr->GetLowGUID();
	if(m_homeFlags[plr->GetTeam()]->IsInWorld())
		m_homeFlags[plr->GetTeam()]->RemoveFromWorld(false);

	PlaySoundToAll(plr->IsTeamHorde() ? SOUND_HORDE_CAPTURE : SOUND_ALLIANCE_CAPTURE);
	SetWorldState(plr->IsTeamHorde() ? WORLDSTATE_WSG_ALLIANCE_FLAG_DISPLAY : WORLDSTATE_WSG_HORDE_FLAG_DISPLAY, 2);
}

void WarsongGulch::HookOnPlayerKill(Player* plr, Player* pVictim)
{
	plr->m_bgScore.KillingBlows++;
	UpdatePvPData();
}

void WarsongGulch::HookOnHK(Player* plr)
{
	plr->m_bgScore.HonorableKills++;
	UpdatePvPData();
}

void WarsongGulch::OnAddPlayer(Player* plr)
{
	if(!m_started && plr->IsInWorld())
	{
		plr->CastSpell(plr, BG_PREPARATION, true);
		plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_CAPTURED] = 0;
		plr->m_bgScore.MiscData[BG_SCORE_WSG_FLAGS_RETURNED] = 0;
	}
	UpdatePvPData();
}

void WarsongGulch::OnRemovePlayer(Player* plr)
{
	/* drop the flag if we have it */
	if(plr->m_bgHasFlag)
		HookOnMount(plr);

	plr->RemoveAura(BG_PREPARATION);
}

LocationVector WarsongGulch::GetStartingCoords(uint32 Team)
{
	if(Team)		// Horde
		return LocationVector(933.989685f, 1430.735840f, 345.537140f, M_PI_FLOAT);
	else			// Alliance
		return LocationVector(1519.530273f, 1481.868408f, 352.023743f, M_PI_FLOAT);
}

void WarsongGulch::HookOnPlayerDeath(Player* plr)
{
	plr->m_bgScore.Deaths++;

	/* do we have the flag? */
	if(plr->m_bgHasFlag)
		plr->RemoveAura(23333 + (plr->GetTeam() * 2));

	UpdatePvPData();
}

void WarsongGulch::HookOnMount(Player* plr)
{
	/* do we have the flag? */
	if(m_flagHolders[plr->GetTeam()] == plr->GetLowGUID())
		HookOnFlagDrop(plr);
}

bool WarsongGulch::HookHandleRepop(Player* plr)
{
	LocationVector dest;
	if(plr->IsTeamHorde())
		dest.ChangeCoords(1032.644775f, 1388.316040f, 340.559937f, 0.043200f);
	else
		dest.ChangeCoords(1423.218872f, 1554.663574f, 342.833801f, 3.124139f);
	plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), dest);
	return true;
}

void WarsongGulch::SpawnBuff(uint32 x)
{
	switch(x)
	{
		case 0:
			m_buffs[x] = SpawnGameObject(179871, 489, 1449.9296875f, 1470.70971679688f, 342.634552001953f, -1.64060950279236f, 0, 114, 1);
			m_buffs[x]->SetParentRotation(2, 0.73135370016098f);
			m_buffs[x]->SetParentRotation(3, -0.681998312473297f);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
			break;
		case 1:
			m_buffs[x] = SpawnGameObject(179899, 489, 1005.17071533203f, 1447.94567871094f, 335.903228759766f, 1.64060950279236f, 0, 114, 1);
			m_buffs[x]->SetParentRotation(2, 0.73135370016098f);
			m_buffs[x]->SetParentRotation(3, 0.681998372077942f);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
			break;
		case 2:
			m_buffs[x] = SpawnGameObject(179904, 489, 1317.50573730469f, 1550.85070800781f, 313.234375f, -0.26179963350296f, 0, 114, 1);
			m_buffs[x]->SetParentRotation(2, 0.130526319146156f);
			m_buffs[x]->SetParentRotation(3, -0.991444826126099f);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
			break;
		case 3:
			m_buffs[x] = SpawnGameObject(179906, 489, 1110.45129394531f, 1353.65563964844f, 316.518096923828f, -0.68067866563797f, 0, 114, 1);
			m_buffs[x]->SetParentRotation(2, 0.333806991577148f);
			m_buffs[x]->SetParentRotation(3, -0.94264143705368f);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
			break;
		case 4:
			m_buffs[x] = SpawnGameObject(179905, 489, 1320.09375f, 1378.78967285156f, 314.753234863281f, 1.18682384490967f, 0, 114, 1);
			m_buffs[x]->SetParentRotation(2, 0.559192895889282f);
			m_buffs[x]->SetParentRotation(3, 0.829037606716156f);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
			break;
		case 5:
			m_buffs[x] = SpawnGameObject(179907, 489, 1139.68774414063f, 1560.28771972656f, 306.843170166016f, -2.4434609413147f, 0, 114, 1);
			m_buffs[x]->SetParentRotation(2, 0.939692616462708f);
			m_buffs[x]->SetParentRotation(3, -0.342020124197006f);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 0, 1);
			m_buffs[x]->SetType(GAMEOBJECT_TYPE_TRAP);
			m_buffs[x]->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
			break;
	}
}

void WarsongGulch::OnCreate()
{
	/* add the buffs to the world */
	for(int i = 0; i < 6; ++i)
	{
		if(!m_buffs[i]->IsInWorld())
			m_buffs[i]->PushToWorld(m_mapMgr);
	}

	// Alliance Gates
	GameObject* gate = SpawnGameObject(179921, 489, 1471.554688f, 1458.778076f, 362.633240f, 0, 33, 114, 2.33271f);
	gate->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179919, 489, 1492.477783f, 1457.912354f, 342.968933f, 0, 33, 114, 2.68149f);
	gate->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179918, 489, 1503.335327f, 1493.465820f, 352.188843f, 0, 33, 114, 2.26f);
	gate->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// Horde Gates
	gate = SpawnGameObject(179916, 489, 949.1663208f, 1423.7717285f, 345.6241455f, -0.5756807f, 32, 114, 0.900901f);
	gate->SetParentRotation(0, -0.0167336f);
	gate->SetParentRotation(1, -0.004956f);
	gate->SetParentRotation(2, -0.283972f);
	gate->SetParentRotation(3, 0.9586736f);
	gate->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	gate = SpawnGameObject(179917, 489, 953.0507202f, 1459.8424072f, 340.6525573f, -1.9966197f, 32, 114, 0.854700f);
	gate->SetParentRotation(0, -0.1971825f);
	gate->SetParentRotation(1, 0.1575096f);
	gate->SetParentRotation(2, -0.8239487f);
	gate->SetParentRotation(3, 0.5073640f);
	gate->SetByte(GAMEOBJECT_BYTES_1, 3, 100);
	gate->PushToWorld(m_mapMgr);
	m_gates.push_back(gate);

	// Should be set from config
	SetWorldState( WORLDSTATE_WSG_MAX_SCORE, 3 );

	/* spawn spirit guides */
	AddSpiritGuide(SpawnSpiritGuide(1423.218872f, 1554.663574f, 342.833801f, 3.124139f, 0));
	AddSpiritGuide(SpawnSpiritGuide(1032.644775f, 1388.316040f, 340.559937f, 0.043200f, 1));
}

void WarsongGulch::OnStart()
{
	for(uint32 i = 0; i < 2; ++i)
	{
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
		{
			(*itr)->RemoveAura(BG_PREPARATION);
		}
	}

	/* open the gates */
	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
		(*itr)->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
	}

	DespawnGates(5000);

	/* add the flags to the world */
	for(int i = 0; i < 2; ++i)
	{
		if(!m_homeFlags[i]->IsInWorld())
			m_homeFlags[i]->PushToWorld(m_mapMgr);
	}

	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Alliance's flag is now placed at her base.");
	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Horde's flag is now placed at her base.");

	/* correct? - burlex */
	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);

	m_started = true;
}

void WarsongGulch::HookOnShadowSight()
{
}

void WarsongGulch::HookGenerateLoot(Player* plr, Object* pOCorpse)
{
}

void WarsongGulch::HookOnUnitKill(Player* plr, Unit* pVictim)
{
}

void WarsongGulch::SetIsWeekend(bool isweekend)
{
	m_isWeekend = isweekend;
}

void WarsongGulch::DespawnGates(uint32 delay)
{
	if(delay != 0)
	{
		sEventMgr.AddEvent(this, &WarsongGulch::DespawnGates, (uint32)0, EVENT_GAMEOBJECT_EXPIRE, delay, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
		return;
	}
	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
		(*itr)->Despawn(0, 0);
	}
	m_gates.clear();
}