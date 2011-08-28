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


class Prisoner12 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;
			GossipMenu* Menu;
			Creature* Prisoner12 = TO_CREATURE(pObject);
			if(Prisoner12 == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9164))
				Menu->AddItem(0, "Release Him.", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* Prisoner12 = TO_CREATURE(pObject);
			if(Prisoner12 == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9164);
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							en->SetMobCount(0, en->GetMobCount(0) + 1);
							en->SendUpdateAddKill(0);
							en->UpdatePlayerFields();

							if(!Prisoner12)
								return;

							Prisoner12->Despawn(5000, 6 * 60 * 1000);
							Prisoner12->SetStandState(STANDSTATE_STAND);
							Prisoner12->SetEmoteState(7);
							return;
						}
						break;
					}
			}
		}

};


class Prisoner22 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* Prisoner22 = TO_CREATURE(pObject);
			if(Prisoner22 == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9164))
				Menu->AddItem(0, "Release Him.", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* Prisoner22 = TO_CREATURE(pObject);
			if(Prisoner22 == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9164);
						if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
						{
							en->SetMobCount(1, en->GetMobCount(1) + 1);
							en->SendUpdateAddKill(1);
							en->UpdatePlayerFields();

							if(!Prisoner22)
								return;

							Prisoner22->Despawn(5000, 6 * 60 * 1000);
							Prisoner22->SetStandState(STANDSTATE_STAND);
							Prisoner22->SetEmoteState(7);
							return;
						}
						break;
					}
			}
		}

};


class Prisoner32 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			if(!plr)
				return;

			GossipMenu* Menu;
			Creature* Prisoner32 = TO_CREATURE(pObject);
			if(Prisoner32 == NULL)
				return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9164))
				Menu->AddItem(0, "Release Him.", 1);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char* EnteredCode)
		{
			if(!plr)
				return;

			Creature* Prisoner32 = TO_CREATURE(pObject);
			if(Prisoner32 == NULL)
				return;

			switch(IntId)
			{
				case 0:
					GossipHello(pObject, plr);
					break;

				case 1:
					{
						QuestLogEntry* en = plr->GetQuestLogForEntry(9164);
						if(en && en->GetMobCount(2) < en->GetQuest()->required_mobcount[2])
						{
							en->SetMobCount(2, en->GetMobCount(2) + 1);
							en->SendUpdateAddKill(2);
							en->UpdatePlayerFields();

							if(!Prisoner32)
								return;

							Prisoner32->Despawn(5000, 6 * 60 * 1000);
							Prisoner32->SetStandState(STANDSTATE_STAND);
							Prisoner32->SetEmoteState(7);
							return;
						}
						break;
					}
			}
		}

};

class PrisonersatDeatholme : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(PrisonersatDeatholme);
		PrisonersatDeatholme(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnLoad()
		{
			_unit->SetStandState(STANDSTATE_DEAD);
			_unit->GetAIInterface()->m_canMove = false;
		}
};

class VanquishingAquantion : public GameObjectAIScript
{
	public:
		VanquishingAquantion(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new VanquishingAquantion(GO); }

		void OnActivate(Player* pPlayer)
		{
			QuestLogEntry* qle = pPlayer->GetQuestLogForEntry(9174);
			if(qle == NULL)
				return;

			Creature* naga = sEAS.SpawnCreature(pPlayer, 16292, 7938, -7632, 114, 3.05f, 0);
			naga->Despawn(6 * 60 * 1000, 0);
		}
};

void SetupGhostlands(ScriptMgr* mgr)
{
	GossipScript* Prisoner12Gossip = new Prisoner12();
	mgr->register_gossip_script(16208, Prisoner12Gossip);

	GossipScript* Prisoner22Gossip = new Prisoner22();
	mgr->register_gossip_script(16206, Prisoner22Gossip);

	GossipScript* Prisoner32Gossip = new Prisoner32();
	mgr->register_gossip_script(16209, Prisoner32Gossip);

	mgr->register_creature_script(16208, &PrisonersatDeatholme::Create);
	mgr->register_creature_script(16206, &PrisonersatDeatholme::Create);
	mgr->register_creature_script(16209, &PrisonersatDeatholme::Create);

	mgr->register_gameobject_script(181157, &VanquishingAquantion::Create);
}
