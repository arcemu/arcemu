/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team 
 * Copyright (C) 2007 Moon++ Team <http://www.moonplusplus.com/>
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

class SCRIPT_DECL WarsongGulchBattlemaster : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();
        if(Team > 1) Team = 1;
        
        // Check if the player can be entered into the bg or not.
        if(plr->getLevel() < 10)
        {
            uint32 FactMessages[2] = { 7599, 7688 };

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
        }
        else
        {
            uint32 FactMessages[2] = { 7689, 7705 }; // need to find the second one

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
            Menu->AddItem( 0, "I would like to enter the battleground.", 1);
        }
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
        // Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

        plr->GetSession()->SendBattlegroundList(((Creature*)pObject), 2);  // WSG = 2
    }

    void Destroy()
    {
        delete this;
    }
};

class SCRIPT_DECL ArathiBasinBattlemaster : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();
        if(Team > 1) Team = 1;

        // Check if the player can be entered into the bg or not.
        if(plr->getLevel() < 20)
        {
            uint32 FactMessages[2] = { 7700, 7667 };

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
        }
        else
        {
            uint32 FactMessages[2] = { 7700, 7555 }; // need to find the second one

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
            Menu->AddItem( 0, "I would like to enter the battleground.", 1);
        }

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		// Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		plr->GetSession()->SendBattlegroundList(((Creature*)pObject), 3);  // AB = 3
    }

    void Destroy()
    {
        delete this;
    }
};

class SCRIPT_DECL AlteracValleyBattlemaster : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();
        if(Team > 1) Team = 1;

        // Check if the player can be entered into the bg or not.
        if(plr->getLevel() < 60)
        {
            uint32 FactMessages[2] = { 7658, 7658 };

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
        }
        else
        {
            uint32 FactMessages[2] = { 7658, 7659 }; // need to find the second one

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
            Menu->AddItem( 0, "I would like to enter the battleground.", 1);
        }

        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		// Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

		plr->GetSession()->SendBattlegroundList(((Creature*)pObject), 1);  // AV = 1
    }

    void Destroy()
    {
        delete this;
    }
};

class SCRIPT_DECL EyeOfTheStormBattlemaster : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();
        if(Team > 1) Team = 1;
        
        // Check if the player can be entered into the bg or not.
        if(plr->getLevel() < 70)
        {
            uint32 FactMessages[2] = { 7599, 7688 };

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
        }
        else
        {
            uint32 FactMessages[2] = { 7689, 7705 }; // need to find the second one

            // Send "you cannot enter" message.
            objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), FactMessages[Team], plr);
            Menu->AddItem( 0, "I would like to enter the battleground.", 1);
        }
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
        // Send battleground list.
		if(pObject->GetTypeId()!=TYPEID_UNIT)
			return;

        plr->GetSession()->SendBattlegroundList(((Creature*)pObject), 7);  // EOTS = 7
    }

    void Destroy()
    {
        delete this;
    }
};

void SetupBattlemaster(ScriptMgr * mgr)
{
	GossipScript * wsg = (GossipScript*) new WarsongGulchBattlemaster;
	GossipScript * ab = (GossipScript*) new ArathiBasinBattlemaster;
	GossipScript * av = (GossipScript*) new AlteracValleyBattlemaster;
	GossipScript * eots = (GossipScript*) new EyeOfTheStormBattlemaster;

    /* Battlemaster List */
    mgr->register_gossip_script(19910, wsg); // Gargok
    mgr->register_gossip_script(15105, wsg); // Warsong Emissary
    mgr->register_gossip_script(20118, wsg); // Jihi
    mgr->register_gossip_script(16696, wsg); // Krukk
    mgr->register_gossip_script(2804, wsg);  // Kurden Bloodclaw
    mgr->register_gossip_script(20272, wsg); // Lylandor
    mgr->register_gossip_script(20269, wsg); // Montok Redhands
    mgr->register_gossip_script(19908, wsg); // Su'ura Swiftarrow
    mgr->register_gossip_script(15102, wsg); // Silverwing Emissary
    mgr->register_gossip_script(14981, wsg); // Elfarran
    mgr->register_gossip_script(14982, wsg); // Lylandris
    mgr->register_gossip_script(2302, wsg);  // Aethalas
    mgr->register_gossip_script(10360, wsg); // Kergul Bloodaxe
    mgr->register_gossip_script(3890, wsg);  // Brakgul Deathbringer
    mgr->register_gossip_script(20273, ab); // Adam Eternum
    mgr->register_gossip_script(16694, ab); // Karen Wentworth
    mgr->register_gossip_script(20274, ab); // Keldor the Lost
    mgr->register_gossip_script(15007, ab); // Sir Malory Wheeler
    mgr->register_gossip_script(19855, ab); // Sir Maximus Adams
    mgr->register_gossip_script(19905, ab); // The Black Bride
    mgr->register_gossip_script(20120, ab); // Tolo
    mgr->register_gossip_script(15008, ab); // Lady Hoteshem
    mgr->register_gossip_script(857, ab);   // Donald Osgood
    mgr->register_gossip_script(907, ab);   // Keras Wolfheart
    mgr->register_gossip_script(12198, ab); // Martin Lindsev
    mgr->register_gossip_script(14990, ab); // Defilers Emissary
    mgr->register_gossip_script(15006, ab); // Deze Snowbane
    mgr->register_gossip_script(14991, ab); // League of Arathor Emissary
    mgr->register_gossip_script(347, av);   // Grizzle Halfmane
    mgr->register_gossip_script(19907, av); // Grumbol Grimhammer
    mgr->register_gossip_script(16695, av); // Gurak
    mgr->register_gossip_script(20271, av); // Haelga Slatefist
    mgr->register_gossip_script(20119, av); // Mahul
    mgr->register_gossip_script(19906, av); // Usha Eyegouge
    mgr->register_gossip_script(20276, av); // Wolf-Sister Maka
    mgr->register_gossip_script(7410, av);  // Thelman Slatefist
    mgr->register_gossip_script(12197, av); // Glordrum Steelbeard
    mgr->register_gossip_script(5118, av);  // Brogun Stoneshield
    mgr->register_gossip_script(15106, av); // Frostwolf Emissary
    mgr->register_gossip_script(15103, av); // Stormpike Emissary
    mgr->register_gossip_script(14942, av); // Kartra Bloodsnarl
	mgr->register_gossip_script(20388, eots); // Althallen Brightblade
	mgr->register_gossip_script(20385, eots); // Andrissa Heartspear
	mgr->register_gossip_script(20390, eots); // Duyash the Cruel
	mgr->register_gossip_script(20383, eots); // Enlae
	mgr->register_gossip_script(22013, eots); // Eye of the Storm Emissary
	mgr->register_gossip_script(22015, eots); // Eye of the Storm Envoy
	mgr->register_gossip_script(20362, eots); // Iravar
	mgr->register_gossip_script(20381, eots); // Jovil
	mgr->register_gossip_script(20374, eots); // Kandaar
	mgr->register_gossip_script(20386, eots); // Lyrlia Blackshield
	mgr->register_gossip_script(20382, eots); // Mitia
	mgr->register_gossip_script(20384, eots); // Yula the Fair

   //cleanup:
   //removed Sandfury Soul Eater(hes a npc in Zul'Farrak and has noting to do whit the battleground masters) 
   //added Warsong Emissary, Stormpike Emissary , League of Arathor Emissary
}
