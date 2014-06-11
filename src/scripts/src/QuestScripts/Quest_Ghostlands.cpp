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

// TODO: add waypoints for gossip option 1 event.
class PrisonerGossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 1);
			if(plr->HasQuest(9164) && !plr->HasFinishedQuest(9164))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "<Administer the restorative draught.>", 0);
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* /*EnteredCode*/)
		{
			if (Id == 0)
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 8456);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "You're free to go now. The way out is safe.", 1);
			}
			else if(Id == 1)
			{
				int8 npc_id = -1;
				switch(pObject->GetEntry())
				{
					case 16206: npc_id = 0; break;
					case 16208: npc_id = 1; break;
					case 16209: npc_id = 2; break;
				}

				QuestLogEntry* en = plr->GetQuestLogForEntry(9164);
				if(npc_id >= 0 && en && en->GetMobCount(npc_id) < en->GetQuest()->required_mobcount[npc_id])
				{
 					en->SetMobCount(npc_id, en->GetMobCount(npc_id) + 1);
					en->SendUpdateAddKill(0);
					en->UpdatePlayerFields();

					Creature* pUnit = TO_CREATURE(pObject);
					pUnit->Despawn(5000, 6 * 60 * 1000);
					pUnit->SetStandState(STANDSTATE_STAND);
					pUnit->SetEmoteState(7);
				}
				Arcemu::Gossip::Menu::Complete(plr);
			}
			else
				Arcemu::Gossip::Menu::Complete(plr);
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
			if(pPlayer->HasQuest(9174))
			{
				Creature* naga = sEAS.SpawnCreature(pPlayer, 16292, 7938, -7632, 114, 3.05f, 0);
				naga->Despawn(6 * 60 * 1000, 0);
			}
		}
};

void SetupGhostlands(ScriptMgr* mgr)
{
	mgr->register_creature_gossip(16208, new PrisonerGossip);
	mgr->register_creature_gossip(16206, new PrisonerGossip);
	mgr->register_creature_gossip(16209, new PrisonerGossip);

	mgr->register_creature_script(16208, &PrisonersatDeatholme::Create);
	mgr->register_creature_script(16206, &PrisonersatDeatholme::Create);
	mgr->register_creature_script(16209, &PrisonersatDeatholme::Create);

	mgr->register_gameobject_script(181157, &VanquishingAquantion::Create);
}
