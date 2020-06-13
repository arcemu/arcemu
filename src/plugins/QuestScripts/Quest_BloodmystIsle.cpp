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

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
    Menu->SendTo(plr);

class TheKesselRun : public QuestScript
{
	public:
		void OnQuestStart(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			if(!mTarget)
				return;
			if(!mTarget->HasSpell(30829))
				mTarget->CastSpell(mTarget, 30829, true);
		}
};



class TheKesselRun1 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* highchief = TO_CREATURE(pObject);
			if(highchief == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9663))
				Menu->AddItem(0, "Warn him", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* highchief = TO_CREATURE(pObject);
			if(highchief == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9663);
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							en->SetMobCount(0, en->GetMobCount(0) + 1);
							en->SendUpdateAddKill(0);
							en->UpdatePlayerFields();
							return;
						}
						break;
					}
			}
		}

};


class TheKesselRun2 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* highchief = TO_CREATURE(pObject);
			if(highchief == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9663))
				Menu->AddItem(0, "Warn him", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* highchief = TO_CREATURE(pObject);
			if(highchief == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9663);
						if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
						{
							en->SetMobCount(1, en->GetMobCount(1) + 1);
							en->SendUpdateAddKill(1);
							en->UpdatePlayerFields();
							return;
						}
						break;
					}
			}
		}

};


class TheKesselRun3 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* highchief = TO_CREATURE(pObject);
			if(highchief == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9663))
				Menu->AddItem(0, "Warn him", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* highchief = TO_CREATURE(pObject);
			if(highchief == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9663);
						if(en && en->GetMobCount(2) < en->GetQuest()->required_mobcount[2])
						{
							en->SetMobCount(2, en->GetMobCount(2) + 1);
							en->SendUpdateAddKill(2);
							en->UpdatePlayerFields();
							return;
						}
						break;
					}
			}
		}

};

//-----------------------------------------------------------------------------------------------------------------------

int fulborgskilled = 0;


class SavingPrincessStillpine : public GameObjectAIScript
{
	public:
		SavingPrincessStillpine(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new SavingPrincessStillpine(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(9667);
			if(qle == NULL)
				return;

			if(qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
			{
				qle->SetMobCount(0, qle->GetMobCount(0) + 1);
				qle->SendUpdateAddKill(0);
				qle->UpdatePlayerFields();
			}

			Creature* princess = pPlayer->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 17682);
			if(!princess)
				return;

			princess->Despawn(1000, 6 * 60 * 1000);
			return;
		}
};

class HighChiefBristlelimb : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HighChiefBristlelimb);
		HighChiefBristlelimb(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnDied(Unit* mKiller)
		{
			fulborgskilled++;
			if(mKiller->IsPlayer())
			{
				Player* mPlayer = TO_PLAYER(mKiller);

				if(fulborgskilled > 8 && mPlayer->GetQuestLogForEntry(9667))
				{
					_unit->GetMapMgr()->GetInterface()->SpawnCreature(17702, -2419, -12166, 33, 3.45f, true, false, 0, 0)->Despawn(18000000, 0);
					fulborgskilled = 0;
					_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, "Chief, we need your help!");
				}
			}
		}

};

class WebbedCreature : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(WebbedCreature);
		WebbedCreature(Creature* pCreature) : CreatureAIScript(pCreature)
		{
		};

		void OnCombatStart(Unit* pTarget)
		{
			_unit->GetAIInterface()->disable_melee = true;
			_unit->Root();
			_unit->GetAIInterface()->StopMovement(0);
		};

		void OnCombatStop(Unit* pTarget)
		{
			_unit->GetAIInterface()->disable_melee = false;
			_unit->Unroot();
		};

		void OnDied(Unit* pKiller)
		{
			Player* QuestHolder = NULL;
			if(pKiller->IsPlayer())
				QuestHolder = TO_PLAYER(pKiller);
			else if(pKiller->IsPet() && TO_PET(pKiller)->GetPetOwner() != NULL)
				QuestHolder = TO_PET(pKiller)->GetPetOwner();

			if(QuestHolder == NULL)
				return;

			// M4ksiu: I don't think the method is correct, but it can stay the way it was until someone gives proper infos
			QuestLogEntry* Quest = QuestHolder->GetQuestLogForEntry(9670);
			Creature* RandomCreature = NULL;
			if(Quest == NULL)
			{
				// Creatures from Bloodmyst Isle
				uint32 Id[ 51 ] = { 17681, 17887, 17550, 17323, 17338, 17341, 17333, 17340, 17353, 17320, 17339, 17337, 17715, 17322, 17494, 17654, 17342, 17328, 17331, 17325, 17321, 17330, 17522, 17329, 17524, 17327, 17661, 17352, 17334, 17326, 17324, 17673, 17336, 17346, 17589, 17609, 17608, 17345, 17527, 17344, 17347, 17525, 17713, 17523, 17348, 17606, 17604, 17607, 17610, 17358, 17588 };
				RandomCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Id[ RandomUInt(50) ], _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
				if(RandomCreature != NULL)
				{
					RandomCreature->m_noRespawn = true;
					RandomCreature->Despawn(60000, 0);
				};

				return;
			}
			else
			{
				uint32 Id[ 8 ] = { 17681, 17321, 17330, 17522, 17673, 17336, 17346, 17589 };
				RandomCreature = _unit->GetMapMgr()->GetInterface()->SpawnCreature(Id[ RandomUInt(7) ], _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), _unit->GetOrientation(), true, false, 0, 0);
				if(RandomCreature != NULL)
				{
					RandomCreature->m_noRespawn = true;
					RandomCreature->Despawn(60000, 0);
					if(RandomCreature->GetEntry() == 17681 && Quest->GetMobCount(0) < Quest->GetQuest()->required_mobcount[ 0 ])
					{
						Quest->SetMobCount(0, Quest->GetMobCount(0) + 1);
						Quest->SendUpdateAddKill(0);
						Quest->UpdatePlayerFields();
					};
				};
			};
		};
};

void SetupBloodmystIsle(ScriptMgr* mgr)
{
	QuestScript* KesselRun = new TheKesselRun();
	mgr->register_quest_script(9663, KesselRun);

	GossipScript* gossip1 = new TheKesselRun1();
	mgr->register_gossip_script(17440, gossip1);

	GossipScript* gossip2 = new TheKesselRun2();
	mgr->register_gossip_script(17116, gossip2);

	GossipScript* gossip3 = new TheKesselRun3();
	mgr->register_gossip_script(17240, gossip3);


	mgr->register_gameobject_script(181928, &SavingPrincessStillpine::Create);

	mgr->register_creature_script(17320, &HighChiefBristlelimb::Create);
	mgr->register_creature_script(17321, &HighChiefBristlelimb::Create);

	mgr->register_creature_script(17680, &WebbedCreature::Create);
}