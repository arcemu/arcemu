/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
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

#define QUEST_THE_DEFIAS_BROTHERHOOD 155

#define NPC_THE_DEFIAS_TRAITOR 467

static double traitorCoords[3] = { -10508.40, 1068.00, 55.21 };

class TheDefiasBrotherhood : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(traitorCoords[0], traitorCoords[1], traitorCoords[2], NPC_THE_DEFIAS_TRAITOR);

			if(creat == NULL)
				return;
			
			creat->m_escorter = mTarget;
			
			creat->GetAIInterface()->setMoveType(MOVEMENTTYPE_FORWARDTHANSTOP);
			creat->GetAIInterface()->setWaypointToMove(0);
			creat->GetAIInterface()->StopMovement(3 * 1000);
			
			// he should combat, however this is needed since there is a bug at aiInterface after killed
			// an attacker npc will run all the way long to spawn point and then run again to current wp
			creat->GetAIInterface()->SetAllowedToEnterCombat(false);
			
			string say = "Follow me, ";
			say += mTarget->GetName();
			say += ". I'll take you to the Defias hideout. But you better protect me or I am as good as dead.";
			
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
			creat->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
			
		}
};

class The_Defias_Traitor : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(The_Defias_Traitor);
		The_Defias_Traitor(Creature* pCreature) : CreatureAIScript(pCreature) {}

		//TODO: OnCombat stuff
		
		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 37)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "The entrance is hidden here in Moonbrook.  Keep your eyes peeled for thieves.  They want me dead.");
			}
				
			else if(iWaypointId == 45)
			{

				string say = "You can go tell Stoutmantle this is where the Defias Gang is holed up, ";
				say += _unit->m_escorter->GetName();
				say += ".";
				
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, say.c_str());
				_unit->Despawn(5 * 1000, 1000);
				
				if(_unit->m_escorter == NULL)
					return;
				
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
		
				//plr->GetQuestLogForEntry(QUEST_THE_DEFIAS_BROTHERHOOD)->SendQuestComplete();
				
			}
		}
};

void SetupWestfall(ScriptMgr* mgr)
{
	mgr->register_quest_script(QUEST_THE_DEFIAS_BROTHERHOOD, new TheDefiasBrotherhood());
	mgr->register_creature_script(NPC_THE_DEFIAS_TRAITOR, &The_Defias_Traitor::Create);
}
