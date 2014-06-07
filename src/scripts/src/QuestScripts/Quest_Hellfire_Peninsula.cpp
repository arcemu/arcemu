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
// Zeth'Gor Must Burn!

class ZethGorMustBurnAlliance : public GameObjectAIScript
{
	public:
		ZethGorMustBurnAlliance(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new ZethGorMustBurnAlliance(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(10895);
			if(pQuest != NULL)
			{
				// M4ksiu - WTF IS THIS FOR? :|
				float SSX = pPlayer->GetPositionX();
				float SSY = pPlayer->GetPositionY();
				float SSZ = pPlayer->GetPositionZ();

				GameObject* pBeacon = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(SSX, SSY, SSZ, 184661);
				if(pBeacon != NULL && pBeacon->GetUInt32Value(GAMEOBJECT_FLAGS) > 0)
				{
					pBeacon->SetUInt32Value(GAMEOBJECT_FLAGS, (pBeacon->GetUInt32Value(GAMEOBJECT_FLAGS) - 1));
				}

				// Northern Zeth'Gor Tower
				if(pQuest->GetMobCount(0) < pQuest->GetQuest()->required_mobcount[0])
				{
					GameObject* pNorthern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-820.0f, 2029.0f, 55.0f, 300150);
					if(pNorthern != NULL && pPlayer->CalcDistance(pPlayer, pNorthern) < 40)      // if reduced the server will crash when out of range
					{
						pQuest->SetMobCount(0, pQuest->GetMobCount(0) + 1);
						pQuest->SendUpdateAddKill(0);
						pQuest->UpdatePlayerFields();

						GameObject* pGameobject = sEAS.SpawnGameobject(pPlayer, 183816, -819.77f, 2029.09f, 55.6082f, 0, 4, 0, 0, 0, 0);
						if(pGameobject != NULL)
						{
							sEAS.GameobjectDelete(pGameobject, 1 * 60 * 1000);
						}

						return;
					}
				}

				// Southern Zeth'Gor Tower
				if(pQuest->GetMobCount(1) < pQuest->GetQuest()->required_mobcount[1])
				{
					GameObject* pSouthern = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-1150.0f, 2110.0f, 84.0f, 300150);
					if(pSouthern != NULL && pPlayer->CalcDistance(pPlayer, pSouthern) < 40)
					{
						pQuest->SetMobCount(1, pQuest->GetMobCount(1) + 1);
						pQuest->SendUpdateAddKill(1);
						pQuest->UpdatePlayerFields();

						GameObject* pGameobject = sEAS.SpawnGameobject(pPlayer, 183816, -1150.53f, 2109.92f, 84.4204f, 0, 4, 0, 0, 0, 0);
						if(pGameobject != NULL)
						{
							sEAS.GameobjectDelete(pGameobject, 1 * 60 * 1000);
						}

						return;
					}
				}

				// Forge Zeth'Gor Tower
				if(pQuest->GetMobCount(2) < pQuest->GetQuest()->required_mobcount[2])
				{
					GameObject* pForge = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-893.0f, 1919.0f, 82.0f, 300150);
					if(pForge != NULL && pPlayer->CalcDistance(pPlayer, pForge) < 40)
					{
						pQuest->SetMobCount(2, pQuest->GetMobCount(2) + 1);
						pQuest->SendUpdateAddKill(2);
						pQuest->UpdatePlayerFields();

						GameObject* pGameobject = sEAS.SpawnGameobject(pPlayer, 183816, -893.499f, 1919.27f, 81.6449f, 0, 4, 0, 0, 0, 0);
						if(pGameobject != NULL)
						{
							sEAS.GameobjectDelete(pGameobject, 1 * 60 * 1000);
						}

						return;
					}
				}

				// Foothill Zeth'Gor Tower
				if(pQuest->GetMobCount(3) < pQuest->GetQuest()->required_mobcount[3])
				{
					GameObject* pFoothill = pPlayer->GetMapMgr()->GetInterface()->GetGameObjectNearestCoords(-978.0f, 1879.0f, 111.0f, 300150);
					if(pFoothill != NULL && pPlayer->CalcDistance(pPlayer, pFoothill) < 40)
					{
						pQuest->SetMobCount(3, pQuest->GetMobCount(3) + 1);
						pQuest->SendUpdateAddKill(3);
						pQuest->UpdatePlayerFields();

						GameObject* pGameobject = sEAS.SpawnGameobject(pPlayer, 183816, -977.713f, 1879.500f, 110.892f, 0, 4, 0, 0, 0, 0);
						if(pGameobject != NULL)
						{
							sEAS.GameobjectDelete(pGameobject, 1 * 60 * 1000);
						}

						return;
					}
				}
				else
				{
					pPlayer->BroadcastMessage("You are to far away!");
				}

			}
			else
			{
				pPlayer->BroadcastMessage("Missing required quest : Zeth'Gor Must Burn");
			}
		}
};



/*--------------------------------------------------------------------------------------------------------*/
// The Dreghood Elders
//TODO: add waypoints on gossip select

#define PrisonerText1 "You've freed me! The winds speak to my people one again and grant us their strength. I thank you, stranger."
#define PrisonerText2 "I am free! I will ask the spirit of fire to return to us so that my people may have the courage to fight their masters! I thank you, $r."
#define PrisonerText3 "I'm free! The spirit of water returns to my people. It will bring us the wisdom we need to survive in this harsh land. I am in your debt, $N."

class PrisonerGossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 10104, pPlayer->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), pPlayer, menu);
			if(QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(10368))
			{
				int32 npc = -1;
				switch(TO_CREATURE(pObject)->GetEntry())
				{
					case 20677: npc = 0; break;
					case 20678: npc = 1; break;
					case 20679: npc = 2; break;
					default: npc = -1; break;
				}
				if(npc >= 0 && pPlayer->GetItemInterface()->HasItem(29501) && pQuest->GetMobCount(npc) < pQuest->GetQuest()->required_mobcount[npc])
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Walk free, Elder. Bring the spirits back to your tribe.", 0);
			}
			menu.Send(pPlayer);
		}

		void OnSelectOption(Object* pObject, Player* pPlayer, uint32 Id, const char* EnteredCode)
		{
			pPlayer->Gossip_Complete();
			int32 npc = -1;
			switch(TO_CREATURE(pObject)->GetEntry())
			{
				case 20677: npc = 0; break;
				case 20678: npc = 1; break;
				case 20679: npc = 2; break;
			}

			if (npc < 0)
				return;

			QuestLogEntry* pQuest = pPlayer->GetQuestLogForEntry(10368);
			if(pQuest && pQuest->GetMobCount(i) < pQuest->GetQuest()->required_mobcount[i])
			{
				pQuest->SetMobCount(npc, pQuest->GetMobCount(npc) + 1);
				pQuest->SendUpdateAddKill(npc);
				pQuest->UpdatePlayerFields();
				switch(npc)
				{
					case 0: pPrisoner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, PrisonerText1); break;
					case 1: pPrisoner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, PrisonerText2); break;
					case 2: pPrisoner->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, PrisonerText3); break;
				}

				pPrisoner->SetStandState(STANDSTATE_STAND);
				pPrisoner->Despawn(5000, 6 * 60 * 1000);
			}
		}

};

// Limbo state?
class PrisonersDreghoodElders : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(PrisonersDreghoodElders);
		PrisonersDreghoodElders(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetStandState(STANDSTATE_SIT);
			_unit->setDeathState(CORPSE);
			_unit->GetAIInterface()->m_canMove = false;
		}
};

/*--------------------------------------------------------------------------------------------------------*/
class AncestralSpiritWolf : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AncestralSpiritWolf);
		AncestralSpiritWolf(Creature* c) : CreatureAIScript(c) {}
		void OnLoad()
		{
			_unit->CastSpell(_unit, 29938, false);
		}
};


/*--------------------------------------------------------------------------------------------------------*/

class HellfireDeadNPC : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HellfireDeadNPC);
		HellfireDeadNPC(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetStandState(7);
			_unit->setDeathState(CORPSE);
			_unit->GetAIInterface()->m_canMove = false;
		}
};

class DarkTidingsAlliance : public QuestScript
{
	public:
		void OnQuestComplete(Player* pPlayer, QuestLogEntry* qLogEntry)
		{
			Creature* pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 17479);
			if(pCreature == NULL)
				return;

			char msg[100];
			sprintf(msg, "Psst, %s, get over here.", pPlayer->GetName());
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, msg);	// Changed Player to Creature. I wonder if it was blizzlike
		}
};

class DarkTidingsHorde : public QuestScript
{
	public:
		void OnQuestComplete(Player* pPlayer, QuestLogEntry* qLogEntry)
		{
			Creature* pCreature = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 17558);
			if(pCreature == NULL)
				return;

			char msg[100];
			sprintf(msg, "Psst, %s, get over here.", pPlayer->GetName());
			pCreature->SendChatMessage(CHAT_MSG_MONSTER_WHISPER, LANG_UNIVERSAL, msg);
		}
};


void SetupHellfirePeninsula(ScriptMgr* mgr)
{
	mgr->register_gameobject_script(184661, &ZethGorMustBurnAlliance::Create);
	Arcemu::Gossip::Script* pPrisonerGossip = new PrisonerGossip;
	mgr->register_creature_gossip(20677, pPrisonerGossip);
	mgr->register_creature_gossip(20678, pPrisonerGossip);
	mgr->register_creature_gossip(20679, pPrisonerGossip);

	/*-------------------------------------------------------------------*/
	// TODO
	//mgr->register_dummy_spell(35460, &FuryOfTheDreghoodElders);


	/*-------------------------------------------------------------------*/
	// NPC States
	mgr->register_creature_script(20677, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20678, &PrisonersDreghoodElders::Create);
	mgr->register_creature_script(20679, &PrisonersDreghoodElders::Create);

	mgr->register_creature_script(17405, &HellfireDeadNPC::Create);
	mgr->register_creature_script(16852, &HellfireDeadNPC::Create);
	mgr->register_creature_script(20158, &HellfireDeadNPC::Create);

	QuestScript* DarkTidingsHordeQuest = new DarkTidingsHorde();
	QuestScript* DarkTidingsAllianceQuest = new DarkTidingsAlliance();
	mgr->register_quest_script(9587, DarkTidingsAllianceQuest);
	mgr->register_quest_script(9588, DarkTidingsHordeQuest);

	mgr->register_creature_script(17077, &AncestralSpiritWolf::Create);
}
