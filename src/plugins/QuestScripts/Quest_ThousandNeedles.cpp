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

class HomewardBound : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 10427);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->SetAllowedToEnterCombat(false);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Please help me, you gotta protect me and stuff, I can't fight on my own!");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

			sEAS.CreateCustomWaypointMap(creat);
			sEAS.WaypointCreate(creat, -5005.66f, -882.705f, -6.05186f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4968.67f, -937.131f, -4.99042f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4968.03f, -958.455f, -5.53631f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4968.48f, -986.432f, -4.80345f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4975.32f, -998.646f, -5.12671f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4970.91f, -1014.12f, -5.47358f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4963.23f, -1026.1f, -4.9985f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4957.29f, -1035.31f, -5.62972f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4952.49f, -1042.99f, -8.61785f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4946.27f, -1053.72f, -15.9727f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4936.83f, -1054.79f, -22.0583f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4929.3f, -1054.22f, -25.8246f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4917.66f, -1051.53f, -30.242f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4909.1f, -1049.48f, -33.0802f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4902.48f, -1052.76f, -33.4847f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4904.48f, -1056.73f, -33.8546f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4910.38f, -1064.18f, -38.8506f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4914.76f, -1069.12f, -42.8664f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4918.88f, -1073.68f, -45.7238f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4924.54f, -1078.76f, -48.3099f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4931.68f, -1084.4f, -51.0179f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4926.66f, -1090.7f, -53.2325f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4919.66f, -1096.19f, -53.3382f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4911.05f, -1099.43f, -53.7044f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4903.92f, -1102.06f, -54.9974f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4897.52f, -1101.48f, -55.4563f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4896.49f, -1104.66f, -55.2742f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4894.82f, -1110.74f, -53.9336f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4892.71f, -1118.51f, -51.0561f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4888.98f, -1125.24f, -50.4447f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4884.62f, -1131.47f, -50.44f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4880.34f, -1137.57f, -50.5127f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4875.03f, -1144.52f, -50.5812f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4868.65f, -1152.74f, -50.9722f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4860.4f, -1160.88f, -50.6212f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4854.03f, -1167.17f, -49.6659f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4849.72f, -1171.42f, -48.9519f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4844.17f, -1176.9f, -48.3381f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4841.19f, -1182.11f, -48.7754f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4836.73f, -1189.87f, -49.3734f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4841.24f, -1195.48f, -49.4889f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4848.26f, -1198.64f, -49.3465f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4855.31f, -1202.44f, -48.8531f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4862.2f, -1206.18f, -48.6235f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4869.06f, -1209.9f, -46.1434f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4877.32f, -1214.39f, -44.1166f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4885.63f, -1218.9f, -43.5811f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4893.63f, -1223.24f, -42.5644f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4899.27f, -1228.69f, -42.2637f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4906.03f, -1235.23f, -41.9083f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4911.57f, -1240.58f, -42.9282f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4916.96f, -1245.79f, -43.973f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4921.24f, -1249.92f, -45.7031f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4924.52f, -1256.67f, -46.0392f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4927.78f, -1263.37f, -46.5725f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4932.77f, -1273.62f, -47.139f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4935.51f, -1279.24f, -47.1182f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4935.57f, -1285.64f, -46.7144f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4935.14f, -1293.67f, -46.2301f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4934.16f, -1302.12f, -46.1045f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4933.4f, -1308.37f, -46.5583f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4931.24f, -1315.97f, -47.6485f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4929.03f, -1323.71f, -48.8701f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4926.87f, -1331.31f, -49.926f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4922.54f, -1338.16f, -50.7557f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4917.05f, -1344.31f, -51.2097f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4910.32f, -1348.82f, -51.5222f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4903.97f, -1353.76f, -50.9117f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4901.07f, -1361.11f, -51.4375f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4901.38f, -1369.51f, -52.6119f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4904.58f, -1376.73f, -52.6119f, 3.098398f, 0, 256, 9900);
			sEAS.WaypointCreate(creat, -4910.54f, -1378.76f, -52.6119f, 3.098398f, 0, 256, 9900);
			sEAS.EnableWaypoints(creat);
		}
};

class Paoka_Swiftmountain : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Paoka_Swiftmountain);
		Paoka_Swiftmountain(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 72)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "I appreciate the help you have shown Pao'ka. I hope this covers any misfortunes this deed has cost you.");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				if(plr->GetQuestLogForEntry(4770))
					plr->GetQuestLogForEntry(4770)->SendQuestComplete();
			}
		}
};

class RumorsforKravel : public QuestScript
{
	public:

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 4452);
			if(creat == NULL)
				return;

			string msg = "Hahah! ";
			msg += mTarget->GetName();
			msg += ", you make quite a partner!";
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, msg.c_str());
			creat->Emote(EMOTE_ONESHOT_LAUGH);
		}
};

void SetupThousandNeedles(ScriptMgr* mgr)
{
	mgr->register_creature_script(10427, &Paoka_Swiftmountain::Create);
	/*mgr->register_quest_script(4770, new HomewardBound());*/
	mgr->register_quest_script(1117, new RumorsforKravel());
}
