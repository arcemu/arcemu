/*
 * EasyWoW MMORPG Server
 * Copyright (C) 2014 EasyWoW <http://github.com/EasyWoW>
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
    SpawnBuffs();

    for (uint8 i = 0; i < 2; i++)
    {
        m_players[i].clear();
        m_pendPlayers[i].clear();

        m_flagHolders[i] = 0;
        if (m_homeFlags[i] = SpawnGameObject(WarsongFlags[i].entry, WarsongFlags[i].map, WarsongFlags[i].x, WarsongFlags[i].y, WarsongFlags[i].z,
            WarsongFlags[i].o, WarsongFlags[i].flags, WarsongFlags[i].faction, WarsongFlags[i].scale))
        {
            m_homeFlags[i]->SetState(WarsongFlags[i].state);
            m_homeFlags[i]->SetAnimProgress(WarsongFlags[i].animprogress);
        }

        // dropped flags
        m_dropFlags[i] = m_mapMgr->CreateGameObject(179785 + i);
        if (!m_dropFlags[i]->CreateFromProto(179785 + i, 489, 0, 0, 0, 0))
            Log.Error("WarsongGulch", "Could not create %s dropped flag", i == 0 ? "Horde" : "Alliance");
        else
        {
            m_dropFlags[i]->SetUInt32Value(GAMEOBJECT_DYNAMIC, 1);
            m_dropFlags[i]->SetScale(2.5f);
        }

        m_scores[i] = 0;
    }

    m_pvpData.clear();
    m_resurrectMap.clear();

    m_lgroup = lgroup;

	m_zoneid = 3277;
}

WarsongGulch::~WarsongGulch()
{
	for(uint8 i = 0; i < 6; ++i)
	{
		// buffs may not be spawned, so delete them if they're not
		if(m_buffs[i] && m_buffs[i]->IsInWorld() == false)
			delete m_buffs[i];
	}

	for(uint8 i = 0; i < 2; ++i)
	{
		if(m_dropFlags[i] && m_dropFlags[i]->IsInWorld() == false)
			delete m_dropFlags[i];

		if(m_homeFlags[i] && m_homeFlags[i]->IsInWorld() == false)
			delete m_homeFlags[i];
	}

	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
        if ((*itr) && !(*itr)->IsInWorld())
			delete(*itr);
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

    // TODO: move this to gameobject script instead.
	if(buffslot >= 0)
	{
		if(m_buffs[buffslot] && m_buffs[buffslot]->IsInWorld())
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
		if(m_flagHolders[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE] && m_dropFlags[plr->IsTeamHorde() ? TEAM_ALLIANCE : TEAM_HORDE]->IsInWorld())
			return; // can't cap while flag dropped

#ifdef ANTI_CHEAT
		float distance = plr->IsTeamAlliance() ? plr->CalcDistance(1540.29f, 1481.34f, 352.64f) : plr->CalcDistance(915.367f, 1433.78f, 346.089f);
		if(distance > 50.0f)
		{
			//50 yards from the spawn, gtfo hacker.
			sCheatLog.writefromsession(plr->GetSession(), "Tried to capture the flag in WSG while being more then 50 yards away. (%f yards)", plr->CalcDistance(915.367f, 1433.78f, 346.089f));
			plr->GetSession()->Disconnect();
			return;
		}
#endif

		/* remove the bool from the player so the flag doesn't drop */
		m_flagHolders[plr->GetTeam()] = 0;
		plr->m_bgHasFlag = false;

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
        uint32 fact = plr->IsTeamHorde() ? FACTION_WARSONG_OUTSTRIDERS : FACTION_SILVERING_SENTINELS;
		for(set<Player*>::iterator itr = m_players[plr->GetTeam()].begin(); itr != m_players[plr->GetTeam()].end(); ++itr)
		{
			(*itr)->m_bgScore.BonusHonor += honorToAdd;
			HonorHandler::AddHonorPointsToPlayer((*itr), honorToAdd);
			plr->ModStanding(fact, repToAdd);
		}

        UpdateTeamScore(plr->GetTeam());
		UpdatePvPData();
	}
}

void WarsongGulch::EventReturnFlags()
{
	for(uint8 x = 0; x < 2; x++)
	{
		if(m_homeFlags[x])
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
		return;

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

void WarsongGulch::HookOnPlayerKill(Player* plr, Player* /*pVictim*/)
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

    if (plr->HasAura(BG_PREPARATION))
	    plr->RemoveAura(BG_PREPARATION);
}

LocationVector WarsongGulch::GetStartingCoords(uint32 Team)
{
    return LocationVector(wsg_start_pos[Team].x, wsg_start_pos[Team].y, wsg_start_pos[Team].z, M_PI_FLOAT);
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
    plr->SafeTeleport(plr->GetMapId(), plr->GetInstanceID(), wsg_repop_pos[plr->GetTeam()]);
	return true;
}

void WarsongGulch::SpawnBuffs()
{
    for (uint8 i = 0; i < WSG_BUFFS_COUNT; i++)
    {
        if (m_buffs[i] = SpawnGameObject(wsg_buffs[i].spawn.entry, wsg_buffs[i].spawn.map, wsg_buffs[i].spawn.x, wsg_buffs[i].spawn.y, wsg_buffs[i].spawn.z, 
            wsg_buffs[i].spawn.o, wsg_buffs[i].spawn.flags, wsg_buffs[i].spawn.faction, wsg_buffs[i].spawn.scale))
        {
            m_buffs[i]->SetParentRotation(2, wsg_buffs[i].orientation[1]);
            m_buffs[i]->SetParentRotation(3, wsg_buffs[i].orientation[2]);
            m_buffs[i]->SetState(wsg_buffs[i].spawn.state);
            m_buffs[i]->SetAnimProgress(wsg_buffs[i].spawn.animprogress);
            m_buffs[i]->PushToWorld(m_mapMgr);
        }
    }
}

void WarsongGulch::OnCreate()
{
    SpawnGates();
	SetWorldState( WORLDSTATE_WSG_MAX_SCORE, 3 );

	/* spawn spirit guides */
    for (uint8 i = 0; i < 2; i++)
        AddSpiritGuide(SpawnSpiritGuide(wsg_spirit_guides[i], i));
}

void WarsongGulch::OnStart()
{
	for(uint8 i = 0; i < 2; ++i)
	{
        // Remove preperation buff
		for(set<Player*>::iterator itr = m_players[i].begin(); itr != m_players[i].end(); ++itr)
			(*itr)->RemoveAura(BG_PREPARATION);

        // add the flags to the world
        if (!m_homeFlags[i]->IsInWorld())
            m_homeFlags[i]->PushToWorld(m_mapMgr);
	}

	// open and despawn gates
	for(list<GameObject*>::iterator itr = m_gates.begin(); itr != m_gates.end(); ++itr)
	{
        (*itr)->SetUInt32Value(GAMEOBJECT_FLAGS, 64);
        (*itr)->SetState(GAMEOBJECT_STATE_OPEN);
        (*itr)->Despawn(5000, 0);
	}

	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Alliance's flag is now placed at her base.");
	SendChatMessage(CHAT_MSG_BG_EVENT_NEUTRAL, 0, "The Horde's flag is now placed at her base.");

	PlaySoundToAll(SOUND_BATTLEGROUND_BEGIN);
	m_started = true;
}

void WarsongGulch::HookOnShadowSight()
{
}

void WarsongGulch::HookGenerateLoot(Player* /*plr*/, Object* /*pOCorpse*/)
{
}

void WarsongGulch::HookOnUnitKill(Player* /*plr*/, Unit* /*pVictim*/)
{
}

void WarsongGulch::SetIsWeekend(bool isweekend)
{
	m_isWeekend = isweekend;
}

void WarsongGulch::EventVictory(uint8 Team)
{
    m_nextPvPUpdateTime = 0;

    sEventMgr.RemoveEvents(this, EVENT_BATTLEGROUND_CLOSE);
    sEventMgr.AddEvent(TO< CBattleground* >(this), &CBattleground::Close, EVENT_BATTLEGROUND_CLOSE, 120000, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);

    AddHonorToTeam(Team, 3 * 185);

    CastSpellOnTeam(Team, 69158);
    CastSpellOnTeam(Team, 69496);
    CastSpellOnTeam(Team, 69497);
    CastSpellOnTeam(Team, 69498);

    AddHonorToTeam(Team, 1 * 185);

    m_mainLock.Release();
}

void WarsongGulch::SpawnGates()
{
    for (uint8 i = 0; i < WSG_GATES_COUNT; i++)
    {
        if (GameObject* gate = SpawnGameObject(WarsongGates[i].entry, MAP_WARSONG_GULCH, WarsongGates[i].x, WarsongGates[i].y, WarsongGates[i].z,
            WarsongGates[i].o, WarsongGates[i].flags, WarsongGates[i].faction, WarsongGates[i].scale))
        {
            m_homeFlags[i]->SetState(WarsongGates[i].state);
            m_homeFlags[i]->SetAnimProgress(WarsongGates[i].animprogress);
            m_gates.push_back(gate);
        }
    }
}

void WarsongGulch::UpdateTeamScore(uint8 Team)
{
    m_scores[Team]++;
    if (m_scores[Team] == 3)
        EventVictory(Team);

    /* increment the score world state */
    SetWorldState(Team == TEAM_HORDE ? WORLDSTATE_WSG_HORDE_SCORE : WORLDSTATE_WSG_ALLIANCE_SCORE, m_scores[Team]);
}
