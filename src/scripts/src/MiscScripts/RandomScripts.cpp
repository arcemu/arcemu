/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Setup.h"

void GuardsOnSalute(Player * pPlayer, Unit * pUnit)
{
	// Check if we are friendly with our Guards (they will salute only when You are)
	if(((pUnit->GetEntry() == 68 || pUnit->GetEntry() == 1976) && pPlayer->GetStandingRank(72) >= FRIENDLY) || ( pUnit->GetEntry() == 3296 && pPlayer->GetStandingRank(76) >= FRIENDLY))
	{
		uint32 EmoteChance = RandomUInt(100);
		if(EmoteChance < 33) // 1/3 chance to get Salute from Guard
			pUnit->Emote(EMOTE_ONESHOT_SALUTE);
	}
}

void GaurdsOnKiss(Player * pPlayer, Unit * pUnit)
{
	// Check if we are friendly with our Guards (they will bow only when You are)
	if (((pUnit->GetEntry() == 68 || pUnit->GetEntry() == 1976) && pPlayer->GetStandingRank(72) >= FRIENDLY) || (pUnit->GetEntry() == 3296 && pPlayer->GetStandingRank(76) >= FRIENDLY))
	{
		uint32 EmoteChance = RandomUInt(100);
		if(EmoteChance < 33) // 1/3 chance to get Bow from Guard
			pUnit->Emote(EMOTE_ONESHOT_BOW);
	}
}

void GuardsOnWave(Player * pPlayer, Unit * pUnit)
{
	// Check if we are friendly with our Guards (they will wave only when You are)
	if (((pUnit->GetEntry() == 68 || pUnit->GetEntry() == 1976) && pPlayer->GetStandingRank(72) >= FRIENDLY) || (pUnit->GetEntry() == 3296 && pPlayer->GetStandingRank(76) >= FRIENDLY))
	{
		uint32 EmoteChance = RandomUInt(100);
		if(EmoteChance < 33) // 1/3 chance to get Bow from Guard
			pUnit->Emote(EMOTE_ONESHOT_WAVE);
	}
}

void OnEmote(Player * pPlayer, uint32 Emote, Unit * pUnit)
{
	pUnit = pPlayer->GetMapMgr()->GetUnit(pPlayer->GetSelection());
	if (!pUnit || !pUnit->isAlive() || pUnit->GetAIInterface()->GetNextTarget())
		return;

	// Switch For Emote Name (You do EmoteName to Script Name link).
	switch(Emote)
	{
	case EMOTE_ONESHOT_SALUTE: // <- Its EMOTE name.
		GuardsOnSalute(pPlayer, pUnit); // <- Its Link.
		break;

	case EMOTE_ONESHOT_KISS:
		GaurdsOnKiss(pPlayer, pUnit);
		break;
	
	case EMOTE_ONESHOT_WAVE:
		GuardsOnWave(pPlayer, pUnit);
		break;
	}
}

void SSCDoors(Player * pPlayer)
{
	//Only opens when the first one steps in, if 669 if you find a way, put it in :P (else was used to increase the time the door stays opened when another one steps on it)
	GameObject *door = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(803.827f, 6869.38f, -38.5434f, 184212);
	if (door && (door->GetByte(GAMEOBJECT_BYTES_1, 0) == 1))
	{
		door->SetByte(GAMEOBJECT_BYTES_1, 0, 0);
		//sEventMgr.AddEvent(door, &GameObject::SetUInt32Value, GAMEOBJECT_STATE, 1, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
	}
	//else
	//{
		//sEventMgr.RemoveEvents(door);
		//sEventMgr.AddEvent(door, &GameObject::SetUInt32Value,GAMEOBJECT_STATE, 0, EVENT_SCRIPT_UPDATE_EVENT, 10000, 1, 0);
	//}
}

void OnAreaTrigger(Player * pPlayer, uint32 AreaTrigger)
{
	switch(AreaTrigger)
	{
	case 4591:
		{
			SSCDoors(pPlayer);
		}break;
	}
}


void SetupRandomScripts(ScriptMgr * mgr)
{	// Register Hook Event here
	mgr->register_hook(SERVER_HOOK_EVENT_ON_EMOTE, (void *)&OnEmote);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_AREATRIGGER, (void *)&OnAreaTrigger);
}