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

class JeanPierrePoulain : public GossipScript
{
		public:
			void GossipHello(Object* pObject, Player* plr)
			{
			GossipMenu* Menu;
				objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 14500, plr);
			if(plr->HasFinishedQuest(13668) || plr->GetQuestLogForEntry(13668) || plr->HasFinishedQuest(13667) || plr->GetQuestLogForEntry(13667))
			{
				Menu->SendTo(plr);
			}
			else
			{
				Menu->AddItem(0, "I'll take the flight."	,1);
				Menu->SendTo(plr);
			}
}			

void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
{
			switch(IntId)
			{
			case 0:
				GossipHello(pObject, Plr);
				break;
			case 1:
				Plr->CastSpell(Plr, 64795, true);
				break;
			}
				Plr->Gossip_Complete();
		}
};

class Wormhole : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player* plr)
    {
		if(plr->_GetSkillLineCurrent(202, false) >= 415)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 14785, plr);
			Menu->AddItem(0, "Borean Tundra"    , 1);
			Menu->AddItem(0, "Howling Fjord"  , 2);
			Menu->AddItem(0, "Sholazar Basin"    , 3);
			Menu->AddItem(0, "Icecrown"      , 4);
			Menu->AddItem(0, "Storm Peaks"        , 5);
			
			uint8 chance = RandomUInt(1);

			if (chance == 1)
				Menu->AddItem(0, "Underground..."		,6);
			
			Menu->SendTo(plr);
		}
	}

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
	{

		switch(IntId)
		{
			case 0:  
			GossipHello(pObject, Plr);
			return;
				break;
			case 1:
			Plr->CastSpell(Plr, 67834, true);
				break;
			case 2:
			Plr->CastSpell(Plr, 67838, true);
				break;
			case 3:
			Plr->CastSpell(Plr, 67835, true);
				break;
			case 4:
			Plr->CastSpell(Plr, 67836, true);
				break;
			case 5:
			Plr->CastSpell(Plr, 67837, true);
				break;
			case 6:
			Plr->CastSpell(Plr, 68081, true);
				break;
		}
		Plr->Gossip_Complete();
	}
};

void SetupRandomScripts(ScriptMgr* mgr)
{
	// Register Hook Event here
	mgr->register_hook(SERVER_HOOK_EVENT_ON_EMOTE, (void*)&OnEmote);
	mgr->register_gossip_script(34244, new JeanPierrePoulain);
	mgr->register_gossip_script(35646,  new Wormhole);
}