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

class TheDefiasBrotherhood : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 467);

			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->SetAllowedToEnterCombat(false);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Okay let's do this, you gotta protect me and stuff, I can't fight on my own!");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, -10521.876953f, 1064.410278f, 54.820744f, 3.220135f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10548.616211f, 1072.964111f, 49.694920f, 3.345799f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10596.891602f, 1057.281982f, 38.864330f, 3.451828f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10663.427734f, 1030.475952f, 32.725643f, 3.098398f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10715.894531f, 1030.825317f, 33.473114f, 3.165157f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10754.050781f, 1016.978821f, 32.843971f, 3.443188f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10917.114258f, 997.355408f, 35.466064f, 3.314383f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10937.306641f, 1020.598755f, 36.644848f, 1.488332f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10933.578125f, 1058.460571f, 36.027996f, 1.472624f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10958.590820f, 1105.329224f, 36.635437f, 2.076595f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10895.253906f, 1248.348877f, 33.385048f, 1.176529f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10890.806641f, 1297.475708f, 34.488716f, 1.698819f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10917.052734f, 1419.003296f, 42.823147f, 1.804847f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10948.415039f, 1422.049927f, 43.411053f, 3.332447f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -10989.025391f, 1410.501953f, 42.863670f, 2.673498f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -11016.258789f, 1434.814697f, 42.987415f, 2.005909f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -11024.154297f, 1488.091064f, 43.177536f, 1.566087f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -11048.541992f, 1513.367798f, 43.154915f, 2.369550f, 0, 256, 0);
			sEAS.WaypointCreate(creat, -11065.412109f, 1526.429321f, 43.328102f, 3.068554f, 0, 256, 0);
			sEAS.EnableWaypoints(creat);
		}
};

class The_Defias_Traitor : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(The_Defias_Traitor);
		The_Defias_Traitor(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 19)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Tell your master that this is where Van Cleef is hiding. I'm outta here!");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(155)->SendQuestComplete();
			}
		}
};

void SetupWestfall(ScriptMgr* mgr)
{
	/*mgr->register_quest_script(155, new TheDefiasBrotherhood());*/
	mgr->register_creature_script(467, &The_Defias_Traitor::Create);
}
