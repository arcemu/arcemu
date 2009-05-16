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
#include "EAS/EasyFunctions.h"

// Prove Your Worth
class ProveYourWorthQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ProveYourWorthQAI);
    ProveYourWorthQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(421);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

// The Dead Fields Quest 437
class Nightlash_Spawned : public CreatureAIScript
{
public:
    ADD_CREATURE_FACTORY_FUNCTION(Nightlash_Spawned);
    Nightlash_Spawned(Creature* pCreature) : CreatureAIScript(pCreature) {}
    void OnDied(Unit *mKiller)
    {
        if(!mKiller->IsPlayer())
        return;
        Player *pPlayer = (Player*)mKiller;
        QuestLogEntry *en = pPlayer->GetQuestLogForEntry(437);
            if(en == NULL)
            return;
        uint8 chance = (uint8)RandomUInt(3); //gives you a 25% chance of Nightlash being spawned
        switch(chance)
        {
        case 1:
            Creature*Nightlash=pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1077.44, 1542.68, 28.85, 1983);
            if (Nightlash==NULL)
            {
            Creature *NightlashSpawn = sEAS.SpawnCreature(pPlayer, 1983, 1077.44, 1542.68, 28.85, 0, 120000);
            TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<Nightlash_Spawned, Creature*>(this, &Nightlash_Spawned::NightlashSpeech, NightlashSpawn), 0, 2000, 1);  
            NightlashSpawn->event_AddEvent(te);
            }
        }
    }
    void NightlashSpeech (Creature* NightL)
    {
        NightL->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Who dares to kill my minions?");
    }
};

void SetupSilverpineForest(ScriptMgr * mgr)
{
	mgr->register_creature_script(1769, &ProveYourWorthQAI::Create);
	mgr->register_creature_script(1772, &Nightlash_Spawned::Create);
    mgr->register_creature_script(1773, &Nightlash_Spawned::Create);
}
