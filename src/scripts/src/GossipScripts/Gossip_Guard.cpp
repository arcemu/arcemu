/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008-2009 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
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

#include "StdAfx.h"
#include "Setup.h"

#ifdef WIN32
#pragma warning(disable:4305)		// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif

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

class SCRIPT_DECL StormwindGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);

        Menu->AddItem(0, "Auction House", 1);
        Menu->AddItem(0, "Bank of Stormwind", 2);
		Menu->AddItem(0, "Stormwind Harbor", 35);
        Menu->AddItem(0, "Deeprun Tram", 3);
        Menu->AddItem(0, "The Inn", 4);
        Menu->AddItem(0, "Gryphon Master", 5);    
        Menu->AddItem(0, "Guild Master", 6);
        Menu->AddItem(0, "Mailbox", 7);
        Menu->AddItem(0, "Stable Master", 8);
        Menu->AddItem(0, "Weapons Trainer", 9);
        Menu->AddItem(0, "Officers' Lounge", 10);
        Menu->AddItem(0, "Battlemaster", 11);
		Menu->AddItem(0, "Barber", 34);
        Menu->AddItem(0, "Class Trainer", 12);
        Menu->AddItem(0, "Profession Trainer", 13);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
            SendQuickMenu(3834);
            Plr->Gossip_SendPOI(-8811.46, 667.46, 7, 7, 0, "Stormwind Auction House");
            break;

        case 2:     // Bank of Stormwind
            SendQuickMenu(764);
            Plr->Gossip_SendPOI(-8916.87, 622.87, 7, 7, 0, "Stormwind Bank");
            break;

        case 3:     // Deeprun Tram
            SendQuickMenu(3813);
            Plr->Gossip_SendPOI(-8378.88, 554.23, 7, 7, 0, "The Deeprun Tram");
            break;

        case 4:     // The Inn
            SendQuickMenu(3860);
            Plr->Gossip_SendPOI(-8869.0, 675.4, 7, 7, 0, "The Gilded Rose");
            break;

        case 5:     // Gryphon Master
            SendQuickMenu(879);
            Plr->Gossip_SendPOI(-8837.0, 493.5, 7, 7, 0, "Stormwind Gryphon Master");
            break;

        case 6:     // Guild Master
            SendQuickMenu(882);
            Plr->Gossip_SendPOI(-8894.0, 611.2, 7, 7, 0, "Stormwind Vistor`s Center");
            break;

        case 7:     // Mailbox
            SendQuickMenu(3518);
            Plr->Gossip_SendPOI(-8876.48, 649.18, 7, 7, 0, "Stormwind Mailbox");
            break;

        case 8:     // Stable Master
            SendQuickMenu(5984);
            Plr->Gossip_SendPOI(-8433.0, 554.7, 7, 7, 0, "Jenova Stoneshield");
            break;

        case 9:     // Weapons Master
            SendQuickMenu(4516);
            Plr->Gossip_SendPOI(-8797.0, 612.8, 7, 7, 0, "Woo Ping");
            break;

        case 10:    // Officers' Lounge
            SendQuickMenu(7047);
            Plr->Gossip_SendPOI(-8759.92, 399.69, 7, 7, 0, "Champions` Hall");
            break;

        case 11:    // Battlemaster
            {
			SendQuickMenu(7499);
            Plr->Gossip_SendPOI(-8393.62, 274.21, 7, 7, 0, "Battlemasters Stormwind");
            }break;

        case 12:    // Class Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
				Menu->AddItem( 0, "Druid"      , 14);
                Menu->AddItem( 0, "Hunter"     , 15);
                Menu->AddItem( 0, "Mage"       , 16);
                Menu->AddItem( 0, "Paladin"    , 17);
                Menu->AddItem( 0, "Priest"     , 18);
                Menu->AddItem( 0, "Rogue"      , 19);
                Menu->AddItem( 0, "Shaman"     , 20);
                Menu->AddItem( 0, "Warlock"    , 21);
				Menu->AddItem( 0, "Warrior"    , 22);
                Menu->SendTo(Plr);
            }break;

        case 13:    // Profession Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
                Menu->AddItem( 0, "Alchemy"           , 23);
                Menu->AddItem( 0, "Blacksmithing"     , 24);
                Menu->AddItem( 0, "Cooking"           , 25);
                Menu->AddItem( 0, "Enchanting"        , 26);
                Menu->AddItem( 0, "Engineering"       , 27);
                Menu->AddItem( 0, "First Aid"         , 28);
                Menu->AddItem( 0, "Fishing"           , 29);
                Menu->AddItem( 0, "Herbalism"         , 30);
                Menu->AddItem( 0, "Leatherworking"    , 31);
                Menu->AddItem( 0, "Mining"            , 32);
                Menu->AddItem( 0, "Skinning"          , 33);
                Menu->AddItem( 0, "Tailoring"         , 34);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////

        case 14: //Druid
            {
                Plr->Gossip_SendPOI(-8751.0, 1124.5, 7, 7, 0, "The Park");
                SendQuickMenu(902);
            }break;

        case 15: //Hunter
            {
                Plr->Gossip_SendPOI(-8413.0, 541.5, 7, 7, 0, "Hunter Lodge");
                SendQuickMenu(905);
            }break;

        case 16: //Mage
            {
                Plr->Gossip_SendPOI(-9012.0, 867.6, 7, 7, 0, "Wizard`s Sanctum");
                SendQuickMenu(899);
            }break;

        case 17: //Paladin
            {
                Plr->Gossip_SendPOI(-8577.0, 881.7, 7, 7, 0, "Cathedral Of Light");
                SendQuickMenu(904);
            }break;

        case 18: //Priest
            {
                Plr->Gossip_SendPOI(-8512.0, 862.4, 7, 7, 0, "Cathedral Of Light");
                SendQuickMenu(903);
            }break;

        case 19: //Rogue
            {
                Plr->Gossip_SendPOI(-8753.0, 367.8, 7, 7, 0, "Stormwind - Rogue House");
                SendQuickMenu(900);
            }break;

        case 20: //Shaman
            {
				Plr->Gossip_SendPOI(-9031.54, 549.87, 7, 7, 0, "Farseer Umbrua");
                SendQuickMenu(10106);
            }break;

		case 21: //Warlock
            {
                Plr->Gossip_SendPOI(-8948.91, 998.35, 7, 7, 0, "The Slaughtered Lamb");
                SendQuickMenu(906);
            }break;

        case 22: //Warrior
            {
                Plr->Gossip_SendPOI(-8714.14, 334.96, 7, 7, 0, "Stormwind Barracks");
                SendQuickMenu(901);
            }break;

        case 23: //Alchemy
            {
                Plr->Gossip_SendPOI(-8988.0, 759.60, 7, 7, 0, "Alchemy Needs");
                SendQuickMenu(919);
            }break;

        case 24: //Blacksmithing
            {
                Plr->Gossip_SendPOI(-8424.0, 616.9, 7, 7, 0, "Therum Deepforge");
                SendQuickMenu(920);
            }break;

        case 25: //Cooking
            {
                Plr->Gossip_SendPOI(-8611.0, 364.6, 7, 7, 0, "Pig and Whistle Tavern");
                SendQuickMenu(921);
            }break;

        case 26: //Enchanting
            {
                Plr->Gossip_SendPOI(-8858.0, 803.7, 7, 7, 0, "Lucan Cordell");
                SendQuickMenu(941);
            }break;

        case 27: //Engineering
            {
                Plr->Gossip_SendPOI(-8347.0, 644.1, 7, 7, 0, "Lilliam Sparkspindle");
                SendQuickMenu(922);
            }break;

        case 28: //First Aid
            {
                Plr->Gossip_SendPOI(-8513.0, 801.8, 7, 7, 0, "Shaina Fuller");
                SendQuickMenu(923);
            }break;

        case 29: //Fishing
            {
                Plr->Gossip_SendPOI(-8803.0, 767.5, 7, 7, 0, "Arnold Leland");
                SendQuickMenu(940);
            }break;

        case 30: //Herbalism
            {
                Plr->Gossip_SendPOI(-8967.0, 779.5, 7, 7, 0, "Alchemy Needs");
                SendQuickMenu(924);
            }break;

        case 31: //Leatherworking
            {
                Plr->Gossip_SendPOI(-8726.0, 477.4, 7, 7, 0, "The Protective Hide");
                SendQuickMenu(925);
            }break;

        case 32: //Mining
            {
                Plr->Gossip_SendPOI(-8434.0, 692.8, 7, 7, 0, "Gelman Stonehand");
                SendQuickMenu(927);
            }break;

        case 33: //Skinning
            {
                Plr->Gossip_SendPOI(-8716.0, 469.4, 7, 7, 0, "The Protective Hide");
                SendQuickMenu(928);
            }break;

        case 34: //Tailoring
            {
                Plr->Gossip_SendPOI(-8938.0, 800.7, 7, 7, 0, "Duncan`s Textiles");
                SendQuickMenu(929);
            }break;
        }
    }
};
// Darnassus Guard
class SCRIPT_DECL DarnassusGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3016, Plr);

        Menu->AddItem( 0, "Auction House"      , 1);
        Menu->AddItem( 0, "The Bank"           , 2);
        Menu->AddItem( 0, "Hippogryph Master"  , 3);
        Menu->AddItem( 0, "Guild Master"       , 4);
        Menu->AddItem( 0, "The Inn"            , 5);
        Menu->AddItem( 0, "Mailbox"            , 6);
        Menu->AddItem( 0, "Stable Master"      , 7);
        Menu->AddItem( 0, "Weapons Trainer"    , 8);
        Menu->AddItem( 0, "Battlemaster"       , 9);
        Menu->AddItem( 0, "Class Trainer"      , 10);
	 	Menu->AddItem( 0, "Profession Trainer" , 11);
		Menu->AddItem( 0, "Lexicon of Power"   , 38);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers (Most/All 'borrowed' from scriptdev)
            /////

        case 1:     // Auction House
			SendQuickMenu(3833);
			Plr->Gossip_SendPOI(9861.23, 2334.55, 7, 7, 0, "Darnassus Auction House");
			break;
			
		case 2:		// The Bank
			SendQuickMenu(3017);
			Plr->Gossip_SendPOI(9938.45, 2512.35, 7, 7, 0, "Darnassus Bank");
			break;
		
		case 3:		// Hippogryph Master
			SendQuickMenu(3018);
			Plr->Gossip_SendPOI(9945.65, 2618.94, 7, 7, 0, "Rut'theran Village");
			break;
			
		case 4:		// Guild Master
			SendQuickMenu(3019);
			Plr->Gossip_SendPOI(10076.40, 2199.59, 7, 7, 0, "Darnassus Guild Master");
			break;
			
		case 5:		// The Inn
			SendQuickMenu(3020);
			Plr->Gossip_SendPOI(10133.29, 2222.52, 7, 7, 0, "Darnassus Inn");
			break;
			
		case 6:		// Mailbox
			SendQuickMenu(3021);
			Plr->Gossip_SendPOI(9942.17, 2495.48, 7, 7, 0, "Darnassus Mailbox");
			break;
		
		case 7:		// Stable Master
			SendQuickMenu(5980);
			Plr->Gossip_SendPOI(10167.20, 2522.66, 7, 7, 0, "Alassin");
			break;
			
		case 8:		// Weapons Trainer
			SendQuickMenu(4517);
			Plr->Gossip_SendPOI(9907.11, 2329.70, 7, 7, 0, "Ilyenia ArcScriptfire");
			break;
			
		case 9:    // Battlemaster
            {
				SendQuickMenu(7519);
				Plr->Gossip_SendPOI(9981.9, 2325.9, 7, 7, 0, "Battlemasters Darnassus");
            }break;
			
		case 10:    // Class Trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
                Menu->AddItem( 0, "Druid"       , 12);
                Menu->AddItem( 0, "Hunter"      , 13);
                Menu->AddItem( 0, "Priest"      , 14);
                Menu->AddItem( 0, "Rogue"       , 15);
                Menu->AddItem( 0, "Warrior"     , 16);
                Menu->SendTo(Plr);
            }break;

        case 11:    // Profession Trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
                Menu->AddItem( 0, "Alchemy"           , 17);
                Menu->AddItem( 0, "Cooking"           , 18);
                Menu->AddItem( 0, "Enchanting"        , 19);
                Menu->AddItem( 0, "First Aid"         , 20);
                Menu->AddItem( 0, "Fishing"           , 21);
                Menu->AddItem( 0, "Herbalism"         , 22);
                Menu->AddItem( 0, "Leatherworking"    , 23);
                Menu->AddItem( 0, "Skinning"          , 24);
                Menu->AddItem( 0, "Tailoring"         , 25);
                Menu->SendTo(Plr);
            }break;
		
		case 12:	// Druid
            {
                Plr->Gossip_SendPOI(10186, 2570.46, 7, 7, 0, "Darnassus Druid Trainer");
                SendQuickMenu(3024);
            }break;

		case 13:	// Hunter
            {
                Plr->Gossip_SendPOI(10177.29, 2511.10, 7, 7, 0, "Darnassus Hunter Trainer");
                SendQuickMenu(3023);
            }break;

        case 14:	// Priest
            {
                Plr->Gossip_SendPOI(9659.12, 2524.88, 7, 7, 0, "Temple of the ArcScript");
                SendQuickMenu(3025);
            }break;
	
        case 15:	// Rogue
            {
                Plr->Gossip_SendPOI(10122, 2599.12, 7, 7, 0, "Darnassus Rogue Trainer");
                SendQuickMenu(3026);
            }break;

        case 16:	// Warrior
            {
                Plr->Gossip_SendPOI(9951.91, 2280.38, 7, 7, 0, "Warrior's Terrace");
                SendQuickMenu(3033);
            }break;
			
		case 17: //Alchemy
            {
                Plr->Gossip_SendPOI(10075.90, 2356.76, 7, 7, 0, "Darnassus Alchemy Trainer");
                SendQuickMenu(3035);
            }break;

        case 18: //Cooking
            {
                Plr->Gossip_SendPOI(10088.59, 2419.21, 7, 7, 0, "Darnassus Cooking Trainer");
                SendQuickMenu(3036);
            }break;
			
        case 19: //Enchanting
            {
                Plr->Gossip_SendPOI(10146.09, 2313.42, 7, 7, 0, "Darnassus Enchanting Trainer");
                SendQuickMenu(3337);
            }break;

        case 20: //First Aid
            {
                Plr->Gossip_SendPOI(10150.09, 2390.43, 7, 7, 0, "Darnassus First Aid Trainer");
                SendQuickMenu(3037);
            }break;

        case 21: //Fishing
            {
                Plr->Gossip_SendPOI(9836.20, 2432.17, 7, 7, 0, "Darnassus Fishing Trainer");
                SendQuickMenu(3038);
            }break;

        case 22: //Herbalism
            {
                Plr->Gossip_SendPOI(9757.17, 2430.16, 7, 7, 0, "Darnassus Herbalism Trainer");
                SendQuickMenu(3039);
            }break;

        case 23: //Leatherworking
            {
                Plr->Gossip_SendPOI(10086.59, 2255.77, 7, 7, 0, "Darnassus Leatherworking Trainer");
                SendQuickMenu(3040);
            }break;

        case 24: //Skinning
            {
                Plr->Gossip_SendPOI(10081.40, 2257.18, 7, 7, 0, "Darnassus Skinning Trainer");
                SendQuickMenu(3042);
            }break;

        case 25: //Tailoring
            {
                Plr->Gossip_SendPOI(10079.70, 2268.19, 7, 7, 0, "Darnassus Tailor");
                SendQuickMenu(3044);
            }break;


		}
	}
    void GossipEnd(Object* pObject, Player* Plr)
    {

    }
};

class SCRIPT_DECL GoldshireGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
		Menu->AddItem( 0, "Bank"                 , 1);
		Menu->AddItem( 0, "Gryphon Master"       , 2);
		Menu->AddItem( 0, "Guild Master"         , 3);
		Menu->AddItem( 0, "Inn"                  , 4);
		Menu->AddItem( 0, "Stable Master"        , 5);
		Menu->AddItem( 0, "Class Trainer"        , 6);
		Menu->AddItem( 0, "Profession Trainer"   , 7);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

	void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {	
	
		case 0:     // Return to start
			GossipHello(pObject, Plr, true);
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
	       Plr->Gossip_SendPOI(-9459.34, 42.08, 99, 6, 0, "Lion's Pride Inn");
	      break;

	   case 5:     //Stable Master
	       SendQuickMenu(5983);
	       Plr->Gossip_SendPOI(-9466.62, 45.87, 99, 6, 0, "Erma");
	        break;

	   case 6:     //Class Trainer
		   {
		   objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);

	       Menu->AddItem( 0, "Druid",                8);
	       Menu->AddItem( 0, "Hunter",               9);
	       Menu->AddItem( 0, "Mage",                 10);
	       Menu->AddItem( 0, "Paladin",              11);
	       Menu->AddItem( 0, "Priest",               12);
	       Menu->AddItem( 0, "Rogue",                13);
		   Menu->AddItem( 0, "Shaman",               14);
	       Menu->AddItem( 0, "Warlock",              15);
		   Menu->AddItem( 0, "Warrior",              16);

			Menu->SendTo(Plr);
		   }break;

		case 7:		//Profession Trainer
			{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);

			Menu->AddItem( 0, "Alchemy"              ,17);
			Menu->AddItem( 0, "Blacksmithing"        ,18);
			Menu->AddItem( 0, "Cooking"              ,19);
			Menu->AddItem( 0, "Enchanting"           ,20);
			Menu->AddItem( 0, "Engineering"          ,21);
			Menu->AddItem( 0, "First Aid"            ,22);
			Menu->AddItem( 0, "Fishing"              ,23);
			Menu->AddItem( 0, "Herbalism"            ,24);
			Menu->AddItem( 0, "Leatherworking"       ,25);
			Menu->AddItem( 0, "Mining"               ,26);
			Menu->AddItem( 0, "Skinning"             ,27);
			Menu->AddItem( 0, "Tailoring"            ,28);
			Menu->SendTo(Plr);
			}break;

		case 8: //Druid
            {
                SendQuickMenu(4265);
            }break;

		case 9: //Hunter
            {
                SendQuickMenu(4266);
            }break;

		case 10: //Mage
            {
                Plr->Gossip_SendPOI(-9471.12, 33.44, 7, 7, 0, "Zaldimar Wefhellt");
                SendQuickMenu(4268);
            }break;

		case 11: //Paladin
            {
                Plr->Gossip_SendPOI(-9469, 108.05, 7, 7, 0, "Brother Wilhelm");
                SendQuickMenu(4269);
            }break;

		case 12: //Priest
            {
                Plr->Gossip_SendPOI(-9461.07, 32.6, 7, 7, 0, "Priestess Josetta");
                SendQuickMenu(4267);
            }break;

		case 13: //Rogue
            {
                Plr->Gossip_SendPOI(-9465.13, 13.29, 7, 7, 0, "Keryn Sylvius");
                SendQuickMenu(4270);
            }break;

		case 14: //Shaman <-- TO DO!
            {
                Plr->Gossip_SendPOI(1781, 53, 7, 7, 0, "Undercity Mage Trainers");
                SendQuickMenu(3513);
            }break;

		case 15: //Warlock
            {
                Plr->Gossip_SendPOI(-9473.21, -4.08, 7, 7, 0, "Maximillian Crowe");
                SendQuickMenu(4272);
            }break;

		case 16: //Warrior
            {
                Plr->Gossip_SendPOI(-9461.82, 109.50, 7, 7, 0, "Lyria Du Lac");
                SendQuickMenu(4271);
            }break;

		case 17: //Alchemy
            {
                Plr->Gossip_SendPOI(-9057.04, 153.63, 7, 7, 0, "Alchemist Mallory");
                SendQuickMenu(4274);
            }break;

		case 18: //Blacksmithing
            {
                Plr->Gossip_SendPOI(-9456.58, 87.90, 7, 7, 0, "Smith Argus");
                SendQuickMenu(4275);
            }break;

		case 19: //Cooking
            {
                Plr->Gossip_SendPOI(-9467.54, -3.16, 7, 7, 0, "Tomas");
                SendQuickMenu(4276);
            }break;

		case 20: //Enchanting
            {
                SendQuickMenu(4277);
            }break;

		case 21: //Engineering
            {
                SendQuickMenu(4278);
            }break;

		case 22: //First Aid
            {
                Plr->Gossip_SendPOI(-9456.82, 30.49, 7, 7, 0, "Michelle Belle");
                SendQuickMenu(4279);
            }break;

		case 23: //Fishing
            {
                Plr->Gossip_SendPOI(-9386.54, -118.73, 7, 7, 0, "Lee Brown");
                SendQuickMenu(4280);
            }break;

		case 24: //Herbalism
            {
                Plr->Gossip_SendPOI(-9060.70, 149.23, 7, 7, 0, "Herbalist Pomeroy");
                SendQuickMenu(4281);        
            }break;

		case 25: //Leatherworking
            {
                Plr->Gossip_SendPOI(-9376.12, -75.23, 7, 7, 0, "Adele Fielder");
                SendQuickMenu(4282);
            }break;

		case 26: //Mining
            {
                SendQuickMenu(4283);
            }break;

		case 27: //Skinning
            {
                Plr->Gossip_SendPOI(-9536.91, -1212.76, 7, 7, 0, "Helene Peltskinner");
                SendQuickMenu(4284);
            }break;

		case 28: //Tailoring
            {
                Plr->Gossip_SendPOI(-9376.12, -75.23, 7, 7, 0, "Eldrin");
                SendQuickMenu(4285);
            }break;
		}
    }   
};

class SCRIPT_DECL UndercityGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
		
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
		Menu->AddItem(0, "Barber", 41);
		Menu->AddItem(0, "A class trainer", 11);
		Menu->AddItem(0, "A profession trainer", 12);
		Menu->AddItem(0, "Lexicon of Power", 42);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////
			
		case 1:     // The bank
            SendQuickMenu(3514);
            Plr->Gossip_SendPOI(1595.64, 232.45, 7, 7, 0, "Undercity Bank");
            break;

        case 2:     // The bat handler
            SendQuickMenu(3515);
            Plr->Gossip_SendPOI(1565.9, 271.43, 7, 7, 0, "Undercity Bat Handler");
            break;

        case 3:     // The guild master
            SendQuickMenu(3516);
            Plr->Gossip_SendPOI(1594.17, 205.57, 7, 7, 0, "Undercity Guild Master");
            break;

        case 4:     // The inn
            SendQuickMenu(3517);
            Plr->Gossip_SendPOI(1639.43, 220.99, 7, 7, 0, "Undercity Inn");
            break;

        case 5:     // The mailbox
            SendQuickMenu(3518);
            Plr->Gossip_SendPOI(1632.68, 219.4, 7, 7, 0, "Undercity Mailbox");
            break;

        case 6:     // The auction house
            SendQuickMenu(3520);
            Plr->Gossip_SendPOI(1647.9, 258.49, 7, 7, 0, "Undercity Auction House");
            break;

        case 7:     // The zeppelin master
            SendQuickMenu(3519);
            Plr->Gossip_SendPOI(2059, 274.86, 7, 7, 0, "Undercity Zeppelin");
            break;

        case 8:     // The weapon master
            SendQuickMenu(4521);
            Plr->Gossip_SendPOI(1670.31, 324.66, 7, 7, 0, "Archibald");
            break;

        case 9:     // The stable master
            SendQuickMenu(5979);
            Plr->Gossip_SendPOI(1634.18, 226.76, 7, 7, 0, "Anya Maulray");
            break;

        case 10:    // The battlemaster
            {
				SendQuickMenu(7527);
				Plr->Gossip_SendPOI(1300.33, 350.92, 7, 7, 0, "Battlemasters Undercity");
            }break;

        case 11:    // A class trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3542, Plr);
                Menu->AddItem( 0, "Mage"		 , 13);
                Menu->AddItem( 0, "Paladin"      , 14);
                Menu->AddItem( 0, "Priest"       , 15);
                Menu->AddItem( 0, "Rogue"        , 16);
                Menu->AddItem( 0, "Warlock"      , 17);
                Menu->AddItem( 0, "Warrior"      , 18);
                Menu->SendTo(Plr);
            }break;

        case 12:    // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
                Menu->AddItem( 0, "Alchemy"           , 19);
                Menu->AddItem( 0, "Blacksmithing"     , 20);
                Menu->AddItem( 0, "Cooking"           , 21);
                Menu->AddItem( 0, "Enchanting"        , 22);
                Menu->AddItem( 0, "Engineering"       , 23);
                Menu->AddItem( 0, "First Aid"         , 24);
                Menu->AddItem( 0, "Fishing"           , 25);
                Menu->AddItem( 0, "Herbalism"         , 26);
                Menu->AddItem( 0, "Leatherworking"    , 27);
                Menu->AddItem( 0, "Mining"            , 28);
                Menu->AddItem( 0, "Skinning"          , 29);
                Menu->AddItem( 0, "Tailoring"         , 30);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////

        case 13: //Mage
            {
                Plr->Gossip_SendPOI(1781, 53, 7, 7, 0, "Undercity Mage Trainers");
                SendQuickMenu(3513);
            }break;

        case 14: //Paladin
            {
                Plr->Gossip_SendPOI(1298.98, 316.51, 7, 7, 0, "Champion Cyssa Downrose");
                SendQuickMenu(3521);
            }break;

        case 15: //Priest
            {
                Plr->Gossip_SendPOI(1758.33, 401.5, 7, 7, 0, "Undercity Priest Trainers");
                SendQuickMenu(3521);
            }break;

        case 16: //Rogue
            {
                Plr->Gossip_SendPOI(1418.56, 65, 7, 7, 0, "Undercity Rogue Trainers");
                SendQuickMenu(3526);
            }break;

        case 17: //Warlock
            {
                Plr->Gossip_SendPOI(1780.92, 53.16, 7, 7, 0, "Undercity Warlock Trainers");
                SendQuickMenu(3526);
            }break;

        case 18: //Warrior
            {
                Plr->Gossip_SendPOI(1775.59, 418.19, 7, 7, 0, "Undercity Warrior Trainers");
                SendQuickMenu(3527);
            }break;

        case 19: //Alchemy
            {
                Plr->Gossip_SendPOI(1419.82, 417.19, 7, 7, 0, "The Apothecarium");
                SendQuickMenu(3528);
            }break;

        case 20: //Blacksmithing
            {
                Plr->Gossip_SendPOI(1696, 285, 7, 7, 0, "Undercity Blacksmithing Trainer");
                SendQuickMenu(3529);
            }break;

        case 21: //Cooking
            {
                Plr->Gossip_SendPOI(1596.34, 274.68, 7, 7, 0, "Undercity Cooking Trainer");
                SendQuickMenu(3530);
            }break;

        case 22: //Enchanting
            {
                Plr->Gossip_SendPOI(1488.54, 280.19, 7, 7, 0, "Undercity Enchanting Trainer");
                SendQuickMenu(3531);
            }break;

        case 23: //Engineering
            {
                Plr->Gossip_SendPOI(1408.58, 143.43, 7, 7, 0, "Undercity Engineering Trainer");
                SendQuickMenu(3532);
            }break;

        case 24: //First Aid
            {
                Plr->Gossip_SendPOI(1519.65, 167.19, 7, 7, 0, "Undercity First Aid Trainer");
                SendQuickMenu(3533);
            }break;

        case 25: //Fishing
            {
                Plr->Gossip_SendPOI(1679.9, 89, 7, 7, 0, "Undercity Fishing Trainer");
                SendQuickMenu(3534);
            }break;

        case 26: //Herbalism
            {
                Plr->Gossip_SendPOI(1558, 349.36, 7, 7, 0, "Undercity Herbalism Trainer");
                SendQuickMenu(3535);
            }break;

        case 27: //Leatherworking
            {
                Plr->Gossip_SendPOI(1498.76, 196.43, 7, 7, 0, "Undercity Leatherworking Trainer");
                SendQuickMenu(3536);
            }break;

        case 28: //Mining
            {
                Plr->Gossip_SendPOI(1642.88, 335.58, 7, 7, 0, "Undercity Mining Trainer");
                SendQuickMenu(3537);
            }break;

        case 29: //Skinning
            {
                Plr->Gossip_SendPOI(1498.6, 196.46, 7, 7, 0, "Undercity Skinning Trainer");
                SendQuickMenu(3538);
            }break;

        case 30: //Tailoring
            {
                Plr->Gossip_SendPOI(1689.55, 193, 7, 7, 0, "Undercity Tailoring Trainer");
                SendQuickMenu(3539);
            }break;
        }
    }
};

class SCRIPT_DECL DolanaarGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4316, Plr);

        Menu->AddItem(0, "The Bank", 1);
		Menu->AddItem(0, "Rut'Theran Ferry", 2);
		Menu->AddItem(0, "The Guild Master", 3);
		Menu->AddItem(0, "The Inn", 4);
		Menu->AddItem(0, "Stable Master", 5);
		Menu->AddItem(0, "Class Trainer", 6);
		Menu->AddItem(0, "Profession Trainer", 7);

        if(AutoSend)
            Menu->SendTo(Plr);
    }
	
    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
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
			Plr->Gossip_SendPOI(9821.49, 960.13, 7, 7, 0, "Dolanaar Inn");
			SendQuickMenu(4320);
			break;
			
		case 5:		// Stable Master
			Plr->Gossip_SendPOI(9808.37, 931.1, 7, 7, 0, "Seriadne");
			SendQuickMenu(5982);
			break;

        case 6:    // Class Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4264, Plr);
                Menu->AddItem( 0, "Druid"       , 8);
                Menu->AddItem( 0, "Hunter"      , 9);
                Menu->AddItem( 0, "Priest"     , 10);
                Menu->AddItem( 0, "Rogue"      , 11);
                Menu->AddItem( 0, "Warrior"    , 12);
                Menu->SendTo(Plr);
            }break;

        case 7:    // Profession Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4273, Plr);
                Menu->AddItem( 0, "Alchemy"           , 13);
                Menu->AddItem( 0, "Cooking"           , 14);
                Menu->AddItem( 0, "Enchanting"        , 15);
                Menu->AddItem( 0, "First Aid"         , 16);
                Menu->AddItem( 0, "Fishing"           , 17);
                Menu->AddItem( 0, "Herbalism"         , 18);
                Menu->AddItem( 0, "Leatherworking"    , 19);
                Menu->AddItem( 0, "Skinning"          , 20);
                Menu->AddItem( 0, "Tailoring"         , 21);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////

        case 8: //Druid
            {
                Plr->Gossip_SendPOI(9741.58, 963.7, 7, 7, 0, "Kal");
                SendQuickMenu(4323);
            }break;
		
		case 9: // Hunter
			{
				Plr->Gossip_SendPOI(9815.12, 926.28, 7, 7, 0, "Dazalar");
				SendQuickMenu(4324);
			}break;
		
		case 10: // Priest
			{
				Plr->Gossip_SendPOI(9906.16, 986.63, 7, 7, 0, "Laurna Morninglight");
				SendQuickMenu(4325);
			}break;

		case 11: // Rogue
			{
				Plr->Gossip_SendPOI(9789, 942.86, 7, 7, 0, "Jannok Breezesong");
				SendQuickMenu(4327);
			}break;
			
		case 12: // Warrior
			{
				Plr->Gossip_SendPOI(9821.96, 950.61, 7, 7, 0, "Kyra Windblade");
				SendQuickMenu(4327);
			}break;

        case 13: // Alchemy
            {
                Plr->Gossip_SendPOI(9767.59, 878.81, 7, 7, 0, "Cyndra Kindwhisper");
                SendQuickMenu(4329);
            }break;
		
		case 14: // Cooking
			{
				Plr->Gossip_SendPOI(9751.19, 906.13, 7, 7, 0, "Zarrin");
				SendQuickMenu(4330);
			}break;
		
		case 15: // Enchanting
			{
				Plr->Gossip_SendPOI(10677.59, 1946.56, 7, 7, 0, "Alanna Raveneye");
				SendQuickMenu(4331);
			}break;
			
		case 16: // First Aid
			{
				Plr->Gossip_SendPOI(9903.12, 999, 7, 7, 0, "Byancie");
				SendQuickMenu(4332);
			}break;
			
		case 17: // Fishing
			{
				SendQuickMenu(4333);
			}break;
			
		case 18: // Herbalism
			{
				Plr->Gossip_SendPOI(9773.78, 875.88, 7, 7, 0, "Malorne Bladeleaf");
				SendQuickMenu(4334);
			}break;
			
		case 19: // Leatherworking
			{
				Plr->Gossip_SendPOI(10152.59, 1681.46, 7, 7, 0, "Nadyia Maneweaver");
				SendQuickMenu(4335);
			}break;
			
		case 20: // Skinning
			{
				Plr->Gossip_SendPOI(10135.59, 1673.18, 7, 7, 0, "Radnaal Maneweaver");
				SendQuickMenu(4336);
			}break;
			
		case 21: // Tailoring
			{
				SendQuickMenu(4337);
			}break;
        }
    }
};

class SCRIPT_DECL SilvermoonGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);

        Menu->AddItem(0, "Auction House", 1);
        Menu->AddItem(0, "The Bank", 2);
		Menu->AddItem(0, "Bat Handler", 3);
        Menu->AddItem(0, "Guild Master", 4);
        Menu->AddItem(0, "The Inn", 5);
        Menu->AddItem(0, "Mailbox", 6);    
        Menu->AddItem(0, "Stable Master", 7);
        Menu->AddItem(0, "Weapon Master", 8);
        Menu->AddItem(0, "Battlemaster", 9);
        Menu->AddItem(0, "Class Trainer", 10);
        Menu->AddItem(0, "Profession Trainer", 11);
        Menu->AddItem(0, "Mana Loom", 12);

        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
            Menu->AddItem(0, "To the west."  , 13);
            Menu->AddItem(0, "To the east."    , 14);
            Menu->SendTo(Plr);
            break;

        case 2:     // The Bank
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9322, Plr);
            Menu->AddItem(0, "The west."  , 15);
            Menu->AddItem(0, "The east."    , 16);
            Menu->SendTo(Plr);
            break;

		case 3:     // Bat Handler
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(9378.45, -7163.94, 7, 7, 0, "Silvermoon City, Flight Master");
            break;

        case 4:     // Guild Master
            SendQuickMenu(9324);
            Plr->Gossip_SendPOI(9480.75, -7345.587, 7, 7, 0, "Silvermoon City, Guild Master");
            break;

        case 5:     // The Inn
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9325, Plr);
            Menu->AddItem(0, "The Silvermoon City Inn."  , 17);
            Menu->AddItem(0, "The Wayfarer's Rest tavern."    , 18);
            Menu->SendTo(Plr);
            break;

        case 6:     // Mailbox
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(9743.078, -7466.4, 7, 7, 0, "Silvermoon City, Mailbox");
            break;

        case 7:     // Stable Master
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(9904.95, -7404.31, 7, 7, 0, "Silvermoon City, Stable Master");
            break;

        case 8:     // Weapon Master
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(9841.17, -7505.13, 7, 7, 0, "Silvermoon City, Weapon Master");
            break;

	    case 9:     // Battlemasters
			SendQuickMenu(9316);
            Plr->Gossip_SendPOI(9850.74, -7563.84, 7, 7, 0, "Silvermoon City, Battlemasters");
            break;

        case 10:    // Class Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
                Menu->AddItem( 0, "Druid"       , 19);
                Menu->AddItem( 0, "Hunter"      , 20);
                Menu->AddItem( 0, "Mage"        , 21);
                Menu->AddItem( 0, "Paladin"     , 22);
                Menu->AddItem( 0, "Priest"      , 23);
                Menu->AddItem( 0, "Rogue"       , 24);
                Menu->AddItem( 0, "Warlock"     , 25);
                Menu->SendTo(Plr);
            }break;

        case 11:    // Profession Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9338, Plr);
                Menu->AddItem( 0, "Alchemy"           , 26);
                Menu->AddItem( 0, "Blacksmithing"     , 27);
                Menu->AddItem( 0, "Cooking"           , 28);
                Menu->AddItem( 0, "Enchanting"        , 29);
                Menu->AddItem( 0, "Engineering"       , 30);
                Menu->AddItem( 0, "First Aid"         , 31);
                Menu->AddItem( 0, "Fishing"           , 32);
				Menu->AddItem( 0, "Herbalism"         , 33);
				Menu->AddItem( 0, "Jewelcrafting"     , 34);
                Menu->AddItem( 0, "Leatherworking"    , 35);
                Menu->AddItem( 0, "Mining"            , 36);
                Menu->AddItem( 0, "Skinning"          , 37);
                Menu->AddItem( 0, "Tailoring"         , 38);
                Menu->SendTo(Plr);
            }break;

		case 12: //Mana Loom
            {
                Plr->Gossip_SendPOI(9751.013, -7074.85, 7, 7, 0, "Silvermoon City, Mana Loom");
                SendQuickMenu(9316);
            }break;

        case 13: //To the west - Auction House no. 1
            {
                Plr->Gossip_SendPOI(9649.429, -7134.027, 7, 7, 0, "Silvermoon City, Auction House");
                SendQuickMenu(9316);
            }break;

        case 14: //To the east - Auction House no. 2
            {
                Plr->Gossip_SendPOI(9682.864, -7515.786, 7, 7, 0, "Silvermoon City, Auction House");
                SendQuickMenu(9316);
            }break;

        case 15:     // The bank - The west.
            SendQuickMenu(9324);
            Plr->Gossip_SendPOI(9522.104, -7208.878, 7, 7, 0, "Silvermoon City, Bank");
            break;

        case 16:     // The bank - The east.
            SendQuickMenu(9324);
            Plr->Gossip_SendPOI(9791.07, -7488.041, 7, 7, 0, "Silvermoon City, Bank");
            break;

        case 17: //The Silvermoon City Inn
            {
                Plr->Gossip_SendPOI(9677.113, -7367.575, 7, 7, 0, "Silvermoon City, Inn");
                SendQuickMenu(9316);
            }break;

        case 18: //The Wayfarer's Rest tavern
            {
                Plr->Gossip_SendPOI(9562.813, -7218.63, 7, 7, 0, "Silvermoon City, Inn");
                SendQuickMenu(9316);
            }break;

        case 19: //Druid
            {
                Plr->Gossip_SendPOI(9700.55, -7262.57, 7, 7, 0, "Silvermoon City, Druid Trainer");
                SendQuickMenu(9316);
            }break;

        case 20: //Hunter
            {
                Plr->Gossip_SendPOI(9930.568, -7412.115, 7, 7, 0, "Silvermoon City, Hunter Trainer");
                SendQuickMenu(9316);
            }break;

        case 21: //Mage
            {
                Plr->Gossip_SendPOI(9996.914, -7104.803, 7, 7, 0, "Silvermoon City, Mage Trainer");
                SendQuickMenu(9316);
            }break;

        case 22: //Paladin
            {
                Plr->Gossip_SendPOI(9850.22, -7516.93, 7, 7, 0, "Silvermoon City, Paladin Trainer");
                SendQuickMenu(9316);
            }break;

        case 23: //Priest
            {
                Plr->Gossip_SendPOI(9935.37, -7131.14, 7, 7, 0, "Silvermoon City, Priest Trainer");
                SendQuickMenu(9316);
            }break;

        case 24: //Rogue
            {
                Plr->Gossip_SendPOI(9739.88, -7374.33, 7, 7, 0, "Silvermoon City, Rogue Trainer");
                SendQuickMenu(9316);
            }break;

        case 25: //Warlock
            {
                Plr->Gossip_SendPOI(9803.052, -7316.967, 7, 7, 0, "Silvermoon City, Warlock Trainer");
                SendQuickMenu(9316);
            }break;

        case 26: //Alchemy
            {
                Plr->Gossip_SendPOI(10000.9, -7216.63, 7, 7, 0, "Silvermoon City, Alchemy");
                SendQuickMenu(9316);
            }break;

        case 27: //Blacksmithing
            {
                Plr->Gossip_SendPOI(9841.43, -7361.53, 7, 7, 0, "Silvermoon City, Blacksmithing");
                SendQuickMenu(9340);
            }break;

        case 28: //Cooking
            {
                Plr->Gossip_SendPOI(9577.26, -7243.6, 7, 7, 0, "Silvermoon City, Cooking");
                SendQuickMenu(9316);
            }break;

        case 29: //Enchanting
            {
                Plr->Gossip_SendPOI(9962.57, -7246.18, 7, 7, 0, "Silvermoon City, Enchanting");
                SendQuickMenu(9341);
            }break;

        case 30: //Engineering
            {
                Plr->Gossip_SendPOI(9808.85, -7287.31, 7, 7, 0, "Silvermoon City, Engineering");
                SendQuickMenu(9316);
            }break;

        case 31: //First Aid
            {
                Plr->Gossip_SendPOI(9588.61, -7337.526, 7, 7, 0, "Silvermoon City, First Aid");
                SendQuickMenu(9316);
            }break;

        case 32: //Fishing
            {
                Plr->Gossip_SendPOI(9601.97, -7332.34, 7, 7, 0, "Silvermoon City, Fishing");
                SendQuickMenu(9316);
            }break;

        case 33: //Herbalism
            {
                Plr->Gossip_SendPOI(9996.96, -7208.39, 7, 7, 0, "Silvermoon City, Herbalism");
                SendQuickMenu(9316);
            }break;

        case 34: //Jewelcrafting
            {
                Plr->Gossip_SendPOI(9552.8, -7502.12, 7, 7, 0, "Silvermoon City, Jewelcrafting");
                SendQuickMenu(9346);
            }break;

        case 35: //Leatherworking
            {
                Plr->Gossip_SendPOI(9502.486, -7425.51, 7, 7, 0, "Silvermoon City, Leatherworking");
                SendQuickMenu(9347);
            }break;

        case 36: //Mining
            {
                Plr->Gossip_SendPOI(9813.73, -7360.19, 7, 7, 0, "Silvermoon City, Mining");
                SendQuickMenu(9348);
            }break;

        case 37: //Skinning
            {
                Plr->Gossip_SendPOI(9513.37, -7429.4, 7, 7, 0, "Silvermoon City, Skinning");
                SendQuickMenu(9316);
            }break;

		case 38: //Tailoring
            {
                Plr->Gossip_SendPOI(9727.56, -7086.65, 7, 7, 0, "Silvermoon City, Tailoring");
                SendQuickMenu(9350);
            }break;
        }
    }
};

class SCRIPT_DECL ExodarGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
        Menu->AddItem(0, "Auction House", 1);
        Menu->AddItem(0, "The Bank", 2);
        Menu->AddItem(0, "Hippogryph Master", 3);
		Menu->AddItem(0, "Guild Master", 4);
        Menu->AddItem(0, "The Inn", 5);    
        Menu->AddItem(0, "Mailbox", 6);
        Menu->AddItem(0, "Stable Master", 7);
        Menu->AddItem(0, "Weapon Master", 8);
        Menu->AddItem(0, "Battlemasters", 9);
        Menu->AddItem(0, "Class Trainer", 10);
        Menu->AddItem(0, "Profession Trainer", 11);
		Menu->AddItem(0, "Lexicon of Power", 43);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-4013.82, -11729.64, 7, 7, 0, "Exodar, Auctioneer");
            break;

        case 2:     // The Bank
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-3923.89, -11544.5, 7, 7, 0, "Exodar, bank");
            break;

        case 3:     // Hippogryph Master
            SendQuickMenu(9530);
            Plr->Gossip_SendPOI(-4058.45, -11789.7, 7, 7, 0, "Exodar, Hippogryph Master");
            break;

        case 4:     // Guild Master
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-4093.38, -11630.39, 7, 7, 0, "Exodar, Guild Master");
            break;

        case 5:     // The Inn
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-3765.34, -11695.8, 7, 7, 0, "Exodar, Inn");
            break;

        case 6:     // Mailbox
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-3913.75, -11606.83, 7, 7, 0, "Exodar, Mailbox");
            break;

        case 7:     // Stable Master
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-3787.01, -11702.7, 7, 7, 0, "Exodar, Stable Master");
            break;

        case 8:     // Weapon Master
            SendQuickMenu(9316);
            Plr->Gossip_SendPOI(-4215.68, -11628.9, 7, 7, 0, "Exodar, Weapon Master");
            break;

	    case 9:     // Battlemasters
            Plr->Gossip_SendPOI(-3999.82, -11368.33, 7, 7, 0, "Exodar, Battlemasters");
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
            Menu->AddItem(0, "Arena Battlemaster"  , 12);
            Menu->SendTo(Plr);
            break;

        case 10:    // Class Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
                Menu->AddItem( 0, "Druid"       , 13);
                Menu->AddItem( 0, "Hunter"      , 14);
                Menu->AddItem( 0, "Mage"        , 15);
                Menu->AddItem( 0, "Paladin"     , 16);
                Menu->AddItem( 0, "Priest"      , 17);
                Menu->AddItem( 0, "Shaman"      , 18);
                Menu->AddItem( 0, "Warrior"     , 19);
                Menu->SendTo(Plr);
            }break;

        case 11:    // Profession Trainers
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9316, Plr);
                Menu->AddItem( 0, "Alchemy"           , 20);
                Menu->AddItem( 0, "Blacksmithing"     , 21);
                Menu->AddItem( 0, "Enchanting"        , 22);
                Menu->AddItem( 0, "Engineering"       , 23);
                Menu->AddItem( 0, "First Aid"         , 24);
                Menu->AddItem( 0, "Fishing"           , 25);
                Menu->AddItem( 0, "Herbalism"         , 26);
				Menu->AddItem( 0, "Jewelcrafting"     , 27);
                Menu->AddItem( 0, "Leatherworking"    , 28);
                Menu->AddItem( 0, "Mining"            , 29);
                Menu->AddItem( 0, "Skinning"          , 30);
                Menu->AddItem( 0, "Tailoring"         , 31);
                Menu->AddItem( 0, "Cooking"           , 32);
                Menu->SendTo(Plr);
            }break;

            //////////////////
            // Battlemaster submenu
            ////////

		case 12://Arena Battlemaster Exodar
            {
                Plr->Gossip_SendPOI(-3725.25, -11688.3, 7, 7, 0, "Arena Battlemaster Exodar");
                SendQuickMenu(9316);
            }break;

        case 13: //Druid
            {
                Plr->Gossip_SendPOI(-4274.81, -11495.3, 7, 7, 0, "Exodar, Druid Trainer"); 
                SendQuickMenu(9316);
            }break;

        case 14: //Hunter
            {
                Plr->Gossip_SendPOI(-4229.36, -11563.36, 7, 7, 0, "Exodar, Hunter Trainers");
                SendQuickMenu(9316);
            }break;

        case 15: //Mage
            {
                Plr->Gossip_SendPOI(-4048.8, -11559.02, 7, 7, 0, "Exodar, Mage Trainers");
                SendQuickMenu(9316);
            }break;

        case 16: //Paladin
            {
                Plr->Gossip_SendPOI(-4176.57, -11476.46, 7, 7, 0, "Exodar, Paladin Trainers");
                SendQuickMenu(9316);
            }break;

        case 17: //Priest
            {
                Plr->Gossip_SendPOI(-3972.38, -11483.2, 7, 7, 0, "Exodar, Priest Trainers");
                SendQuickMenu(9316);
            }break;

        case 18: //Shaman
            {
                Plr->Gossip_SendPOI(-3843.8, -11390.75, 7, 7, 0, "Exodar, Shaman Trainer");
                SendQuickMenu(9556);
            }break;

        case 19: //Warrior
            {
                Plr->Gossip_SendPOI(-4191.11, -11650.45, 7, 7, 0, "Exodar, Warrior Trainers");
                SendQuickMenu(9316);
            }break;

        case 20: //Alchemy
            {
                Plr->Gossip_SendPOI(-4042.37, -11366.3, 7, 7, 0, "Exodar, Alchemist Trainers");
                SendQuickMenu(9316);
            }break;

        case 21: //Blacksmithing
            {
                Plr->Gossip_SendPOI(-4232.4, -11705.23, 7, 7, 0, "Exodar, Blacksmithing Trainers");
                SendQuickMenu(9340);
            }break;

        case 22: //Enchanting
            {
                Plr->Gossip_SendPOI(-3889.3, -11495, 7, 7, 0, "Exodar, Enchanters");
                SendQuickMenu(9341);
            }break;

        case 23: //Engineering
            {
                Plr->Gossip_SendPOI(-4257.93, -11636.53, 7, 7, 0, "Exodar, Engineering");
                SendQuickMenu(9316);
            }break;

        case 24: //First Aid
            {
                Plr->Gossip_SendPOI(-3766.05, -11481.8, 7, 7, 0, "Exodar, First Aid Trainer");
                SendQuickMenu(9316);
            }break;

        case 25: //Fishing
            {
                Plr->Gossip_SendPOI(-3726.64, -11384.43, 7, 7, 0, "Exodar, Fishing Trainer");
                SendQuickMenu(9316);
            }break;

        case 26: //Herbalism
            {
                Plr->Gossip_SendPOI(-4052.5, -11356.6, 7, 7, 0, "Exodar, Herbalism Trainer");
                SendQuickMenu(9316);
            }break;

        case 27: //Jewelcrafting
            {
                Plr->Gossip_SendPOI(-3786.27, -11541.33, 7, 7, 0, "Exodar, Jewelcrafters");
                SendQuickMenu(9346);
            }break;

        case 28: //Leatherworking
            {
                Plr->Gossip_SendPOI(-4134.42, -11772.93, 7, 7, 0, "Exodar, Leatherworking");
                SendQuickMenu(9347);
            }break;

        case 29: //Mining
            {
                Plr->Gossip_SendPOI(-4220.31, -11694.29, 7, 7, 0, "Exodar, Mining Trainers");
                SendQuickMenu(9348);
            }break;

        case 30: //Skinning
            {
                Plr->Gossip_SendPOI(-4134.97, -11760.5, 7, 7, 0, "Exodar, Skinning Trainer");
                SendQuickMenu(9316);
            }break;

		case 31: //Tailoring
            {
                Plr->Gossip_SendPOI(-4095.78, -11746.9, 7, 7, 0, "Exodar, Tailors");
                SendQuickMenu(9350);
            }break;

        case 32: //Cooking
            {
                Plr->Gossip_SendPOI(-3799.69, -11650.51, 7, 7, 0, "Exodar, Cook");
                SendQuickMenu(9316);
            }break;
        }
    }
};

class SCRIPT_DECL OrgrimmarGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
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
		Menu->AddItem(0, "Barber", 39);
        Menu->AddItem(0, "A class trainer", 12);
        Menu->AddItem(0, "A profession trainer", 13);
		Menu->AddItem(0, "Lexicon of Power", 40);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
            SendQuickMenu(2554);
            Plr->Gossip_SendPOI(1631.51, -4375.33, 7, 7, 0, "Bank of Orgrimmar");
            break;

        case 2:     // The wind rider master
            SendQuickMenu(2555);
            Plr->Gossip_SendPOI(1676.6, -4332.72, 7, 7, 0, "The Sky Tower");
            break;

        case 3:     // The guild master
            SendQuickMenu(2556);
            Plr->Gossip_SendPOI(1576.93, -4294.75, 7, 7, 0, "Horde Embassy");
            break;

        case 4:     // The inn
            SendQuickMenu(2557);
            Plr->Gossip_SendPOI(1644.51, -4447.27, 7, 7, 0, "Orgrimmar Inn");
            break;

        case 5:     // The mailbox
            SendQuickMenu(2558);
            Plr->Gossip_SendPOI(1622.53, -4388.79, 7, 7, 0, "Orgrimmar Mailbox");
            break;

        case 6:     // The auction house
            SendQuickMenu(3075);
            Plr->Gossip_SendPOI(1679.21, -4450.1, 7, 7, 0, "Orgrimmar Auction House");
			break;

        case 7:     // The zeppelin master
            SendQuickMenu(3173);
            Plr->Gossip_SendPOI(1337.36, -4632.7, 7, 7, 0, "Orgrimmar Zeppelin Tower");
            break;

        case 8:     // The weapon master
            SendQuickMenu(4519);
            Plr->Gossip_SendPOI(2092.56, -4823.95, 7, 7, 0, "Sayoc & Hanashi");
            break;

	    case 9:     // The stable master
			SendQuickMenu(5974);
            Plr->Gossip_SendPOI(2133.12, -4663.93, 7, 7, 0, "Xon'cha");
            break;

        case 10:    // The officers' lounge
			SendQuickMenu(7046);
			Plr->Gossip_SendPOI(1633.56, -4249.37, 7, 7, 0, "Hall of Legends");
            break;

        case 11:    // The battlemaster
			SendQuickMenu(7521);
			Plr->Gossip_SendPOI(1990.41, -4794.15, 7, 7, 0, "Battlemasters Orgrimmar");
            break;

        case 12:    // A class trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2599, Plr);
                Menu->AddItem( 0, "Hunter"       , 14);
                Menu->AddItem( 0, "Mage"         , 15);
                Menu->AddItem( 0, "Priest"       , 16);
                Menu->AddItem( 0, "Shaman"       , 17);
                Menu->AddItem( 0, "Rogue"        , 18);
                Menu->AddItem( 0, "Warlock"      , 19);
                Menu->AddItem( 0, "Warrior"      , 20);
                Menu->AddItem( 0, "Paladin"      , 21);
                Menu->SendTo(Plr);
            }break;

        case 13:    // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2594, Plr);
                Menu->AddItem( 0, "Alchemy"           , 22);
                Menu->AddItem( 0, "Blacksmithing"     , 23);
                Menu->AddItem( 0, "Cooking"           , 24);
                Menu->AddItem( 0, "Enchanting"        , 25);
                Menu->AddItem( 0, "Engineering"       , 26);
                Menu->AddItem( 0, "First Aid"         , 27);
                Menu->AddItem( 0, "Fishing"           , 28);
                Menu->AddItem( 0, "Herbalism"         , 29);
                Menu->AddItem( 0, "Leatherworking"    , 30);
                Menu->AddItem( 0, "Mining"            , 31);
                Menu->AddItem( 0, "Skinning"          , 32);
                Menu->AddItem( 0, "Tailoring"         , 33);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////

        case 14: //Hunter
            {
                Plr->Gossip_SendPOI(2114.84, -4625.31, 7, 7, 0, "Orgrimmar Hunter's Hall");
                SendQuickMenu(2559);
            }break;

        case 15: //Mage
            {
                Plr->Gossip_SendPOI(1451.26, -4223.33, 7, 7, 0, "Darkbriar Lodge");
                SendQuickMenu(2560);
            }break;

        case 16: //Priest
            {
                Plr->Gossip_SendPOI(1442.21, -4183.24, 7, 7, 0, "Spirit Lodge");
                SendQuickMenu(2561);
            }break;

        case 17: //Shaman
            {
                Plr->Gossip_SendPOI(1925.34, -4181.89, 7, 7, 0, "Thrall's Fortress");
                SendQuickMenu(2562);
            }break;

        case 18: //Rogue
            {
                Plr->Gossip_SendPOI(1773.39, -4278.97, 7, 7, 0, "Shadowswift Brotherhood");
                SendQuickMenu(2563);
            }break;

        case 19: //Warlock
            {
                Plr->Gossip_SendPOI(1849.57, -4359.68, 7, 7, 0, "Darkfire Enclave");
                SendQuickMenu(2564);
            }break;

        case 20: //Warrior
            {
                Plr->Gossip_SendPOI(1983.92, -4794.2, 7, 7, 0, "Hall of the Brave");
                SendQuickMenu(2565);
            }break;

		case 21: //Paladin
            {
                Plr->Gossip_SendPOI(1937.53, -4141.0, 7, 7, 0, "Thrall's Fortress");
                SendQuickMenu(2566);
            }break;

        case 22: //Alchemy
            {
                Plr->Gossip_SendPOI(1955.17, -4475.79, 7, 7, 0, "Yelmak's Alchemy and Potions");
                SendQuickMenu(2497);
            }break;

        case 23: //Blacksmithing
            {
                Plr->Gossip_SendPOI(2054.34, -4831.85, 7, 7, 0, "The Burning Anvil");
                SendQuickMenu(2499);
            }break;

        case 24: //Cooking
            {
                Plr->Gossip_SendPOI(1780.96, -4481.31, 7, 7, 0, "Borstan's Firepit");
                SendQuickMenu(2500);
            }break;

        case 25: //Enchanting
            {
                Plr->Gossip_SendPOI(1917.5, -4434.95, 7, 7, 0, "Godan's Runeworks");
                SendQuickMenu(2501);
            }break;

        case 26: //Engineering
            {
                Plr->Gossip_SendPOI(2038.45, -4744.75, 7, 7, 0, "Nogg's Machine Shop");
                SendQuickMenu(2653);
            }break;

        case 27: //First Aid
            {
                Plr->Gossip_SendPOI(1485.21, -4160.91, 7, 7, 0, "Survival of the Fittest");
                SendQuickMenu(2502);
            }break;

        case 28: //Fishing
            {
                Plr->Gossip_SendPOI(1994.15, -4655.7, 7, 7, 0, "Lumak's Fishing");
                SendQuickMenu(2503);
            }break;

        case 29: //Herbalism
            {
                Plr->Gossip_SendPOI(1898.61, -4454.93, 7, 7, 0, "Jandi's Arboretum");
                SendQuickMenu(2504);
            }break;

        case 30: //Leatherworking
            {
                Plr->Gossip_SendPOI(1852.82, -4562.31, 7, 7, 0, "Kodohide Leatherworkers");
                SendQuickMenu(2513);
            }break;

        case 31: //Mining
            {
                Plr->Gossip_SendPOI(2029.79, -4704, 7, 7, 0, "Red Canyon Mining");
                SendQuickMenu(2515);
            }break;

        case 32: //Skinning
            {
                Plr->Gossip_SendPOI(1852.82, -4562.31, 7, 7, 0, "Kodohide Leatherworkers");
                SendQuickMenu(2516);        
            }break;

		case 33: //Tailoring
            {
                Plr->Gossip_SendPOI(1802.66, -4560.66, 7, 7, 0, "Magar's Cloth Goods");
                SendQuickMenu(2518);
            }break;
        }
    }
};
//Thunderbluff guards
class SCRIPT_DECL ThunderbluffGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
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
		Menu->AddItem(0, "Lexicon of Power", 44);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // The bank
            SendQuickMenu(1292);
            Plr->Gossip_SendPOI(-1257.8, 24.14, 7, 7, 0, "Thunder Bluff Bank");
            break;

        case 2:     // The wind rider master
            SendQuickMenu(1293);
            Plr->Gossip_SendPOI(-1196.43, 28.26, 7, 7, 0, "Wind Rider Roost");
            break;

        case 3:     // The guild master
            SendQuickMenu(1291);
            Plr->Gossip_SendPOI(-1296.5, 127.57, 7, 7, 0, "Thunder Bluff Civic Information");
            break;

        case 4:     // The inn
            SendQuickMenu(3153);
            Plr->Gossip_SendPOI(-1296, 39.7, 7, 7, 0, "Thunder Bluff Inn");
            break;

        case 5:     // The mailbox
            SendQuickMenu(3154);
            Plr->Gossip_SendPOI(-1263.59, 44.36, 7, 7, 0, "Thunder Bluff Mailbox");
            break;

        case 6:     // The auction house
            SendQuickMenu(3155);
            Plr->Gossip_SendPOI(-1205.51, 105.74, 7, 7, 0, "Thunder Bluff Auction house");
			break;

        case 7:     // The weapon master
            SendQuickMenu(4520);
            Plr->Gossip_SendPOI(-1282.31, 89.56, 7, 7, 0, "Ansekhwa");
            break;

	    case 8:     // The stable master
			SendQuickMenu(5977);
            Plr->Gossip_SendPOI(-1270.19, 48.84, 7, 7, 0, "Bulrug");
            break;

        case 9:    // The battlemaster
			SendQuickMenu(7527);
			Plr->Gossip_SendPOI(-1391.22, -81.33, 7, 7, 0, "Battlemasters Thunder Bluff");
            break;

        case 10:    // A class trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3542, Plr);
                Menu->AddItem( 0, "Druid"       , 12);
                Menu->AddItem( 0, "Hunter"      , 13);
                Menu->AddItem( 0, "Mage"        , 14);
                Menu->AddItem( 0, "Priest"      , 15);
                Menu->AddItem( 0, "Shaman"      , 16);
                Menu->AddItem( 0, "Warrior"     , 17);
                Menu->SendTo(Plr);
            }break;

        case 11:    // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
                Menu->AddItem( 0, "Alchemy"           , 18);
                Menu->AddItem( 0, "Blacksmithing"     , 19);
                Menu->AddItem( 0, "Cooking"           , 20);
                Menu->AddItem( 0, "Enchanting"        , 21);
                Menu->AddItem( 0, "First Aid"         , 22);
                Menu->AddItem( 0, "Fishing"           , 23);
                Menu->AddItem( 0, "Herbalism"         , 24);
                Menu->AddItem( 0, "Leatherworking"    , 25);
                Menu->AddItem( 0, "Mining"            , 26);
                Menu->AddItem( 0, "Skinning"          , 27);
                Menu->AddItem( 0, "Tailoring"         , 28);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////

        case 12: //Druid
            {
                Plr->Gossip_SendPOI(-1054.47, -285, 7, 7, 0, "Hall of Elders");
                SendQuickMenu(1294);
            }break;

        case 13: //Hunter
            {
                Plr->Gossip_SendPOI(-1416.32, -114.28, 7, 7, 0, "Hunter's Hall");
                SendQuickMenu(1295);
            }break;

        case 14: //Mage
            {
                Plr->Gossip_SendPOI(-1061.2, 195.5, 7, 7, 0, "Pools of Vision");
                SendQuickMenu(1296);
            }break;

        case 15: //Priest
            {
                Plr->Gossip_SendPOI(-1061.2, 195.5, 7, 7, 0, "Pools of Vision");
                SendQuickMenu(1297);
            }break;

        case 16: //Shaman
            {
                Plr->Gossip_SendPOI(-989.54, 278.25, 7, 7, 0, "Hall of Spirits");
                SendQuickMenu(1298);
            }break;

        case 17: //Warrior
			{
                Plr->Gossip_SendPOI(-1416.32, -114.28, 7, 7, 0, "Hunter's Hall");
                SendQuickMenu(1299);
            }break;

        case 18: //Alchemy
            {
                Plr->Gossip_SendPOI(-1085.56, 27.29, 7, 7, 0, "Bena's Alchemy");
                SendQuickMenu(1332);
            }break;

        case 19: //Blacksmithing
            {
                Plr->Gossip_SendPOI(-1239.75, 104.88, 7, 7, 0, "Karn's Smithy");
                SendQuickMenu(1333);
            }break;

        case 20: //Cooking
            {
                Plr->Gossip_SendPOI(-1214.5, -21.23, 7, 7, 0, "Aska's Kitchen");
                SendQuickMenu(1334);
            }break;

        case 21: //Enchanting
            {
                Plr->Gossip_SendPOI(-1112.65, 48.26, 7, 7, 0, "Dawnstrider Enchanters");
                SendQuickMenu(1335);
            }break;

        case 22: //First Aid
            {
                Plr->Gossip_SendPOI(-996.58, 200.5, 7, 7, 0, "Spiritual Healing");
                SendQuickMenu(1336);
            }break;

        case 23: //Fishing
            {
                Plr->Gossip_SendPOI(-1169.35, -68.87, 7, 7, 0, "Mountaintop Bait & Tackle");
                SendQuickMenu(1337);
            }break;

        case 24: //Herbalism
            {
                Plr->Gossip_SendPOI(-1137.7, -1.51, 7, 7, 0, "Holistic Herbalism");
                SendQuickMenu(1338);
            }break;

        case 25: //Leatherworking
            {
                Plr->Gossip_SendPOI(-1156.22, 66.86, 7, 7, 0, "Thunder Bluff Armorers");
                SendQuickMenu(1339);
            }break;

        case 26: //Mining
            {
                Plr->Gossip_SendPOI(-1249.17, 155, 7, 7, 0, "Stonehoof Geology");
                SendQuickMenu(1340);
            }break;

        case 27: //Skinning
            {
                Plr->Gossip_SendPOI(-1148.56, 51.18, 7, 7, 0, "Mooranta");
                SendQuickMenu(1343); 
            }break;

		case 28: //Tailoring
            {
                Plr->Gossip_SendPOI(-1156.22, 66.86, 7, 7, 0, "Thunder Bluff Armorers");
                SendQuickMenu(1341);
            }break;
        }
    }
};

class SCRIPT_DECL BloodhoofGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3543, Plr);
        Menu->AddItem(0, "The bank", 1);
        Menu->AddItem(0, "The wind rider master", 2);
        Menu->AddItem(0, "The inn", 3);
        Menu->AddItem(0, "The stable master", 4);
        Menu->AddItem(0, "A class trainer", 5);
        Menu->AddItem(0, "A profession trainer", 6);    
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
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
            Plr->Gossip_SendPOI(-2361.38, -349.19, 7, 7, 0, "Bloodhoof Village Inn");      
			break;

        case 4:     // The stable master
            SendQuickMenu(5976);
            Plr->Gossip_SendPOI(-2338.86, -357.56, 7, 7, 0, "Seikwa");
            break;

        case 5:     // A class trainer
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4069, Plr);
                Menu->AddItem( 0, "Druid"       , 7);
                Menu->AddItem( 0, "Hunter"      , 8);
                Menu->AddItem( 0, "Shaman"      , 9);
                Menu->AddItem( 0, "Warrior"    , 10);
                Menu->SendTo(Plr);
			}break;

        case 6:     // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
                Menu->AddItem( 0, "Alchemy"           , 11);
                Menu->AddItem( 0, "Blacksmithing"     , 12);
                Menu->AddItem( 0, "Cooking"           , 13);
                Menu->AddItem( 0, "Enchanting"        , 14);
                Menu->AddItem( 0, "First Aid"         , 15);
                Menu->AddItem( 0, "Fishing"           , 16);
                Menu->AddItem( 0, "Herbalism"         , 17);
                Menu->AddItem( 0, "Leatherworking"    , 18);
                Menu->AddItem( 0, "Mining"            , 19);
                Menu->AddItem( 0, "Skinning"          , 20);
                Menu->AddItem( 0, "Tailoring"         , 21);
                Menu->SendTo(Plr);
            }break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: //Druid
            {
                Plr->Gossip_SendPOI(-2312.15, -443.69, 7, 7, 0, "Gennia Runetotem");
                SendQuickMenu(4054);
            }break;

        case 8: //Hunter
            {
                Plr->Gossip_SendPOI(-2178.14, -406.14, 7, 7, 0, "Yaw Sharpmane");
                SendQuickMenu(4055);
            }break;

        case 9: //Shaman
            {
                Plr->Gossip_SendPOI(-2301.5, -439.87, 7, 7, 0, "Narm Skychaser");
                SendQuickMenu(4056);
            }break;

        case 10: //Warrior
			{
                Plr->Gossip_SendPOI(-2345.43, -494.11, 7, 7, 0, "Krang Stonehoof");
                SendQuickMenu(4057);
            }break;

        case 11: //Alchemy
            {
                SendQuickMenu(4058);
            }break;

        case 12: //Blacksmithing
            {
                SendQuickMenu(4059);
            }break;

        case 13: //Cooking
            {
                Plr->Gossip_SendPOI(-2263.34, -287.91, 7, 7, 0, "Pyall Silentstride");
                SendQuickMenu(4060);
            }break;

        case 14: //Enchanting
            {
				SendQuickMenu(4061);
            }break;

        case 15: //First Aid
            {
                Plr->Gossip_SendPOI(-2353.52, -355.82, 7, 7, 0, "Vira Younghoof");
                SendQuickMenu(4062);
            }break;

        case 16: //Fishing
            {
                Plr->Gossip_SendPOI(-2349.21, -241.37, 7, 7, 0, "Uthan Stillwater");
                SendQuickMenu(4063);
            }break;

        case 17: //Herbalism
            {
                SendQuickMenu(4064);
            }break;

        case 18: //Leatherworking
            {
                Plr->Gossip_SendPOI(-2257.12, -288.63, 7, 7, 0, "Chaw Stronghide");
                SendQuickMenu(4065);
            }break;

        case 19: //Mining
            {
                SendQuickMenu(4066);
            }break;

        case 20: //Skinning
            {
                Plr->Gossip_SendPOI(-2252.94, -291.32, 7, 7, 0, "Yonn Deepcut");
                SendQuickMenu(4067); 
            }break;

		case 21: //Tailoring
            {
                SendQuickMenu(4068);
            }break;
        }
    }
};

class SCRIPT_DECL RazorHillGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4037, Plr);
        Menu->AddItem(0, "The bank", 1);
        Menu->AddItem(0, "The wind rider master", 2);
        Menu->AddItem(0, "The inn", 3);
        Menu->AddItem(0, "The stable master", 4);
        Menu->AddItem(0, "A class trainer", 5);
        Menu->AddItem(0, "A profession trainer", 6);    
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
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
            Plr->Gossip_SendPOI(338.7, -4688.87, 7, 7, 0, "Razor Hill Inn");      
			break;

        case 4:     // The stable master
            SendQuickMenu(5973);
            Plr->Gossip_SendPOI(330.31, -4710.66, 7, 7, 0, "Shoja'my");
            break;

        case 5:     // A class trainer
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4035, Plr);
                Menu->AddItem( 0, "Hunter"       , 7);
				Menu->AddItem( 0, "Mage"         , 8);
				Menu->AddItem( 0, "Priest"       , 9);
				Menu->AddItem( 0, "Rogue"       , 10);
                Menu->AddItem( 0, "Shaman"      , 11);
				Menu->AddItem( 0, "Warlock"     , 12);
                Menu->AddItem( 0, "Warrior"     , 13);
                Menu->SendTo(Plr);
			}break;

        case 6:     // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 3541, Plr);
                Menu->AddItem( 0, "Alchemy"           , 14);
                Menu->AddItem( 0, "Blacksmithing"     , 15);
                Menu->AddItem( 0, "Cooking"           , 16);
                Menu->AddItem( 0, "Enchanting"        , 17);
                Menu->AddItem( 0, "Engineering"       , 18);
                Menu->AddItem( 0, "First Aid"         , 19);
                Menu->AddItem( 0, "Fishing"           , 20);
                Menu->AddItem( 0, "Herbalism"         , 21);
                Menu->AddItem( 0, "Leatherworking"    , 22);
                Menu->AddItem( 0, "Mining"            , 23);
                Menu->AddItem( 0, "Skinning"          , 24);
                Menu->AddItem( 0, "Tailoring"         , 25);
                Menu->SendTo(Plr);
            }break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: //Hunter
            {
                Plr->Gossip_SendPOI(276, -4706.72, 7, 7, 0, "Thotar");
                SendQuickMenu(4013);
            }break;        

        case 8: //Mage
            {
                Plr->Gossip_SendPOI(-839.33, -4935.6, 7, 7, 0, "Un'Thuwa");
                SendQuickMenu(4014);
            }break;

        case 9: //Priest
            {
                Plr->Gossip_SendPOI(296.22, -4828.1, 7, 7, 0, "Tai'jin");
                SendQuickMenu(4015);
            }break;

        case 10: //Rogue
			{
                Plr->Gossip_SendPOI(265.76, -4709, 7, 7, 0, "Kaplak");
                SendQuickMenu(4016);
            }break;

        case 11: //Shaman
            {
                Plr->Gossip_SendPOI(307.79, -4836.97, 7, 7, 0, "Swart");
                SendQuickMenu(4017);
            }break;

        case 12: //Warlock
            {
                Plr->Gossip_SendPOI(355.88, -4836.45, 7, 7, 0, "Dhugru Gorelust");
                SendQuickMenu(4018);
            }break;

        case 13: //Warrior
            {
                Plr->Gossip_SendPOI(312.3, -4824.66, 7, 7, 0, "Tarshaw Jaggedscar");
                SendQuickMenu(4019);
            }break;

        case 14: //Alchemy
            {
				Plr->Gossip_SendPOI(-800.25, -4894.33, 7, 7, 0, "Miao'zan");
                SendQuickMenu(4020);
            }break;

        case 15: //Blacksmithing
            {
				Plr->Gossip_SendPOI(373.24, -4716.45, 7, 7, 0, "Dwukk");
                SendQuickMenu(4021);
            }break;

        case 16: //Cooking
            {
                SendQuickMenu(4022);
            }break;

        case 17: //Enchanting
            {
				SendQuickMenu(4023);
            }break;
			
        case 18: //Engineering
            {
                Plr->Gossip_SendPOI(368.95, -4723.95, 7, 7, 0, "Mukdrak");
                SendQuickMenu(4024);
            }break;

        case 19: //First Aid
            {
                Plr->Gossip_SendPOI(327.17, -4825.62, 7, 7, 0, "Rawrk");
                SendQuickMenu(4025);
            }break;

        case 20: //Fishing
            {
                Plr->Gossip_SendPOI(-1065.48, -4777.43, 7, 7, 0, "Lau'Tiki");
                SendQuickMenu(4026);
            }break;

        case 21: //Herbalism
            {
				Plr->Gossip_SendPOI(-836.25, -4896.89, 7, 7, 0, "Mishiki");
                SendQuickMenu(4027);
            }break;

        case 22: //Leatherworking
            {
                SendQuickMenu(4028);
            }break;

        case 23: //Mining
            {
				Plr->Gossip_SendPOI(366.94, -4705, 7, 7, 0, "Krunn");
                SendQuickMenu(4029);
            }break;

        case 24: //Skinning
            {
                Plr->Gossip_SendPOI(-2252.94, -291.32, 7, 7, 0, "Yonn Deepcut");
                SendQuickMenu(4030); 
            }break;

		case 25: //Tailoring
            {
                SendQuickMenu(4031);
            }break;
        }
    }
};

class SCRIPT_DECL BrillGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "The bank", 1);
        Menu->AddItem(0, "The bat handler", 2);
        Menu->AddItem(0, "The inn", 3);
        Menu->AddItem(0, "The stable master", 4);
        Menu->AddItem(0, "A class trainer", 5);
        Menu->AddItem(0, "A profession trainer", 6);    
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
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
            Plr->Gossip_SendPOI(2246.68, 241.89, 7, 7, 0, "Gallows` End Tavern"); 
			break;

        case 4:     // The stable master
            SendQuickMenu(5978);
            Plr->Gossip_SendPOI(2267.66, 319.32, 7, 7, 0, "Morganus");
            break;

        case 5:     // A class trainer
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
                Menu->AddItem( 0, "Mage"         , 7);
				Menu->AddItem( 0, "Paladin"      , 8);
				Menu->AddItem( 0, "Priest"       , 9);
				Menu->AddItem( 0, "Rogue"       , 10);
				Menu->AddItem( 0, "Warlock"     , 11);
                Menu->AddItem( 0, "Warrior"     , 12);
                Menu->SendTo(Plr);
			}break;

        case 6:     // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4300, Plr);
                Menu->AddItem( 0, "Alchemy"           , 13);
                Menu->AddItem( 0, "Blacksmithing"     , 14);
                Menu->AddItem( 0, "Cooking"           , 15);
                Menu->AddItem( 0, "Enchanting"        , 16);
                Menu->AddItem( 0, "Engineering"       , 17);
                Menu->AddItem( 0, "First Aid"         , 18);
                Menu->AddItem( 0, "Fishing"           , 19);
                Menu->AddItem( 0, "Herbalism"         , 20);
                Menu->AddItem( 0, "Leatherworking"    , 21);
                Menu->AddItem( 0, "Mining"            , 22);
                Menu->AddItem( 0, "Skinning"          , 23);
                Menu->AddItem( 0, "Tailoring"         , 24);
                Menu->SendTo(Plr);
            }break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: //Mage
            {
                Plr->Gossip_SendPOI(2259.18, 240.93, 7, 7, 0, "Cain Firesong");
                SendQuickMenu(4077);
            }break;        

        case 8: //Paladin
            {
                SendQuickMenu(0); // Need to add correct text
            }break;

        case 9: //Priest
            {
                Plr->Gossip_SendPOI(2259.18, 240.93, 7, 7, 0, "Dark Cleric Beryl");
                SendQuickMenu(4078);
            }break;

        case 10: //Rogue
			{
                Plr->Gossip_SendPOI(2259.18, 240.93, 7, 7, 0, "Marion Call");
                SendQuickMenu(4079);
            }break;

        case 11: //Warlock
            {
                Plr->Gossip_SendPOI(2259.18, 240.93, 7, 7, 0, "Rupert Boch");
                SendQuickMenu(4080);
            }break;

        case 12: //Warrior
            {
                Plr->Gossip_SendPOI(2256.48, 240.32, 7, 7, 0, "Austil de Mon");
                SendQuickMenu(4081);
            }break;

        case 13: //Alchemy
            {
				Plr->Gossip_SendPOI(2263.25, 344.23, 7, 7, 0, "Carolai Anise");
                SendQuickMenu(4082);
            }break;

        case 14: //Blacksmithing
            {
                SendQuickMenu(4083);
            }break;

        case 15: //Cooking
            {
                SendQuickMenu(4084);
            }break;

        case 16: //Enchanting
            {
				Plr->Gossip_SendPOI(2250.35, 249.12, 7, 7, 0, "Vance Undergloom");
				SendQuickMenu(4085);
            }break;
			
        case 17: //Engineering
            {
                SendQuickMenu(4086);
            }break;

        case 18: //First Aid
            {
                Plr->Gossip_SendPOI(2246.68, 241.89, 7, 7, 0, "Nurse Neela");
                SendQuickMenu(4087);
            }break;

        case 19: //Fishing
            {
                Plr->Gossip_SendPOI(2292.37, -10.72, 7, 7, 0, "Clyde Kellen");
                SendQuickMenu(4088);
            }break;

        case 20: //Herbalism
            {
				Plr->Gossip_SendPOI(2268.21, 331.69, 7, 7, 0, "Faruza");
                SendQuickMenu(4089);
            }break;

        case 21: //Leatherworking
            {
				Plr->Gossip_SendPOI(2027, 78.72, 7, 7, 0, "Shelene Rhobart");
                SendQuickMenu(4090);
            }break;

        case 22: //Mining
            {
                SendQuickMenu(4091);
            }break;

        case 23: //Skinning
            {
                Plr->Gossip_SendPOI(2027, 78.72, 7, 7, 0, "Rand Rhobart");
                SendQuickMenu(4092); 
            }break;

		case 24: //Tailoring
            {
				Plr->Gossip_SendPOI(2160.45, 659.93, 7, 7, 0, "Bowen Brisboise");
                SendQuickMenu(4093);
            }break;
        }
    }
};
// Ironforge Guard
class SCRIPT_DECL IronforgeGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2760, Plr);
        Menu->AddItem(0, "Auction House", 1);
        Menu->AddItem(0, "Bank of Ironforge", 2);
        Menu->AddItem(0, "Deeprun Tram", 3);
        Menu->AddItem(0, "Gryphon Master", 4);
        Menu->AddItem(0, "Guild Master", 5);
        Menu->AddItem(0, "The Inn", 6);    
        Menu->AddItem(0, "Mailbox", 7);
        Menu->AddItem(0, "Stable Master", 8);
        Menu->AddItem(0, "Weapons Trainer", 9);
        Menu->AddItem(0, "Battlemaster", 10);
		Menu->AddItem(0, "Barber", 36);
        Menu->AddItem(0, "Class Trainer", 11);
        Menu->AddItem(0, "Profession Trainer", 12);
		Menu->AddItem(0, "Lexicon of Power", 37);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     // Auction House
            SendQuickMenu(3014);
            Plr->Gossip_SendPOI(-4957.39, -911.6, 7, 7, 0, "Ironforge Auction House");
            break;

        case 2:     // Bank of Ironforge
            SendQuickMenu(2761);
            Plr->Gossip_SendPOI(-4891.91, -991.47, 7, 7, 0, "The Vault");
            break;

        case 3:     // Deeprun Tram
            SendQuickMenu(3814);
            Plr->Gossip_SendPOI(-4835.27, -1294.69, 7, 7, 0, "Deeprun Tram");
            break;

        case 4:     // Gryphon Master
            SendQuickMenu(2762);
            Plr->Gossip_SendPOI(-4821.52, -1152.3, 7, 7, 0, "Ironforge Gryphon Master");
            break;

        case 5:     // Guild Master
            SendQuickMenu(2764);
            Plr->Gossip_SendPOI(-5021, -996.45, 7, 7, 0, "Ironforge Visitor's Center");
            break;

        case 6:     // The Inn
            SendQuickMenu(2768);
            Plr->Gossip_SendPOI(-4850.47, -872.57, 7, 7, 0, "Stonefire Tavern");
			break;

        case 7:     // Mailbox
            SendQuickMenu(2769);
            Plr->Gossip_SendPOI(-4845.7, -880.55, 7, 7, 0, "Ironforge Mailbox");
            break;

	    case 8:     // Stable Master
			SendQuickMenu(5986);
            Plr->Gossip_SendPOI(-5010.2, -1262, 7, 7, 0, "Ulbrek Firehand");
            break;

        case 9:    // Weapon Trainer
			SendQuickMenu(4518);
			Plr->Gossip_SendPOI(-5040, -1201.88, 7, 7, 0, "Bixi and Buliwyf");
            break;

        case 10:    // Battlemaster
			SendQuickMenu(7527);
			Plr->Gossip_SendPOI(-5038.54, -1266.44, 7, 7, 0, "Battlemasters Ironforge");
            break;

        case 11:    // A class trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2766, Plr);
                Menu->AddItem( 0, "Hunter"      , 13);
                Menu->AddItem( 0, "Mage"        , 14);
                Menu->AddItem( 0, "Paladin"     , 15);
                Menu->AddItem( 0, "Priest"      , 16);
                Menu->AddItem( 0, "Rogue"       , 17);
                Menu->AddItem( 0, "Warlock"     , 18);
                Menu->AddItem( 0, "Warrior"     , 19);
                Menu->AddItem( 0, "Shaman"      , 20);
                Menu->SendTo(Plr);
            }break;

        case 12:    // A profession trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2793, Plr);
                Menu->AddItem( 0, "Alchemy"           , 21);
                Menu->AddItem( 0, "Blacksmithing"     , 22);
                Menu->AddItem( 0, "Cooking"           , 23);
                Menu->AddItem( 0, "Enchanting"        , 24);
				Menu->AddItem( 0, "Engineering"       , 25);
                Menu->AddItem( 0, "First Aid"         , 26);
                Menu->AddItem( 0, "Fishing"           , 27);
                Menu->AddItem( 0, "Herbalism"         , 28);
                Menu->AddItem( 0, "Leatherworking"    , 29);
                Menu->AddItem( 0, "Mining"            , 30);
                Menu->AddItem( 0, "Skinning"          , 31);
                Menu->AddItem( 0, "Tailoring"         , 32);
                Menu->SendTo(Plr);
            }break;

            ////////////////
            // Class trainer submenu
            ////////

        case 13: //Hunter
            {
                Plr->Gossip_SendPOI(-5023, -1253.68, 7, 7, 0, "Hall of Arms");
                SendQuickMenu(2770);
            }break;

        case 14: //Mage
            {
                Plr->Gossip_SendPOI(-4627, -926.45, 7, 7, 0, "Hall of Mysteries");
                SendQuickMenu(2771);
            }break;

        case 15: //Paladin
            {
                Plr->Gossip_SendPOI(-4627.02, -926.45, 7, 7, 0, "Hall of Mysteries");
                SendQuickMenu(2773);
            }break;

        case 16: //Priest
            {
                Plr->Gossip_SendPOI(-4627, -926.45, 7, 7, 0, "Hall of Mysteries");
                SendQuickMenu(2772);
            }break;

        case 17: //Rogue
            {
                Plr->Gossip_SendPOI(-4647.83, -1124, 7, 7, 0, "Ironforge Rogue Trainer");
                SendQuickMenu(2774);
            }break;

        case 18: //Warlock
			{
                Plr->Gossip_SendPOI(-4605, -1110.45, 7, 7, 0, "Ironforge Warlock Trainer");
                SendQuickMenu(2775);
            }break;

        case 19: //Warrior
			{
                Plr->Gossip_SendPOI(-5023.08, -1253.68, 7, 7, 0, "Hall of Arms");
                SendQuickMenu(2776);
            }break;

        case 20: //Shaman
			{
                Plr->Gossip_SendPOI(-4722.02, -1150.66, 7, 7, 0, "Ironforge Shaman Trainer");
                SendQuickMenu(1299);
            }break;

        case 21: //Alchemy
            {
                Plr->Gossip_SendPOI(-4858.5, -1241.83, 7, 7, 0, "Berryfizz's Potions and Mixed Drinks");
                SendQuickMenu(2794);
            }break;

        case 22: //Blacksmithing
            {
                Plr->Gossip_SendPOI(-4796.97, -1110.17, 7, 7, 0, "The Great Forge");
                SendQuickMenu(2795);
            }break;

        case 23: //Cooking
            {
                Plr->Gossip_SendPOI(-4767.83, -1184.59, 7, 7, 0, "The Bronze Kettle");
                SendQuickMenu(2796);
            }break;

        case 24: //Enchanting
            {
                Plr->Gossip_SendPOI(-4803.72, -1196.53, 7, 7, 0, "Thistlefuzz Arcanery");
                SendQuickMenu(2797);
            }break;

		case 25: //Engineering
            {
                Plr->Gossip_SendPOI(-4799.56, -1250.23, 7, 7, 0, "Springspindle's Gadgets");
                SendQuickMenu(2798);
            }break;

        case 26: //First Aid
            {
                Plr->Gossip_SendPOI(-4881.6, -1153.13, 7, 7, 0, "Ironforge Physician");
                SendQuickMenu(2799);
            }break;

        case 27: //Fishing
            {
                Plr->Gossip_SendPOI(-4597.91, -1091.93, 7, 7, 0, "Traveling Fisherman");
                SendQuickMenu(2800);
            }break;

        case 28: //Herbalism
            {
                Plr->Gossip_SendPOI(-4876.9, -1151.92, 7, 7, 0, "Ironforge Physician");
                SendQuickMenu(2801);
            }break;

        case 29: //Leatherworking
            {
                Plr->Gossip_SendPOI(-4745, -1027.57, 7, 7, 0, "Finespindle's Leather Goods");
                SendQuickMenu(2802);
            }break;

        case 30: //Mining
            {
                Plr->Gossip_SendPOI(-4705.06, -1116.43, 7, 7, 0, "Deepmountain Mining Guild");
                SendQuickMenu(2804);
            }break;

        case 31: //Skinning
            {
                Plr->Gossip_SendPOI(-4745, -1027.57, 7, 7, 0, "Finespindle's Leather Goods");
                SendQuickMenu(2805); 
            }break;

		case 32: //Tailoring
            {
                Plr->Gossip_SendPOI(-4719.60, -1056.96, 7, 7, 0, "Stonebrow's Clothier");
                SendQuickMenu(2807);
            }break;
        }
    }
};

class SCRIPT_DECL KharanosGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "Bank", 1);
        Menu->AddItem(0, "Gryphon Master", 2);
        Menu->AddItem(0, "Guild Master", 3);
        Menu->AddItem(0, "The Inn", 4);
        Menu->AddItem(0, "Stable Master", 5);
        Menu->AddItem(0, "Class Trainer", 6);
        Menu->AddItem(0, "Profession Trainer", 7);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
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
            Plr->Gossip_SendPOI(-5582.66, -525.89, 7, 7, 0, "Thunderbrew Distillery");
			break;

        case 5:     //Stable Master
            SendQuickMenu(5985);
            Plr->Gossip_SendPOI(-5604, -509.58, 7, 7, 0, "Shelby Stoneflint");
            break;

        case 6:     //Class Trainer
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
                Menu->AddItem( 0, "Hunter"       , 8);
				Menu->AddItem( 0, "Mage"         , 9);
				Menu->AddItem( 0, "Paladin"      ,10);
				Menu->AddItem( 0, "Priest"      , 11);
                Menu->AddItem( 0, "Rogue"       , 12);
				Menu->AddItem( 0, "Warlock"     , 13);
                Menu->AddItem( 0, "Warrior"     , 14);
                Menu->SendTo(Plr);
			}break;

        case 7:     // Profession Trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4300, Plr);
                Menu->AddItem( 0, "Alchemy"           , 15);
                Menu->AddItem( 0, "Blacksmithing"     , 16);
                Menu->AddItem( 0, "Cooking"           , 17);
                Menu->AddItem( 0, "Enchanting"        , 18);
                Menu->AddItem( 0, "Engineering"       , 19);
                Menu->AddItem( 0, "First Aid"         , 20);
                Menu->AddItem( 0, "Fishing"           , 21);
                Menu->AddItem( 0, "Herbalism"         , 22);
                Menu->AddItem( 0, "Leatherworking"    , 23);
                Menu->AddItem( 0, "Mining"            , 24);
                Menu->AddItem( 0, "Skinning"          , 25);
                Menu->AddItem( 0, "Tailoring"         , 26);
                Menu->SendTo(Plr);
            }break;


            ////////////////
            // Class trainer submenu
            ////////

        case 8: //Hunter
            {
                Plr->Gossip_SendPOI(-5618.29, -454.25, 7, 7, 0, "Grif Wildheart");
                SendQuickMenu(4293);
            }break;

        case 9: //Mage
            {
                Plr->Gossip_SendPOI(-5585.6, -539.99, 7, 7, 0, "Magis Sparkmantle");
                SendQuickMenu(4294);
            }break;

        case 10: //Paladin
            {
                Plr->Gossip_SendPOI(-5585.6, -539.99, 7, 7, 0, "Azar Stronghammer");
                SendQuickMenu(4295);
            }break;

        case 11: //Priest
			{
                Plr->Gossip_SendPOI(-5591.74, -525.61, 7, 7, 0, "Maxan Anvol");
                SendQuickMenu(4296);
            }break;

        case 12: //Rogue
            {
                Plr->Gossip_SendPOI(-5602.75, -542.4, 7, 7, 0, "Hogral Bakkan");
                SendQuickMenu(4297);
            }break;

        case 13: //Warlock
            {
                Plr->Gossip_SendPOI(-5641.97, -523.76, 7, 7, 0, "Gimrizz Shadowcog");
                SendQuickMenu(4298);
            }break;

        case 14: //Warrior
            {
                Plr->Gossip_SendPOI(-5604.79, -529.38, 7, 7, 0, "Granis Swiftaxe");
                SendQuickMenu(4299);
            }break;

        case 15: //Alchemy
            {
                SendQuickMenu(4301);
            }break;

        case 16: //Blacksmithing
            {
				Plr->Gossip_SendPOI(-5584.72, -428.41, 7, 7, 0, "Tognus Flintfire");
                SendQuickMenu(4302);
            }break;

        case 17: //Cooking
            {
				Plr->Gossip_SendPOI(-5596.85, -541.43, 7, 7, 0, "Gremlock Pilsnor");
                SendQuickMenu(4303);
            }break;

        case 18: //Enchanting
            {
				SendQuickMenu(4304);
            }break;
			
        case 19: //Engineering
            {
                SendQuickMenu(4305);
            }break;

        case 20: //First Aid
            {
                Plr->Gossip_SendPOI(-5603.67, -523.57, 7, 7, 0, "Thamner Pol");
                SendQuickMenu(4306);
            }break;

        case 21: //Fishing
            {
                Plr->Gossip_SendPOI(-5202.39, -51.36, 7, 7, 0, "Paxton Ganter");
                SendQuickMenu(4307);
            }break;

        case 22: //Herbalism
            {
                SendQuickMenu(4308);
            }break;

        case 23: //Leatherworking
            {
                SendQuickMenu(4310);
            }break;

        case 24: //Mining
            {
				Plr->Gossip_SendPOI(-5531, -666.53, 7, 7, 0, "Yarr Hamerstone");
                SendQuickMenu(4311);
            }break;

        case 25: //Skinning
            {
                SendQuickMenu(4312);
            }break;

		case 26: //Tailoring
            {
                SendQuickMenu(4313);
            }break;
        }
    }
};

class SCRIPT_DECL FalconwingGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "Bat Handler", 1);
        Menu->AddItem(0, "Guild Master", 2);
        Menu->AddItem(0, "The Inn", 3);
        Menu->AddItem(0, "Stable Master", 4);
        Menu->AddItem(0, "Class Trainer", 5);
        Menu->AddItem(0, "Profession Trainer", 6);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     //Bat Handler
            SendQuickMenu(2593);
			Plr->Gossip_SendPOI(9376.4, -7164.92, 7, 7, 0, "Silvermoon City, Flight Master");
            break;

        case 2:     //Guild Master
            SendQuickMenu(2593);
			break;
			
        case 3:     //The Inn
            SendQuickMenu(2593);
            Plr->Gossip_SendPOI(9476.916, -6859.2, 7, 7, 0, "Falconwing Square, Innkeeper");
			break;

        case 4:     //Stable Master
            SendQuickMenu(2593);
            Plr->Gossip_SendPOI(9487.6, -6830.59, 7, 7, 0, "Falconwing Square, Stable Master");
            break;

        case 5:     //Class Trainer
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
                Menu->AddItem( 0, "Druid"        , 7);
				Menu->AddItem( 0, "Hunter"       , 8);
				Menu->AddItem( 0, "Mage"         , 9);
				Menu->AddItem( 0, "Paladin"     , 10);
                Menu->AddItem( 0, "Priest"      , 11);
				Menu->AddItem( 0, "Rogue"       , 12);
                Menu->AddItem( 0, "Warlock"     , 13);
                Menu->SendTo(Plr);
			}break;

        case 6:     // Profession Trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
                Menu->AddItem( 0, "Alchemy"           , 14);
                Menu->AddItem( 0, "Blacksmithing"     , 15);
                Menu->AddItem( 0, "Cooking"           , 16);
                Menu->AddItem( 0, "Enchanting"        , 17);
                Menu->AddItem( 0, "Engineering"       , 18);
                Menu->AddItem( 0, "First Aid"         , 19);
                Menu->AddItem( 0, "Fishing"           , 20);
                Menu->AddItem( 0, "Herbalism"         , 21);
				Menu->AddItem( 0, "Jewelcrafting"     , 22);
                Menu->AddItem( 0, "Leatherworking"    , 23);
                Menu->AddItem( 0, "Mining"            , 24);
                Menu->AddItem( 0, "Skinning"          , 25);
                Menu->AddItem( 0, "Tailoring"         , 26);
                Menu->SendTo(Plr);
            }break;


            ////////////////
            // Class trainer submenu
            ////////

        case 7: //Druid
            {
                SendQuickMenu(2593);
            }break;

        case 8: //Hunter
            {
                Plr->Gossip_SendPOI(9529.2, -6864.58, 7, 7, 0, "Falconwing Square, Hunter Trainer");
                SendQuickMenu(2593);
            }break;

        case 9: //Mage
            {
                Plr->Gossip_SendPOI(9462.24, -6853.45, 7, 7, 0, "Falconwing Square, Mage Trainer");
                SendQuickMenu(2593);
            }break;

        case 10: //Paladin <-- Needs to change flag to other sign (don't know how to describe it)
			{
                Plr->Gossip_SendPOI(9516.05, -6870.96, 7, 7, 0, "Falconwing Square, Paladin Trainer");
                SendQuickMenu(2593);
            }break;

        case 11: //Priest
            {
                Plr->Gossip_SendPOI(9466.62, -6844.23, 7, 7, 0, "Falconwing Square, Priest Trainer");
                SendQuickMenu(2593);
            }break;

        case 12: //Rogue
            {
                Plr->Gossip_SendPOI(9534.15, -6876.0, 7, 7, 0, "Falconwing Square, Rogue Trainer");
                SendQuickMenu(2593);
            }break;

        case 13: //Warlock
            {
                Plr->Gossip_SendPOI(9467.63, -6862.82, 7, 7, 0, "Falconwing Square, Warlock Trainer");
                SendQuickMenu(2593);
            }break;

        case 14: //Alchemy
            {
				Plr->Gossip_SendPOI(8661.36, -6367.0, 7, 7, 0, "Saltheril's Haven, Alchemist");
                SendQuickMenu(2593);
            }break;

        case 15: //Blacksmithing
            {
				Plr->Gossip_SendPOI(8986.43, -7419.07, 7, 7, 0, "Farstrider Retreat, Blacksmith");
                SendQuickMenu(2593);
            }break;

        case 16: //Cooking
            {
				Plr->Gossip_SendPOI(9494.86, -6879.45, 7, 7, 0, "Falconwing Square, Cook");
                SendQuickMenu(2593);
            }break;

        case 17: //Enchanting
            {
				Plr->Gossip_SendPOI(8657.6, -6366.7, 7, 7, 0, "Saltheril's Haven, Enchanter");
				SendQuickMenu(2593);
            }break;
			
        case 18: //Engineering
            {
                SendQuickMenu(2593);
            }break;

        case 19: //First Aid
            {
                Plr->Gossip_SendPOI(9479.53, -6880.07, 7, 7, 0, "Falconwing Square, First Aid");
                SendQuickMenu(2593);
            }break;

        case 20: //Fishing
            {
                SendQuickMenu(2593);
            }break;

        case 21: //Herbalism
            {
				Plr->Gossip_SendPOI(8678.92, -6329.09, 7, 7, 0, "Saltheril's Haven, Herbalist");
                SendQuickMenu(2593);
            }break;

        case 22: //Jewelcrafting
            {
				Plr->Gossip_SendPOI(9484.79, -6876.58, 7, 7, 0, "Falconwing Square, Jewelcrafter");
                SendQuickMenu(2593);
            }break;

        case 23: //Leatherworking
            {
				Plr->Gossip_SendPOI(9363.75, -7130.75, 7, 7, 0, "Eversong Woods, Leatherworker");
                SendQuickMenu(2593);
            }break;

        case 24: //Mining
            {
                SendQuickMenu(2593);
            }break;

        case 25: //Skinning
            {
				Plr->Gossip_SendPOI(9362.89, -7134.58, 7, 7, 0, "Eversong Woods, Skinner");
                SendQuickMenu(2593);
            }break;

		case 26: //Tailoring
            {
				Plr->Gossip_SendPOI(8680.36, -6327.51, 7, 7, 0, "Saltheril's Haven, Tailor");
                SendQuickMenu(2593);
            }break;
        }
    }
};

class SCRIPT_DECL AzureWatchGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}

    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
        Menu->AddItem(0, "Bank", 1);
        Menu->AddItem(0, "Hippogryph Master", 2);
        Menu->AddItem(0, "Guild Master", 3);
        Menu->AddItem(0, "Inn", 4);
        Menu->AddItem(0, "Stable", 5);
        Menu->AddItem(0, "Class Trainer", 6);
		Menu->AddItem(0, "Profession Trainer", 7);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Main menu handlers
            /////

        case 1:     //Bank
            SendQuickMenu(2593);
            break;

        case 2:     //Hippogryph Master
            SendQuickMenu(2593);
			break;
			
        case 3:     //Guild Master
            SendQuickMenu(2593);
			break;

        case 4:     //Inn
            SendQuickMenu(2593);
            Plr->Gossip_SendPOI(-4127.81, -12467.7, 7, 7, 0, "Azure Watch, Innkeeper");
            break;

        case 5:     //Stable Master
            SendQuickMenu(2593);
            Plr->Gossip_SendPOI(-4146.42, -12492.7, 7, 7, 0, "Azure Watch, Stable Master");
            break;

        case 6:     //Class Trainer
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 4292, Plr);
                Menu->AddItem( 0, "Druid"        , 8);
				Menu->AddItem( 0, "Hunter"       , 9);
				Menu->AddItem( 0, "Mage"        , 10);
				Menu->AddItem( 0, "Paladin"     , 11);
                Menu->AddItem( 0, "Priest"      , 12);
				Menu->AddItem( 0, "Shaman"      , 13);
                Menu->AddItem( 0, "Warrior"     , 14);
                Menu->SendTo(Plr);
			}break;

        case 7:     //Profession Trainer
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2593, Plr);
                Menu->AddItem( 0, "Alchemy"           , 15);
                Menu->AddItem( 0, "Blacksmithing"     , 16);
                Menu->AddItem( 0, "Cooking"           , 17);
                Menu->AddItem( 0, "Enchanting"        , 18);
                Menu->AddItem( 0, "Engineering"       , 19);
                Menu->AddItem( 0, "First Aid"         , 20);
                Menu->AddItem( 0, "Fishing"           , 21);
                Menu->AddItem( 0, "Herbalism"         , 22);
				Menu->AddItem( 0, "Jewelcrafting"     , 23);
                Menu->AddItem( 0, "Leatherworking"    , 24);
                Menu->AddItem( 0, "Mining"            , 25);
                Menu->AddItem( 0, "Skinning"          , 26);
                Menu->AddItem( 0, "Tailoring"         , 27);
                Menu->SendTo(Plr);
            }break;


            ////////////////
            // Class trainer submenu
            ////////

        case 8: //Druid
            {
                SendQuickMenu(2593);
            }break;

        case 9: //Hunter
            {
                Plr->Gossip_SendPOI(-4203.65, -12467.7, 7, 7, 0, "Azure Watch, Hunter Trainer");
                SendQuickMenu(2593);
            }break;

        case 10: //Mage
            {
                Plr->Gossip_SendPOI(-4149.62, -12530.1, 7, 7, 0, "Azure Watch, Mage Trainer");
                SendQuickMenu(2593);
            }break;

        case 11: //Paladin
			{
                Plr->Gossip_SendPOI(-4138.98, -12468.5, 7, 7, 0, "Azure Watch, Paladin Trainer");
                SendQuickMenu(2593);
            }break;

        case 12: //Priest
            {
                Plr->Gossip_SendPOI(-4131.66, -12478.6, 7, 7, 0, "Azure Watch, Priest Trainer");
                SendQuickMenu(2593);
            }break;

        case 13: //Shaman
            {
                Plr->Gossip_SendPOI(-4162.33, -12456.1, 7, 7, 0, "Azure Watch, Shaman Trainer");
                SendQuickMenu(2593);
            }break;

        case 14: //Warrior
            {
                Plr->Gossip_SendPOI(-4165.05, -12536.4, 7, 7, 0, "Azure Watch, Warrior Trainer");
                SendQuickMenu(2593);
            }break;

        case 15: //Alchemy
            {
                Plr->Gossip_SendPOI(-4191.15, -12470, 7, 7, 0, "Azure Watch, Alchemist");
                SendQuickMenu(2593);
            }break;

        case 16: //Blacksmithing
            {
				Plr->Gossip_SendPOI(-4726.29, -12387.0, 7, 7, 0, "Odesyus' Landing, Blacksmith");
                SendQuickMenu(2593);
            }break;

        case 17: //Cooking
            {
				Plr->Gossip_SendPOI(-4708.59, -12400.3, 7, 7, 0, "Odesyus' Landing, Cook");
                SendQuickMenu(2593);
            }break;

        case 18: //Enchanting
            {
				SendQuickMenu(2593);
            }break;
			
        case 19: //Engineering
            {
                Plr->Gossip_SendPOI(-4157.57, -12470.2, 7, 7, 0, "Azure Watch, Engineering Trainer");
                SendQuickMenu(2593);
            }break;

        case 20: //First Aid
            {
                Plr->Gossip_SendPOI(-4199.1, -12469.9, 7, 7, 0, "Azure Watch, First Aid Trainer");
                SendQuickMenu(2593);
            }break;

        case 21: //Fishing
            {
                Plr->Gossip_SendPOI(-4266.34, -12985.4, 7, 7, 0, "Ammen Ford, Fisherwoman");
                SendQuickMenu(2593);
            }break;

        case 22: //Herbalism
            {
                Plr->Gossip_SendPOI(-4189.43, -12459.4, 7, 7, 0, "Azure Watch, Herbalist");
                SendQuickMenu(2593);
            }break;

        case 23: //Jewelcrafting
            {
                SendQuickMenu(2593);
            }break;

        case 24: //Leatherworking
            {
                Plr->Gossip_SendPOI(-3442.68, -12322.2, 7, 7, 0, "Stillpine Hold, Leatherworker");
                SendQuickMenu(2593);
            }break;

        case 25: //Mining
            {
				Plr->Gossip_SendPOI(-4179.89, -12493.1, 7, 7, 0, "Azure Watch, Mining Trainer");
                SendQuickMenu(2593);
            }break;

        case 26: //Skinning
            {
				Plr->Gossip_SendPOI(-3431.17, -12316.5, 7, 7, 0, "Stillpine Hold, Skinner");
                SendQuickMenu(2593);
            }break;

		case 27: //Tailoring
            {
				Plr->Gossip_SendPOI(-4711.54, -12386.7, 7, 7, 0, "Odesyus' Landing, Tailor");
                SendQuickMenu(2593);
            }break;
        }
    }
};

/*****************************************************************************************/
/* Shattrath Guards   original structure by AeThIs. Translated, updated and  by Pepsi1x1 */
/*****************************************************************************************/

class SCRIPT_DECL ShattrathGuard : public GossipScript
{
public:
	void Destroy()
	{
		delete this;
	}
    void GossipHello(Object* pObject, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10524, Plr);

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

        if(AutoSend)
            Menu->SendTo(Plr);
    }

   void GossipSelectOption(Object* pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
            GossipHello(pObject, Plr, true);
            break;

            //////////////////////
            // Menus
            /////

        case 1:     // World's End Tavern
            SendQuickMenu(10394);
            Plr->Gossip_SendPOI(-1760.4, 5166.9, 7, 7, 0, "World's End Tavern");
            break;

        case 2:     // Shattrath Banks
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10395, Plr);
                Menu->AddItem( 0, "Aldor Bank"          , 12);
                Menu->AddItem( 0, "Scryers Bank"         , 13);
                Menu->SendTo(Plr);
            }break;


        case 3:     // Inn's
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10398, Plr);
                Menu->AddItem( 0, "Aldor inn"          , 14);
                Menu->AddItem( 0, "Scryers inn"         , 15);
                Menu->SendTo(Plr);
            }break;

        case 4:     // Gryphon Master
            SendQuickMenu(10402);
            Plr->Gossip_SendPOI(-1831.9, 5298.2, 7, 7, 0, "Gryphon Master");
            break;

        case 5:     // Mailboxes
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10403, Plr);
                Menu->AddItem( 0, "Aldor inn"          , 16);
                Menu->AddItem( 0, "Scryers inn"         , 17);
				Menu->AddItem( 0, "Aldor Bank"          , 18);
                Menu->AddItem( 0, "Scryers Bank"         , 19);
                Menu->SendTo(Plr);
            }break;

        case 6:     // Stable Masters
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10404, Plr);
                Menu->AddItem( 0, "Aldor Stable"          , 20);
                Menu->AddItem( 0, "Scryers Stable"         , 21);
                Menu->SendTo(Plr);
            }break;

        case 7:     // Battlemasters
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10405, Plr);
                Menu->AddItem( 0, "Alliance Battlemasters"          , 22);
                Menu->AddItem( 0, "Horde & Arena Battlemasters"         , 23);
                Menu->SendTo(Plr);
            }break;

        case 8:     // Proffesion Trainers
			{
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10391, Plr);
                Menu->AddItem( 0, "Alchemy"          , 24);
                Menu->AddItem( 0, "Blacksmithing"         , 25);
				Menu->AddItem( 0, "Cooking"          , 26);
                Menu->AddItem( 0, "Enchanting"         , 27);
                Menu->AddItem( 0, "First Aid"          , 28);
                Menu->AddItem( 0, "Jewelcrafting"         , 29);
				Menu->AddItem( 0, "Leatherworking"          , 30);
                Menu->AddItem( 0, "Skinning"         , 31);
                Menu->SendTo(Plr);
            }break;

        case 9:     // Mana Loom
            SendQuickMenu(10408);
            Plr->Gossip_SendPOI(-2073.9, 5265.7, 7, 7, 0, "Mana Loom");
            break;

        case 10:    // Alchemy Lab
            SendQuickMenu(10409);
            Plr->Gossip_SendPOI(-1648.1, 5537.3, 7, 7, 0, "Alchemy Lab");
            break;

        case 11:    // Gem Merchants
            {
                objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 10410, Plr);
                Menu->AddItem( 0, "Aldor Gem Merchant"          , 32);
                Menu->AddItem( 0, "Scryer Gem Merchant"         , 33);
                Menu->SendTo(Plr);
            }break;


            //////////////////////
            // Banks
            /////


        case 12: //Aldor Bank
            {
                Plr->Gossip_SendPOI(-1730.8, 5496.2, 7, 7, 0, "Aldor Bank");
                SendQuickMenu(10396);
            }break;

        case 13: //Scryers Bank
            {
                Plr->Gossip_SendPOI(-1999.6, 5362.0, 7, 7, 0, "Scryers Bank");
                SendQuickMenu(10397);
            }break;


            //////////////////////
            // Inns
            /////


        case 14: //Aldor Inn
            {
                Plr->Gossip_SendPOI(-1897.5, 5767.6, 7, 7, 0, "Aldor inn");
                SendQuickMenu(10399);
            }break;

        case 15: //Scryers Inn
            {
                Plr->Gossip_SendPOI(-2180.6, 5403.9, 7, 7, 0, "Scryers inn");
                SendQuickMenu(10401);
            }break;


            //////////////////////
            // Mailboxes
            /////


        case 16: //Aldor Inn
            {
                Plr->Gossip_SendPOI(-1886.9, 5761.5, 7, 7, 0, "Aldor Inn");
                SendQuickMenu(10399);
            }break;

        case 17: //Scryers Bank
            {
                Plr->Gossip_SendPOI(-2175.0, 5411.7, 7, 7, 0, "Scryers Bank");
                SendQuickMenu(10397);
            }break;

        case 18: //Aldor Bank
            {
                Plr->Gossip_SendPOI(-1695.5, 5521.8, 7, 7, 0, "Aldor Bank");
                SendQuickMenu(10396);
            }break;

        case 19: //Scryers Inn
            {
                Plr->Gossip_SendPOI(-2033.0, 5336.1, 7, 7, 0, "Scryers Inn");
                SendQuickMenu(10401);
            }break;


            //////////////////////
            // Stable Masters
            /////


        case 20: //Aldor Stable Master
            {
                Plr->Gossip_SendPOI(-1889.6, 5761.5, 7, 7, 0, "Aldor Stable");
                SendQuickMenu(10399);
            }break;

        case 21: //Scryers Stable Master
            {
                Plr->Gossip_SendPOI(-2170.0, 5404.6, 7, 7, 0, "Scryers Stable");
                SendQuickMenu(10401);
            }break;


            //////////////////////
            // Battlemasters
            /////


        case 22: //Alliance Battlemaster
            {
                Plr->Gossip_SendPOI(-1831.9, 5298.2, 7, 7, 0, "Alliance Battlemasters");
                SendQuickMenu(10406);
            }break;

        case 23: //Horde Battle Master and Arena Battle Master
            {
                Plr->Gossip_SendPOI(-1968.7, 5262.2, 7, 7, 0, "Horde & Arena Battlemasters");
                SendQuickMenu(10407);
            }break;






            //////////////////////
            // Profession Trainers
            /////


        case 24: //Alchemy
            {
                Plr->Gossip_SendPOI(-1661.0, 5538, 7, 7, 0, "Alchemy Trainer");
                SendQuickMenu(10413);
            }break;

        case 25: //Blacksmithing
            {
                Plr->Gossip_SendPOI(-1847.7, 5230.3, 7, 7, 0, "Blacksmithing Trainer");
                SendQuickMenu(10400);
            }break;

        case 26: //Cooking
            {
                Plr->Gossip_SendPOI(-2067.4, 5316.5, 7, 7, 0, "Cooking Trainer");
                SendQuickMenu(10414);
            }break;

        case 27: //Enchanting
            {
                Plr->Gossip_SendPOI(-2278.5, 5567.7, 7, 7, 0, "Enchanting Trainer");
                SendQuickMenu(10415);
            }break;

        case 28: //First Aid
            {
                Plr->Gossip_SendPOI(-1592.0, 5263.7, 7, 7, 0, "First Aid Trainer");
                SendQuickMenu(10416);
            }break;

        case 29: //Jewelcrafting
            {
                Plr->Gossip_SendPOI(-1653.3, 5665.1, 7, 7, 0, "Jewelcrafting Trainer");
                SendQuickMenu(10417);
            }break;

        case 30: //Leatherworking
            {
                Plr->Gossip_SendPOI(-2060.9, 5256.6, 7, 7, 0, "Leatherworking Trainer");
                SendQuickMenu(10418);
            }break;

        case 31: //Skinning
            {
                Plr->Gossip_SendPOI(-2047.9, 5299.6, 7, 7, 0, "Skinning Trainer");
                SendQuickMenu(10419);
            }break;


            //////////////////////
            // Gem Merchants
            /////


        case 32: //Aldor gem merchant
            {
                Plr->Gossip_SendPOI(-1649.3, 5668.6, 7, 7, 0, "Aldor gem merchant");
                SendQuickMenu(10411);
            }break;

        case 33: //Scryers gem merchant
            {
                Plr->Gossip_SendPOI(-2193.9, 5422.1, 7, 7, 0, "Scryers gem merchant");
                SendQuickMenu(10412);
            }break;
/* Blizzard can eat a dick for now.
		case 34: //Stormwind Barber
			{
				Plr->Gossip_SendPOI(61.3, 65.8, 7, 7, 0, "Stormwind Barber");
				SendQuickMenu();
			}break;

		case 35: //Stormwind Harbor
			{
				Plr->Gossip_SendPOI(61.3, 65.8, 7, 7, 0, "Stormwind Harbor");
				SendQuickMenu();
			}break;
		
		case 36: //Ironforge Barber shop
			{
				Plr->Gossip_SendPOI(26.2, 51.2, 7, 7, 0, "Ironforge Barber");
				SendQuickMenu();
			}break;

		case 37: //Ironforge Inscription
			{
				Plr->Gossip_SendPOI(60.1, 44.0, 7, 7, 0, "Ironforge Inscription");
				SendQuickMenu();
			}break;

		case 38: //Darnassus Inscription
			{
				Plr->Gossip_SendPOI(58.1, 15.8, 7, 7, 0, "Darnassus Inscription");
				SendQuickMenu();
			}break;

		case 39: //Orgrimmar Barber
			{
				Plr->Gossip_SendPOI(47.3, 54.4, 7, 7, 0, "Orgrimmar Barber");
				SendQuickMenu();
			}break;

		case 40: //Orgrimmar Inscription
			{
				Plr->Gossip_SendPOI(56.9, 46.0, 7, 7, 0, "Orgrimmar Inscription");
				SendQuickMenu();
			}break;

		case 41: //Undercity Barber
			{
				Plr->Gossip_SendPOI(70.7, 46.8, 7, 7, 0, "Undercity Barber");
				SendQuickMenu();
			}break;

		case 42: //Undercity Inscription
			{
				Plr->Gossip_SendPOI(61.0, 58.2, 7, 7, 0, "Undercity Inscription");
				SendQuickMenu();
			}break;

		case 43: //Exodar Inscription
			{
				Plr->Gossip_SendPOI(39.7, 39.2, 7, 7, 0, "Exodar Inscription");
				SendQuickMenu();;
			}break;

		case 44: //Thunderbluff Inscription
			{
				Plr->Gossip_SendPOI(28.9, 21.5, 7, 7, 0, "Thunderbluf Inscription");
				SendQuickMenu();
			}break;
			
// Metopious Loreseeker + Lofwyr Le'Fleur + Joboba Mezbreaker + Illusionist Karina 
		case 45: //Arena
			{ 
				Plr->Gossip_SendPOI(54.8, 52.1, 7, 7, 0, "Dalaran Arena");
				SendQuickMenu();
			}

		case 46: //Bank
			{ 
				Plr->Gossip_SendPOI(32.7, 55.7, 7, 7, 0, "Dalaran Bank");
				SendQuickMenu();
			}		
		
		case 46: //Inn
			{ 
				Plr->Gossip_SendPOI(35.4, 56.9, 7, 7, 0, "Dalaran Inn");
				SendQuickMenu();
			}
			*/

        }
    }
};
void SetupGuardGossip(ScriptMgr * mgr)
{
	GossipScript * gold = (GossipScript*) new GoldshireGuard();
	GossipScript * sw = (GossipScript*) new StormwindGuard();
//	GossipScript * swh = (GossipScript*) new StormwindHarborGuard(); //Added in 3.0.2 Stormwind Harbor Guard
	GossipScript * darn = (GossipScript*) new DarnassusGuard();
	GossipScript * dol = (GossipScript*) new DolanaarGuard();
	GossipScript * blood = (GossipScript*) new BloodhoofGuard();
	GossipScript * razor = (GossipScript*) new RazorHillGuard();
	GossipScript * brill = (GossipScript*) new BrillGuard();
	GossipScript * irf = (GossipScript*) new IronforgeGuard();
	GossipScript * khar = (GossipScript*) new KharanosGuard();
	GossipScript * falcon = (GossipScript*) new FalconwingGuard();
	GossipScript * azure = (AzureWatchGuard*) new AzureWatchGuard();
	GossipScript * under = (GossipScript*) new UndercityGuard();
	GossipScript * silver = (SilvermoonGuard*) new SilvermoonGuard();
	GossipScript * exodar = (ExodarGuard*) new ExodarGuard();
	GossipScript * ogri = (OrgrimmarGuard*) new OrgrimmarGuard();
	GossipScript * thun = (ThunderbluffGuard*) new ThunderbluffGuard();
	GossipScript * shattr = (ShattrathGuard*) new ShattrathGuard();
	//GossipScript * dal = (GossipScript*) new DalaranGuard(); //Dalaran guards

    /* Guard List */
    mgr->register_gossip_script(1423,  gold);				// Stormwind Guard 
    mgr->register_gossip_script(68,    sw);					// Stormwind City Guard
    mgr->register_gossip_script(1976,  sw);					// Stormwind City Patroller
//	mgr->register_gossip_script(29712, swh);				// Stormwind Harbor Guard Added in 3.0.2
    mgr->register_gossip_script(4262,  darn);				// Darnassus Sentinel
    mgr->register_gossip_script(5624,  under);				// Undercity Guardian
    mgr->register_gossip_script(3571,  dol);				// Teldrassil Sentinel
	mgr->register_gossip_script(16222, silver);				// Silvermoon City Guardian
	mgr->register_gossip_script(16733, exodar);				// Exodar Peacekeeper
	mgr->register_gossip_script(20674, exodar);				// Shield of Velen
	mgr->register_gossip_script(3296, ogri);				// Orgrimmar Grunt
	mgr->register_gossip_script(3084, thun);				// Bluffwatcher
	mgr->register_gossip_script(3222, blood);				// Brave Wildrunner
	mgr->register_gossip_script(3224, blood);				// Brave Cloudmane
	mgr->register_gossip_script(3220, blood);				// Brave Darksky
	mgr->register_gossip_script(3219, blood);				// Brave Leaping Deer
	mgr->register_gossip_script(3217, blood);				// Brave Dawneagle
	mgr->register_gossip_script(3215, blood);				// Brave Strongbash
	mgr->register_gossip_script(3218, blood);				// Brave Swiftwind
	mgr->register_gossip_script(3221, blood);				// Brave Rockhorn
	mgr->register_gossip_script(3223, blood);				// Brave Rainchaser
	mgr->register_gossip_script(3212, blood);				// Brave Ironhorn
	mgr->register_gossip_script(5953, razor);				// Razor Hill Grunt
	mgr->register_gossip_script(5725, brill);				// Deathguard Lundmark
	mgr->register_gossip_script(1738, brill);				// Deathguard Terrence
	mgr->register_gossip_script(1652, brill);				// Deathguard Burgess
	mgr->register_gossip_script(1746, brill);				// Deathguard Cyrus
	mgr->register_gossip_script(1745, brill);				// Deathguard Morris
	mgr->register_gossip_script(1743, brill);				// Deathguard Lawrence
	mgr->register_gossip_script(1744, brill);				// Deathguard Mort
	mgr->register_gossip_script(1496, brill);				// Deathguard Dillinger
	mgr->register_gossip_script(1742, brill);				// Deathguard Bartholomew
	mgr->register_gossip_script(5595, irf);					// Ironforge Guard
	mgr->register_gossip_script(727,  khar);				// Ironforge Mountaineer
	mgr->register_gossip_script(16221,falcon);				// Silvermoon Guardian
	mgr->register_gossip_script(18038,azure);				// Azuremyst Peacekeeper
	mgr->register_gossip_script(19687,shattr);			    // Shattrath City Guard -by AeThIs
    mgr->register_gossip_script(18568,shattr);			    // Shattrath City Guard Aruspice -by AeThIs
	///////////////////////////// WOTLK //////////////////////
	/*
    mgr->register_gossip_script(,dal);				// Metopious Loreseeker - Dalaran Sewers
	mgr->register_gossip_script(,dal);				// Lofwyr Le'Fleur - Dalaran Sewers
	mgr->register_gossip_script(,dal);				// Joboba Mezbreaker - Dalaran Sewers
	mgr->register_gossip_script(,dal);			    // Illusionist Karina - Dalaran Sewers
	mgr->register_gossip_script(32685,dal);			// Kiz Proudbreeze - Dalaran
	Fuck this for now, I'll collect all my data then spend two days doing all these scripts.
	I'm going to re-write Goldshire when I finish, it's incorrect.

	TODO:

	Dalaran Guards, informants.
	Guards from other areas of Northrend, forget the name atm.
	*/
}	
	
	// To Bloodhoof Guards - I don't know if those are all guards with dialog menu,
	// but they were all I could find. Same to Deathguards. 

	// Completed:
	//  - Add new guards added in 3.0.2 - DONE
	//  - Add new gossip menus to guards, with POI - DONE

	// ToDo:
	// Verify that we've covered every Guard gossip menu

