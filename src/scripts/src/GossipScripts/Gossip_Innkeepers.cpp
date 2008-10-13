/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
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

class SCRIPT_DECL InnkeeperGossip : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()
	{
		delete this;
	}
};

void InnkeeperGossip::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
    GossipMenu *Menu;
    uint32 TextID = 820;
    // Hello (First) Menu.
    objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), TextID, Plr);

    if( pObject->GetUInt32Value(UNIT_NPC_FLAGS) & UNIT_NPC_FLAG_VENDOR )
        Menu->AddItem( 1, "I would like to browse your goods.", 1 );
    
    Menu->AddItem( 5, "Make this inn your home.", 2 );
    Menu->AddItem( 0, "What can I do at an inn?", 3 );
    if(AutoSend)
        Menu->SendTo(Plr);
}

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), textid, Plr); \
    Menu->SendTo(Plr);

void InnkeeperGossip::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
	Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
	if(pCreature==NULL)
		return;

    switch(IntId)
    {
    case 1:     // VENDOR
        Plr->GetSession()->SendInventoryList(pCreature);
        break;
    case 2:     // BINDER
        Plr->GetSession()->SendInnkeeperBind(pCreature);
        break;
    case 3:     // WHAT CAN I DO ?
        // Prepare second menu
        GossipMenu *Menu;
        objmgr.CreateGossipMenuForPlayer(&Menu, pCreature->GetGUID(), 1853, Plr);
        Menu->AddItem(5, "Make this inn your home.", 2);
        Menu->SendTo(Plr);
        break;
    }
}

void InnkeeperGossip::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupInnkeepers(ScriptMgr * mgr)
{
	GossipScript * gs = (GossipScript*) new InnkeeperGossip();
    /* Innkeeper List */
    mgr->register_gossip_script(15174, gs);      //Calandrath
    mgr->register_gossip_script(18251, gs);      //Caregiver Abidaar
    mgr->register_gossip_script(16739, gs);      //Caregiver Breel
    mgr->register_gossip_script(16553, gs);      //Caregiver Chellan
    mgr->register_gossip_script(18914, gs);      //Caregiver Isel
    mgr->register_gossip_script(18906, gs);      //Caregiver Ophera Windfury
    mgr->register_gossip_script(17553, gs);      //Caregiver Topher Loaal
    mgr->register_gossip_script(21746, gs);      //Caretaker Aluuro
    mgr->register_gossip_script(19352, gs);      //Dreg Cloudsweeper
    mgr->register_gossip_script(19531, gs);      //Eyonix
    mgr->register_gossip_script(21110, gs);      //Fizit "Doc" Clocktock
    mgr->register_gossip_script(16602, gs);      //Floyd Pinkus
    mgr->register_gossip_script(19470, gs);      //Gholah
    mgr->register_gossip_script(23143, gs);      //Horus
    mgr->register_gossip_script(11116, gs);      //Innkeeper Abeqwa
    mgr->register_gossip_script(9501, gs);      //Innkeeper Adegwa
    mgr->register_gossip_script(22922, gs);      //Innkeeper Aelerya
    mgr->register_gossip_script(6740, gs);      //Innkeeper Allison
    mgr->register_gossip_script(2352, gs);      //Innkeeper Anderson
    mgr->register_gossip_script(6739, gs);      //Innkeeper Bates
    mgr->register_gossip_script(18905, gs);      //Innkeeper Bazil Olof'tazun
    mgr->register_gossip_script(1247, gs);      //Innkeeper Belm
    mgr->register_gossip_script(19296, gs);      //Innkeeper Biribi
    mgr->register_gossip_script(3934, gs);      //Innkeeper Boorand Plainswind
    mgr->register_gossip_script(6727, gs);      //Innkeeper Brianna
    mgr->register_gossip_script(7714, gs);      //Innkeeper Byula
    mgr->register_gossip_script(18907, gs);      //Innkeeper Coryth Stoktron
    mgr->register_gossip_script(19319, gs);      //Innkeeper Darg Bloodclaw
    mgr->register_gossip_script(15433, gs);      //Innkeeper Delaniel
    mgr->register_gossip_script(16458, gs);      //Innkeeper Faralia
    mgr->register_gossip_script(295, gs);      //Innkeeper Farley
    mgr->register_gossip_script(5111, gs);      //Innkeeper Firebrew
    mgr->register_gossip_script(7733, gs);      //Innkeeper Fizzgrimble
    mgr->register_gossip_script(7737, gs);      //Innkeeper Greul
    mgr->register_gossip_script(18957, gs);      //Innkeeper Grilka
    mgr->register_gossip_script(6928, gs);      //Innkeeper Grosk
    mgr->register_gossip_script(6929, gs);      //Innkeeper Gryshka
    mgr->register_gossip_script(19232, gs);      //Innkeeper Haelthol
    mgr->register_gossip_script(6734, gs);      //Innkeeper Hearthstove
    mgr->register_gossip_script(8931, gs);      //Innkeeper Heather
    mgr->register_gossip_script(1464, gs);      //Innkeeper Helbrek
    mgr->register_gossip_script(6272, gs);      //Innkeeper Janene
    mgr->register_gossip_script(7731, gs);      //Innkeeper Jayka
    mgr->register_gossip_script(17630, gs);      //Innkeeper Jovia
    mgr->register_gossip_script(16542, gs);      //Innkeeper Kalarin
    mgr->register_gossip_script(6930, gs);      //Innkeeper Karakul
    mgr->register_gossip_script(6747, gs);      //Innkeeper Kauth
    mgr->register_gossip_script(12196, gs);      //Innkeeper Kaylisk
    mgr->register_gossip_script(6736, gs);      //Innkeeper Keldamyr
    mgr->register_gossip_script(18908, gs);      //Innkeeper Kerp
    mgr->register_gossip_script(6738, gs);      //Innkeeper Kimlya
    mgr->register_gossip_script(11103, gs);      //Innkeeper Lyshaerya
    mgr->register_gossip_script(6741, gs);      //Innkeeper Norman
    mgr->register_gossip_script(6746, gs);      //Innkeeper Pala
    mgr->register_gossip_script(19571, gs);      //Innkeeper Remi Dodoso
    mgr->register_gossip_script(5688, gs);      //Innkeeper Renee
    mgr->register_gossip_script(6735, gs);      //Innkeeper Saelienne
    mgr->register_gossip_script(19495, gs);      //Innkeeper Shaunessy
    mgr->register_gossip_script(6737, gs);      //Innkeeper Shaussiy
    mgr->register_gossip_script(2388, gs);      //Innkeeper Shay
    mgr->register_gossip_script(9356, gs);      //Innkeeper Shul'kar
    mgr->register_gossip_script(7736, gs);      //Innkeeper Shyria
    mgr->register_gossip_script(11106, gs);      //Innkeeper Sikewa
    mgr->register_gossip_script(6807, gs);      //Innkeeper Skindle
    mgr->register_gossip_script(5814, gs);      //Innkeeper Thulbek
    mgr->register_gossip_script(7744, gs);      //Innkeeper Thulfram
    mgr->register_gossip_script(6790, gs);      //Innkeeper Trelayne
    mgr->register_gossip_script(16618, gs);      //Innkeeper Velandra
    mgr->register_gossip_script(11118, gs);      //Innkeeper Vizzie
    mgr->register_gossip_script(6791, gs);      //Innkeeper Wiley
    mgr->register_gossip_script(16256, gs);      //Jessica Chambers
    mgr->register_gossip_script(14731, gs);      //Lard
    mgr->register_gossip_script(15397, gs);      //Marniel Amberlight
    mgr->register_gossip_script(18913, gs);      //Matron Tikkit
    mgr->register_gossip_script(21088, gs);      //Matron Varah
    mgr->register_gossip_script(6778, gs);      //Melika Isenstrider
    mgr->register_gossip_script(18245, gs);      //Merajit
    mgr->register_gossip_script(19046, gs);      //Minalei
    mgr->register_gossip_script(21744, gs);      //Roldemar
    mgr->register_gossip_script(16826, gs);      //Sid Limbardi
    mgr->register_gossip_script(6806, gs);      //Tannok Frosthammer

    //cleanup:
    //added 36 new Innkeeper's ,81 working innkeeper's now :P
    //removed Innkeeper Monica(she dos not have gossip option she is from Old Hillsbrad Foothills)
}
