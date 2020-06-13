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

class MissingInAction : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();


			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 349);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Okay let's do!");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, -8769.745117f, -2186.538818f, 141.841599f, 3.457182f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8783.108398f, -2197.826416f, 140.165878f, 3.597175f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8828.141602f, -2188.636963f, 138.618134f, 4.815924f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8809.825195f, -2233.663574f, 144.356613f, 5.723056f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8795.662109f, -2242.221436f, 146.450958f, 5.408901f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8775.052734f, -2261.419922f, 149.575378f, 0.355398f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8690.659180f, -2241.627930f, 153.590225f, 5.829625f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8664.609375f, -2254.313232f, 154.770416f, 4.164592f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8675.779297f, -2278.719482f, 155.470612f, 4.643681f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8679.915039f, -2324.837646f, 155.916428f, 3.819015f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8760.700195f, -2386.090088f, 156.647003f, 4.206810f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8810.614258f, -2479.427246f, 133.146484f, 3.788514f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8927.923828f, -2590.238525f, 132.446716f, 3.439012f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -8992.492188f, -2607.978516f, 130.872818f, 2.472971f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -9123.129883f, -2498.676270f, 116.559822f, 1.946754f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -9165.129883f, -2376.718994f, 94.946968f, 2.406212f, 0, 256, 1826);
			sEAS.WaypointCreate(creat, -9278.547852f, -2296.741699f, 68.041824f, 2.861743f, 0, 256, 1826);
			sEAS.EnableWaypoints(creat);
		}
};

class Corporal_Keeshan : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Corporal_Keeshan);
		Corporal_Keeshan(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 19)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Tell Marshal Marris. I'm outta here!");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(219)->SendQuestComplete();
			}
		}
};

void SetupRedrigeMountains(ScriptMgr* mgr)
{
	mgr->register_creature_script(349, &Corporal_Keeshan::Create);
	/*mgr->register_quest_script(219, new MissingInAction());*/
}
