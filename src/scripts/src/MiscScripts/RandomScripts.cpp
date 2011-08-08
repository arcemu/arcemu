/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
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

#include "Setup.h"

void GuardsOnSalute(Player* pPlayer, Unit* pUnit)
{
	if(pPlayer == NULL || pUnit == NULL)
		return;

	// Check if we are friendly with our Guards (they will salute only when You are)
	if(((pUnit->GetEntry() == 68 || pUnit->GetEntry() == 1976) && pPlayer->GetStandingRank(72) >= STANDING_FRIENDLY) || (pUnit->GetEntry() == 3296 && pPlayer->GetStandingRank(76) >= STANDING_FRIENDLY))
	{
		uint32 EmoteChance = RandomUInt(100);
		if(EmoteChance < 33) // 1/3 chance to get Salute from Guard
			pUnit->Emote(EMOTE_ONESHOT_SALUTE);
	}
}

void GaurdsOnKiss(Player* pPlayer, Unit* pUnit)
{
	if(pPlayer == NULL || pUnit == NULL)
		return;

	// Check if we are friendly with our Guards (they will bow only when You are)
	if(((pUnit->GetEntry() == 68 || pUnit->GetEntry() == 1976) && pPlayer->GetStandingRank(72) >= STANDING_FRIENDLY) || (pUnit->GetEntry() == 3296 && pPlayer->GetStandingRank(76) >= STANDING_FRIENDLY))
	{
		uint32 EmoteChance = RandomUInt(100);
		if(EmoteChance < 33) // 1/3 chance to get Bow from Guard
			pUnit->Emote(EMOTE_ONESHOT_BOW);
	}
}

void GuardsOnWave(Player* pPlayer, Unit* pUnit)
{
	if(pPlayer == NULL || pUnit == NULL)
		return;

	// Check if we are friendly with our Guards (they will wave only when You are)
	if(((pUnit->GetEntry() == 68 || pUnit->GetEntry() == 1976) && pPlayer->GetStandingRank(72) >= STANDING_FRIENDLY) || (pUnit->GetEntry() == 3296 && pPlayer->GetStandingRank(76) >= STANDING_FRIENDLY))
	{
		uint32 EmoteChance = RandomUInt(100);
		if(EmoteChance < 33) // 1/3 chance to get Bow from Guard
			pUnit->Emote(EMOTE_ONESHOT_WAVE);
	}
}

void OnEmote(Player* pPlayer, uint32 Emote, Unit* pUnit)
{
	if(!pUnit || !pUnit->isAlive() || pUnit->GetAIInterface()->getNextTarget())
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

void SetupRandomScripts(ScriptMgr* mgr)
{
	// Register Hook Event here
	mgr->register_hook(SERVER_HOOK_EVENT_ON_EMOTE, (void*)&OnEmote);
}