/********************************************************************
  Artifact Emulation Services
  All Services Are Provided by Free Use of Services.
  Productions by ArcEmu are only mend for use of game play.
  
  The game services that ArcEmu provides are only for exploration
  and not of use to be a full striving game server replacement.
  For this use please play on the official services of Blizzard.

  The uses above are not ment to be released for those services
  since this is in the state of internet piracy.

  ArcEmu only provides the services on voluntary base and is not
  willing to provide the sources to secondary parties.

  All Copyrights and Trademarks are under name of ArcEmu

  Copyrights (c) 2008 ArcEmu
********************************************************************/

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL TeleportNPC : public GossipScript
{
public:
    ADD_GOSSIP_FACTORY_FUNCTION(TeleportNPC);
    void GossipHello(Creature * pCreature, Player * Plr, bool AutoSend)
    {
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1, Plr);

		Menu->AddItem(5, "Set Hearthstone here", 98);
		if (Plr->GetTeam() > 0) 
		{ 
		Menu->AddItem(0, "Horde Cities", 1);
	/*	UNCOMMENT THE LINE BELOW THIS IF YOU WANT IT TO WORK */
	//Menu->Additem(0, "Horde Mall", 2);
		} 
		else
		{ Menu->AddItem(0, "Alliance Cities", 3); }
        Menu->AddItem(0, "Outland Locations", 4);
	/*	UNCOMMENT THE LINE BELOW THIS IF YOU WANT IT TO WORK */
	//Menu->Additem(0, "Alliance Mall", 5);
        if(AutoSend)
            Menu->SendTo(Plr);
    }

    void GossipSelectOption(Creature* pCreature, Player* Plr, uint32 Id, uint32 IntId)
    {
        GossipMenu * Menu;
        switch(IntId)
        {
        case 0:     // Return to start
				GossipHello(pCreature, Plr, true);
            break;

		case 98:     // BINDER
				Plr->GetSession()->SendInnkeeperBind(pCreature);
			break;

        case 1:     // Horde
                objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Silvermoon", 6);
                Menu->AddItem(5, "Orgrimmar", 7);
                Menu->AddItem(5, "Thunderbluff", 8);
                Menu->AddItem(5, "UnderCity", 9);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;

		case 3:     // Alliance
                objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1, Plr);
                Menu->AddItem(5, "The Exodar", 10);
                Menu->AddItem(5, "Stormwind", 11);
                Menu->AddItem(5, "Ironforge", 12);
                Menu->AddItem(5, "Darnassus", 13);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
            break;

        case 4:     // Outland
	        if(Plr->getLevel() < 58)
	        {
				pCreature->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You do not qualify for entry to Outland." );
                Plr->Gossip_Complete();
			}else{
                objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1, Plr);
                Menu->AddItem(5, "Hellfire Peninsula", 14);
                Menu->AddItem(5, "Zangermarsh", 15);
                Menu->AddItem(5, "Nagrand", 16);
                Menu->AddItem(5, "Blades Edge Mountains", 17);
                Menu->AddItem(5, "Netherstorm", 18);
                Menu->AddItem(5, "Terokkar Forest", 19);
                Menu->AddItem(5, "Shadowmoon Valley", 20);
				Menu->AddItem(0, "[Back]", 99);
                Menu->SendTo(Plr);
			}
            break;

            //////////////////
            // Horde submenu
            ////////

        case 6://Silvermoon
            {
                Plr->EventTeleport(530, 9400.486328, -7278.376953, 14.206780);

            }break;

        case 7://Orgrimmar
            {
                Plr->EventTeleport(1, 1371.068970, -4370.801758, 26.052483);
            }break;

        case 8://ThunderBluff
            {
                Plr->EventTeleport(1, -1304.569946, 205.285004, 68.681396);
            }break;

		case 9://UnderCity
            {
                Plr->EventTeleport(0, 2050.203125, 285.650604, 56.994549);
            }break;

            ////////////////
            // Alliance Menu
            ////////

        case 10: //Exodar
            {
                Plr->EventTeleport(530, -4072.202393, -12014.337891, -1.277277);
            }break;

        case 11: //Stormwind
            {
                Plr->EventTeleport(0, -9100.480469, 406.950745, 92.594185);
            }break;

        case 12: //Ironforge
            {
                Plr->EventTeleport(0, -5028.265137, -825.976563, 495.301575);
            }break;

        case 13: //Darnassus
            {
                Plr->EventTeleport(1, 9985.907227, 1971.155640, 1326.815674);
            }break;

            ////////////////
            // Outland Menu
            ////////

        case 14: //Hellfire Peninsula
            {
                Plr->EventTeleport(530, -248.160004, 922.348999, 84.379799);
            }break;

        case 15: //Zangermarsh
            {
                Plr->EventTeleport(530, -225.863632, 5405.927246, 22.346397);
            }break;

        case 16: //Nagrand
            {
                Plr->EventTeleport(530, -468.232330, 8418.666016, 28.031298);

            }break;

        case 17: //Blades Edge Mountains
            {
                Plr->EventTeleport(530, 1471.672852, 6828.047852, 107.759239);

            }break;

        case 18: //Netherstorm
            {
                Plr->EventTeleport(530, 3396.123779, 4182.208008, 137.097992);

            }break;

        case 19: //Terokkar Forest
            {
                Plr->EventTeleport(530, -1202.426636, 5313.692871, 33.774723);

            }break;

        case 20: //Shadowmoon Valley
            {
                Plr->EventTeleport(530, -2859.522461, 3182.34773, 10.008426);

				}break;
/*	UNCOMMENT THIS IF YOU WANT IT TO WORK = MapID, X, Y, Z */

	/*
		case 2: //Horde Mall
            {
                Plr->EventTeleport(1, X, Y, Z);

				}break;
	*/


	/*	UNCOMMENT THE LINE BELOW THIS IF YOU WANT IT TO WORK = MapID, X, Y, Z */

	/*
		case 5: //Alliance Mall
          {
              Plr->EventTeleport(0, X, Y, Z);
	
				}break;

	*/
       		case 99: //main menu
			{
				objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1, Plr);

			Menu->AddItem(5, "Set Hearthstone here", 98);
			if (Plr->GetTeam() > 0) 
			{ 
			Menu->AddItem(0, "Horde Cities", 1);
		/* Menu->Additem(0, "Horde Mall", 2); Custom teleport location Horde
			} 
			else
			{ Menu->AddItem(0, "Alliance Cities", 3); }
			Menu->AddItem(0, "Outland Locations", 4);
		/* Menu->Additem(0, "Alliance Mall", 5); Custom teleport location Alliance */
			if(AutoSend)
            Menu->SendTo(Plr);
			}

        }
    }
};

void SetupCustom_Teleporters(ScriptMgr * mgr)
{
    /* Teleporter List */
    mgr->register_gossip_script(30001, &TeleportNPC::Create);          // Osciron
}

// Be sure to make an npc with the id of 30001 =D
