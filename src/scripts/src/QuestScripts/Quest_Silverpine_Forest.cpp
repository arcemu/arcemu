/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
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

class EscortingErland : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 3465);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I to the horror I fear wolfs, do not allow to approach them to me closely. Follow me");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, 1408.243286f, 1086.476929f, 53.684814f, 4.145067f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1400.292236f, 1070.908325f, 52.454960f, 2.442609f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1370.173218f, 1087.330811f, 52.136669f, 3.921230f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1344.565063f, 1063.532349f, 52.780121f, 3.053365f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1291.855225f, 1065.194336f, 54.129812f, 2.291528f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1278.805664f, 1080.154541f, 53.967297f, 1.062379f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1293.010742f, 1106.864624f, 50.572147f, 1.476284f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1298.289917f, 1148.772827f, 52.434540f, 1.625511f, 0, 256, 2684);
			sEAS.WaypointCreate(creat, 1289.469727f, 1200.744995f, 52.651005f, 1.488066f, 0, 256, 2684);
			sEAS.EnableWaypoints(creat);
		}
};

class Deathstalker_Erland : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Deathstalker_Erland);
		Deathstalker_Erland(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 9)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Thanks, you helped me to overcome this obstacle");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(435)->SendQuestComplete();
			}
		}
};

class Nightlash : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Nightlash);
		Nightlash(Creature* pCreature) : CreatureAIScript(pCreature) {}
		void OnDied(Unit* mKiller)
		{
			if(mKiller->IsPlayer())
			{
				Player* mPlayer = TO_PLAYER(mKiller);

				if(!_unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(1069.889404f, 1544.777558f, 28.331335f, 1983) && (rand() % 5 > 2) && mPlayer->GetQuestLogForEntry(437)) //random number I picked between 2-8
				{
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(1983, 1069.889404f, 1544.777558f, 28.331335f, 3.99f, true, false, 0, 0)->Despawn(600000, 0);
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Nightlash avenge us!!");//not sure this is 100% blizzlike, but looks nice
				}
			}
		}
};

void SetupSilverpineForest(ScriptMgr* mgr)
{
	/*mgr->register_quest_script(435, new EscortingErland());*/
	mgr->register_creature_script(1978, &Deathstalker_Erland::Create);
	mgr->register_creature_script(1773, &Nightlash::Create);
	mgr->register_creature_script(1772, &Nightlash::Create);
}
