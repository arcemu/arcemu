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
				Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(354), 3);

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
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(355), 4);
						Menu->SendTo(Plr);
					}
					break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7756, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(356), 5);
						Menu->SendTo(Plr);
					}
					break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7757, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(357), 6);
						Menu->SendTo(Plr);
					}
					break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7758, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(358), 7);
						Menu->SendTo(Plr);
					}
					break;
				case 7:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7759, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(359), 8);
						Menu->SendTo(Plr);
					}
					break;
				case 8:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7760, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(360), 9);
						Menu->SendTo(Plr);
					}
					break;
				case 9:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7761, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(361), 10);
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
				Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(362), 3);

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
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(363), 4);
						Menu->SendTo(Plr);
					}
					break;
				case 4:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7763, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(364), 5);
						Menu->SendTo(Plr);
					}
					break;
				case 5:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7764, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(365), 6);
						Menu->SendTo(Plr);
					}
					break;
				case 6:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7765, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(366), 7);
						Menu->SendTo(Plr);
					}
					break;
				case 7:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7766, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(367), 8);
						Menu->SendTo(Plr);
					}
					break;
				case 8:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7767, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(368), 9);
						Menu->SendTo(Plr);
					}
					break;
				case 9:
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 7768, Plr);
						Menu->AddItem(0, Plr->GetSession()->LocalizedGossipTexts(369), 10);
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
void SetupSilithus(ScriptMgr* mgr)
{
	GossipScript* dearestNatalia1 = new DearestNatalia1();
	GossipScript* dearestNatalia2 = new DearestNatalia2();
	mgr->register_gossip_script(15170, dearestNatalia1);
	mgr->register_gossip_script(15171, dearestNatalia2);
}
