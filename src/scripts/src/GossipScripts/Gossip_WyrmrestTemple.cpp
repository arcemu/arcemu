/*
 * FrozenThrone Scripts
 * Copyright (C) 2008-2009 FrozenThrone Shard <http://www.dowlee.it/ft>
 */

#include "Setup.h"

//	Wyrmrest Temple Flighter
// Top to Bottom & Top to Middle
#define CN_TOP 26949
#define GOSSIP_TOP_TO_BOTTOM "Yes, please, I would like to return to the ground level of the temple."
#define GOSSIP_TOP_TO_MIDDLE "I would like to go Lord Afrasastrasz in the middle of the temple."
#define NPCTEXT_TOP 12714
// Middle to Top & Middle to Bottom
#define CN_MIDDLE 27575
#define GOSSIP_MIDDLE_TO_TOP "My lord, I need to get to the top of the temple."
#define GOSSIP_MIDDLE_TO_BOTTOM "Can I get a ride back to ground level, Lord Afrasastrasz?"
#define NPCTEXT_MIDDLE 12887
// Bottom to Top & Bottom to Middle
#define CN_BOTTOM 26443
#define GOSSIP_BOTTOM_TO_TOP "Steward, please allow me to ride one of the drakes to the queen's chamber at the top of the temple."
#define GOSSIP_BOTTOM_TO_MIDDLE "Can you spare a drake to take me to Lord Afrasastrasz in the middle of the temple?"
#define NPCTEXT_BOTTOM 12713

class SCRIPT_DECL WyrmrestTemple_FlightGossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 0);
			switch(pObject->GetEntry())
			{
				case CN_TOP:
					{
						menu.setTextID(NPCTEXT_TOP);
						menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_TOP_TO_BOTTOM, 1);
						menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_TOP_TO_MIDDLE, 2);
					}
					break;
				case CN_MIDDLE:
					{
						menu.setTextID(NPCTEXT_MIDDLE);
						menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_MIDDLE_TO_TOP, 3);
						menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_MIDDLE_TO_BOTTOM, 4);
					}
					break;
				case CN_BOTTOM:
					{
						menu.setTextID(NPCTEXT_BOTTOM);
						menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_BOTTOM_TO_TOP, 5);
						menu.AddItem(Arcemu::Gossip::ICON_CHAT, GOSSIP_BOTTOM_TO_MIDDLE, 6);
					}
					break;
			}

			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			Arcemu::Gossip::Menu::Complete(plr);
			switch(Id)
			{
				case 1:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(879), 6371, 0);     // Drake
					break;
				case 2:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(880), 6371, 0);     // Drake
					break;
				case 3:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(881), 6371, 0);     // Drake
					break;
				case 4:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(882), 6371, 0);     // Drake
					break;
				case 5:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(878), 6371, 0);     // Drake
					break;
				case 6:
					plr->TaxiStart(sTaxiMgr.GetTaxiPath(883), 6371, 0);     // Drake
					break;
				default:
					break;
			}
		}

};

void SetupWyrmrestTempleGossip(ScriptMgr* mgr)
{
	Arcemu::Gossip::Script* WyrmrestTempleFlightGossip = new WyrmrestTemple_FlightGossip;

	mgr->register_creature_gossip(CN_TOP, WyrmrestTempleFlightGossip);	// Torastrasza <Majordomo to the Ruling Council>
	mgr->register_creature_gossip(CN_MIDDLE, WyrmrestTempleFlightGossip);	// Lord Afrasastrasz <Commander of Wyrmrest Temple Defenses>
	mgr->register_creature_gossip(CN_BOTTOM, WyrmrestTempleFlightGossip);	// Tariolstrasz <Steward of Wyrmrest Temple>
}
