/*
 * Moon++ Scripts for Ascent MMORPG Server
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

/************************************************************************/
/* GENERAL GUARD SCRIPT                                                 */
/************************************************************************/

// Covers *all* guard types, scripting their texts to guide players around.
// Enable this define to make all gossip texts have a "back" / "I was looking
// for somethinge else" button added.

#define BACK_BUTTON

#ifdef BACK_BUTTON

// Make code neater with this define.
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

#else

// Make code neater with this define.
#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, Plr); \
    Menu->AddItem(0, "I was looking for something else.", 0); \
    Menu->SendTo(Plr);

#endif

/************************************************************************/
/* Stormwind CITY Guards                                                */
/************************************************************************/

class StormwindGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 933, plr);

			Menu->AddItem(0, "Auction House"		, 1);
			Menu->AddItem(0, "Bank of Stormwind"	, 2);
			Menu->AddItem(0, "Stormwind Harbor"		, 3);
			Menu->AddItem(0, "Deeprun Tram"			, 4);
			Menu->AddItem(0, "The Inn"				, 5);
			Menu->AddItem(0, "Gryphon Master"		, 6);
			Menu->AddItem(0, "Guild Master"			, 7);
			Menu->AddItem(0, "Mailbox"				, 8);
			Menu->AddItem(0, "Stable Master"		, 9);
			Menu->AddItem(0, "Weapons Trainer"		, 10);
			Menu->AddItem(0, "Officers' Lounge"		, 11);
			Menu->AddItem(0, "Battlemaster"			, 12);
			Menu->AddItem(0, "Barber"		, 13);
			Menu->AddItem(0, "Class Trainer"		, 14);
			Menu->AddItem(0, "Profession Trainer"	, 15);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // Auction House
					SendQuickMenu(3834);
					Plr->Gossip_SendPOI(-8811.46f, 667.46f, 7, 6, 0, "Stormwind Auction House");
					break;

				case 2:     // Bank of Stormwind
					SendQuickMenu(764);
					Plr->Gossip_SendPOI(-8916.87f, 622.87f, 7, 6, 0, "Stormwind Bank");
					break;

				case 3:     // Stormwind Harbor
					SendQuickMenu(13439);
					Plr->Gossip_SendPOI(-8634.77f, 949.64f, 7, 6, 0, "Stormwind Harbor");
					break;

				case 4:     // Deeprun Tram
					SendQuickMenu(3813);
					Plr->Gossip_SendPOI(-8378.88f, 554.23f, 7, 6, 0, "The Deeprun Tram");
					break;

				case 5:     // The Inn
					SendQuickMenu(3860);
					Plr->Gossip_SendPOI(-8869.0f, 675.4f, 7, 6, 0, "The Gilded Rose");
					break;

				case 6:     // Gryphon Master
					SendQuickMenu(879);
					Plr->Gossip_SendPOI(-8837.0f, 493.5f, 7, 6, 0, "Stormwind Gryphon Master");
					break;

				case 7:     // Guild Master
					SendQuickMenu(882);
					Plr->Gossip_SendPOI(-8894.0f, 611.2f, 7, 6, 0, "Stormwind Vistor`s Center");
					break;

				case 8:     // Mailbox
					SendQuickMenu(3861);
					Plr->Gossip_SendPOI(-8876.48f, 649.18f, 7, 6, 0, "Stormwind Mailbox");
					break;

				case 9:     // Stable Master
					SendQuickMenu(5984);
					Plr->Gossip_SendPOI(-8433.0f, 554.7f, 7, 6, 0, "Jenova Stoneshield");
					break;

				case 10:     // Weapons Master
					SendQuickMenu(4516);
					Plr->Gossip_SendPOI(-8797.0f, 612.8f, 7, 6, 0, "Woo Ping");
					break;

				case 11:    // Officers' Lounge
					SendQuickMenu(7047);
					Plr->Gossip_SendPOI(-8759.92f, 399.69f, 7, 6, 0, "Champions` Hall");
					break;

				case 12:    // Battlemaster
					{
						SendQuickMenu(10218);
						Plr->Gossip_SendPOI(-8393.62f, 274.21f, 7, 6, 0, "Battlemasters Stormwind");
					}
					break;

				case 13:     // Barber
					SendQuickMenu(13882);
					Plr->Gossip_SendPOI(-8743.15f, 660.36f, 7, 6, 0, "Stormwind Barber");
					break;

				case 14:    // Class Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 898, Plr);
						Menu->AddItem(0, "Druid"      , 16);
						Menu->AddItem(0, "Hunter"     , 17);
						Menu->AddItem(0, "Mage"       , 18);
						Menu->AddItem(0, "Paladin"    , 19);
						Menu->AddItem(0, "Priest"     , 20);
						Menu->AddItem(0, "Rogue"      , 21);
						Menu->AddItem(0, "Shaman"     , 22);
						Menu->AddItem(0, "Warlock"    , 23);
						Menu->AddItem(0, "Warrior"    , 24);
						Menu->SendTo(Plr);
					}
					break;

				case 15:    // Profession Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 918, Plr);
						Menu->AddItem(0, "Alchemy"           , 25);
						Menu->AddItem(0, "Blacksmithing"     , 26);
						Menu->AddItem(0, "Cooking"           , 27);
						Menu->AddItem(0, "Enchanting"        , 28);
						Menu->AddItem(0, "Engineering"       , 29);
						Menu->AddItem(0, "First Aid"         , 30);
						Menu->AddItem(0, "Fishing"           , 31);
						Menu->AddItem(0, "Herbalism"         , 32);
						Menu->AddItem(0, "Inscription"		, 33);
						Menu->AddItem(0, "Leatherworking"    , 34);
						Menu->AddItem(0, "Mining"            , 35);
						Menu->AddItem(0, "Skinning"          , 36);
						Menu->AddItem(0, "Tailoring"         , 37);
						Menu->SendTo(Plr);
					}
					break;

					////////////////
					// Class trainer submenu
					////////

				case 16: //Druid
					{
						Plr->Gossip_SendPOI(-8751.0f, 1124.5f, 7, 6, 0, "The Park");
						SendQuickMenu(902);
					}
					break;

				case 17: //Hunter
					{
						Plr->Gossip_SendPOI(-8413.0f, 541.5f, 7, 6, 0, "Hunter Lodge");
						SendQuickMenu(905);
					}
					break;

				case 18: //Mage
					{
						Plr->Gossip_SendPOI(-9012.0f, 867.6f, 7, 6, 0, "Wizard`s Sanctum");
						SendQuickMenu(899);
					}
					break;

				case 19: //Paladin
					{
						Plr->Gossip_SendPOI(-8577.0f, 881.7f, 7, 6, 0, "Cathedral Of Light");
						SendQuickMenu(904);
					}
					break;

				case 20: //Priest
					{
						Plr->Gossip_SendPOI(-8512.0f, 862.4f, 7, 6, 0, "Cathedral Of Light");
						SendQuickMenu(903);
					}
					break;

				case 21: //Rogue
					{
						Plr->Gossip_SendPOI(-8753.0f, 367.8f, 7, 6, 0, "Stormwind - Rogue House");
						SendQuickMenu(900);
					}
					break;

				case 22: //Shaman
					{
						Plr->Gossip_SendPOI(-9031.54f, 549.87f, 7, 6, 0, "Farseer Umbrua");
						SendQuickMenu(10106);
					}
					break;

				case 23: //Warlock
					{
						Plr->Gossip_SendPOI(-8948.91f, 998.35f, 7, 6, 0, "The Slaughtered Lamb");
						SendQuickMenu(906);
					}
					break;

				case 24: //Warrior
					{
						Plr->Gossip_SendPOI(-8714.14f, 334.96f, 7, 6, 0, "Stormwind Barracks");
						SendQuickMenu(901);
					}
					break;

				case 25: //Alchemy
					{
						Plr->Gossip_SendPOI(-8988.0f, 759.60f, 7, 6, 0, "Alchemy Needs");
						SendQuickMenu(919);
					}
					break;

				case 26: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-8424.0f, 616.9f, 7, 6, 0, "Therum Deepforge");
						SendQuickMenu(920);
					}
					break;

				case 27: //Cooking
					{
						Plr->Gossip_SendPOI(-8611.0f, 364.6f, 7, 6, 0, "Pig and Whistle Tavern");
						SendQuickMenu(921);
					}
					break;

				case 28: //Enchanting
					{
						Plr->Gossip_SendPOI(-8858.0f, 803.7f, 7, 6, 0, "Lucan Cordell");
						SendQuickMenu(941);
					}
					break;

				case 29: //Engineering
					{
						Plr->Gossip_SendPOI(-8347.0f, 644.1f, 7, 6, 0, "Lilliam Sparkspindle");
						SendQuickMenu(922);
					}
					break;

				case 30: //First Aid
					{
						Plr->Gossip_SendPOI(-8513.0f, 801.8f, 7, 6, 0, "Shaina Fuller");
						SendQuickMenu(923);
					}
					break;

				case 31: //Fishing
					{
						Plr->Gossip_SendPOI(-8803.0f, 767.5f, 7, 6, 0, "Arnold Leland");
						SendQuickMenu(940);
					}
					break;

				case 32: //Herbalism
					{
						Plr->Gossip_SendPOI(-8967.0f, 779.5f, 7, 6, 0, "Alchemy Needs");
						SendQuickMenu(924);
					}
					break;

				case 33: //Inscription
					{
						Plr->Gossip_SendPOI(-8853.33f, 857.66f, 7, 6, 0, "Stormwind Inscription");
						SendQuickMenu(13881);
					}
					break;

				case 34: //Leatherworking
					{
						Plr->Gossip_SendPOI(-8726.0f, 477.4f, 7, 6, 0, "The Protective Hide");
						SendQuickMenu(925);
					}
					break;

				case 35: //Mining
					{
						Plr->Gossip_SendPOI(-8434.0f, 692.8f, 7, 6, 0, "Gelman Stonehand");
						SendQuickMenu(927);
					}
					break;

				case 36: //Skinning
					{
						Plr->Gossip_SendPOI(-8716.0f, 469.4f, 7, 6, 0, "The Protective Hide");
						SendQuickMenu(928);
					}
					break;

				case 37: //Tailoring
					{
						Plr->Gossip_SendPOI(-8938.0f, 800.7f, 7, 6, 0, "Duncan`s Textiles");
						SendQuickMenu(929);
					}
					break;
			}
		}
};

class DarnassusGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3016, plr);

			Menu->AddItem(0, "Auction House"      , 1);
			Menu->AddItem(0, "The Bank"           , 2);
			Menu->AddItem(0, "Hippogryph Master"  , 3);
			Menu->AddItem(0, "Guild Master"       , 4);
			Menu->AddItem(0, "The Inn"            , 5);
			Menu->AddItem(0, "Mailbox"            , 6);
			Menu->AddItem(0, "Stable Master"      , 7);
			Menu->AddItem(0, "Weapons Trainer"    , 8);
			Menu->AddItem(0, "Battlemaster"       , 9);
			Menu->AddItem(0, "Class Trainer"      , 10);
			Menu->AddItem(0, "Profession Trainer" , 11);
			Menu->AddItem(0, "Lexicon of Power"   , 27);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers (Most/All 'borrowed' from scriptdev)
					/////

				case 1:     // Auction House
					SendQuickMenu(3833);
					Plr->Gossip_SendPOI(9861.23f, 2334.55f, 7, 6, 0, "Darnassus Auction House");
					break;

				case 2:		// The Bank
					SendQuickMenu(3017);
					Plr->Gossip_SendPOI(9938.45f, 2512.35f, 7, 6, 0, "Darnassus Bank");
					break;

				case 3:		// Hippogryph Master
					SendQuickMenu(3018);
					Plr->Gossip_SendPOI(9945.65f, 2618.94f, 7, 6, 0, "Rut'theran Village");
					break;

				case 4:		// Guild Master
					SendQuickMenu(3019);
					Plr->Gossip_SendPOI(10076.40f, 2199.59f, 7, 6, 0, "Darnassus Guild Master");
					break;

				case 5:		// The Inn
					SendQuickMenu(3020);
					Plr->Gossip_SendPOI(10133.29f, 2222.52f, 7, 6, 0, "Darnassus Inn");
					break;

				case 6:		// Mailbox
					SendQuickMenu(3021);
					Plr->Gossip_SendPOI(9942.17f, 2495.48f, 7, 6, 0, "Darnassus Mailbox");
					break;

				case 7:		// Stable Master
					SendQuickMenu(5980);
					Plr->Gossip_SendPOI(10167.20f, 2522.66f, 7, 6, 0, "Alassin");
					break;

				case 8:		// Weapons Trainer
					SendQuickMenu(4517);
					Plr->Gossip_SendPOI(9907.11f, 2329.70f, 7, 6, 0, "Ilyenia Moonfire");
					break;

				case 9:    // Battlemaster
					{
						SendQuickMenu(7519);
						Plr->Gossip_SendPOI(9981.9f, 2325.9f, 7, 6, 0, "Battlemasters Darnassus");
					}
					break;

				case 10:    // Class Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
						Menu->AddItem(0, "Druid"       , 12);
						Menu->AddItem(0, "Hunter"      , 13);
						Menu->AddItem(0, "Priest"      , 14);
						Menu->AddItem(0, "Rogue"       , 15);
						Menu->AddItem(0, "Warrior"     , 16);
						Menu->SendTo(Plr);
					}
					break;

				case 11:    // Profession Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
						Menu->AddItem(0, "Alchemy"           , 17);
						Menu->AddItem(0, "Cooking"           , 18);
						Menu->AddItem(0, "Enchanting"        , 19);
						Menu->AddItem(0, "First Aid"         , 20);
						Menu->AddItem(0, "Fishing"           , 21);
						Menu->AddItem(0, "Herbalism"         , 22);
						Menu->AddItem(0, "Inscription"       , 23);
						Menu->AddItem(0, "Leatherworking"    , 24);
						Menu->AddItem(0, "Skinning"          , 25);
						Menu->AddItem(0, "Tailoring"         , 26);
						Menu->SendTo(Plr);
					}
					break;

				case 12:	// Druid
					{
						Plr->Gossip_SendPOI(10186, 2570.46f, 7, 6, 0, "Darnassus Druid Trainer");
						SendQuickMenu(3024);
					}
					break;

				case 13:	// Hunter
					{
						Plr->Gossip_SendPOI(10177.29f, 2511.10f, 7, 6, 0, "Darnassus Hunter Trainer");
						SendQuickMenu(3023);
					}
					break;

				case 14:	// Priest
					{
						Plr->Gossip_SendPOI(9659.12f, 2524.88f, 7, 6, 0, "Temple of the Moon");
						SendQuickMenu(3025);
					}
					break;

				case 15:	// Rogue
					{
						Plr->Gossip_SendPOI(10122, 2599.12f, 7, 6, 0, "Darnassus Rogue Trainer");
						SendQuickMenu(3026);
					}
					break;

				case 16:	// Warrior
					{
						Plr->Gossip_SendPOI(9951.91f, 2280.38f, 7, 6, 0, "Warrior's Terrace");
						SendQuickMenu(3033);
					}
					break;

				case 17: //Alchemy
					{
						Plr->Gossip_SendPOI(10075.90f, 2356.76f, 7, 6, 0, "Darnassus Alchemy Trainer");
						SendQuickMenu(3035);
					}
					break;

				case 18: //Cooking
					{
						Plr->Gossip_SendPOI(10088.59f, 2419.21f, 7, 6, 0, "Darnassus Cooking Trainer");
						SendQuickMenu(3036);
					}
					break;

				case 19: //Enchanting
					{
						Plr->Gossip_SendPOI(10146.09f, 2313.42f, 7, 6, 0, "Darnassus Enchanting Trainer");
						SendQuickMenu(3337);
					}
					break;

				case 20: //First Aid
					{
						Plr->Gossip_SendPOI(10150.09f, 2390.43f, 7, 6, 0, "Darnassus First Aid Trainer");
						SendQuickMenu(3037);
					}
					break;

				case 21: //Fishing
					{
						Plr->Gossip_SendPOI(9836.20f, 2432.17f, 7, 6, 0, "Darnassus Fishing Trainer");
						SendQuickMenu(3038);
					}
					break;

				case 22: //Herbalism
					{
						Plr->Gossip_SendPOI(9757.17f, 2430.16f, 7, 6, 0, "Darnassus Herbalism Trainer");
						SendQuickMenu(3039);
					}
					break;
				case 23: //Inscription
					{
						Plr->Gossip_SendPOI(10146.09f, 2313.42f, 7, 6, 0, "Darnassus Inscription Trainer");
						SendQuickMenu(13886);
					}
					break;

				case 24: //Leatherworking
					{
						Plr->Gossip_SendPOI(10086.59f, 2255.77f, 7, 6, 0, "Darnassus Leatherworking Trainer");
						SendQuickMenu(3040);
					}
					break;

				case 25: //Skinning
					{
						Plr->Gossip_SendPOI(10081.40f, 2257.18f, 7, 6, 0, "Darnassus Skinning Trainer");
						SendQuickMenu(3042);
					}
					break;

				case 26: //Tailoring
					{
						Plr->Gossip_SendPOI(10079.70f, 2268.19f, 7, 6, 0, "Darnassus Tailor");
						SendQuickMenu(3044);
					}
					break;

				case 27: //Lexicon of Power
					{
						Plr->Gossip_SendPOI(10146.09f, 2313.42f, 7, 6, 0, "Darnassus Inscription Trainer");
						SendQuickMenu(14174);
					}
					break;
			}
		}
		void GossipEnd(Object* pObject, Player* Plr)
		{

		}
};

class GoldshireGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4259, plr);
			Menu->AddItem(0, "Bank"                 , 1);
			Menu->AddItem(0, "Gryphon Master"       , 2);
			Menu->AddItem(0, "Guild Master"         , 3);
			Menu->AddItem(0, "Inn"                  , 4);
			Menu->AddItem(0, "Stable Master"        , 5);
			Menu->AddItem(0, "Class Trainer"        , 6);
			Menu->AddItem(0, "Profession Trainer"   , 7);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{

				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     //Bank
					SendQuickMenu(4260);
					break;

				case 2:     //Gryphon Master
					SendQuickMenu(4261);
					break;

				case 3:     //Guild Master
					SendQuickMenu(4262);
					break;

				case 4:     //Inn
					SendQuickMenu(4263);
					Plr->Gossip_SendPOI(-9459.34f, 42.08f, 99, 6, 0, "Lion's Pride Inn");
					break;

				case 5:     //Stable Master
					SendQuickMenu(5983);
					Plr->Gossip_SendPOI(-9466.62f, 45.87f, 99, 6, 0, "Erma");
					break;

				case 6:     //Class Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);

						Menu->AddItem(0, "Druid",                8);
						Menu->AddItem(0, "Hunter",               9);
						Menu->AddItem(0, "Mage",                 10);
						Menu->AddItem(0, "Paladin",              11);
						Menu->AddItem(0, "Priest",               12);
						Menu->AddItem(0, "Rogue",                13);
						Menu->AddItem(0, "Shaman",               14);
						Menu->AddItem(0, "Warlock",              15);
						Menu->AddItem(0, "Warrior",              16);

						Menu->SendTo(Plr);
					}
					break;

				case 7:		//Profession Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);

						Menu->AddItem(0, "Alchemy"              , 17);
						Menu->AddItem(0, "Blacksmithing"        , 18);
						Menu->AddItem(0, "Cooking"              , 19);
						Menu->AddItem(0, "Enchanting"           , 20);
						Menu->AddItem(0, "Engineering"          , 21);
						Menu->AddItem(0, "First Aid"            , 22);
						Menu->AddItem(0, "Fishing"              , 23);
						Menu->AddItem(0, "Herbalism"            , 24);
						Menu->AddItem(0, "Inscription"       , 25);
						Menu->AddItem(0, "Leatherworking"       , 26);
						Menu->AddItem(0, "Mining"               , 27);
						Menu->AddItem(0, "Skinning"             , 28);
						Menu->AddItem(0, "Tailoring"            , 29);
						Menu->SendTo(Plr);
					}
					break;

				case 8: //Druid
					{
						SendQuickMenu(4265);
					}
					break;

				case 9: //Hunter
					{
						SendQuickMenu(4266);
					}
					break;

				case 10: //Mage
					{
						Plr->Gossip_SendPOI(-9471.12f, 33.44f, 7, 6, 0, "Zaldimar Wefhellt");
						SendQuickMenu(4268);
					}
					break;

				case 11: //Paladin
					{
						Plr->Gossip_SendPOI(-9469, 108.05f, 7, 6, 0, "Brother Wilhelm");
						SendQuickMenu(4269);
					}
					break;

				case 12: //Priest
					{
						Plr->Gossip_SendPOI(-9461.07f, 32.6f, 7, 6, 0, "Priestess Josetta");
						SendQuickMenu(4267);
					}
					break;

				case 13: //Rogue
					{
						Plr->Gossip_SendPOI(-9465.13f, 13.29f, 7, 6, 0, "Keryn Sylvius");
						SendQuickMenu(4270);
					}
					break;

				case 14: //Shaman
					{
						SendQuickMenu(10101);
					}
					break;

				case 15: //Warlock
					{
						Plr->Gossip_SendPOI(-9473.21f, -4.08f, 7, 6, 0, "Maximillian Crowe");
						SendQuickMenu(4272);
					}
					break;

				case 16: //Warrior
					{
						Plr->Gossip_SendPOI(-9461.82f, 109.50f, 7, 6, 0, "Lyria Du Lac");
						SendQuickMenu(4271);
					}
					break;

				case 17: //Alchemy
					{
						Plr->Gossip_SendPOI(-9057.04f, 153.63f, 7, 6, 0, "Alchemist Mallory");
						SendQuickMenu(4274);
					}
					break;

				case 18: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-9456.58f, 87.90f, 7, 6, 0, "Smith Argus");
						SendQuickMenu(4275);
					}
					break;

				case 19: //Cooking
					{
						Plr->Gossip_SendPOI(-9467.54f, -3.16f, 7, 6, 0, "Tomas");
						SendQuickMenu(4276);
					}
					break;

				case 20: //Enchanting
					{
						SendQuickMenu(4277);
					}
					break;

				case 21: //Engineering
					{
						SendQuickMenu(4278);
					}
					break;

				case 22: //First Aid
					{
						Plr->Gossip_SendPOI(-9456.82f, 30.49f, 7, 6, 0, "Michelle Belle");
						SendQuickMenu(4279);
					}
					break;

				case 23: //Fishing
					{
						Plr->Gossip_SendPOI(-9386.54f, -118.73f, 7, 6, 0, "Lee Brown");
						SendQuickMenu(4280);
					}
					break;

				case 24: //Herbalism
					{
						Plr->Gossip_SendPOI(-9060.70f, 149.23f, 7, 6, 0, "Herbalist Pomeroy");
						SendQuickMenu(4281);
					}
					break;

				case 25: //Inscription
					{
						Plr->Gossip_SendPOI(-8853.33f, 857.66f, 7, 6, 0, "Stormwind Inscription");
						SendQuickMenu(13881);
					}
					break;

				case 26: //Leatherworking
					{
						Plr->Gossip_SendPOI(-9376.12f, -75.23f, 7, 6, 0, "Adele Fielder");
						SendQuickMenu(4282);
					}
					break;

				case 27: //Mining
					{
						SendQuickMenu(4283);
					}
					break;

				case 28: //Skinning
					{
						Plr->Gossip_SendPOI(-9536.91f, -1212.76f, 7, 6, 0, "Helene Peltskinner");
						SendQuickMenu(4284);
					}
					break;

				case 29: //Tailoring
					{
						Plr->Gossip_SendPOI(-9376.12f, -75.23f, 7, 6, 0, "Eldrin");
						SendQuickMenu(4285);
					}
					break;
			}
		}
};

class UndercityGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, plr);

			Menu->AddItem(0, "The bank", 1);
			Menu->AddItem(0, "The bat handler", 2);
			Menu->AddItem(0, "The guild master", 3);
			Menu->AddItem(0, "The inn", 4);
			Menu->AddItem(0, "The mailbox", 5);
			Menu->AddItem(0, "The auction house", 6);
			Menu->AddItem(0, "The zeppelin master", 7);
			Menu->AddItem(0, "The weapon master", 8);
			Menu->AddItem(0, "The stable master", 9);
			Menu->AddItem(0, "The battlemaster", 10);
			Menu->AddItem(0, "A class trainer", 11);
			Menu->AddItem(0, "A profession trainer", 12);
			Menu->AddItem(0, "Locksmith", 32);
			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The bank
					SendQuickMenu(3514);
					Plr->Gossip_SendPOI(1595.64f, 232.45f, 7, 6, 0, "Undercity Bank");
					break;

				case 2:     // The bat handler
					SendQuickMenu(3515);
					Plr->Gossip_SendPOI(1565.9f, 271.43f, 7, 6, 0, "Undercity Bat Handler");
					break;

				case 3:     // The guild master
					SendQuickMenu(3516);
					Plr->Gossip_SendPOI(1594.17f, 205.57f, 7, 6, 0, "Undercity Guild Master");
					break;

				case 4:     // The inn
					SendQuickMenu(3517);
					Plr->Gossip_SendPOI(1639.43f, 220.99f, 7, 6, 0, "Undercity Inn");
					break;

				case 5:     // The mailbox
					SendQuickMenu(3518);
					Plr->Gossip_SendPOI(1632.68f, 219.4f, 7, 6, 0, "Undercity Mailbox");
					break;

				case 6:     // The auction house
					SendQuickMenu(3520);
					Plr->Gossip_SendPOI(1647.9f, 258.49f, 7, 6, 0, "Undercity Auction House");
					break;

				case 7:     // The zeppelin master
					SendQuickMenu(3519);
					Plr->Gossip_SendPOI(2059, 274.86f, 7, 6, 0, "Undercity Zeppelin");
					break;

				case 8:     // The weapon master
					SendQuickMenu(4521);
					Plr->Gossip_SendPOI(1670.31f, 324.66f, 7, 6, 0, "Archibald");
					break;

				case 9:     // The stable master
					SendQuickMenu(5979);
					Plr->Gossip_SendPOI(1634.18f, 226.76f, 7, 6, 0, "Anya Maulray");
					break;

				case 10:    // The battlemaster
					{
						SendQuickMenu(7527);
						Plr->Gossip_SendPOI(1300.33f, 350.92f, 7, 6, 0, "Battlemasters Undercity");
					}
					break;

				case 11:    // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3542, Plr);
						Menu->AddItem(0, "Mage"		 , 13);
						Menu->AddItem(0, "Paladin"      , 14);
						Menu->AddItem(0, "Priest"       , 15);
						Menu->AddItem(0, "Rogue"        , 16);
						Menu->AddItem(0, "Warlock"      , 17);
						Menu->AddItem(0, "Warrior"      , 18);
						Menu->SendTo(Plr);
					}
					break;

				case 12:    // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
						Menu->AddItem(0, "Alchemy"           , 19);
						Menu->AddItem(0, "Blacksmithing"     , 20);
						Menu->AddItem(0, "Cooking"           , 21);
						Menu->AddItem(0, "Enchanting"        , 22);
						Menu->AddItem(0, "Engineering"       , 23);
						Menu->AddItem(0, "First Aid"         , 24);
						Menu->AddItem(0, "Fishing"           , 25);
						Menu->AddItem(0, "Herbalism"         , 26);
						Menu->AddItem(0, "Leatherworking"    , 27);
						Menu->AddItem(0, "Mining"            , 28);
						Menu->AddItem(0, "Skinning"          , 29);
						Menu->AddItem(0, "Tailoring"         , 30);
						Menu->SendTo(Plr);
					}
					break;

					////////////////
					// Class trainer submenu
					////////

				case 13: //Mage
					{
						Plr->Gossip_SendPOI(1781, 53, 7, 6, 0, "Undercity Mage Trainers");
						SendQuickMenu(3513);
					}
					break;

				case 14: //Paladin
					{
						Plr->Gossip_SendPOI(1298.98f, 316.51f, 7, 6, 0, "Champion Cyssa Downrose");
						SendQuickMenu(3521);
					}
					break;

				case 15: //Priest
					{
						Plr->Gossip_SendPOI(1758.33f, 401.5f, 7, 6, 0, "Undercity Priest Trainers");
						SendQuickMenu(3521);
					}
					break;

				case 16: //Rogue
					{
						Plr->Gossip_SendPOI(1418.56f, 65, 7, 6, 0, "Undercity Rogue Trainers");
						SendQuickMenu(3526);
					}
					break;

				case 17: //Warlock
					{
						Plr->Gossip_SendPOI(1780.92f, 53.16f, 7, 6, 0, "Undercity Warlock Trainers");
						SendQuickMenu(3526);
					}
					break;

				case 18: //Warrior
					{
						Plr->Gossip_SendPOI(1775.59f, 418.19f, 7, 6, 0, "Undercity Warrior Trainers");
						SendQuickMenu(3527);
					}
					break;

				case 19: //Alchemy
					{
						Plr->Gossip_SendPOI(1419.82f, 417.19f, 7, 6, 0, "The Apothecarium");
						SendQuickMenu(3528);
					}
					break;

				case 20: //Blacksmithing
					{
						Plr->Gossip_SendPOI(1696, 285, 7, 6, 0, "Undercity Blacksmithing Trainer");
						SendQuickMenu(3529);
					}
					break;

				case 21: //Cooking
					{
						Plr->Gossip_SendPOI(1596.34f, 274.68f, 7, 6, 0, "Undercity Cooking Trainer");
						SendQuickMenu(3530);
					}
					break;

				case 22: //Enchanting
					{
						Plr->Gossip_SendPOI(1488.54f, 280.19f, 7, 6, 0, "Undercity Enchanting Trainer");
						SendQuickMenu(3531);
					}
					break;

				case 23: //Engineering
					{
						Plr->Gossip_SendPOI(1408.58f, 143.43f, 7, 6, 0, "Undercity Engineering Trainer");
						SendQuickMenu(3532);
					}
					break;

				case 24: //First Aid
					{
						Plr->Gossip_SendPOI(1519.65f, 167.19f, 7, 6, 0, "Undercity First Aid Trainer");
						SendQuickMenu(3533);
					}
					break;

				case 25: //Fishing
					{
						Plr->Gossip_SendPOI(1679.9f, 89, 7, 6, 0, "Undercity Fishing Trainer");
						SendQuickMenu(3534);
					}
					break;

				case 26: //Herbalism
					{
						Plr->Gossip_SendPOI(1558, 349.36f, 7, 6, 0, "Undercity Herbalism Trainer");
						SendQuickMenu(3535);
					}
					break;

				case 27: //Leatherworking
					{
						Plr->Gossip_SendPOI(1498.76f, 196.43f, 7, 6, 0, "Undercity Leatherworking Trainer");
						SendQuickMenu(3536);
					}
					break;

				case 28: //Mining
					{
						Plr->Gossip_SendPOI(1642.88f, 335.58f, 7, 6, 0, "Undercity Mining Trainer");
						SendQuickMenu(3537);
					}
					break;

				case 29: //Skinning
					{
						Plr->Gossip_SendPOI(1498.6f, 196.46f, 7, 6, 0, "Undercity Skinning Trainer");
						SendQuickMenu(3538);
					}
					break;

				case 30: //Tailoring
					{
						Plr->Gossip_SendPOI(1689.55f, 193, 7, 6, 0, "Undercity Tailoring Trainer");
						SendQuickMenu(3539);
					}
					break;
				case 32:     // Locksmith
					{
						Plr->Gossip_SendPOI(1632.68f, 219.4f, 7, 99, 0, "Undercity Locksmith");
						SendQuickMenu(14916);
					}break;
			}
		}
};

class UndercityGuardOverseer : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* Plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), 15321);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The auction house", 1);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The bank", 2);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Barber", 3);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The bat handler", 4);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The battlemaster", 5);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The guild master", 6);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The inn", 7);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Locksmith", 8);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The mailbox", 9);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The stable master", 10);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The weapon master", 11);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The zeppelin master", 12);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "A class trainer", 13);
			menu.AddItem(Arcemu::Gossip::ICON_CHAT, "A profession trainer", 14);
			menu.Send(Plr);
		}

		void OnSelectOption(Object* pObject, Player* Plr, uint32 Id, const char * Code)
		{
			switch(Id)
			{
			case 0:	OnHello(pObject, Plr);	break;
			case 1:     // The auction house
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14900, Plr);
					Plr->Gossip_SendPOI(1647.9f, 258.49f, 7, 99, 0, "Undercity Auction House");
				}break;
			case 2:     // The bank
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14901, Plr);
					Plr->Gossip_SendPOI(1595.64f, 232.45f, 7, 99, 0, "Undercity Bank");
				}break;
			case 3:     // Barber
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14902, Plr);
					Plr->Gossip_SendPOI(1576.20f, 193.14f, 7, 99, 0, "Undercity Barber");
				}break;
			case 4:     // The bat handler
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14903, Plr);
					Plr->Gossip_SendPOI(1565.9f, 271.43f, 7, 99, 0, "Undercity Bat Handler");
				}break;
			case 5:    // The battlemaster
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14904, Plr);
					Plr->Gossip_SendPOI(1300.33f, 350.92f, 7, 99, 0, "Battlemasters Undercity");
				}break;
			case 6:     // The guild master
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14911, Plr);
					Plr->Gossip_SendPOI(1594.17f, 205.57f, 7, 99, 0, "Undercity Guild Master");
				}break;
			case 7:     // The inn
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14913, Plr);
					Plr->Gossip_SendPOI(1639.43f, 220.99f, 7, 99, 0, "Undercity Inn");
				}break;
			case 8:     // Locksmith
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14916, Plr);
					Plr->Gossip_SendPOI(1499.68f, 53.28f, 7, 99, 0, "Undercity Locksmith");
				}break;
			case 9:     // The mailbox
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14918, Plr);
					Plr->Gossip_SendPOI(1632.68f, 219.4f, 7, 99, 0, "Undercity Mailbox");
				}break;
			case 10:     // The stable master
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14924, Plr);
					Plr->Gossip_SendPOI(1634.18f, 226.76f, 7, 99, 0, "Anya Maulray");
				}break;
			case 11:     // The weapon master
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14928, Plr);
					Plr->Gossip_SendPOI(1670.31f, 324.66f, 7, 99, 0, "Archibald");
				}break;
			case 12:     // The zeppelin master
				{
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14929, Plr);
					Plr->Gossip_SendPOI(2059.0f, 274.86f, 7, 99, 0, "Undercity Zeppelin");
				}break;
			case 13:    // A class trainer
				{
					Arcemu::Gossip::Menu menu(pObject->GetGUID(), 3542);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Mage"         , 15);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Paladin"      , 16);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Priest"       , 17);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Rogue"        , 18);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Warlock"      , 19);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Warrior"      , 20);
					menu.Send(Plr);
				}break;
			case 14:    // A profession trainer
				{
					Arcemu::Gossip::Menu menu(pObject->GetGUID(), 3541);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Alchemy"           , 21);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Blacksmithing"     , 22);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Cooking"           , 23);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Enchanting"        , 24);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Engineering"       , 25);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "First Aid"         , 26);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Fishing"           , 27);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Herbalism"         , 28);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Inscription"       , 29);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Leatherworking"    , 30);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Mining"            , 31);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Skinning"          , 32);
					menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Tailoring"         , 33);
					menu.Send(Plr);
				}break;
			case 15: //Mage
				{
					Plr->Gossip_SendPOI(1781.0f, 53.0f, 7, 99, 0, "Undercity Mage Trainers");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3513, Plr);
				}break;
			case 16: //Paladin
				{
					Plr->Gossip_SendPOI(1298.98f, 316.51f, 7, 99, 0, "Champion Cyssa Downrose");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3521, Plr);
				}break;
			case 17: //Priest
				{
					Plr->Gossip_SendPOI(1758.33f, 401.5f, 7, 99, 0, "Undercity Priest Trainers");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3521, Plr);
				}break;
			case 18: //Rogue
				{
					Plr->Gossip_SendPOI(1418.56f, 65.0f, 7, 99, 0, "Undercity Rogue Trainers");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3526, Plr);
				}break;
			case 19: //Warlock
				{
					Plr->Gossip_SendPOI(1780.92f, 53.16f, 7, 99, 0, "Undercity Warlock Trainers");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3526, Plr);
				}break;
			case 20: //Warrior
				{
					Plr->Gossip_SendPOI(1775.59f, 418.19f, 7, 99, 0, "Undercity Warrior Trainers");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3527, Plr);
				}break;
			case 21: //Alchemy
				{
					Plr->Gossip_SendPOI(1419.82f, 417.19f, 7, 99, 0, "The Apothecarium");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3528, Plr);
				}break;
			case 22: //Blacksmithing
				{
					Plr->Gossip_SendPOI(1696.0f, 285.0f, 7, 99, 0, "Undercity Blacksmithing Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3529, Plr);
				}break;
			case 23: //Cooking
				{
					Plr->Gossip_SendPOI(1596.34f, 274.68f, 7, 99, 0, "Undercity Cooking Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3530, Plr);
				}break;
			case 24: //Enchanting
				{
					Plr->Gossip_SendPOI(1488.54f, 280.19f, 7, 99, 0, "Undercity Enchanting Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3531, Plr);
				}break;
			case 25: //Engineering
				{
					Plr->Gossip_SendPOI(1408.58f, 143.43f, 7, 99, 0, "Undercity Engineering Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3532, Plr);
				}break;
			case 26: //First Aid
				{
					Plr->Gossip_SendPOI(1519.65f, 167.19f, 7, 99, 0, "Undercity First Aid Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3533, Plr);
				}break;
			case 27: //Fishing
				{
					Plr->Gossip_SendPOI(1679.9f, 89.0f, 7, 99, 0, "Undercity Fishing Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3534, Plr);
				}break;
			case 28: //Herbalism
				{
					Plr->Gossip_SendPOI(1558.0f, 349.36f, 7, 99, 0, "Undercity Herbalism Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3535, Plr);
				}break;
			case 29: //Inscription
				{
					Plr->Gossip_SendPOI(1558.0f, 349.36f, 7, 99, 0, "Undercity Inscription");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14914, Plr);
				}break;
			case 30: //Leatherworking
				{
					Plr->Gossip_SendPOI(1498.76f, 196.43f, 7, 99, 0, "Undercity Leatherworking Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3536, Plr);
				}break;
			case 31: //Mining
				{
					Plr->Gossip_SendPOI(1642.88f, 335.58f, 7, 99, 0, "Undercity Mining Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3537, Plr);
				}break;
			case 32: //Skinning
				{
					Plr->Gossip_SendPOI(1498.6f, 196.46f, 7, 99, 0, "Undercity Skinning Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3538, Plr);
				}break;
			case 33: //Tailoring
				{
					Plr->Gossip_SendPOI(1689.55f, 193.0f, 7, 99, 0, "Undercity Tailoring Trainer");
					Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 3539, Plr);
				}break;
			}
		}
};

class TeldrassilGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4316, plr);

			Menu->AddItem(0, "The Bank", 1);
			Menu->AddItem(0, "Rut'Theran Ferry", 2);
			Menu->AddItem(0, "The Guild Master", 3);
			Menu->AddItem(0, "The Inn", 4);
			Menu->AddItem(0, "Stable Master", 5);
			Menu->AddItem(0, "Class Trainer", 6);
			Menu->AddItem(0, "Profession Trainer", 7);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The Bank
					SendQuickMenu(4317);
					break;

				case 2:		// Rut'theran erry
					SendQuickMenu(4318);
					break;

				case 3:		// The Guild Master
					SendQuickMenu(4319);
					break;

				case 4:		// The Inn
					Plr->Gossip_SendPOI(9821.49f, 960.13f, 7, 6, 0, "Dolanaar Inn");
					SendQuickMenu(4320);
					break;

				case 5:		// Stable Master
					Plr->Gossip_SendPOI(9808.37f, 931.1f, 7, 6, 0, "Seriadne");
					SendQuickMenu(5982);
					break;

				case 6:    // Class Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
						Menu->AddItem(0, "Druid"       , 8);
						Menu->AddItem(0, "Hunter"      , 9);
						Menu->AddItem(0, "Priest"     , 10);
						Menu->AddItem(0, "Rogue"      , 11);
						Menu->AddItem(0, "Warrior"    , 12);
						Menu->SendTo(Plr);
					}
					break;

				case 7:    // Profession Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
						Menu->AddItem(0, "Alchemy"           , 13);
						Menu->AddItem(0, "Cooking"           , 14);
						Menu->AddItem(0, "Enchanting"        , 15);
						Menu->AddItem(0, "First Aid"         , 16);
						Menu->AddItem(0, "Fishing"           , 17);
						Menu->AddItem(0, "Herbalism"         , 18);
						Menu->AddItem(0, "Inscription"       , 19);
						Menu->AddItem(0, "Leatherworking"    , 20);
						Menu->AddItem(0, "Skinning"          , 21);
						Menu->AddItem(0, "Tailoring"         , 22);
						Menu->SendTo(Plr);
					}
					break;

					////////////////
					// Class trainer submenu
					////////

				case 8: //Druid
					{
						Plr->Gossip_SendPOI(9741.58f, 963.7f, 7, 6, 0, "Kal");
						SendQuickMenu(4323);
					}
					break;

				case 9: // Hunter
					{
						Plr->Gossip_SendPOI(9815.12f, 926.28f, 7, 6, 0, "Dazalar");
						SendQuickMenu(4324);
					}
					break;

				case 10: // Priest
					{
						Plr->Gossip_SendPOI(9906.16f, 986.63f, 7, 6, 0, "Laurna Morninglight");
						SendQuickMenu(4325);
					}
					break;

				case 11: // Rogue
					{
						Plr->Gossip_SendPOI(9789, 942.86f, 7, 6, 0, "Jannok Breezesong");
						SendQuickMenu(4326);
					}
					break;

				case 12: // Warrior
					{
						Plr->Gossip_SendPOI(9821.96f, 950.61f, 7, 6, 0, "Kyra Windblade");
						SendQuickMenu(4327);
					}
					break;

				case 13: // Alchemy
					{
						Plr->Gossip_SendPOI(9767.59f, 878.81f, 7, 6, 0, "Cyndra Kindwhisper");
						SendQuickMenu(4329);
					}
					break;

				case 14: // Cooking
					{
						Plr->Gossip_SendPOI(9751.19f, 906.13f, 7, 6, 0, "Zarrin");
						SendQuickMenu(4330);
					}
					break;

				case 15: // Enchanting
					{
						Plr->Gossip_SendPOI(10677.59f, 1946.56f, 7, 6, 0, "Alanna Raveneye");
						SendQuickMenu(4331);
					}
					break;

				case 16: // First Aid
					{
						Plr->Gossip_SendPOI(9903.12f, 999, 7, 6, 0, "Byancie");
						SendQuickMenu(4332);
					}
					break;

				case 17: // Fishing
					{
						SendQuickMenu(4333);
					}
					break;

				case 18: // Herbalism
					{
						Plr->Gossip_SendPOI(9773.78f, 875.88f, 7, 6, 0, "Malorne Bladeleaf");
						SendQuickMenu(4334);
					}
					break;

				case 19: // Inscription
					{
						Plr->Gossip_SendPOI(10146.09f, 2313.42f, 7, 6, 0, "Darnassus Inscription Trainer");
						SendQuickMenu(13886);
					}
					break;

				case 20: // Leatherworking
					{
						Plr->Gossip_SendPOI(10152.59f, 1681.46f, 7, 6, 0, "Nadyia Maneweaver");
						SendQuickMenu(4335);
					}
					break;

				case 21: // Skinning
					{
						Plr->Gossip_SendPOI(10135.59f, 1673.18f, 7, 6, 0, "Radnaal Maneweaver");
						SendQuickMenu(4336);
					}
					break;

				case 22: // Tailoring
					{
						SendQuickMenu(4337);
					}
					break;
			}
		}
};

class SilvermoonGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, plr);

			Menu->AddItem(0, "Auction House"		, 1);
			Menu->AddItem(0, "The Bank"				, 2);
			Menu->AddItem(0, "Dragonhawk Master"	, 3);
			Menu->AddItem(0, "Guild Master"			, 4);
			Menu->AddItem(0, "The Inn"				, 5);
			Menu->AddItem(0, "Mailbox"				, 6);
			Menu->AddItem(0, "Stable Master"		, 7);
			Menu->AddItem(0, "Weapon Master"		, 8);
			Menu->AddItem(0, "Battlemaster"			, 9);
			Menu->AddItem(0, "Class Trainer"		, 10);
			Menu->AddItem(0, "Profession Trainer"	, 11);
			Menu->AddItem(0, "Mana Loom"			, 12);
			Menu->AddItem(0, "Lexicon of Power"		, 40);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // Auction House
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9317, Plr);
					Menu->AddItem(0, "To the west."  , 13);
					Menu->AddItem(0, "To the east."    , 14);
					Menu->SendTo(Plr);
					break;

				case 2:     // The Bank
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9320, Plr);
					Menu->AddItem(0, "The west."  , 15);
					Menu->AddItem(0, "The east."    , 16);
					Menu->SendTo(Plr);
					break;

				case 3:     // Dragonhawk Master
					SendQuickMenu(9323);
					Plr->Gossip_SendPOI(9378.45f, -7163.94f, 7, 6, 0, "Silvermoon City, Flight Master");
					break;

				case 4:     // Guild Master
					SendQuickMenu(9324);
					Plr->Gossip_SendPOI(9480.75f, -7345.587f, 7, 6, 0, "Silvermoon City, Guild Master");
					break;

				case 5:     // The Inn
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9325, Plr);
					Menu->AddItem(0, "The Silvermoon City Inn."  , 17);
					Menu->AddItem(0, "The Wayfarer's Rest tavern."    , 18);
					Menu->SendTo(Plr);
					break;

				case 6:     // Mailbox
					SendQuickMenu(9326);
					Plr->Gossip_SendPOI(9743.078f, -7466.4f, 7, 6, 0, "Silvermoon City, Mailbox");
					break;

				case 7:     // Stable Master
					SendQuickMenu(9327);
					Plr->Gossip_SendPOI(9904.95f, -7404.31f, 7, 6, 0, "Silvermoon City, Stable Master");
					break;

				case 8:     // Weapon Master
					SendQuickMenu(9328);
					Plr->Gossip_SendPOI(9841.17f, -7505.13f, 7, 6, 0, "Silvermoon City, Weapon Master");
					break;

				case 9:     // Battlemasters
					SendQuickMenu(9329);
					Plr->Gossip_SendPOI(9850.74f, -7563.84f, 7, 6, 0, "Silvermoon City, Battlemasters");
					break;

				case 10:    // Class Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9331, Plr);
						Menu->AddItem(0, "Druid"       , 19);
						Menu->AddItem(0, "Hunter"      , 20);
						Menu->AddItem(0, "Mage"        , 21);
						Menu->AddItem(0, "Paladin"     , 22);
						Menu->AddItem(0, "Priest"      , 23);
						Menu->AddItem(0, "Rogue"       , 24);
						Menu->AddItem(0, "Warlock"     , 25);
						Menu->SendTo(Plr);
					}
					break;

				case 11:    // Profession Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9338, Plr);
						Menu->AddItem(0, "Alchemy"           , 26);
						Menu->AddItem(0, "Blacksmithing"     , 27);
						Menu->AddItem(0, "Cooking"           , 28);
						Menu->AddItem(0, "Enchanting"        , 29);
						Menu->AddItem(0, "Engineering"       , 30);
						Menu->AddItem(0, "First Aid"         , 31);
						Menu->AddItem(0, "Fishing"           , 32);
						Menu->AddItem(0, "Herbalism"         , 33);
						Menu->AddItem(0, "Inscription"       , 34);
						Menu->AddItem(0, "Jewelcrafting"     , 35);
						Menu->AddItem(0, "Leatherworking"    , 36);
						Menu->AddItem(0, "Mining"            , 37);
						Menu->AddItem(0, "Skinning"          , 38);
						Menu->AddItem(0, "Tailoring"         , 39);
						Menu->SendTo(Plr);
					}
					break;

				case 12: //Mana Loom
					{
						Plr->Gossip_SendPOI(9751.013f, -7074.85f, 7, 6, 0, "Silvermoon City, Mana Loom");
						SendQuickMenu(10502);
					}
					break;

				case 13: //To the west - Auction House no. 1
					{
						Plr->Gossip_SendPOI(9649.429f, -7134.027f, 7, 6, 0, "Silvermoon City, Auction House");
						SendQuickMenu(9318);
					}
					break;

				case 14: //To the east - Auction House no. 2
					{
						Plr->Gossip_SendPOI(9682.864f, -7515.786f, 7, 6, 0, "Silvermoon City, Auction House");
						SendQuickMenu(9319);
					}
					break;

				case 15:     // The bank - The west.
					SendQuickMenu(9321);
					Plr->Gossip_SendPOI(9522.104f, -7208.878f, 7, 6, 0, "Silvermoon City, Bank");
					break;

				case 16:     // The bank - The east.
					SendQuickMenu(9322);
					Plr->Gossip_SendPOI(9791.07f, -7488.041f, 7, 6, 0, "Silvermoon City, Bank");
					break;

				case 17: //The Silvermoon City Inn
					{
						Plr->Gossip_SendPOI(9677.113f, -7367.575f, 7, 6, 0, "Silvermoon City, Inn");
						SendQuickMenu(9325);
					}
					break;

				case 18: //The Wayfarer's Rest tavern
					{
						Plr->Gossip_SendPOI(9562.813f, -7218.63f, 7, 6, 0, "Silvermoon City, Inn");
						SendQuickMenu(9603);
					}
					break;

				case 19: //Druid
					{
						Plr->Gossip_SendPOI(9700.55f, -7262.57f, 7, 6, 0, "Silvermoon City, Druid Trainer");
						SendQuickMenu(9330);
					}
					break;

				case 20: //Hunter
					{
						Plr->Gossip_SendPOI(9930.568f, -7412.115f, 7, 6, 0, "Silvermoon City, Hunter Trainer");
						SendQuickMenu(9332);
					}
					break;

				case 21: //Mage
					{
						Plr->Gossip_SendPOI(9996.914f, -7104.803f, 7, 6, 0, "Silvermoon City, Mage Trainer");
						SendQuickMenu(9333);
					}
					break;

				case 22: //Paladin
					{
						Plr->Gossip_SendPOI(9850.22f, -7516.93f, 7, 6, 0, "Silvermoon City, Paladin Trainer");
						SendQuickMenu(9334);
					}
					break;

				case 23: //Priest
					{
						Plr->Gossip_SendPOI(9935.37f, -7131.14f, 7, 6, 0, "Silvermoon City, Priest Trainer");
						SendQuickMenu(9335);
					}
					break;

				case 24: //Rogue
					{
						Plr->Gossip_SendPOI(9739.88f, -7374.33f, 7, 6, 0, "Silvermoon City, Rogue Trainer");
						SendQuickMenu(9336);
					}
					break;

				case 25: //Warlock
					{
						Plr->Gossip_SendPOI(9803.052f, -7316.967f, 7, 6, 0, "Silvermoon City, Warlock Trainer");
						SendQuickMenu(9337);
					}
					break;

				case 26: //Alchemy
					{
						Plr->Gossip_SendPOI(10000.9f, -7216.63f, 7, 6, 0, "Silvermoon City, Alchemy");
						SendQuickMenu(9339);
					}
					break;

				case 27: //Blacksmithing
					{
						Plr->Gossip_SendPOI(9841.43f, -7361.53f, 7, 6, 0, "Silvermoon City, Blacksmithing");
						SendQuickMenu(9340);
					}
					break;

				case 28: //Cooking
					{
						Plr->Gossip_SendPOI(9577.26f, -7243.6f, 7, 6, 0, "Silvermoon City, Cooking");
						SendQuickMenu(9624);
					}
					break;

				case 29: //Enchanting
					{
						Plr->Gossip_SendPOI(9962.57f, -7246.18f, 7, 6, 0, "Silvermoon City, Enchanting");
						SendQuickMenu(9341);
					}
					break;

				case 30: //Engineering
					{
						Plr->Gossip_SendPOI(9808.85f, -7287.31f, 7, 6, 0, "Silvermoon City, Engineering");
						SendQuickMenu(9342);
					}
					break;

				case 31: //First Aid
					{
						Plr->Gossip_SendPOI(9588.61f, -7337.526f, 7, 6, 0, "Silvermoon City, First Aid");
						SendQuickMenu(9343);
					}
					break;

				case 32: //Fishing
					{
						Plr->Gossip_SendPOI(9601.97f, -7332.34f, 7, 6, 0, "Silvermoon City, Fishing");
						SendQuickMenu(9344);
					}
					break;

				case 33: //Herbalism
					{
						Plr->Gossip_SendPOI(9996.96f, -7208.39f, 7, 6, 0, "Silvermoon City, Herbalism");
						SendQuickMenu(9345);
					}
					break;
				case 34: //Inscription
					{
						Plr->Gossip_SendPOI(9957.12f, -7242.85f, 7, 6, 0, "Silvermoon City, Inscription");
						SendQuickMenu(13893);
					}
					break;

				case 35: //Jewelcrafting
					{
						Plr->Gossip_SendPOI(9552.8f, -7502.12f, 7, 6, 0, "Silvermoon City, Jewelcrafting");
						SendQuickMenu(9346);
					}
					break;

				case 36: //Leatherworking
					{
						Plr->Gossip_SendPOI(9502.486f, -7425.51f, 7, 6, 0, "Silvermoon City, Leatherworking");
						SendQuickMenu(9347);
					}
					break;

				case 37: //Mining
					{
						Plr->Gossip_SendPOI(9813.73f, -7360.19f, 7, 6, 0, "Silvermoon City, Mining");
						SendQuickMenu(9348);
					}
					break;

				case 38: //Skinning
					{
						Plr->Gossip_SendPOI(9513.37f, -7429.4f, 7, 6, 0, "Silvermoon City, Skinning");
						SendQuickMenu(9349);
					}
					break;

				case 39: //Tailoring
					{
						Plr->Gossip_SendPOI(9727.56f, -7086.65f, 7, 6, 0, "Silvermoon City, Tailoring");
						SendQuickMenu(9350);
					}
					break;
				case 40: //Lexicon of Power
					{
						Plr->Gossip_SendPOI(9957.12f, -7242.85f, 7, 6, 0, "Silvermoon City, Inscription");
						SendQuickMenu(14174);
					}
					break;
			}
		}
};

class ExodarGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9551, plr);
			Menu->AddItem(0, "Auction House"		, 1);
			Menu->AddItem(0, "The Bank"				, 2);
			Menu->AddItem(0, "Hippogryph Master"	, 3);
			Menu->AddItem(0, "Guild Master"			, 4);
			Menu->AddItem(0, "The Inn"				, 5);
			Menu->AddItem(0, "Mailbox"				, 6);
			Menu->AddItem(0, "Stable Master"		, 7);
			Menu->AddItem(0, "Weapon Master"		, 8);
			Menu->AddItem(0, "Battlemasters"		, 9);
			Menu->AddItem(0, "Class Trainer"		, 10);
			Menu->AddItem(0, "Profession Trainer"	, 11);
			Menu->AddItem(0, "Lexicon of Power"		, 34);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // Auction House
					SendQuickMenu(9528);
					Plr->Gossip_SendPOI(-4013.82f, -11729.64f, 7, 6, 0, "Exodar, Auctioneer");
					break;

				case 2:     // The Bank
					SendQuickMenu(9529);
					Plr->Gossip_SendPOI(-3923.89f, -11544.5f, 7, 6, 0, "Exodar, bank");
					break;

				case 3:     // Hippogryph Master
					SendQuickMenu(9530);
					Plr->Gossip_SendPOI(-4058.45f, -11789.7f, 7, 6, 0, "Exodar, Hippogryph Master");
					break;

				case 4:     // Guild Master
					SendQuickMenu(9539);
					Plr->Gossip_SendPOI(-4093.38f, -11630.39f, 7, 6, 0, "Exodar, Guild Master");
					break;

				case 5:     // The Inn
					SendQuickMenu(9545);
					Plr->Gossip_SendPOI(-3765.34f, -11695.8f, 7, 6, 0, "Exodar, Inn");
					break;

				case 6:     // Mailbox
					SendQuickMenu(10254);
					Plr->Gossip_SendPOI(-3913.75f, -11606.83f, 7, 6, 0, "Exodar, Mailbox");
					break;

				case 7:     // Stable Master
					SendQuickMenu(9558);
					Plr->Gossip_SendPOI(-3787.01f, -11702.7f, 7, 6, 0, "Exodar, Stable Master");
					break;

				case 8:     // Weapon Master
					SendQuickMenu(9565);
					Plr->Gossip_SendPOI(-4215.68f, -11628.9f, 7, 6, 0, "Exodar, Weapon Master");
					break;

				case 9:     // Battlemasters
					Plr->Gossip_SendPOI(-3999.82f, -11368.33f, 7, 6, 0, "Exodar, Battlemasters");
					objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9531, Plr);
					Menu->AddItem(0, "Arena Battlemaster"  , 12);
					Menu->SendTo(Plr);
					break;

				case 10:    // Class Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9533, Plr);
						Menu->AddItem(0, "Druid"       , 13);
						Menu->AddItem(0, "Hunter"      , 14);
						Menu->AddItem(0, "Mage"        , 15);
						Menu->AddItem(0, "Paladin"     , 16);
						Menu->AddItem(0, "Priest"      , 17);
						Menu->AddItem(0, "Shaman"      , 18);
						Menu->AddItem(0, "Warrior"     , 19);
						Menu->SendTo(Plr);
					}
					break;

				case 11:    // Profession Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9555, Plr);
						Menu->AddItem(0, "Alchemy"           , 20);
						Menu->AddItem(0, "Blacksmithing"     , 21);
						Menu->AddItem(0, "Enchanting"        , 22);
						Menu->AddItem(0, "Engineering"       , 23);
						Menu->AddItem(0, "First Aid"         , 24);
						Menu->AddItem(0, "Fishing"           , 25);
						Menu->AddItem(0, "Herbalism"         , 26);
						Menu->AddItem(0, "Inscription"       , 27);
						Menu->AddItem(0, "Jewelcrafting"     , 28);
						Menu->AddItem(0, "Leatherworking"    , 29);
						Menu->AddItem(0, "Mining"            , 30);
						Menu->AddItem(0, "Skinning"          , 31);
						Menu->AddItem(0, "Tailoring"         , 32);
						Menu->AddItem(0, "Cooking"           , 33);
						Menu->SendTo(Plr);
					}
					break;

					//////////////////
					// Battlemaster submenu
					////////

				case 12://Arena Battlemaster Exodar
					{
						Plr->Gossip_SendPOI(-3725.25f, -11688.3f, 7, 6, 0, "Arena Battlemaster Exodar");
						SendQuickMenu(10223);
					}
					break;

				case 13: //Druid
					{
						Plr->Gossip_SendPOI(-4274.81f, -11495.3f, 7, 6, 0, "Exodar, Druid Trainer");
						SendQuickMenu(9534);
					}
					break;

				case 14: //Hunter
					{
						Plr->Gossip_SendPOI(-4229.36f, -11563.36f, 7, 6, 0, "Exodar, Hunter Trainers");
						SendQuickMenu(9544);
					}
					break;

				case 15: //Mage
					{
						Plr->Gossip_SendPOI(-4048.8f, -11559.02f, 7, 6, 0, "Exodar, Mage Trainers");
						SendQuickMenu(9550);
					}
					break;

				case 16: //Paladin
					{
						Plr->Gossip_SendPOI(-4176.57f, -11476.46f, 7, 6, 0, "Exodar, Paladin Trainers");
						SendQuickMenu(9553);
					}
					break;

				case 17: //Priest
					{
						Plr->Gossip_SendPOI(-3972.38f, -11483.2f, 7, 6, 0, "Exodar, Priest Trainers");
						SendQuickMenu(9554);
					}
					break;

				case 18: //Shaman
					{
						Plr->Gossip_SendPOI(-3843.8f, -11390.75f, 7, 6, 0, "Exodar, Shaman Trainer");
						SendQuickMenu(9556);
					}
					break;

				case 19: //Warrior
					{
						Plr->Gossip_SendPOI(-4191.11f, -11650.45f, 7, 6, 0, "Exodar, Warrior Trainers");
						SendQuickMenu(9562);
					}
					break;

				case 20: //Alchemy
					{
						Plr->Gossip_SendPOI(-4042.37f, -11366.3f, 7, 6, 0, "Exodar, Alchemist Trainers");
						SendQuickMenu(9527);
					}
					break;

				case 21: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-4232.4f, -11705.23f, 7, 6, 0, "Exodar, Blacksmithing Trainers");
						SendQuickMenu(9532);
					}
					break;

				case 22: //Enchanting
					{
						Plr->Gossip_SendPOI(-3889.3f, -11495, 7, 6, 0, "Exodar, Enchanters");
						SendQuickMenu(9535);
					}
					break;

				case 23: //Engineering
					{
						Plr->Gossip_SendPOI(-4257.93f, -11636.53f, 7, 6, 0, "Exodar, Engineering");
						SendQuickMenu(9536);
					}
					break;

				case 24: //First Aid
					{
						Plr->Gossip_SendPOI(-3766.05f, -11481.8f, 7, 6, 0, "Exodar, First Aid Trainer");
						SendQuickMenu(9537);
					}
					break;

				case 25: //Fishing
					{
						Plr->Gossip_SendPOI(-3726.64f, -11384.43f, 7, 6, 0, "Exodar, Fishing Trainer");
						SendQuickMenu(9538);
					}
					break;

				case 26: //Herbalism
					{
						Plr->Gossip_SendPOI(-4052.5f, -11356.6f, 7, 6, 0, "Exodar, Herbalism Trainer");
						SendQuickMenu(9543);
					}
					break;

				case 27: //Inscription
					{
						Plr->Gossip_SendPOI(-3889.3f, -11495, 7, 6, 0, "Exodar, Inscription");
						SendQuickMenu(13887);
					}
					break;

				case 28: //Jewelcrafting
					{
						Plr->Gossip_SendPOI(-3786.27f, -11541.33f, 7, 6, 0, "Exodar, Jewelcrafters");
						SendQuickMenu(9547);
					}
					break;

				case 29: //Leatherworking
					{
						Plr->Gossip_SendPOI(-4134.42f, -11772.93f, 7, 6, 0, "Exodar, Leatherworking");
						SendQuickMenu(9549);
					}
					break;

				case 30: //Mining
					{
						Plr->Gossip_SendPOI(-4220.31f, -11694.29f, 7, 6, 0, "Exodar, Mining Trainers");
						SendQuickMenu(9552);
					}
					break;

				case 31: //Skinning
					{
						Plr->Gossip_SendPOI(-4134.97f, -11760.5f, 7, 6, 0, "Exodar, Skinning Trainer");
						SendQuickMenu(9557);
					}
					break;

				case 32: //Tailoring
					{
						Plr->Gossip_SendPOI(-4095.78f, -11746.9f, 7, 6, 0, "Exodar, Tailors");
						SendQuickMenu(9350);
					}
					break;

				case 33: //Cooking
					{
						Plr->Gossip_SendPOI(-3799.69f, -11650.51f, 7, 6, 0, "Exodar, Cook");
						SendQuickMenu(9559);
					}
					break;

				case 34: //Lexicon of Power
					{
						Plr->Gossip_SendPOI(-3889.3f, -11495, 7, 6, 0, "Exodar, Inscription");
						SendQuickMenu(14174);
					}
					break;
			}
		}
};

class OrgrimmarGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, plr);
			Menu->AddItem(0, "The bank", 1);
			Menu->AddItem(0, "The wind rider master", 2);
			Menu->AddItem(0, "The guild master", 3);
			Menu->AddItem(0, "The inn", 4);
			Menu->AddItem(0, "The mailbox", 5);
			Menu->AddItem(0, "The auction house", 6);
			Menu->AddItem(0, "The zeppelin master", 7);
			Menu->AddItem(0, "The weapon master", 8);
			Menu->AddItem(0, "The stable master", 9);
			Menu->AddItem(0, "The officers' lounge", 10);
			Menu->AddItem(0, "The battlemaster", 11);
			Menu->AddItem(0, "A class trainer", 12);
			Menu->AddItem(0, "A profession trainer", 13);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The bank
					SendQuickMenu(2554);
					Plr->Gossip_SendPOI(1631.51f, -4375.33f, 7, 6, 0, "Bank of Orgrimmar");
					break;

				case 2:     // The wind rider master
					SendQuickMenu(2555);
					Plr->Gossip_SendPOI(1676.6f, -4332.72f, 7, 6, 0, "The Sky Tower");
					break;

				case 3:     // The guild master
					SendQuickMenu(2556);
					Plr->Gossip_SendPOI(1576.93f, -4294.75f, 7, 6, 0, "Horde Embassy");
					break;

				case 4:     // The inn
					SendQuickMenu(2557);
					Plr->Gossip_SendPOI(1644.51f, -4447.27f, 7, 6, 0, "Orgrimmar Inn");
					break;

				case 5:     // The mailbox
					SendQuickMenu(2558);
					Plr->Gossip_SendPOI(1622.53f, -4388.79f, 7, 6, 0, "Orgrimmar Mailbox");
					break;

				case 6:     // The auction house
					SendQuickMenu(3075);
					Plr->Gossip_SendPOI(1679.21f, -4450.1f, 7, 6, 0, "Orgrimmar Auction House");
					break;

				case 7:     // The zeppelin master
					SendQuickMenu(3173);
					Plr->Gossip_SendPOI(1337.36f, -4632.7f, 7, 6, 0, "Orgrimmar Zeppelin Tower");
					break;

				case 8:     // The weapon master
					SendQuickMenu(4519);
					Plr->Gossip_SendPOI(2092.56f, -4823.95f, 7, 6, 0, "Sayoc & Hanashi");
					break;

				case 9:     // The stable master
					SendQuickMenu(5974);
					Plr->Gossip_SendPOI(2133.12f, -4663.93f, 7, 6, 0, "Xon'cha");
					break;

				case 10:    // The officers' lounge
					SendQuickMenu(7046);
					Plr->Gossip_SendPOI(1633.56f, -4249.37f, 7, 6, 0, "Hall of Legends");
					break;

				case 11:    // The battlemaster
					SendQuickMenu(7521);
					Plr->Gossip_SendPOI(1990.41f, -4794.15f, 7, 6, 0, "Battlemasters Orgrimmar");
					break;

				case 12:    // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2599, Plr);
						Menu->AddItem(0, "Hunter"       , 14);
						Menu->AddItem(0, "Mage"         , 15);
						Menu->AddItem(0, "Priest"       , 16);
						Menu->AddItem(0, "Shaman"       , 17);
						Menu->AddItem(0, "Rogue"        , 18);
						Menu->AddItem(0, "Warlock"      , 19);
						Menu->AddItem(0, "Warrior"      , 20);
						Menu->AddItem(0, "Paladin"      , 21);
						Menu->SendTo(Plr);
					}
					break;

				case 13:    // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2594, Plr);
						Menu->AddItem(0, "Alchemy"           , 22);
						Menu->AddItem(0, "Blacksmithing"     , 23);
						Menu->AddItem(0, "Cooking"           , 24);
						Menu->AddItem(0, "Enchanting"        , 25);
						Menu->AddItem(0, "Engineering"       , 26);
						Menu->AddItem(0, "First Aid"         , 27);
						Menu->AddItem(0, "Fishing"           , 28);
						Menu->AddItem(0, "Herbalism"         , 29);
						Menu->AddItem(0, "Leatherworking"    , 30);
						Menu->AddItem(0, "Mining"            , 31);
						Menu->AddItem(0, "Skinning"          , 32);
						Menu->AddItem(0, "Tailoring"         , 33);
						Menu->SendTo(Plr);
					}
					break;

					////////////////
					// Class trainer submenu
					////////

				case 14: //Hunter
					{
						Plr->Gossip_SendPOI(2114.84f, -4625.31f, 7, 6, 0, "Orgrimmar Hunter's Hall");
						SendQuickMenu(2559);
					}
					break;

				case 15: //Mage
					{
						Plr->Gossip_SendPOI(1451.26f, -4223.33f, 7, 6, 0, "Darkbriar Lodge");
						SendQuickMenu(2560);
					}
					break;

				case 16: //Priest
					{
						Plr->Gossip_SendPOI(1442.21f, -4183.24f, 7, 6, 0, "Spirit Lodge");
						SendQuickMenu(2561);
					}
					break;

				case 17: //Shaman
					{
						Plr->Gossip_SendPOI(1925.34f, -4181.89f, 7, 6, 0, "Thrall's Fortress");
						SendQuickMenu(2562);
					}
					break;

				case 18: //Rogue
					{
						Plr->Gossip_SendPOI(1773.39f, -4278.97f, 7, 6, 0, "Shadowswift Brotherhood");
						SendQuickMenu(2563);
					}
					break;

				case 19: //Warlock
					{
						Plr->Gossip_SendPOI(1849.57f, -4359.68f, 7, 6, 0, "Darkfire Enclave");
						SendQuickMenu(2564);
					}
					break;

				case 20: //Warrior
					{
						Plr->Gossip_SendPOI(1983.92f, -4794.2f, 7, 6, 0, "Hall of the Brave");
						SendQuickMenu(2565);
					}
					break;

				case 21: //Paladin
					{
						Plr->Gossip_SendPOI(1937.53f, -4141.0f, 7, 6, 0, "Thrall's Fortress");
						SendQuickMenu(2566);
					}
					break;

				case 22: //Alchemy
					{
						Plr->Gossip_SendPOI(1955.17f, -4475.79f, 7, 6, 0, "Yelmak's Alchemy and Potions");
						SendQuickMenu(2497);
					}
					break;

				case 23: //Blacksmithing
					{
						Plr->Gossip_SendPOI(2054.34f, -4831.85f, 7, 6, 0, "The Burning Anvil");
						SendQuickMenu(2499);
					}
					break;

				case 24: //Cooking
					{
						Plr->Gossip_SendPOI(1780.96f, -4481.31f, 7, 6, 0, "Borstan's Firepit");
						SendQuickMenu(2500);
					}
					break;

				case 25: //Enchanting
					{
						Plr->Gossip_SendPOI(1917.5f, -4434.95f, 7, 6, 0, "Godan's Runeworks");
						SendQuickMenu(2501);
					}
					break;

				case 26: //Engineering
					{
						Plr->Gossip_SendPOI(2038.45f, -4744.75f, 7, 6, 0, "Nogg's Machine Shop");
						SendQuickMenu(2653);
					}
					break;

				case 27: //First Aid
					{
						Plr->Gossip_SendPOI(1485.21f, -4160.91f, 7, 6, 0, "Survival of the Fittest");
						SendQuickMenu(2502);
					}
					break;

				case 28: //Fishing
					{
						Plr->Gossip_SendPOI(1994.15f, -4655.7f, 7, 6, 0, "Lumak's Fishing");
						SendQuickMenu(2503);
					}
					break;

				case 29: //Herbalism
					{
						Plr->Gossip_SendPOI(1898.61f, -4454.93f, 7, 6, 0, "Jandi's Arboretum");
						SendQuickMenu(2504);
					}
					break;

				case 30: //Leatherworking
					{
						Plr->Gossip_SendPOI(1852.82f, -4562.31f, 7, 6, 0, "Kodohide Leatherworkers");
						SendQuickMenu(2513);
					}
					break;

				case 31: //Mining
					{
						Plr->Gossip_SendPOI(2029.79f, -4704, 7, 6, 0, "Red Canyon Mining");
						SendQuickMenu(2515);
					}
					break;

				case 32: //Skinning
					{
						Plr->Gossip_SendPOI(1852.82f, -4562.31f, 7, 6, 0, "Kodohide Leatherworkers");
						SendQuickMenu(2516);
					}
					break;

				case 33: //Tailoring
					{
						Plr->Gossip_SendPOI(1802.66f, -4560.66f, 7, 6, 0, "Magar's Cloth Goods");
						SendQuickMenu(2518);
					}
					break;
			}
		}
};

class ThunderbluffGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, plr);
			Menu->AddItem(0, "The bank", 1);
			Menu->AddItem(0, "The wind rider master", 2);
			Menu->AddItem(0, "The guild master", 3);
			Menu->AddItem(0, "The inn", 4);
			Menu->AddItem(0, "The mailbox", 5);
			Menu->AddItem(0, "The auction house", 6);
			Menu->AddItem(0, "The weapon master", 7);
			Menu->AddItem(0, "The stable master", 8);
			Menu->AddItem(0, "The battlemaster", 9);
			Menu->AddItem(0, "A class trainer", 10);
			Menu->AddItem(0, "A profession trainer", 11);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The bank
					SendQuickMenu(1292);
					Plr->Gossip_SendPOI(-1257.8f, 24.14f, 7, 6, 0, "Thunder Bluff Bank");
					break;

				case 2:     // The wind rider master
					SendQuickMenu(1293);
					Plr->Gossip_SendPOI(-1196.43f, 28.26f, 7, 6, 0, "Wind Rider Roost");
					break;

				case 3:     // The guild master
					SendQuickMenu(1291);
					Plr->Gossip_SendPOI(-1296.5f, 127.57f, 7, 6, 0, "Thunder Bluff Civic Information");
					break;

				case 4:     // The inn
					SendQuickMenu(3153);
					Plr->Gossip_SendPOI(-1296, 39.7f, 7, 6, 0, "Thunder Bluff Inn");
					break;

				case 5:     // The mailbox
					SendQuickMenu(3154);
					Plr->Gossip_SendPOI(-1263.59f, 44.36f, 7, 6, 0, "Thunder Bluff Mailbox");
					break;

				case 6:     // The auction house
					SendQuickMenu(3155);
					Plr->Gossip_SendPOI(-1205.51f, 105.74f, 7, 6, 0, "Thunder Bluff Auction house");
					break;

				case 7:     // The weapon master
					SendQuickMenu(4520);
					Plr->Gossip_SendPOI(-1282.31f, 89.56f, 7, 6, 0, "Ansekhwa");
					break;

				case 8:     // The stable master
					SendQuickMenu(5977);
					Plr->Gossip_SendPOI(-1270.19f, 48.84f, 7, 6, 0, "Bulrug");
					break;

				case 9:    // The battlemaster
					SendQuickMenu(7527);
					Plr->Gossip_SendPOI(-1391.22f, -81.33f, 7, 6, 0, "Battlemasters Thunder Bluff");
					break;

				case 10:    // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3542, Plr);
						Menu->AddItem(0, "Druid"       , 12);
						Menu->AddItem(0, "Hunter"      , 13);
						Menu->AddItem(0, "Mage"        , 14);
						Menu->AddItem(0, "Priest"      , 15);
						Menu->AddItem(0, "Shaman"      , 16);
						Menu->AddItem(0, "Warrior"     , 17);
						Menu->SendTo(Plr);
					}
					break;

				case 11:    // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
						Menu->AddItem(0, "Alchemy"           , 18);
						Menu->AddItem(0, "Blacksmithing"     , 19);
						Menu->AddItem(0, "Cooking"           , 20);
						Menu->AddItem(0, "Enchanting"        , 21);
						Menu->AddItem(0, "First Aid"         , 22);
						Menu->AddItem(0, "Fishing"           , 23);
						Menu->AddItem(0, "Herbalism"         , 24);
						Menu->AddItem(0, "Leatherworking"    , 25);
						Menu->AddItem(0, "Mining"            , 26);
						Menu->AddItem(0, "Skinning"          , 27);
						Menu->AddItem(0, "Tailoring"         , 28);
						Menu->SendTo(Plr);
					}
					break;

					////////////////
					// Class trainer submenu
					////////

				case 12: //Druid
					{
						Plr->Gossip_SendPOI(-1054.47f, -285, 7, 6, 0, "Hall of Elders");
						SendQuickMenu(1294);
					}
					break;

				case 13: //Hunter
					{
						Plr->Gossip_SendPOI(-1416.32f, -114.28f, 7, 6, 0, "Hunter's Hall");
						SendQuickMenu(1295);
					}
					break;

				case 14: //Mage
					{
						Plr->Gossip_SendPOI(-1061.2f, 195.5f, 7, 6, 0, "Pools of Vision");
						SendQuickMenu(1296);
					}
					break;

				case 15: //Priest
					{
						Plr->Gossip_SendPOI(-1061.2f, 195.5f, 7, 6, 0, "Pools of Vision");
						SendQuickMenu(1297);
					}
					break;

				case 16: //Shaman
					{
						Plr->Gossip_SendPOI(-989.54f, 278.25f, 7, 6, 0, "Hall of Spirits");
						SendQuickMenu(1298);
					}
					break;

				case 17: //Warrior
					{
						Plr->Gossip_SendPOI(-1416.32f, -114.28f, 7, 6, 0, "Hunter's Hall");
						SendQuickMenu(1299);
					}
					break;

				case 18: //Alchemy
					{
						Plr->Gossip_SendPOI(-1085.56f, 27.29f, 7, 6, 0, "Bena's Alchemy");
						SendQuickMenu(1332);
					}
					break;

				case 19: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-1239.75f, 104.88f, 7, 6, 0, "Karn's Smithy");
						SendQuickMenu(1333);
					}
					break;

				case 20: //Cooking
					{
						Plr->Gossip_SendPOI(-1214.5f, -21.23f, 7, 6, 0, "Aska's Kitchen");
						SendQuickMenu(1334);
					}
					break;

				case 21: //Enchanting
					{
						Plr->Gossip_SendPOI(-1112.65f, 48.26f, 7, 6, 0, "Dawnstrider Enchanters");
						SendQuickMenu(1335);
					}
					break;

				case 22: //First Aid
					{
						Plr->Gossip_SendPOI(-996.58f, 200.5f, 7, 6, 0, "Spiritual Healing");
						SendQuickMenu(1336);
					}
					break;

				case 23: //Fishing
					{
						Plr->Gossip_SendPOI(-1169.35f, -68.87f, 7, 6, 0, "Mountaintop Bait & Tackle");
						SendQuickMenu(1337);
					}
					break;

				case 24: //Herbalism
					{
						Plr->Gossip_SendPOI(-1137.7f, -1.51f, 7, 6, 0, "Holistic Herbalism");
						SendQuickMenu(1338);
					}
					break;

				case 25: //Leatherworking
					{
						Plr->Gossip_SendPOI(-1156.22f, 66.86f, 7, 6, 0, "Thunder Bluff Armorers");
						SendQuickMenu(1339);
					}
					break;

				case 26: //Mining
					{
						Plr->Gossip_SendPOI(-1249.17f, 155, 7, 6, 0, "Stonehoof Geology");
						SendQuickMenu(1340);
					}
					break;

				case 27: //Skinning
					{
						Plr->Gossip_SendPOI(-1148.56f, 51.18f, 7, 6, 0, "Mooranta");
						SendQuickMenu(1343);
					}
					break;

				case 28: //Tailoring
					{
						Plr->Gossip_SendPOI(-1156.22f, 66.86f, 7, 6, 0, "Thunder Bluff Armorers");
						SendQuickMenu(1341);
					}
					break;
			}
		}
};

class BloodhoofGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, plr);
			Menu->AddItem(0, "The bank", 1);
			Menu->AddItem(0, "The wind rider master", 2);
			Menu->AddItem(0, "The inn", 3);
			Menu->AddItem(0, "The stable master", 4);
			Menu->AddItem(0, "A class trainer", 5);
			Menu->AddItem(0, "A profession trainer", 6);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The bank
					SendQuickMenu(4051);
					break;

				case 2:     // The wind rider master
					SendQuickMenu(4052);
					break;

				case 3:     // The inn
					SendQuickMenu(4053);
					Plr->Gossip_SendPOI(-2361.38f, -349.19f, 7, 6, 0, "Bloodhoof Village Inn");
					break;

				case 4:     // The stable master
					SendQuickMenu(5976);
					Plr->Gossip_SendPOI(-2338.86f, -357.56f, 7, 6, 0, "Seikwa");
					break;

				case 5:     // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4069, Plr);
						Menu->AddItem(0, "Druid"       , 7);
						Menu->AddItem(0, "Hunter"      , 8);
						Menu->AddItem(0, "Shaman"      , 9);
						Menu->AddItem(0, "Warrior"    , 10);
						Menu->SendTo(Plr);
					}
					break;

				case 6:     // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
						Menu->AddItem(0, "Alchemy"           , 11);
						Menu->AddItem(0, "Blacksmithing"     , 12);
						Menu->AddItem(0, "Cooking"           , 13);
						Menu->AddItem(0, "Enchanting"        , 14);
						Menu->AddItem(0, "First Aid"         , 15);
						Menu->AddItem(0, "Fishing"           , 16);
						Menu->AddItem(0, "Herbalism"         , 17);
						Menu->AddItem(0, "Leatherworking"    , 18);
						Menu->AddItem(0, "Mining"            , 19);
						Menu->AddItem(0, "Skinning"          , 20);
						Menu->AddItem(0, "Tailoring"         , 21);
						Menu->SendTo(Plr);
					}
					break;


					////////////////
					// Class trainer submenu
					////////

				case 7: //Druid
					{
						Plr->Gossip_SendPOI(-2312.15f, -443.69f, 7, 6, 0, "Gennia Runetotem");
						SendQuickMenu(4054);
					}
					break;

				case 8: //Hunter
					{
						Plr->Gossip_SendPOI(-2178.14f, -406.14f, 7, 6, 0, "Yaw Sharpmane");
						SendQuickMenu(4055);
					}
					break;

				case 9: //Shaman
					{
						Plr->Gossip_SendPOI(-2301.5f, -439.87f, 7, 6, 0, "Narm Skychaser");
						SendQuickMenu(4056);
					}
					break;

				case 10: //Warrior
					{
						Plr->Gossip_SendPOI(-2345.43f, -494.11f, 7, 6, 0, "Krang Stonehoof");
						SendQuickMenu(4057);
					}
					break;

				case 11: //Alchemy
					{
						SendQuickMenu(4058);
					}
					break;

				case 12: //Blacksmithing
					{
						SendQuickMenu(4059);
					}
					break;

				case 13: //Cooking
					{
						Plr->Gossip_SendPOI(-2263.34f, -287.91f, 7, 6, 0, "Pyall Silentstride");
						SendQuickMenu(4060);
					}
					break;

				case 14: //Enchanting
					{
						SendQuickMenu(4061);
					}
					break;

				case 15: //First Aid
					{
						Plr->Gossip_SendPOI(-2353.52f, -355.82f, 7, 6, 0, "Vira Younghoof");
						SendQuickMenu(4062);
					}
					break;

				case 16: //Fishing
					{
						Plr->Gossip_SendPOI(-2349.21f, -241.37f, 7, 6, 0, "Uthan Stillwater");
						SendQuickMenu(4063);
					}
					break;

				case 17: //Herbalism
					{
						SendQuickMenu(4064);
					}
					break;

				case 18: //Leatherworking
					{
						Plr->Gossip_SendPOI(-2257.12f, -288.63f, 7, 6, 0, "Chaw Stronghide");
						SendQuickMenu(4065);
					}
					break;

				case 19: //Mining
					{
						SendQuickMenu(4066);
					}
					break;

				case 20: //Skinning
					{
						Plr->Gossip_SendPOI(-2252.94f, -291.32f, 7, 6, 0, "Yonn Deepcut");
						SendQuickMenu(4067);
					}
					break;

				case 21: //Tailoring
					{
						SendQuickMenu(4068);
					}
					break;
			}
		}
};

class RazorHillGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4037, plr);
			Menu->AddItem(0, "The bank", 1);
			Menu->AddItem(0, "The wind rider master", 2);
			Menu->AddItem(0, "The inn", 3);
			Menu->AddItem(0, "The stable master", 4);
			Menu->AddItem(0, "A class trainer", 5);
			Menu->AddItem(0, "A profession trainer", 6);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The bank
					SendQuickMenu(4032);
					break;

				case 2:     // The wind rider master
					SendQuickMenu(4033);
					break;

				case 3:     // The inn
					SendQuickMenu(4034);
					Plr->Gossip_SendPOI(338.7f, -4688.87f, 7, 6, 0, "Razor Hill Inn");
					break;

				case 4:     // The stable master
					SendQuickMenu(5973);
					Plr->Gossip_SendPOI(330.31f, -4710.66f, 7, 6, 0, "Shoja'my");
					break;

				case 5:     // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4035, Plr);
						Menu->AddItem(0, "Hunter"       , 7);
						Menu->AddItem(0, "Mage"         , 8);
						Menu->AddItem(0, "Priest"       , 9);
						Menu->AddItem(0, "Rogue"       , 10);
						Menu->AddItem(0, "Shaman"      , 11);
						Menu->AddItem(0, "Warlock"     , 12);
						Menu->AddItem(0, "Warrior"     , 13);
						Menu->SendTo(Plr);
					}
					break;

				case 6:     // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
						Menu->AddItem(0, "Alchemy"           , 14);
						Menu->AddItem(0, "Blacksmithing"     , 15);
						Menu->AddItem(0, "Cooking"           , 16);
						Menu->AddItem(0, "Enchanting"        , 17);
						Menu->AddItem(0, "Engineering"       , 18);
						Menu->AddItem(0, "First Aid"         , 19);
						Menu->AddItem(0, "Fishing"           , 20);
						Menu->AddItem(0, "Herbalism"         , 21);
						Menu->AddItem(0, "Leatherworking"    , 22);
						Menu->AddItem(0, "Mining"            , 23);
						Menu->AddItem(0, "Skinning"          , 24);
						Menu->AddItem(0, "Tailoring"         , 25);
						Menu->SendTo(Plr);
					}
					break;


					////////////////
					// Class trainer submenu
					////////

				case 7: //Hunter
					{
						Plr->Gossip_SendPOI(276, -4706.72f, 7, 6, 0, "Thotar");
						SendQuickMenu(4013);
					}
					break;

				case 8: //Mage
					{
						Plr->Gossip_SendPOI(-839.33f, -4935.6f, 7, 6, 0, "Un'Thuwa");
						SendQuickMenu(4014);
					}
					break;

				case 9: //Priest
					{
						Plr->Gossip_SendPOI(296.22f, -4828.1f, 7, 6, 0, "Tai'jin");
						SendQuickMenu(4015);
					}
					break;

				case 10: //Rogue
					{
						Plr->Gossip_SendPOI(265.76f, -4709, 7, 6, 0, "Kaplak");
						SendQuickMenu(4016);
					}
					break;

				case 11: //Shaman
					{
						Plr->Gossip_SendPOI(307.79f, -4836.97f, 7, 6, 0, "Swart");
						SendQuickMenu(4017);
					}
					break;

				case 12: //Warlock
					{
						Plr->Gossip_SendPOI(355.88f, -4836.45f, 7, 6, 0, "Dhugru Gorelust");
						SendQuickMenu(4018);
					}
					break;

				case 13: //Warrior
					{
						Plr->Gossip_SendPOI(312.3f, -4824.66f, 7, 6, 0, "Tarshaw Jaggedscar");
						SendQuickMenu(4019);
					}
					break;

				case 14: //Alchemy
					{
						Plr->Gossip_SendPOI(-800.25f, -4894.33f, 7, 6, 0, "Miao'zan");
						SendQuickMenu(4020);
					}
					break;

				case 15: //Blacksmithing
					{
						Plr->Gossip_SendPOI(373.24f, -4716.45f, 7, 6, 0, "Dwukk");
						SendQuickMenu(4021);
					}
					break;

				case 16: //Cooking
					{
						SendQuickMenu(4022);
					}
					break;

				case 17: //Enchanting
					{
						SendQuickMenu(4023);
					}
					break;

				case 18: //Engineering
					{
						Plr->Gossip_SendPOI(368.95f, -4723.95f, 7, 6, 0, "Mukdrak");
						SendQuickMenu(4024);
					}
					break;

				case 19: //First Aid
					{
						Plr->Gossip_SendPOI(327.17f, -4825.62f, 7, 6, 0, "Rawrk");
						SendQuickMenu(4025);
					}
					break;

				case 20: //Fishing
					{
						Plr->Gossip_SendPOI(-1065.48f, -4777.43f, 7, 6, 0, "Lau'Tiki");
						SendQuickMenu(4026);
					}
					break;

				case 21: //Herbalism
					{
						Plr->Gossip_SendPOI(-836.25f, -4896.89f, 7, 6, 0, "Mishiki");
						SendQuickMenu(4027);
					}
					break;

				case 22: //Leatherworking
					{
						SendQuickMenu(4028);
					}
					break;

				case 23: //Mining
					{
						Plr->Gossip_SendPOI(366.94f, -4705, 7, 6, 0, "Krunn");
						SendQuickMenu(4029);
					}
					break;

				case 24: //Skinning
					{
						Plr->Gossip_SendPOI(-2252.94f, -291.32f, 7, 6, 0, "Yonn Deepcut");
						SendQuickMenu(4030);
					}
					break;

				case 25: //Tailoring
					{
						SendQuickMenu(4031);
					}
					break;
			}
		}
};

class BrillGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, plr);
			Menu->AddItem(0, "The bank", 1);
			Menu->AddItem(0, "The bat handler", 2);
			Menu->AddItem(0, "The inn", 3);
			Menu->AddItem(0, "The stable master", 4);
			Menu->AddItem(0, "A class trainer", 5);
			Menu->AddItem(0, "A profession trainer", 6);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // The bank
					SendQuickMenu(4074);
					break;

				case 2:     // The bat handler
					SendQuickMenu(4075);
					break;

				case 3:     // The inn
					SendQuickMenu(4076);
					Plr->Gossip_SendPOI(2246.68f, 241.89f, 7, 6, 0, "Gallows` End Tavern");
					break;

				case 4:     // The stable master
					SendQuickMenu(5978);
					Plr->Gossip_SendPOI(2267.66f, 319.32f, 7, 6, 0, "Morganus");
					break;

				case 5:     // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
						Menu->AddItem(0, "Mage"         , 7);
						Menu->AddItem(0, "Paladin"      , 8);
						Menu->AddItem(0, "Priest"       , 9);
						Menu->AddItem(0, "Rogue"       , 10);
						Menu->AddItem(0, "Warlock"     , 11);
						Menu->AddItem(0, "Warrior"     , 12);
						Menu->SendTo(Plr);
					}
					break;

				case 6:     // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4300, Plr);
						Menu->AddItem(0, "Alchemy"           , 13);
						Menu->AddItem(0, "Blacksmithing"     , 14);
						Menu->AddItem(0, "Cooking"           , 15);
						Menu->AddItem(0, "Enchanting"        , 16);
						Menu->AddItem(0, "Engineering"       , 17);
						Menu->AddItem(0, "First Aid"         , 18);
						Menu->AddItem(0, "Fishing"           , 19);
						Menu->AddItem(0, "Herbalism"         , 20);
						Menu->AddItem(0, "Leatherworking"    , 21);
						Menu->AddItem(0, "Mining"            , 22);
						Menu->AddItem(0, "Skinning"          , 23);
						Menu->AddItem(0, "Tailoring"         , 24);
						Menu->SendTo(Plr);
					}
					break;


					////////////////
					// Class trainer submenu
					////////

				case 7: //Mage
					{
						Plr->Gossip_SendPOI(2259.18f, 240.93f, 7, 6, 0, "Cain Firesong");
						SendQuickMenu(4077);
					}
					break;

				case 8: //Paladin
					{
						SendQuickMenu(0); // Need to add correct text
					}
					break;

				case 9: //Priest
					{
						Plr->Gossip_SendPOI(2259.18f, 240.93f, 7, 6, 0, "Dark Cleric Beryl");
						SendQuickMenu(4078);
					}
					break;

				case 10: //Rogue
					{
						Plr->Gossip_SendPOI(2259.18f, 240.93f, 7, 6, 0, "Marion Call");
						SendQuickMenu(4079);
					}
					break;

				case 11: //Warlock
					{
						Plr->Gossip_SendPOI(2259.18f, 240.93f, 7, 6, 0, "Rupert Boch");
						SendQuickMenu(4080);
					}
					break;

				case 12: //Warrior
					{
						Plr->Gossip_SendPOI(2256.48f, 240.32f, 7, 6, 0, "Austil de Mon");
						SendQuickMenu(4081);
					}
					break;

				case 13: //Alchemy
					{
						Plr->Gossip_SendPOI(2263.25f, 344.23f, 7, 6, 0, "Carolai Anise");
						SendQuickMenu(4082);
					}
					break;

				case 14: //Blacksmithing
					{
						SendQuickMenu(4083);
					}
					break;

				case 15: //Cooking
					{
						SendQuickMenu(4084);
					}
					break;

				case 16: //Enchanting
					{
						Plr->Gossip_SendPOI(2250.35f, 249.12f, 7, 6, 0, "Vance Undergloom");
						SendQuickMenu(4085);
					}
					break;

				case 17: //Engineering
					{
						SendQuickMenu(4086);
					}
					break;

				case 18: //First Aid
					{
						Plr->Gossip_SendPOI(2246.68f, 241.89f, 7, 6, 0, "Nurse Neela");
						SendQuickMenu(4087);
					}
					break;

				case 19: //Fishing
					{
						Plr->Gossip_SendPOI(2292.37f, -10.72f, 7, 6, 0, "Clyde Kellen");
						SendQuickMenu(4088);
					}
					break;

				case 20: //Herbalism
					{
						Plr->Gossip_SendPOI(2268.21f, 331.69f, 7, 6, 0, "Faruza");
						SendQuickMenu(4089);
					}
					break;

				case 21: //Leatherworking
					{
						Plr->Gossip_SendPOI(2027, 78.72f, 7, 6, 0, "Shelene Rhobart");
						SendQuickMenu(4090);
					}
					break;

				case 22: //Mining
					{
						SendQuickMenu(4091);
					}
					break;

				case 23: //Skinning
					{
						Plr->Gossip_SendPOI(2027, 78.72f, 7, 6, 0, "Rand Rhobart");
						SendQuickMenu(4092);
					}
					break;

				case 24: //Tailoring
					{
						Plr->Gossip_SendPOI(2160.45f, 659.93f, 7, 6, 0, "Bowen Brisboise");
						SendQuickMenu(4093);
					}
					break;
			}
		}
};

class IronforgeGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2760, plr);
			Menu->AddItem(0, "Auction House"			, 1);
			Menu->AddItem(0, "Bank of Ironforge"		, 2);
			Menu->AddItem(0, "Deeprun Tram"				, 3);
			Menu->AddItem(0, "Gryphon Master"			, 4);
			Menu->AddItem(0, "Guild Master"				, 5);
			Menu->AddItem(0, "The Inn"					, 6);
			Menu->AddItem(0, "Mailbox"					, 7);
			Menu->AddItem(0, "Stable Master"			, 8);
			Menu->AddItem(0, "Weapons Trainer"			, 9);
			Menu->AddItem(0, "Battlemaster"				, 10);
			Menu->AddItem(0, "Barber"					, 11);
			Menu->AddItem(0, "Class Trainer"			, 12);
			Menu->AddItem(0, "Profession Trainer"		, 13);
			Menu->AddItem(0, "Lexicon of Power"			, 35);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     // Auction House
					SendQuickMenu(3014);
					Plr->Gossip_SendPOI(-4957.39f, -911.6f, 7, 6, 0, "Ironforge Auction House");
					break;

				case 2:     // Bank of Ironforge
					SendQuickMenu(2761);
					Plr->Gossip_SendPOI(-4891.91f, -991.47f, 7, 6, 0, "The Vault");
					break;

				case 3:     // Deeprun Tram
					SendQuickMenu(3814);
					Plr->Gossip_SendPOI(-4835.27f, -1294.69f, 7, 6, 0, "Deeprun Tram");
					break;

				case 4:     // Gryphon Master
					SendQuickMenu(2762);
					Plr->Gossip_SendPOI(-4821.52f, -1152.3f, 7, 6, 0, "Ironforge Gryphon Master");
					break;

				case 5:     // Guild Master
					SendQuickMenu(2764);
					Plr->Gossip_SendPOI(-5021, -996.45f, 7, 6, 0, "Ironforge Visitor's Center");
					break;

				case 6:     // The Inn
					SendQuickMenu(2768);
					Plr->Gossip_SendPOI(-4850.47f, -872.57f, 7, 6, 0, "Stonefire Tavern");
					break;

				case 7:     // Mailbox
					SendQuickMenu(2769);
					Plr->Gossip_SendPOI(-4845.7f, -880.55f, 7, 6, 0, "Ironforge Mailbox");
					break;

				case 8:     // Stable Master
					SendQuickMenu(5986);
					Plr->Gossip_SendPOI(-5010.2f, -1262, 7, 6, 0, "Ulbrek Firehand");
					break;

				case 9:    // Weapon Trainer
					SendQuickMenu(4518);
					Plr->Gossip_SendPOI(-5040, -1201.88f, 7, 6, 0, "Bixi and Buliwyf");
					break;

				case 10:    // Battlemaster
					SendQuickMenu(10216);
					Plr->Gossip_SendPOI(-5038.54f, -1266.44f, 7, 6, 0, "Battlemasters Ironforge");
					break;

				case 11:    // Barber
					SendQuickMenu(13885);
					Plr->Gossip_SendPOI(-4838.49f, -919.18f, 7, 6, 0, "Ironforge Barber");
					break;

				case 12:    // A class trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2766, Plr);
						Menu->AddItem(0, "Hunter"      , 14);
						Menu->AddItem(0, "Mage"        , 15);
						Menu->AddItem(0, "Paladin"     , 16);
						Menu->AddItem(0, "Priest"      , 17);
						Menu->AddItem(0, "Rogue"       , 18);
						Menu->AddItem(0, "Warlock"     , 19);
						Menu->AddItem(0, "Warrior"     , 20);
						Menu->AddItem(0, "Shaman"      , 21);
						Menu->SendTo(Plr);
					}
					break;

				case 13:    // A profession trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2793, Plr);
						Menu->AddItem(0, "Alchemy"           , 22);
						Menu->AddItem(0, "Blacksmithing"     , 23);
						Menu->AddItem(0, "Cooking"           , 24);
						Menu->AddItem(0, "Enchanting"        , 25);
						Menu->AddItem(0, "Engineering"       , 26);
						Menu->AddItem(0, "First Aid"         , 27);
						Menu->AddItem(0, "Fishing"           , 28);
						Menu->AddItem(0, "Herbalism"         , 29);
						Menu->AddItem(0, "Inscription"		  , 30);
						Menu->AddItem(0, "Leatherworking"    , 31);
						Menu->AddItem(0, "Mining"            , 32);
						Menu->AddItem(0, "Skinning"          , 33);
						Menu->AddItem(0, "Tailoring"         , 34);
						Menu->SendTo(Plr);
					}
					break;

					////////////////
					// Class trainer submenu
					////////

				case 14: //Hunter
					{
						Plr->Gossip_SendPOI(-5023, -1253.68f, 7, 6, 0, "Hall of Arms");
						SendQuickMenu(2770);
					}
					break;

				case 15: //Mage
					{
						Plr->Gossip_SendPOI(-4627, -926.45f, 7, 6, 0, "Hall of Mysteries");
						SendQuickMenu(2771);
					}
					break;

				case 16: //Paladin
					{
						Plr->Gossip_SendPOI(-4627.02f, -926.45f, 7, 6, 0, "Hall of Mysteries");
						SendQuickMenu(2773);
					}
					break;

				case 17: //Priest
					{
						Plr->Gossip_SendPOI(-4627, -926.45f, 7, 6, 0, "Hall of Mysteries");
						SendQuickMenu(2772);
					}
					break;

				case 18: //Rogue
					{
						Plr->Gossip_SendPOI(-4647.83f, -1124, 7, 6, 0, "Ironforge Rogue Trainer");
						SendQuickMenu(2774);
					}
					break;

				case 19: //Warlock
					{
						Plr->Gossip_SendPOI(-4605, -1110.45f, 7, 6, 0, "Ironforge Warlock Trainer");
						SendQuickMenu(2775);
					}
					break;

				case 20: //Warrior
					{
						Plr->Gossip_SendPOI(-5023.08f, -1253.68f, 7, 6, 0, "Hall of Arms");
						SendQuickMenu(2776);
					}
					break;

				case 21: //Shaman
					{
						Plr->Gossip_SendPOI(-4722.02f, -1150.66f, 7, 6, 0, "Ironforge Shaman Trainer");
						SendQuickMenu(10842);
					}
					break;

				case 22: //Alchemy
					{
						Plr->Gossip_SendPOI(-4858.5f, -1241.83f, 7, 6, 0, "Berryfizz's Potions and Mixed Drinks");
						SendQuickMenu(2794);
					}
					break;

				case 23: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-4796.97f, -1110.17f, 7, 6, 0, "The Great Forge");
						SendQuickMenu(2795);
					}
					break;

				case 24: //Cooking
					{
						Plr->Gossip_SendPOI(-4767.83f, -1184.59f, 7, 6, 0, "The Bronze Kettle");
						SendQuickMenu(2796);
					}
					break;

				case 25: //Enchanting
					{
						Plr->Gossip_SendPOI(-4803.72f, -1196.53f, 7, 6, 0, "Thistlefuzz Arcanery");
						SendQuickMenu(2797);
					}
					break;

				case 26: //Engineering
					{
						Plr->Gossip_SendPOI(-4799.56f, -1250.23f, 7, 6, 0, "Springspindle's Gadgets");
						SendQuickMenu(2798);
					}
					break;

				case 27: //First Aid
					{
						Plr->Gossip_SendPOI(-4881.6f, -1153.13f, 7, 6, 0, "Ironforge Physician");
						SendQuickMenu(2799);
					}
					break;

				case 28: //Fishing
					{
						Plr->Gossip_SendPOI(-4597.91f, -1091.93f, 7, 6, 0, "Traveling Fisherman");
						SendQuickMenu(2800);
					}
					break;

				case 29: //Herbalism
					{
						Plr->Gossip_SendPOI(-4876.9f, -1151.92f, 7, 6, 0, "Ironforge Physician");
						SendQuickMenu(2801);
					}
					break;

				case 30: //Inscription
					{
						Plr->Gossip_SendPOI(-4801.72f, -1189.41f, 7, 6, 0, "Ironforge Inscription");
						SendQuickMenu(13884);
					}
					break;

				case 31: //Leatherworking
					{
						Plr->Gossip_SendPOI(-4745, -1027.57f, 7, 6, 0, "Finespindle's Leather Goods");
						SendQuickMenu(2802);
					}
					break;

				case 32: //Mining
					{
						Plr->Gossip_SendPOI(-4705.06f, -1116.43f, 7, 6, 0, "Deepmountain Mining Guild");
						SendQuickMenu(2804);
					}
					break;

				case 33: //Skinning
					{
						Plr->Gossip_SendPOI(-4745, -1027.57f, 7, 6, 0, "Finespindle's Leather Goods");
						SendQuickMenu(2805);
					}
					break;

				case 34: //Tailoring
					{
						Plr->Gossip_SendPOI(-4719.60f, -1056.96f, 7, 6, 0, "Stonebrow's Clothier");
						SendQuickMenu(2807);
					}
					break;

				case 35: //Lexicon of Power
					{
						Plr->Gossip_SendPOI(-4801.72f, -1189.41f, 7, 6, 0, "Ironforge Inscription");
						SendQuickMenu(14174);
					}
					break;
			}
		}
};

class KharanosGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4287, plr);
			Menu->AddItem(0, "Bank", 1);
			Menu->AddItem(0, "Gryphon Master", 2);
			Menu->AddItem(0, "Guild Master", 3);
			Menu->AddItem(0, "The Inn", 4);
			Menu->AddItem(0, "Stable Master", 5);
			Menu->AddItem(0, "Class Trainer", 6);
			Menu->AddItem(0, "Profession Trainer", 7);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     //Bank
					SendQuickMenu(4288);
					break;

				case 2:     //Gryphon Master
					SendQuickMenu(4289);
					break;

				case 3:     //Guild Master
					SendQuickMenu(4290);
					break;

				case 4:     //The Inn
					SendQuickMenu(4291);
					Plr->Gossip_SendPOI(-5582.66f, -525.89f, 7, 6, 0, "Thunderbrew Distillery");
					break;

				case 5:     //Stable Master
					SendQuickMenu(5985);
					Plr->Gossip_SendPOI(-5604, -509.58f, 7, 6, 0, "Shelby Stoneflint");
					break;

				case 6:     //Class Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
						Menu->AddItem(0, "Hunter"       , 8);
						Menu->AddItem(0, "Mage"         , 9);
						Menu->AddItem(0, "Paladin"      , 10);
						Menu->AddItem(0, "Priest"      , 11);
						Menu->AddItem(0, "Rogue"       , 12);
						Menu->AddItem(0, "Warlock"     , 13);
						Menu->AddItem(0, "Warrior"     , 14);
						Menu->SendTo(Plr);
					}
					break;

				case 7:     // Profession Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4300, Plr);
						Menu->AddItem(0, "Alchemy"           , 15);
						Menu->AddItem(0, "Blacksmithing"     , 16);
						Menu->AddItem(0, "Cooking"           , 17);
						Menu->AddItem(0, "Enchanting"        , 18);
						Menu->AddItem(0, "Engineering"       , 19);
						Menu->AddItem(0, "First Aid"         , 20);
						Menu->AddItem(0, "Fishing"           , 21);
						Menu->AddItem(0, "Herbalism"         , 22);
						Menu->AddItem(0, "Inscription"		  , 23);
						Menu->AddItem(0, "Leatherworking"    , 24);
						Menu->AddItem(0, "Mining"            , 25);
						Menu->AddItem(0, "Skinning"          , 26);
						Menu->AddItem(0, "Tailoring"         , 27);
						Menu->SendTo(Plr);
					}
					break;


					////////////////
					// Class trainer submenu
					////////

				case 8: //Hunter
					{
						Plr->Gossip_SendPOI(-5618.29f, -454.25f, 7, 6, 0, "Grif Wildheart");
						SendQuickMenu(4293);
					}
					break;

				case 9: //Mage
					{
						Plr->Gossip_SendPOI(-5585.6f, -539.99f, 7, 6, 0, "Magis Sparkmantle");
						SendQuickMenu(4294);
					}
					break;

				case 10: //Paladin
					{
						Plr->Gossip_SendPOI(-5585.6f, -539.99f, 7, 6, 0, "Azar Stronghammer");
						SendQuickMenu(4295);
					}
					break;

				case 11: //Priest
					{
						Plr->Gossip_SendPOI(-5591.74f, -525.61f, 7, 6, 0, "Maxan Anvol");
						SendQuickMenu(4296);
					}
					break;

				case 12: //Rogue
					{
						Plr->Gossip_SendPOI(-5602.75f, -542.4f, 7, 6, 0, "Hogral Bakkan");
						SendQuickMenu(4297);
					}
					break;

				case 13: //Warlock
					{
						Plr->Gossip_SendPOI(-5641.97f, -523.76f, 7, 6, 0, "Gimrizz Shadowcog");
						SendQuickMenu(4298);
					}
					break;

				case 14: //Warrior
					{
						Plr->Gossip_SendPOI(-5604.79f, -529.38f, 7, 6, 0, "Granis Swiftaxe");
						SendQuickMenu(4299);
					}
					break;

				case 15: //Alchemy
					{
						SendQuickMenu(4301);
					}
					break;

				case 16: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-5584.72f, -428.41f, 7, 6, 0, "Tognus Flintfire");
						SendQuickMenu(4302);
					}
					break;

				case 17: //Cooking
					{
						Plr->Gossip_SendPOI(-5596.85f, -541.43f, 7, 6, 0, "Gremlock Pilsnor");
						SendQuickMenu(4303);
					}
					break;

				case 18: //Enchanting
					{
						SendQuickMenu(4304);
					}
					break;

				case 19: //Engineering
					{
						SendQuickMenu(4305);
					}
					break;

				case 20: //First Aid
					{
						Plr->Gossip_SendPOI(-5603.67f, -523.57f, 7, 6, 0, "Thamner Pol");
						SendQuickMenu(4306);
					}
					break;

				case 21: //Fishing
					{
						Plr->Gossip_SendPOI(-5202.39f, -51.36f, 7, 6, 0, "Paxton Ganter");
						SendQuickMenu(4307);
					}
					break;

				case 22: //Herbalism
					{
						SendQuickMenu(4308);
					}
					break;

				case 23: //Inscription
					{
						Plr->Gossip_SendPOI(-4801.72f, -1189.41f, 7, 6, 0, "Ironforge Inscription");
						SendQuickMenu(13884);
					}
					break;

				case 24: //Leatherworking
					{
						SendQuickMenu(4310);
					}
					break;

				case 25: //Mining
					{
						Plr->Gossip_SendPOI(-5531, -666.53f, 7, 6, 0, "Yarr Hamerstone");
						SendQuickMenu(4311);
					}
					break;

				case 26: //Skinning
					{
						SendQuickMenu(4312);
					}
					break;

				case 27: //Tailoring
					{
						SendQuickMenu(4313);
					}
					break;
			}
		}
};

class FalconwingGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, plr);
			Menu->AddItem(0, "Bat Handler", 1);
			Menu->AddItem(0, "Guild Master", 2);
			Menu->AddItem(0, "The Inn", 3);
			Menu->AddItem(0, "Stable Master", 4);
			Menu->AddItem(0, "Class Trainer", 5);
			Menu->AddItem(0, "Profession Trainer", 6);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     //Bat Handler
					SendQuickMenu(2593);
					Plr->Gossip_SendPOI(9376.4f, -7164.92f, 7, 6, 0, "Silvermoon City, Flight Master");
					break;

				case 2:     //Guild Master
					SendQuickMenu(2593);
					break;

				case 3:     //The Inn
					SendQuickMenu(2593);
					Plr->Gossip_SendPOI(9476.916f, -6859.2f, 7, 6, 0, "Falconwing Square, Innkeeper");
					break;

				case 4:     //Stable Master
					SendQuickMenu(2593);
					Plr->Gossip_SendPOI(9487.6f, -6830.59f, 7, 6, 0, "Falconwing Square, Stable Master");
					break;

				case 5:     //Class Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
						Menu->AddItem(0, "Druid"        , 7);
						Menu->AddItem(0, "Hunter"       , 8);
						Menu->AddItem(0, "Mage"         , 9);
						Menu->AddItem(0, "Paladin"     , 10);
						Menu->AddItem(0, "Priest"      , 11);
						Menu->AddItem(0, "Rogue"       , 12);
						Menu->AddItem(0, "Warlock"     , 13);
						Menu->SendTo(Plr);
					}
					break;

				case 6:     // Profession Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
						Menu->AddItem(0, "Alchemy"           , 14);
						Menu->AddItem(0, "Blacksmithing"     , 15);
						Menu->AddItem(0, "Cooking"           , 16);
						Menu->AddItem(0, "Enchanting"        , 17);
						Menu->AddItem(0, "Engineering"       , 18);
						Menu->AddItem(0, "First Aid"         , 19);
						Menu->AddItem(0, "Fishing"           , 20);
						Menu->AddItem(0, "Herbalism"         , 21);
						Menu->AddItem(0, "Jewelcrafting"     , 22);
						Menu->AddItem(0, "Leatherworking"    , 23);
						Menu->AddItem(0, "Mining"            , 24);
						Menu->AddItem(0, "Skinning"          , 25);
						Menu->AddItem(0, "Tailoring"         , 26);
						Menu->SendTo(Plr);
					}
					break;


					////////////////
					// Class trainer submenu
					////////

				case 7: //Druid
					{
						SendQuickMenu(2593);
					}
					break;

				case 8: //Hunter
					{
						Plr->Gossip_SendPOI(9529.2f, -6864.58f, 7, 6, 0, "Falconwing Square, Hunter Trainer");
						SendQuickMenu(2593);
					}
					break;

				case 9: //Mage
					{
						Plr->Gossip_SendPOI(9462.24f, -6853.45f, 7, 6, 0, "Falconwing Square, Mage Trainer");
						SendQuickMenu(2593);
					}
					break;

				case 10: //Paladin <-- Needs to change flag to other sign (don't know how to describe it)
					{
						Plr->Gossip_SendPOI(9516.05f, -6870.96f, 7, 6, 0, "Falconwing Square, Paladin Trainer");
						SendQuickMenu(2593);
					}
					break;

				case 11: //Priest
					{
						Plr->Gossip_SendPOI(9466.62f, -6844.23f, 7, 6, 0, "Falconwing Square, Priest Trainer");
						SendQuickMenu(2593);
					}
					break;

				case 12: //Rogue
					{
						Plr->Gossip_SendPOI(9534.15f, -6876.0f, 7, 6, 0, "Falconwing Square, Rogue Trainer");
						SendQuickMenu(2593);
					}
					break;

				case 13: //Warlock
					{
						Plr->Gossip_SendPOI(9467.63f, -6862.82f, 7, 6, 0, "Falconwing Square, Warlock Trainer");
						SendQuickMenu(2593);
					}
					break;

				case 14: //Alchemy
					{
						Plr->Gossip_SendPOI(8661.36f, -6367.0f, 7, 6, 0, "Saltheril's Haven, Alchemist");
						SendQuickMenu(2593);
					}
					break;

				case 15: //Blacksmithing
					{
						Plr->Gossip_SendPOI(8986.43f, -7419.07f, 7, 6, 0, "Farstrider Retreat, Blacksmith");
						SendQuickMenu(2593);
					}
					break;

				case 16: //Cooking
					{
						Plr->Gossip_SendPOI(9494.86f, -6879.45f, 7, 6, 0, "Falconwing Square, Cook");
						SendQuickMenu(2593);
					}
					break;

				case 17: //Enchanting
					{
						Plr->Gossip_SendPOI(8657.6f, -6366.7f, 7, 6, 0, "Saltheril's Haven, Enchanter");
						SendQuickMenu(2593);
					}
					break;

				case 18: //Engineering
					{
						SendQuickMenu(2593);
					}
					break;

				case 19: //First Aid
					{
						Plr->Gossip_SendPOI(9479.53f, -6880.07f, 7, 6, 0, "Falconwing Square, First Aid");
						SendQuickMenu(2593);
					}
					break;

				case 20: //Fishing
					{
						SendQuickMenu(2593);
					}
					break;

				case 21: //Herbalism
					{
						Plr->Gossip_SendPOI(8678.92f, -6329.09f, 7, 6, 0, "Saltheril's Haven, Herbalist");
						SendQuickMenu(2593);
					}
					break;

				case 22: //Jewelcrafting
					{
						Plr->Gossip_SendPOI(9484.79f, -6876.58f, 7, 6, 0, "Falconwing Square, Jewelcrafter");
						SendQuickMenu(2593);
					}
					break;

				case 23: //Leatherworking
					{
						Plr->Gossip_SendPOI(9363.75f, -7130.75f, 7, 6, 0, "Eversong Woods, Leatherworker");
						SendQuickMenu(2593);
					}
					break;

				case 24: //Mining
					{
						SendQuickMenu(2593);
					}
					break;

				case 25: //Skinning
					{
						Plr->Gossip_SendPOI(9362.89f, -7134.58f, 7, 6, 0, "Eversong Woods, Skinner");
						SendQuickMenu(2593);
					}
					break;

				case 26: //Tailoring
					{
						Plr->Gossip_SendPOI(8680.36f, -6327.51f, 7, 6, 0, "Saltheril's Haven, Tailor");
						SendQuickMenu(2593);
					}
					break;
			}
		}
};

class AzureWatchGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10066, plr);
			Menu->AddItem(0, "Bank"					, 1);
			Menu->AddItem(0, "Hippogryph Master"	, 2);
			Menu->AddItem(0, "Guild Master"			, 3);
			Menu->AddItem(0, "Inn"					, 4);
			Menu->AddItem(0, "Stable"				, 5);
			Menu->AddItem(0, "Class Trainer"		, 6);
			Menu->AddItem(0, "Profession Trainer"	, 7);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Main menu handlers
					/////

				case 1:     //Bank
					SendQuickMenu(10067);
					break;

				case 2:     //Hippogryph Master
					SendQuickMenu(10071);
					break;

				case 3:     //Guild Master
					SendQuickMenu(10073);
					break;

				case 4:     //Inn
					SendQuickMenu(10074);
					Plr->Gossip_SendPOI(-4127.81f, -12467.7f, 7, 6, 0, "Azure Watch, Innkeeper");
					break;

				case 5:     //Stable Master
					SendQuickMenu(10075);
					Plr->Gossip_SendPOI(-4146.42f, -12492.7f, 7, 6, 0, "Azure Watch, Stable Master");
					break;

				case 6:     //Class Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10076, Plr);
						Menu->AddItem(0, "Druid"       , 8);
						Menu->AddItem(0, "Hunter"      , 9);
						Menu->AddItem(0, "Mage"		, 10);
						Menu->AddItem(0, "Paladin"     , 11);
						Menu->AddItem(0, "Priest"      , 12);
						Menu->AddItem(0, "Shaman"      , 13);
						Menu->AddItem(0, "Warrior"     , 14);
						Menu->SendTo(Plr);
					}
					break;

				case 7:     //Profession Trainer
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10087, Plr);
						Menu->AddItem(0, "Alchemy"           , 15);
						Menu->AddItem(0, "Blacksmithing"     , 16);
						Menu->AddItem(0, "Cooking"           , 17);
						Menu->AddItem(0, "Enchanting"        , 18);
						Menu->AddItem(0, "Engineering"       , 19);
						Menu->AddItem(0, "First Aid"         , 20);
						Menu->AddItem(0, "Fishing"           , 21);
						Menu->AddItem(0, "Herbalism"         , 22);
						Menu->AddItem(0, "Inscription"       , 23);
						Menu->AddItem(0, "Jewelcrafting"     , 24);
						Menu->AddItem(0, "Leatherworking"    , 25);
						Menu->AddItem(0, "Mining"            , 26);
						Menu->AddItem(0, "Skinning"          , 27);
						Menu->AddItem(0, "Tailoring"         , 28);
						Menu->SendTo(Plr);
					}
					break;


					////////////////
					// Class trainer submenu
					////////

				case 8: //Druid
					{
						SendQuickMenu(10077);
					}
					break;

				case 9: //Hunter
					{
						Plr->Gossip_SendPOI(-4203.65f, -12467.7f, 7, 6, 0, "Azure Watch, Hunter Trainer");
						SendQuickMenu(10078);
					}
					break;

				case 10: //Mage
					{
						Plr->Gossip_SendPOI(-4149.62f, -12530.1f, 7, 6, 0, "Azure Watch, Mage Trainer");
						SendQuickMenu(10081);
					}
					break;

				case 11: //Paladin
					{
						Plr->Gossip_SendPOI(-4138.98f, -12468.5f, 7, 6, 0, "Azure Watch, Paladin Trainer");
						SendQuickMenu(10083);
					}
					break;

				case 12: //Priest
					{
						Plr->Gossip_SendPOI(-4131.66f, -12478.6f, 7, 6, 0, "Azure Watch, Priest Trainer");
						SendQuickMenu(10084);
					}
					break;

				case 13: //Shaman
					{
						Plr->Gossip_SendPOI(-4162.33f, -12456.1f, 7, 6, 0, "Azure Watch, Shaman Trainer");
						SendQuickMenu(10085);
					}
					break;

				case 14: //Warrior
					{
						Plr->Gossip_SendPOI(-4165.05f, -12536.4f, 7, 6, 0, "Azure Watch, Warrior Trainer");
						SendQuickMenu(10086);
					}
					break;

				case 15: //Alchemy
					{
						Plr->Gossip_SendPOI(-4191.15f, -12470, 7, 6, 0, "Azure Watch, Alchemist");
						SendQuickMenu(10088);
					}
					break;

				case 16: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-4726.29f, -12387.0f, 7, 6, 0, "Odesyus' Landing, Blacksmith");
						SendQuickMenu(10089);
					}
					break;

				case 17: //Cooking
					{
						Plr->Gossip_SendPOI(-4708.59f, -12400.3f, 7, 6, 0, "Odesyus' Landing, Cook");
						SendQuickMenu(10090);
					}
					break;

				case 18: //Enchanting
					{
						SendQuickMenu(10091);
					}
					break;

				case 19: //Engineering
					{
						Plr->Gossip_SendPOI(-4157.57f, -12470.2f, 7, 6, 0, "Azure Watch, Engineering Trainer");
						SendQuickMenu(10092);
					}
					break;

				case 20: //First Aid
					{
						Plr->Gossip_SendPOI(-4199.1f, -12469.9f, 7, 6, 0, "Azure Watch, First Aid Trainer");
						SendQuickMenu(10093);
					}
					break;

				case 21: //Fishing
					{
						Plr->Gossip_SendPOI(-4266.34f, -12985.4f, 7, 6, 0, "Ammen Ford, Fisherwoman");
						SendQuickMenu(10094);
					}
					break;

				case 22: //Herbalism
					{
						Plr->Gossip_SendPOI(-4189.43f, -12459.4f, 7, 6, 0, "Azure Watch, Herbalist");
						SendQuickMenu(10095);
					}
					break;

				case 23: //Inscription
					{
						Plr->Gossip_SendPOI(-3889.3f, -11495, 7, 6, 0, "Exodar, Inscription");
						SendQuickMenu(13887);
					}
					break;

				case 24: //Jewelcrafting
					{
						SendQuickMenu(10100);
					}
					break;

				case 25: //Leatherworking
					{
						Plr->Gossip_SendPOI(-3442.68f, -12322.2f, 7, 6, 0, "Stillpine Hold, Leatherworker");
						SendQuickMenu(10096);
					}
					break;

				case 26: //Mining
					{
						Plr->Gossip_SendPOI(-4179.89f, -12493.1f, 7, 6, 0, "Azure Watch, Mining Trainer");
						SendQuickMenu(10097);
					}
					break;

				case 27: //Skinning
					{
						Plr->Gossip_SendPOI(-3431.17f, -12316.5f, 7, 6, 0, "Stillpine Hold, Skinner");
						SendQuickMenu(10098);
					}
					break;

				case 28: //Tailoring
					{
						Plr->Gossip_SendPOI(-4711.54f, -12386.7f, 7, 6, 0, "Odesyus' Landing, Tailor");
						SendQuickMenu(10099);
					}
					break;
			}
		}
};

/*****************************************************************************************/
/* Shattrath Guards   original structure by AeThIs. Translated, updated and  by Pepsi1x1 */
/*****************************************************************************************/

class ShattrathGuard : public GossipScript
{
	public:
		void GossipHello(Object* pObject, Player* plr)
		{
			GossipMenu* Menu;
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, plr);

			Menu->AddItem(0, "World's End Tavern", 1);
			Menu->AddItem(0, "Bank", 2);
			Menu->AddItem(0, "Inn", 3);
			Menu->AddItem(0, "Flight Master", 4);
			Menu->AddItem(0, "Mailbox", 5);
			Menu->AddItem(0, "Stable Master", 6);
			Menu->AddItem(0, "Battlemaster", 7);
			Menu->AddItem(0, "Profession Trainer", 8);
			Menu->AddItem(0, "Mana Loom", 9);
			Menu->AddItem(0, "Alchemy Lab", 10);
			Menu->AddItem(0, "Gem Merchant", 11);

			Menu->SendTo(plr);
		}

		void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char* Code)
		{
			GossipMenu* Menu;
			switch(IntId)
			{
				case 0:     // Return to start
					GossipHello(pObject, Plr);
					break;

					//////////////////////
					// Menus
					/////

				case 1:     // World's End Tavern
					SendQuickMenu(10394);
					Plr->Gossip_SendPOI(-1760.4f, 5166.9f, 7, 6, 0, "World's End Tavern");
					break;

				case 2:     // Shattrath Banks
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10395, Plr);
						Menu->AddItem(0, "Aldor Bank"          , 12);
						Menu->AddItem(0, "Scryers Bank"         , 13);
						Menu->SendTo(Plr);
					}
					break;


				case 3:     // Inn's
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10398, Plr);
						Menu->AddItem(0, "Aldor inn"          , 14);
						Menu->AddItem(0, "Scryers inn"         , 15);
						Menu->SendTo(Plr);
					}
					break;

				case 4:     // Gryphon Master
					SendQuickMenu(10402);
					Plr->Gossip_SendPOI(-1831.9f, 5298.2f, 7, 6, 0, "Gryphon Master");
					break;

				case 5:     // Mailboxes
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10403, Plr);
						Menu->AddItem(0, "Aldor inn"          , 16);
						Menu->AddItem(0, "Scryers inn"         , 17);
						Menu->AddItem(0, "Aldor Bank"          , 18);
						Menu->AddItem(0, "Scryers Bank"         , 19);
						Menu->SendTo(Plr);
					}
					break;

				case 6:     // Stable Masters
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10404, Plr);
						Menu->AddItem(0, "Aldor Stable"          , 20);
						Menu->AddItem(0, "Scryers Stable"         , 21);
						Menu->SendTo(Plr);
					}
					break;

				case 7:     // Battlemasters
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10405, Plr);
						Menu->AddItem(0, "Alliance Battlemasters"          , 22);
						Menu->AddItem(0, "Horde & Arena Battlemasters"         , 23);
						Menu->SendTo(Plr);
					}
					break;

				case 8:     // Proffesion Trainers
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10391, Plr);
						Menu->AddItem(0, "Alchemy"          , 24);
						Menu->AddItem(0, "Blacksmithing"         , 25);
						Menu->AddItem(0, "Cooking"          , 26);
						Menu->AddItem(0, "Enchanting"         , 27);
						Menu->AddItem(0, "First Aid"          , 28);
						Menu->AddItem(0, "Jewelcrafting"         , 29);
						Menu->AddItem(0, "Leatherworking"          , 30);
						Menu->AddItem(0, "Skinning"         , 31);
						Menu->SendTo(Plr);
					}
					break;

				case 9:     // Mana Loom
					SendQuickMenu(10408);
					Plr->Gossip_SendPOI(-2073.9f, 5265.7f, 7, 6, 0, "Mana Loom");
					break;

				case 10:    // Alchemy Lab
					SendQuickMenu(10409);
					Plr->Gossip_SendPOI(-1648.1f, 5537.3f, 7, 6, 0, "Alchemy Lab");
					break;

				case 11:    // Gem Merchants
					{
						objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10410, Plr);
						Menu->AddItem(0, "Aldor Gem Merchant"          , 32);
						Menu->AddItem(0, "Scryer Gem Merchant"         , 33);
						Menu->SendTo(Plr);
					}
					break;


					//////////////////////
					// Banks
					/////


				case 12: //Aldor Bank
					{
						Plr->Gossip_SendPOI(-1730.8f, 5496.2f, 7, 6, 0, "Aldor Bank");
						SendQuickMenu(10396);
					}
					break;

				case 13: //Scryers Bank
					{
						Plr->Gossip_SendPOI(-1999.6f, 5362.0f, 7, 6, 0, "Scryers Bank");
						SendQuickMenu(10397);
					}
					break;


					//////////////////////
					// Inns
					/////


				case 14: //Aldor Inn
					{
						Plr->Gossip_SendPOI(-1897.5f, 5767.6f, 7, 6, 0, "Aldor inn");
						SendQuickMenu(10399);
					}
					break;

				case 15: //Scryers Inn
					{
						Plr->Gossip_SendPOI(-2180.6f, 5403.9f, 7, 6, 0, "Scryers inn");
						SendQuickMenu(10401);
					}
					break;


					//////////////////////
					// Mailboxes
					/////


				case 16: //Aldor Inn
					{
						Plr->Gossip_SendPOI(-1886.9f, 5761.5f, 7, 6, 0, "Aldor Inn");
						SendQuickMenu(10399);
					}
					break;

				case 17: //Scryers Bank
					{
						Plr->Gossip_SendPOI(-2175.0f, 5411.7f, 7, 6, 0, "Scryers Bank");
						SendQuickMenu(10397);
					}
					break;

				case 18: //Aldor Bank
					{
						Plr->Gossip_SendPOI(-1695.5f, 5521.8f, 7, 6, 0, "Aldor Bank");
						SendQuickMenu(10396);
					}
					break;

				case 19: //Scryers Inn
					{
						Plr->Gossip_SendPOI(-2033.0f, 5336.1f, 7, 6, 0, "Scryers Inn");
						SendQuickMenu(10401);
					}
					break;


					//////////////////////
					// Stable Masters
					/////


				case 20: //Aldor Stable Master
					{
						Plr->Gossip_SendPOI(-1889.6f, 5761.5f, 7, 6, 0, "Aldor Stable");
						SendQuickMenu(10399);
					}
					break;

				case 21: //Scryers Stable Master
					{
						Plr->Gossip_SendPOI(-2170.0f, 5404.6f, 7, 6, 0, "Scryers Stable");
						SendQuickMenu(10401);
					}
					break;


					//////////////////////
					// Battlemasters
					/////


				case 22: //Alliance Battlemaster
					{
						Plr->Gossip_SendPOI(-1831.9f, 5298.2f, 7, 6, 0, "Alliance Battlemasters");
						SendQuickMenu(10406);
					}
					break;

				case 23: //Horde Battle Master and Arena Battle Master
					{
						Plr->Gossip_SendPOI(-1968.7f, 5262.2f, 7, 6, 0, "Horde & Arena Battlemasters");
						SendQuickMenu(10407);
					}
					break;






					//////////////////////
					// Profession Trainers
					/////


				case 24: //Alchemy
					{
						Plr->Gossip_SendPOI(-1661.0f, 5538, 7, 6, 0, "Alchemy Trainer");
						SendQuickMenu(10413);
					}
					break;

				case 25: //Blacksmithing
					{
						Plr->Gossip_SendPOI(-1847.7f, 5230.3f, 7, 6, 0, "Blacksmithing Trainer");
						SendQuickMenu(10400);
					}
					break;

				case 26: //Cooking
					{
						Plr->Gossip_SendPOI(-2067.4f, 5316.5f, 7, 6, 0, "Cooking Trainer");
						SendQuickMenu(10414);
					}
					break;

				case 27: //Enchanting
					{
						Plr->Gossip_SendPOI(-2278.5f, 5567.7f, 7, 6, 0, "Enchanting Trainer");
						SendQuickMenu(10415);
					}
					break;

				case 28: //First Aid
					{
						Plr->Gossip_SendPOI(-1592.0f, 5263.7f, 7, 6, 0, "First Aid Trainer");
						SendQuickMenu(10416);
					}
					break;

				case 29: //Jewelcrafting
					{
						Plr->Gossip_SendPOI(-1653.3f, 5665.1f, 7, 6, 0, "Jewelcrafting Trainer");
						SendQuickMenu(10417);
					}
					break;

				case 30: //Leatherworking
					{
						Plr->Gossip_SendPOI(-2060.9f, 5256.6f, 7, 6, 0, "Leatherworking Trainer");
						SendQuickMenu(10418);
					}
					break;

				case 31: //Skinning
					{
						Plr->Gossip_SendPOI(-2047.9f, 5299.6f, 7, 6, 0, "Skinning Trainer");
						SendQuickMenu(10419);
					}
					break;


					//////////////////////
					// Gem Merchants
					/////


				case 32: //Aldor gem merchant
					{
						Plr->Gossip_SendPOI(-1649.3f, 5668.6f, 7, 6, 0, "Aldor gem merchant");
						SendQuickMenu(10411);
					}
					break;

				case 33: //Scryers gem merchant
					{
						Plr->Gossip_SendPOI(-2193.9f, 5422.1f, 7, 6, 0, "Scryers gem merchant");
						SendQuickMenu(10412);
					}
					break;

			}
		}
};

class DalaranGuard : public Arcemu::Gossip::Script
{
public:
	void OnHello(Object *pObject, Player *Plr)
	{
		Arcemu::Gossip::Menu menu(pObject->GetGUID(), 50000);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Arena"				,1);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Auction House"		,2);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Bank"				,3);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Barber"			,4);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Battlemasters"		,5);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Capital Portals"	,6);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Flight Master"		,7);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Guild Master"		,8);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Inn"				,9);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Locksmith"			,77);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Mailbox"			,10);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Points of Interest", 11);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Stable Master"		,12);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Trainers"			,13);
		menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Vendors"			,14);
		menu.Send(Plr);
	}

	void OnSelectOption(Object *pObject, Player *Plr, uint32 Id, const char * Code)
	{
		switch(Id)
		{
		case 0: OnHello(pObject, Plr); break;
		case 1:		// Arena
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13976);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Eastern Sewer Entrance", 15);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Western Sewer Entrance", 16);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Well Entrance"			, 17);
				menu.Send(Plr);
			}break;
		case 2:		// Auction House
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14010);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Alliance Quarter"	, 18);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Horde Quarter"		, 19);
				menu.Send(Plr);
			}break;
		case 3:		// Bank
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14007);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Northern Bank"	, 20);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Southern Bank"	, 21);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Sewers"		, 22); // Sewers 1
				menu.Send(Plr);
			}break;
		case 4:		// Barber
			{
				Plr->Gossip_SendPOI(5889.67f, 621.21f, 7, 99, 0, "Barber");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14003, Plr);
			}break;
		case 5:		// Battlemasters
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13977);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Alliance Quarter"	, 18);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Horde Quarter"		, 19);
				menu.Send(Plr);
			}break;
		case 6:		// Capital Portals
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13977);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Alliance Quarter"	, 18);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Horde Quarter"		, 19);
				menu.Send(Plr);
			}break;
		case 7:		// Flight Master
			{
				Plr->Gossip_SendPOI(5815.91f, 453.64f, 7, 99, 0, "Flight Master");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 10086, Plr);
			}break;
		case 8:		// Guild Master
			{
				Plr->Gossip_SendPOI(5762.21f, 622.84f, 7, 99, 0, "Guild Master");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 10095, Plr);
			}break;
		case 9:		// Inn
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14002);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Alliance Inn"	, 24);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Horde Inn"		, 25);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Sewers"		, 26); // Sewers 2
				menu.Send(Plr);
			}break;
		case 10:	// Mailbox
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 10090);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Inn"				, 9);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Bank"				, 3);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Krasus' Landing"	, 74);
				menu.Send(Plr);
			}break;
		case 11:	// Points of Interest
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 10056);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Alliance Quarter"	, 18);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Horde Quarter"		, 19);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Violet Citadel"	, 27);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Violet Hold"		, 28);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Sewers"				, 22); // Sewers 1
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Trade District"		, 29);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Krasus'Landing"		, 74);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Antonidas Memorial"	, 30);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Runeweaver Square"		, 31);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Eventide"			, 32);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Cemetary"				, 33);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Lexicon of Power"		, 34);
				menu.Send(Plr);
			}break;
		case 12:	// Stable Master
			{
				Plr->Gossip_SendPOI(5859.68f, 557.57f, 7, 99, 0, "Stable Master");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 10083, Plr);
			}break;
		case 13:	// Trainers
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 10082);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Class Trainer"			, 35);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Cold Weather Flying Trainer", 76);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Portal Trainer"		, 36);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Profession Trainer"	, 37);
				menu.Send(Plr);
			}break;
		case 14:	// Vendors
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 10173);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Armor"					, 38);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Clothing"				, 39);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Emblem Gear"			, 40);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Flowers"				, 41);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Fruit"					, 42);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "General Goods"			, 43);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Jewelry"				, 44);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Pet Supplies & Exotic Mounts", 45);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Pie, Pastry & Cakes"	, 46);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Reagents & Magical Goods", 47);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Toys"					, 48);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Trade Supplies"		, 43);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Trinkets. Relics & Off-hand items", 49);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Weapons"				, 50);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Wine & Cheese"			, 51);
				menu.Send(Plr);
			}break;
		case 15:	// Eastern Sewer Entrance
			{
				Plr->Gossip_SendPOI(5801.77f, 551.07f, 7, 99, 0, "Eastern Sewer Entrance");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13961, Plr);
			}break;
		case 16:	// Western Sewer Entrance
			{
				Plr->Gossip_SendPOI(5815.21f, 762.65f, 7, 99, 0, "Western Sewer Entrance");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13960, Plr);
			}break;
		case 17:	// Well Entrance
			{
				Plr->Gossip_SendPOI(5791.60f, 561.82f, 7, 99, 0, "Well Entrance");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13969, Plr);
			}break;
		case 18:	// The Alliance Quarter
			{
				Plr->Gossip_SendPOI(5763.20f, 711.77f, 7, 99, 0, "The Alliance Quarter");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13973, Plr);
			}break;
		case 19:	// The Horde Quarter
			{
				Plr->Gossip_SendPOI(5860.35f, 592.97f, 7, 99, 0, "The Horde Quarter");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13972, Plr);
			}break;
		case 20:	// Northern Bank
			{
				Plr->Gossip_SendPOI(5979.58f, 608.16f, 7, 99, 0, "Northern Bank");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14005, Plr);
			}break;
		case 21:	// Southern Bank
			{
				Plr->Gossip_SendPOI(5631.20f, 694.15f, 7, 99, 0, "Southern Bank");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14006, Plr);
			}break;
		case 22:	// Sewers 1
			{
				Plr->Gossip_SendPOI(5766.31f, 731.66f, 7, 99, 0, "Sewers");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13974, Plr);
			}break;
		case 24:	// Alliance Inn
			{
				Plr->Gossip_SendPOI(5718.01f, 689.28f, 7, 99, 0, "Alliance Inn");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13992, Plr);
			}break;
		case 25:	// Horde Inn
			{
				Plr->Gossip_SendPOI(5890.22f, 500.58f, 7, 99, 0, "Horde Inn");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13993, Plr);
			}break;	
		case 26:	// Sewers 2
			{
				Plr->Gossip_SendPOI(5761.06f, 714.45f, 7, 99, 0, "Sewers");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13974, Plr);
			}break;	
		case 27:	// The Violet Citadel
			{
				Plr->Gossip_SendPOI(5795.11f, 769.15f, 7, 99, 0, "The Violet Citadel");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13971, Plr);
			}break;
		case 28:	// The Violet Hold
			{
				Plr->Gossip_SendPOI(5720.40f, 537.46f, 7, 99, 0, "The Violet Hold");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13970, Plr);
			}break;
		case 29:	// Trade District
			{
				Plr->Gossip_SendPOI(5900.88f, 726.65f, 7, 99, 0, "Trade District");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13980, Plr);
			}break;
		case 30:	// Antonidas Memorial
			{
				Plr->Gossip_SendPOI(5951.74f, 683.53f, 7, 99, 0, "Antonidas Memorial");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13968, Plr);
			}break;
		case 31:	// Runeweaver Square
			{
				Plr->Gossip_SendPOI(5810.96f, 632.72f, 7, 99, 0, "Runeweaver Square");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13967, Plr);
			}break;
		case 32:	// The Eventide
			{
				Plr->Gossip_SendPOI(5694.66f, 650.82f, 7, 99, 0, "The Eventide");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13966, Plr);
			}break;
		case 33:	// Cemetary
			{
				Plr->Gossip_SendPOI(5853.35f, 769.06f, 7, 99, 0, "Cemetary");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13965, Plr);
			}break;	
		case 34:	// Lexicon of Power
			{
				Plr->Gossip_SendPOI(5860.99f, 707.23f, 7, 99, 0, "Lexicon of Power");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14174, Plr);
			}break;	
		case 35:	// Class Trainers
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14018);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Alliance Quarter"	, 18);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Horde Quarter"		, 19);
				menu.Send(Plr);
			}break;
		case 36:	// Portal Trainer
			{
				Plr->Gossip_SendPOI(5810.07f, 581.0f, 7, 99, 0, "Portal Trainer");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13999, Plr);
			}break;
		case 37:	// Profession Trainer
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13996);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Alchemy"		, 52);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Blacksmithing"	, 53);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Cooking"		, 54);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Enchanting"	, 55);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "engineering"	, 56);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "First Aid"		, 57);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Fishing"		, 58);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Herbalism"		, 59);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Inscription"	, 60);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Jewelcrafting"	, 61);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Leatherworking", 62);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Mining"		, 63);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Skinning"		, 64);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Tailoring"		, 65);
				menu.Send(Plr);
			}break;
		case 38:	// Armor
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14117);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Cloth Armor"          , 66);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Leather Armor"          , 67);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Mail Armor"          , 68);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Plate Armor"          , 69);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Shields"          , 70);
				menu.Send(Plr);
			}break;
		case 39:	// Clothing
			{
				Plr->Gossip_SendPOI(5793.11f, 686.44f, 7, 99, 0, "Clothing");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14112, Plr);
			}break;
		case 40:	// Emblem Gear
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14108);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Alliance Quarter"          , 18);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "The Horde Quarter"          , 19);
				menu.Send(Plr);
			}break;
		case 41:	// Flowers
			{
				Plr->Gossip_SendPOI(5772.27f, 576.61f, 7, 99, 0, "Flowers");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 10159, Plr);
			}break;
		case 42:	// Fruit
			{
				Plr->Gossip_SendPOI(5754.02f, 694.18f, 7, 99, 0, "Fruit");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14106, Plr);
			}break;
		case 43:	// General Goods
			{
				Plr->Gossip_SendPOI(5681.22f, 616.57f, 7, 99, 0, "General Goods");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14105, Plr);
			}break;
		case 44:	// Jewelry
			{
				Plr->Gossip_SendPOI(5874.27f, 719.18f, 7, 99, 0, "Portal Trainer");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13984, Plr);
			}break;
		case 45:	// Pet Supplies & Exotic Mounts
			{
				Plr->Gossip_SendPOI(5833.13f, 572.72f, 7, 99, 0, "Pet Supplies & Exotic Mounts");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14103, Plr);
			}break;
		case 46:	// Pie, Pastry & Cakes
			{
				Plr->Gossip_SendPOI(5905.86f, 629.29f, 7, 99, 0, "Pie, Pastry & Cakes");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14102, Plr);
			}break;
		case 47:	// Reagents & Magical Goods
			{
				Plr->Gossip_SendPOI(5758.7f, 737.24f, 7, 99, 0, "Reagents & Magical Goods");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14104, Plr);
			}break;
		case 48:	// Toys
			{
				Plr->Gossip_SendPOI(5813.48f, 688.49f, 7, 99, 0, "Toys");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14100, Plr);
			}break;
		case 49:	// Trinkets. Relics & Off-hand items
			{
				Plr->Gossip_SendPOI(5755.53f, 642.03f, 7, 99, 0, "Trinkets. Relics & Off-hand items");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14110, Plr);
			}break;
		case 50:	// Weapons
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 14113);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Melee Weapons"          , 71);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Ranged & Thrown Weapons"          , 72);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Staves & Wands"          , 73);
				menu.Send(Plr);
			}break;
		case 51:	// Wine & Cheese
			{
				Plr->Gossip_SendPOI(5885.66f, 606.89f, 7, 99, 0, "Wine & Cheese");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14095, Plr);
			}break;
		case 52:	// Alchemy
			{
				Plr->Gossip_SendPOI(5888.05f, 702.44f, 7, 99, 0, "Alchemy");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13995, Plr);
			}break;
		case 53:	// Blacksmithing
			{
				Plr->Gossip_SendPOI(5904.86f, 680.12f, 7, 99, 0, "Blacksmithing");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13994, Plr);
			}break;
		case 54:	// Cooking
			{
				Arcemu::Gossip::Menu menu(pObject->GetGUID(), 13991);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Alliance Inn"          , 24);
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Horde Inn"          , 25);
				menu.Send(Plr);
			}break;
		case 55:	// Enchanting
			{
				Plr->Gossip_SendPOI(5840.01f, 726.53f, 7, 99, 0, "Enchanting");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13990, Plr);
			}break;
		case 56:	// Engineering
			{
				Plr->Gossip_SendPOI(5922.55f, 727.05f, 7, 99, 0, "Engineering");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13989, Plr);
			}break;
		case 57:	// First Aid
			{
				Plr->Gossip_SendPOI(5862.76f, 743.71f, 7, 99, 0, "First Aid");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13988, Plr);
			}break;
		case 58:	// Fishing
			{
				Plr->Gossip_SendPOI(5707.45f, 614.57f, 7, 99, 0, "Fishing");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13987, Plr);
			}break;
		case 59:	// Herbalism
			{
				Plr->Gossip_SendPOI(5873.99f, 689.44f, 7, 99, 0, "Herbalism");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13986, Plr);
			}break;
		case 60:	// Inscription
			{
				Plr->Gossip_SendPOI(5861.8f, 704.3f, 7, 99, 0, "Inscription");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13985, Plr);
			}break;
		case 61:	// Jewelcrafting
			{
				Plr->Gossip_SendPOI(5874.27f, 719.18f, 7, 99, 0, "Jewelcrafting");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13984, Plr);
			}break;
		case 62:	// Leatherworking
			{
				Plr->Gossip_SendPOI(5903.9f, 751.97f, 7, 99, 0, "Leatherworking");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13982, Plr);
			}break;
		case 63:	// Mining
			{
				Plr->Gossip_SendPOI(5923.27f, 709.86f, 7, 99, 0, "Mining");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13983, Plr);
			}break;
		case 64:	// Skinning
			{
				Plr->Gossip_SendPOI(5903.9f, 751.97f, 7, 99, 0, "Skinning");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13982, Plr);
			}break;
		case 65:	// Tailoring
			{
				Plr->Gossip_SendPOI(5881.78f, 746.64f, 7, 99, 0, "Tailoring");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 13981, Plr);
			}break;
		case 66:	// Cloth Armor
			{
				Plr->Gossip_SendPOI(5793.11f, 686.44f, 7, 99, 0, "Cloth Armor");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14112, Plr);
			}break;
		case 67:	// Leather Armor
			{
				Plr->Gossip_SendPOI(5667.76f, 627.16f, 7, 99, 0, "Leather Armor");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14111, Plr);
			}break;
		case 68:	// Mail Armor
			{
				Plr->Gossip_SendPOI(5667.76f, 627.16f, 7, 99, 0, "Mail Armor");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14111, Plr);
			}break;
		case 69:	// Plate Armor
			{
				Plr->Gossip_SendPOI(5914.85f, 667.22f, 7, 99, 0, "Plate Armor");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14109, Plr);
			}break;
		case 70:	// Shields
			{
				Plr->Gossip_SendPOI(5914.85f, 667.22f, 7, 99, 0, "Shields");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14109, Plr);
			}break;
		case 71:	// Melee Weapons
			{
				Plr->Gossip_SendPOI(5725.11f, 602.57f, 7, 99, 0, "Melee Weapons");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14098, Plr);
			}break;
		case 72:	// Ranged & Thrown Weapons
			{
				Plr->Gossip_SendPOI(5778.50f, 556.18f, 7, 99, 0, "Ranged & Thrown Weapons");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14097, Plr);
			}break;
		case 73:	// Staves & Wands
			{
				Plr->Gossip_SendPOI(5665.40f, 644.91f, 7, 99, 0, "Staves & Wands");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14096, Plr);
			}break;
		case 74:	// Krasu's Landing
			{
				Plr->Gossip_SendPOI(5815.91f, 453.64f, 7, 99, 0, "Krasus' Landing");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14009, Plr);
			}break;
		case 75:	// Trinkets, Relics & Off-hand Items
			{
				Plr->Gossip_SendPOI(5752.0f, 645.427f, 7, 99, 0, "Dalaran Trinkets");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14110, Plr);
			}break;
		case 76:	// Cold weather flying trainer
			{
				Plr->Gossip_SendPOI(5815.91f, 453.64f, 7, 99, 0, "Flight Master");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 60059, Plr);
			}break;
		case 77:	// Locksmith
			{
				Plr->Gossip_SendPOI(5934.0f, 575.626f, 7, 99, 0, "Dalaran Locksmith");
				Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 14004, Plr);
			}break;
		}
	}
};

void SetupGuardGossip(ScriptMgr* mgr)
{
	/* Guard List */
	mgr->register_gossip_script(1423,  new GoldshireGuard);			// Stormwind Guard
	mgr->register_gossip_script(68,    new StormwindGuard);			// Stormwind City Guard
	mgr->register_gossip_script(1976,  new StormwindGuard);			// Stormwind City Patroller
	mgr->register_gossip_script(29712,	new StormwindGuard);		// Stormwind Harbor Guard
	mgr->register_gossip_script(4262,  new DarnassusGuard);			// Darnassus Sentinel
	mgr->register_gossip_script(5624,  new UndercityGuard);			// Undercity Guardian
	mgr->register_creature_gossip(36213, new UndercityGuardOverseer);	// Kor'kron Overseer
	mgr->register_gossip_script(3571,  new TeldrassilGuard);		// Teldrassil Sentinel
	mgr->register_gossip_script(16222, new SilvermoonGuard);		// Silvermoon City Guardian
	mgr->register_gossip_script(16733, new ExodarGuard);	        // Exodar Peacekeeper
	mgr->register_gossip_script(20674, new ExodarGuard);	        // Shield of Velen
	mgr->register_gossip_script(3296, new OrgrimmarGuard);	        // Orgrimmar Grunt
	mgr->register_gossip_script(3084, new ThunderbluffGuard);		// Bluffwatcher
	mgr->register_gossip_script(3222, new BloodhoofGuard);			// Brave Wildrunner
	mgr->register_gossip_script(3224, new BloodhoofGuard);			// Brave Cloudmane
	mgr->register_gossip_script(3220, new BloodhoofGuard);			// Brave Darksky
	mgr->register_gossip_script(3219, new BloodhoofGuard);			// Brave Leaping Deer
	mgr->register_gossip_script(3217, new BloodhoofGuard);			// Brave Dawneagle
	mgr->register_gossip_script(3215, new BloodhoofGuard);			// Brave Strongbash
	mgr->register_gossip_script(3218, new BloodhoofGuard);			// Brave Swiftwind
	mgr->register_gossip_script(3221, new BloodhoofGuard);			// Brave Rockhorn
	mgr->register_gossip_script(3223, new BloodhoofGuard);			// Brave Rainchaser
	mgr->register_gossip_script(3212, new BloodhoofGuard);			// Brave Ironhorn
	mgr->register_gossip_script(5953, new RazorHillGuard);			// Razor Hill Grunt
	mgr->register_gossip_script(5725, new BrillGuard);				// Deathguard Lundmark
	mgr->register_gossip_script(1738, new BrillGuard);				// Deathguard Terrence
	mgr->register_gossip_script(1652, new BrillGuard);				// Deathguard Burgess
	mgr->register_gossip_script(1746, new BrillGuard);				// Deathguard Cyrus
	mgr->register_gossip_script(1745, new BrillGuard);				// Deathguard Morris
	mgr->register_gossip_script(1743, new BrillGuard);				// Deathguard Lawrence
	mgr->register_gossip_script(1744, new BrillGuard);				// Deathguard Mort
	mgr->register_gossip_script(1496, new BrillGuard);				// Deathguard Dillinger
	mgr->register_gossip_script(1742, new BrillGuard);				// Deathguard Bartholomew
	mgr->register_gossip_script(5595, new IronforgeGuard);			// Ironforge Guard
	mgr->register_gossip_script(727,  new KharanosGuard);			// Ironforge Mountaineer
	mgr->register_gossip_script(16221, new FalconwingGuard);		// Silvermoon Guardian
	mgr->register_gossip_script(18038, new AzureWatchGuard);		// Azuremyst Peacekeeper
	mgr->register_gossip_script(19687, new ShattrathGuard);			// Shattrath City Guard -by AeThIs
	mgr->register_gossip_script(18568, new ShattrathGuard);			// Shattrath City Guard Aruspice -by AeThIs
	mgr->register_gossip_script(18549, new ShattrathGuard);			// Shattrath City Guard -by AeThIs

	//Dalaran guards
	Arcemu::Gossip::Script * DalaranGossip = new DalaranGuard;
	mgr->register_creature_gossip(32675, DalaranGossip);
	mgr->register_creature_gossip(32676, DalaranGossip);
	mgr->register_creature_gossip(32677, DalaranGossip);
	mgr->register_creature_gossip(32678, DalaranGossip);
	mgr->register_creature_gossip(32679, DalaranGossip);
	mgr->register_creature_gossip(32680, DalaranGossip);
	mgr->register_creature_gossip(32681, DalaranGossip);
	mgr->register_creature_gossip(32683, DalaranGossip);
	mgr->register_creature_gossip(32684, DalaranGossip);
	mgr->register_creature_gossip(32685, DalaranGossip);
	mgr->register_creature_gossip(32686, DalaranGossip);
	mgr->register_creature_gossip(32687, DalaranGossip);
	mgr->register_creature_gossip(32688, DalaranGossip);
	mgr->register_creature_gossip(32689, DalaranGossip);
	mgr->register_creature_gossip(32690, DalaranGossip);
	mgr->register_creature_gossip(32691, DalaranGossip);
	mgr->register_creature_gossip(32692, DalaranGossip);
	mgr->register_creature_gossip(32693, DalaranGossip);
}
