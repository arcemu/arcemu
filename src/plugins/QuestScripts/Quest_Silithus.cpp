/*
 * Yelly Database Scripts
 * Copyright (C) 2006-2008 U2 Team <http://www.undzwei.eu/>
 * Copyright (C) 2007-2008 Yelly Team
 * Copyright (C) 2009 WhyScripts Team <http://www.whydb.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

// quest #8304 - Dearest Natalia
class SCRIPT_DECL DearestNatalia1 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7736, Plr);

			if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
				Menu->AddItem(0, "Hello, Rutgar. The Commander has sent me here to gather some information about his missing wife.", 3);

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:
					GossipHello(pObject, Plr);
					break;
				case 3:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7755, Plr);
						Menu->AddItem(0, "That sounds dangerous.", 4);
						Menu->SendTo(Plr);
					}
					break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7756, Plr);
						Menu->AddItem(0, "What happened to her after that?", 5);
						Menu->SendTo(Plr);
					}
					break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7757, Plr);
						Menu->AddItem(0, "Natalia?", 6);
						Menu->SendTo(Plr);
					}
					break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7758, Plr);
						Menu->AddItem(0, "What demands?", 7);
						Menu->SendTo(Plr);
					}
					break;
				case 7:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7759, Plr);
						Menu->AddItem(0, "Lost it? What do you mean?", 8);
						Menu->SendTo(Plr);
					}
					break;
				case 8:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7760, Plr);
						Menu->AddItem(0, "Possessed by what?", 9);
						Menu->SendTo(Plr);
					}
					break;
				case 9:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7761, Plr);
						Menu->AddItem(0, "I'll be back once I straighten this mess out.", 10);
						Menu->SendTo(Plr);
					}
					break;
				case 10:
					{
						QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);
						if(en && en->GetMobCount(1) < en->GetQuest()->required_mobcount[1])
						{
							en->SetMobCount(1, 1);
							en->SendUpdateAddKill(1);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		}
};

class SCRIPT_DECL DearestNatalia2 : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* Plr)
		{
			GossipMenu* Menu;
			QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7735, Plr);

			// you need to speak to Rutgar first !!
			if(en && (en->GetMobCount(0) < en->GetQuest()->required_mobcount[0]) && (en->GetMobCount(1) == 1))
				Menu->AddItem(0, "Hello, Frankal. I've heard that you might have some information as to the whereabouts of Mistress Natalia Mar'alith.", 3);

			Menu->SendTo(Plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:
					GossipHello(pObject, Plr);
					break;
				case 3:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7762, Plr);
						Menu->AddItem(0, "That's what I like to hear.", 4);
						Menu->SendTo(Plr);
					}
					break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7763, Plr);
						Menu->AddItem(0, "That's odd.", 5);
						Menu->SendTo(Plr);
					}
					break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7764, Plr);
						Menu->AddItem(0, "You couldn't handle a lone night elf priestess?", 6);
						Menu->SendTo(Plr);
					}
					break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7765, Plr);
						Menu->AddItem(0, "I've been meaning to ask you about that monkey.", 7);
						Menu->SendTo(Plr);
					}
					break;
				case 7:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7766, Plr);
						Menu->AddItem(0, "Then what?", 8);
						Menu->SendTo(Plr);
					}
					break;
				case 8:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7767, Plr);
						Menu->AddItem(0, "What a story! So she went into Hive'Regal and that was the last you saw of her?", 9);
						Menu->SendTo(Plr);
					}
					break;
				case 9:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7768, Plr);
						Menu->AddItem(0, "Thanks for the information, Frankal.", 10);
						Menu->SendTo(Plr);
					}
					break;
				case 10:
					{
						QuestLogEntry* en = Plr->GetQuestLogForEntry(8304);
						if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
						{
							en->SetMobCount(0, 1);
							en->SendUpdateAddKill(0);
							en->UpdatePlayerFields();
						}
					}
					break;
			}
		}
};

#define GOSSIP1 "What do you know of it"
#define GOSSIP2 "I am listening, Demitrian."
#define GOSSIP3 "Continue, please."
#define GOSSIP4 "A battle?"
#define GOSSIP5 "<Nod>"
#define GOSSIP6 "Caught unaware? How?"
#define GOSSIP7 "o what did Ragnaros do next?"

class highlord_demitrianGossip : public GossipScript
{
	public:
		 void GossipHello(Object* pObject, Player* pPlayer)
		{
			//Send quests and gossip menu.
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6812, pPlayer->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), pPlayer, menu);
			if (pPlayer->HasItemCount(18563, 1, false) && pPlayer->HasItemCount(18564, 1, false))
			{
				if (pPlayer->HasItemCount(19016, 0, false))
				{
				menu.AddItem(0, GOSSIP1, 1);
				}
			}
			menu.Send(pPlayer);
};

		void GossipHello1(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6842, pPlayer->GetSession()->language);

			menu.AddItem(0, GOSSIP2, 2);
			menu.Send(pPlayer);
		};

		void GossipHello2(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6843, pPlayer->GetSession()->language);

			menu.AddItem(0, GOSSIP3, 3);
			menu.Send(pPlayer);
		};

		void GossipHello3(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6844, pPlayer->GetSession()->language);

			menu.AddItem(0, GOSSIP4, 4);
			menu.Send(pPlayer);
		};

		void GossipHello4(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6867, pPlayer->GetSession()->language);

			menu.AddItem(0, GOSSIP5, 5);
			menu.Send(pPlayer);
		};


		void GossipHello5(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6868, pPlayer->GetSession()->language);

			menu.AddItem(0, GOSSIP6, 6);
			menu.Send(pPlayer);
		};

		void GossipHello6(Object* pObject, Player* pPlayer)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 6869, pPlayer->GetSession()->language);

			menu.AddItem(0, GOSSIP7, 7);
			menu.Send(pPlayer);
		};

		void GossipSelectOption(Object* pObject, Player*  pPlayer, uint32 Id, uint32 IntId, const char* Code)
		{
			switch(IntId)
			{
				case 1:
					GossipHello1(pObject, pPlayer);
					break;
				case 2:
					GossipHello2(pObject, pPlayer);
					break;
				case 3:
					GossipHello3(pObject, pPlayer);
					break;
				case 4:
					GossipHello4(pObject, pPlayer);
					break;
				case 5:
					GossipHello5(pObject, pPlayer);
					break;
				case 6:
					GossipHello6(pObject, pPlayer);
					break;
				case 7:
					sEAS.AddItem(19016, pPlayer);
					pPlayer->Gossip_Complete();
					break;
			};
	};
};

class Thunderan : public QuestScript
{
	public:
		void OnQuestComplete(Player* mTarget, QuestLogEntry* qLogEntry)
		{
			sEAS.SpawnCreature(mTarget, 14435, -6241.0f, 1715.0f, 4.8f, 0.605017f,0, 1);
		}
};

void SetupSilithus(ScriptMgr* mgr)
{
	GossipScript* dearestNatalia1 = new DearestNatalia1();
	GossipScript* dearestNatalia2 = new DearestNatalia2();
	mgr->register_gossip_script(15170, dearestNatalia1);
	mgr->register_gossip_script(15171, dearestNatalia2);
	mgr->register_creature_gossip(14347, new highlord_demitrianGossip);
	mgr->register_quest_script(7786, new Thunderan);
}
