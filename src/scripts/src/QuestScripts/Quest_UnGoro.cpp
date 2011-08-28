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

/*--------------------------------------------------------------------------------------------------------*/


class RingoDeadNPC : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(RingoDeadNPC);
		RingoDeadNPC(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetStandState(7);
			_unit->setDeathState(CORPSE);
			_unit->GetAIInterface()->m_canMove = false;
		}
};



/*--------------------------------------------------------------------------------------------------------*/
// The Northern Pylon

class NorthernPylon : public GameObjectAIScript
{
	public:
		NorthernPylon(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new NorthernPylon(GO); }

		void OnActivate(Player* pPlayer)
		{
			if(pPlayer->HasFinishedQuest(4284))
			{
				QuestLogEntry* en = pPlayer->GetQuestLogForEntry(4285);
				if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
				{
					uint32 newcount = en->GetMobCount(0) + 1;
					en->SetMobCount(0, newcount);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();
					return;
				}
			}
			else if(pPlayer->HasFinishedQuest(4284) == false)
			{
				pPlayer->BroadcastMessage("You need to have completed the quest : Crystals of Power");
			}
		}
};



/*--------------------------------------------------------------------------------------------------------*/
// The Eastern Pylon

class EasternPylon : public GameObjectAIScript
{
	public:
		EasternPylon(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new EasternPylon(GO); }

		void OnActivate(Player* pPlayer)
		{
			if(pPlayer->HasFinishedQuest(4284))
			{
				QuestLogEntry* en = pPlayer->GetQuestLogForEntry(4287);
				if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
				{
					uint32 newcount = en->GetMobCount(0) + 1;
					en->SetMobCount(0, newcount);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();
					return;
				}
			}
			else if(pPlayer->HasFinishedQuest(4284) == false)
			{
				pPlayer->BroadcastMessage("You need to have completed the quest : Crystals of Power");
			}
		}
};



/*--------------------------------------------------------------------------------------------------------*/
//The Western Pylon

class WesternPylon : public GameObjectAIScript
{
	public:
		WesternPylon(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new WesternPylon(GO); }

		void OnActivate(Player* pPlayer)
		{
			if(pPlayer->HasFinishedQuest(4284))
			{
				QuestLogEntry* en = pPlayer->GetQuestLogForEntry(4288);
				if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
				{
					uint32 newcount = en->GetMobCount(0) + 1;
					en->SetMobCount(0, newcount);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();
					return;
				}
			}
			else if(pPlayer->HasFinishedQuest(4284) == false)
			{
				pPlayer->BroadcastMessage("You need to have completed the quest : Crystals of Power");
			}
		}
};

/*--------------------------------------------------------------------------------------------------------*/

class ChasingAMe01 : public QuestScript
{
	public:

		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			float SSX = mTarget->GetPositionX();
			float SSY = mTarget->GetPositionY();
			float SSZ = mTarget->GetPositionZ();

			Creature* creat = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(SSX, SSY, SSZ, 9623);
			if(creat == NULL)
				return;
			creat->m_escorter = mTarget;
			creat->GetAIInterface()->setMoveType(11);
			creat->GetAIInterface()->StopMovement(3000);
			creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "He-l-p Me... ts.. ts...");
			creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);


			sEAS.CreateCustomWaypointMap(creat);

			sEAS.WaypointCreate(creat, -6305.657715f, -1977.757080f, -268.076447f, 5.564124f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6292.354492f, -1988.233032f, -265.271667f, 4.821922f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6296.333984f, -2004.225342f, -268.766388f, 3.337522f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6329.421387f, -2007.737549f, -258.587250f, 3.769490f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6343.778809f, -2017.559204f, -256.952026f, 3.577064f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6360.504883f, -2030.157959f, -261.204926f, 3.526014f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6381.367676f, -2038.451904f, -262.319946f, 2.713128f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6404.608398f, -2028.813721f, -262.817230f, 1.146257f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6399.632813f, -2018.668091f, -263.569824f, 0.800682f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6383.180664f, -2003.231689f, -270.639984f, 0.631821f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6351.983887f, -1976.397827f, -276.039001f, 1.138403f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6342.651855f, -1958.451050f, -274.056122f, 1.805992f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6353.083008f, -1918.406006f, -264.135101f, 1.515395f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6350.737305f, -1900.465942f, -258.695831f, 2.677785f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6366.535645f, -1892.092651f, -256.424347f, 2.025904f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6378.548828f, -1866.535278f, -260.363281f, 1.154112f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6370.422852f, -1842.526978f, -259.409515f, 1.711744f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6374.264648f, -1825.782349f, -260.584442f, 1.955218f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6382.359375f, -1811.540527f, -266.374359f, 2.901623f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6383.307129f, -1794.137207f, -267.334686f, 1.821700f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6399.292980f, -1710.144897f, -273.734283f, 1.252285f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6361.579102f, -1582.413574f, -272.221008f, 1.798137f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6391.618652f, -1409.568237f, -272.190521f, 1.711742f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6407.588867f, -1305.676880f, -271.632935f, 0.553279f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6349.980469f, -1276.069580f, -266.971375f, 1.236575f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6324.551758f, -1233.252441f, -267.178619f, 0.451176f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6288.604492f, -1215.046265f, -267.426117f, 2.177482f, 1, 256, 8841);
			sEAS.WaypointCreate(creat, -6298.290039f, -1182.650024f, -269.101013f, 3.211410f, 1, 256, 8841);
			sEAS.EnableWaypoints(creat);
		}
};

class A_Me01  : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(A_Me01);
		A_Me01(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 28)
			{
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Tr..........");
				_unit->Despawn(5000, 1000);
				sEAS.DeleteWaypoints(_unit);
				if(_unit->m_escorter == NULL)
					return;
				Player* plr = _unit->m_escorter;
				_unit->m_escorter = NULL;
				plr->GetQuestLogForEntry(4245)->SendQuestComplete();
			}
		}
};


void SetupUnGoro(ScriptMgr* mgr)
{
	mgr->register_creature_script(9999, &RingoDeadNPC::Create);
	mgr->register_gameobject_script(164955, &NorthernPylon::Create);
	mgr->register_gameobject_script(164957, &EasternPylon::Create);
	mgr->register_gameobject_script(164956, &WesternPylon::Create);
	/*mgr->register_quest_script(4245, new ChasingAMe01());*/
	mgr->register_creature_script(9623, &A_Me01::Create);
}
