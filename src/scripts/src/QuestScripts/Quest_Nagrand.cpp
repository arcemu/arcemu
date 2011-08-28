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

//*********************************************************************************************
//							   The Ring of Blood
//*********************************************************************************************


class Quest_The_Ring_of_Blood_The_Final_Challenge : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Creature* pMogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(pMogor != NULL)
			{
				pMogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Prepare yourselves!");
				Unit* Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

				if(Qgiver != NULL)
				{
					char msg[256];
					snprintf((char*)msg, 256, "Mogor has challenged you. You have to accept! Get in the right of blood if you are ready to fight.");
					Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
					string msg2 = "For the first time in the Ring of Bloods history. Mogor has chosen to exercise his right of the battle! On this wartorn ground, ";
					msg2 += mTarget->GetName();
					msg2 += "  will face Mogor, hero of the Warmaul!";
					Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg2.c_str(), 32000);
				}

				pMogor->SetUInt64Value(UNIT_FIELD_FLAGS, 0);
				pMogor->GetAIInterface()->SetAllowedToEnterCombat(true);
				pMogor->GetAIInterface()->MoveTo(-704.669f, 7871.08f, 45.0387f, 1.59531f);
				pMogor->SetFacing(1.908516f);
				pMogor->SetFaction(14);
			}
		}

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Creature* mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor != NULL)
				mogor->Despawn(1000, 0);

			mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18069, -712.443115f, 7932.182129f, 59.430191f, 4.515952f, true, false, 0, 0);
		}
};

class Quest_The_Ring_of_Blood_The_Warmaul_Champion : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* pQgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(pQgiver != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				pQgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "They had to ship the champion in from the Blade's Edge gladiator pits. He was training on mountain giants - three at a time.";
				//char msg2[256];
				//snprintf((char*)msg2, 256, "They had to ship the champion in from the Blade's Edge gladiator pits. He was training on mountain giants - three at a time.", mTarget->GetName());
				pQgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg2.c_str(), 4000);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18402, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
			};
		};

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* pMogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(pMogor != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "WUT!? UNPOSSIBLE!! You fight Mogor now! Mogor destroy!");
				pMogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
			};

			Creature* pWarmaulChamp = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18402);

			if(pWarmaulChamp != NULL)
				pWarmaulChamp->Despawn(1000, 0);
		};

};

class Quest_The_Ring_of_Blood_Skragath : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "From the parts unknown: Ska'gath! Can ";
				msg2 += mTarget->GetName();
				msg2 += " possibly survive the onslaught of void energies?";
				//char msg2[256];
				//snprintf((char*)msg2, 256, "From the parts unknown: Ska'gath! Can %s possibly survive the onslaught of void energies?", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg2.c_str(), 4000);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18401, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
			};
		};

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Mogor not impressed! Skra'gat wuz made of da air and shadow! Soft like da squishy orcies!");
				mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
			};

			Creature* pSkragath = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18401);

			if(pSkragath != NULL)
				pSkragath->Despawn(1000, 0);
		};
};

class Quest_The_Ring_of_Blood_Rokdar_the_Sundered_Lord : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "Hailing from the mountains of Blade's Edge comes Rokdar the Sundered Lord! ";
				msg2 += mTarget->GetName();
				msg2 += " is in for the fight of his life.";
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg2.c_str(), 4000);

				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18400, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
			};
		};

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor != NULL)
				mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "WUT!? UNPOSSIBLE!!");

			Creature*  pRokdar = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18400);

			if(pRokdar != NULL)
				pRokdar->Despawn(1000, 0);
		};
};

class Quest_The_Ring_of_Blood_The_Blue_Brothers : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Creature* pBrokentoe = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18398);

			if(pBrokentoe != NULL)
				pBrokentoe->Despawn(1000, 0);

			Unit* Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				string msg2 = "The battle is about to begin! The unmerciful Murkblood twins versus ";
				msg2 += mTarget->GetName();
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg2.c_str(), 4000);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18399, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18399, -708.076f, 7870.41f, 44.8457f, 1.59531f, true, false, 0, 0);
			};
		};

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "...");
				mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
			};

			Creature* pBrother1 = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18399);
			if(pBrother1 != NULL)
				pBrother1->Despawn(1000, 0);

			Creature* pBrother2 = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18399);
			if(pBrother2 != NULL)
				pBrother2->Despawn(1000, 0);
		};
};

class Quest_The_Ring_of_Blood_Brokentoe : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "Get in the Ring of Blood, %s . The fight is about to start!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
				mTarget->GetMapMgr()->GetInterface()->SpawnCreature(18398, -704.669f, 7871.08f, 45.0387f, 1.59531f, true, false, 0, 0)->Despawn(600000, 0);
			};
		};

		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			Unit* Qgiver = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18471);

			if(Qgiver != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "%s is victorious!", mTarget->GetName());
				Qgiver->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
			};

			Unit* mogor = mTarget->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(mTarget->GetPositionX(), mTarget->GetPositionY(), 0, 18069);

			if(mogor != NULL)
			{
				char msg[256];
				snprintf((char*)msg, 256, "...");
				mogor->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, msg);
			};
		};
};

class mogorQAI : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(mogorQAI);
		mogorQAI(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			_unit->SetUInt64Value(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_9);
			_unit->GetAIInterface()->SetAllowedToEnterCombat(false);
		};

};

class NotOnMyWatch : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(NotOnMyWatch);
		NotOnMyWatch(Creature* pCreature) : CreatureAIScript(pCreature) {};

		void OnCombatStart(Unit* mTarget)
		{
			RegisterAIUpdateEvent(1000);
			_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "In Nagrand, food hunt ogre!");
		};

		void OnCombatStop()
		{
			RemoveAIUpdateEvent();
		};

		void OnDied(Unit* mTarget)
		{
			RemoveAIUpdateEvent();
		};

		void OnLoad()
		{
			_unit->SetFaction(14);
			RemoveAIUpdateEvent();
		};

		void AIUpdate()
		{
			if(_unit->GetHealthPct() < 30)
			{
				Unit* pUnit = _unit->GetAIInterface()->GetMostHated();
				if(pUnit != NULL && pUnit->IsPlayer())
					TO_PLAYER(pUnit)->EventAttackStop();

				_unit->SetFaction(35);
				_unit->GetAIInterface()->WipeHateList();
				_unit->GetAIInterface()->WipeTargetList();
				_unit->SetStandState(STANDSTATE_SIT);
				_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);

				_unit->Despawn(180000, 0);

				RemoveAIUpdateEvent();
			};
		};

};

class LumpGossipScript : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			Menu->AddItem(0, "Why are Boulderfist out this far? You know this is Kurenai territory!", 1);
			Menu->SendTo(plr);
		};

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			Creature* Lump = TO_CREATURE(pObject);
			if(Lump == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;
				case 1:
					if(plr->GetQuestLogForEntry(9918))
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9918);
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							uint32 newcount = en->GetMobCount(0) + 1;
							en->SetMobCount(0, newcount);
							en->SendUpdateAddKill(0);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		};

};




void SetupNagrand(ScriptMgr* mgr)
{
	mgr->register_creature_script(18351, &NotOnMyWatch::Create);
	mgr->register_creature_script(18069, &mogorQAI::Create);

	mgr->register_quest_script(9977, new Quest_The_Ring_of_Blood_The_Final_Challenge());
	mgr->register_quest_script(9973, new Quest_The_Ring_of_Blood_The_Warmaul_Champion());
	mgr->register_quest_script(9972, new Quest_The_Ring_of_Blood_Skragath());
	mgr->register_quest_script(9970, new Quest_The_Ring_of_Blood_Rokdar_the_Sundered_Lord());
	mgr->register_quest_script(9967, new Quest_The_Ring_of_Blood_The_Blue_Brothers());
	mgr->register_quest_script(9962, new Quest_The_Ring_of_Blood_Brokentoe());

	GossipScript* LumpGossip = new LumpGossipScript;
	mgr->register_gossip_script(18351, LumpGossip);

}

