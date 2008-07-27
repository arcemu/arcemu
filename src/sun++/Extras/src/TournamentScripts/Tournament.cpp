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

#include "StdAfx.h"
#include "Setup.h"

class SCRIPT_DECL Teleporter : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {
/*
INSERT INTO `npc_text` (`entry`,`prob0`,`text0_0`,`text0_1`,`text1_0`,`text1_1`,`text2_0`,`text2_1`,`text3_0`,`text3_1`,`text4_0`,`text4_1`,`text5_0`,`text5_1`,`text6_0`,`text6_1`,`text7_0`,`text7_1`) VALUES ('9999','1','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.','I can teleport you to another race\'s zone.')
*/
        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();

        if(Team == 0)
        {
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9999, plr);
			
			Menu->AddItem( 0, "Draenei", 0);
			Menu->AddItem( 0, "Dwarf & Gnome", 1);
			Menu->AddItem( 0, "Human", 2);
			Menu->AddItem( 0, "Night Elf", 3);
        }
        else if(Team == 1)
        {
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9999, plr);

			Menu->AddItem( 0, "Blood Elf", 4);
			Menu->AddItem( 0, "Orc & Troll", 5);
			Menu->AddItem( 0, "Taureen", 6);
			Menu->AddItem( 0, "Undead", 7);
        }
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;

/*
mysql> select DISTINCT(race),mapid,positionx,positiony,positionz from playercreateinfo;
+------+-------+-----------+-----------+-----------+
| race | mapid | positionx | positiony | positionz |
+------+-------+-----------+-----------+-----------+
|    1 |     0 |  -8949.95 |  -132.493 |   83.5312 | Human
|    2 |     1 |  -618.518 |  -4251.67 |    38.718 | Orc
|    3 |     0 |  -6240.32 |   331.033 |   382.758 | Dwarf
|    4 |     1 |   10311.3 |   832.463 |   1326.41 | Night
|    5 |     0 |   1676.35 |   1677.45 |    121.67 | Undead
|    6 |     1 |  -2917.58 |   -257.98 |   52.9968 | Tauren
|    7 |     0 |  -6237.02 |   329.659 |   382.703 | Gnome
|    8 |     1 |  -618.518 |  -4251.67 |    38.718 | Troll
|   11 |   530 |  -3961.64 |  -13931.2 |   100.615 | Draenei
|   10 |   530 |   10349.6 |  -6357.29 |   33.4026 | Blood Elf
+------+-------+-----------+-----------+-----------+
*/
		switch(IntId)
		{
		case 0: //Draenei
			plr->SafeTeleport(530, 0, -3961.64f, -13931.2f, 100.615f, 1.0f);
			break;
		case 1: //Dwarf & Gnome
			plr->SafeTeleport(0, 0, -6240.32f, 331.033f, 382.758f, 1.0f);
			break;
		case 2: //Human
			plr->SafeTeleport(0, 0, -8949.95f, -132.493f, 83.5312f, 1.0f);
			break;
		case 3: //Night Elf
			plr->SafeTeleport(1, 0, 10311.3f, 832.463f, 1326.41f, 1.0f);
			break;
		case 4: //Blood Elf
			plr->SafeTeleport(530, 0, 10349.6f, -6357.29f, 33.4026f, 1.0f);
			break;
		case 5: //Orc & Troll
			plr->SafeTeleport(1, 0, -618.518f, -4251.67f, 38.718f, 1.0f);
			break;
		case 6: //Taureen
			plr->SafeTeleport(1, 0, -2917.58f, -257.98f, 52.9968f, 1.0f);
			break;
		case 7: //Undead
			plr->SafeTeleport(0, 0, 1676.35f, 1677.45f, 121.67f, 1.0f);
			break;
		}
    }

    void Destroy()
    {
        delete this;
    }
};

class SCRIPT_DECL Paymaster : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {

        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9998, plr);			
		Menu->AddItem( 0, "5,000 Gold", 0);
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;

		switch(IntId)
		{
			case 0:
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, 50000000); //Using Set instead of Mod, thats how blizz does it
				break;
		}
    }

	void GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

    void Destroy()
    {
        delete this;
    }
};

class SCRIPT_DECL AccessoriesVendor : public GossipScript
{
public:
    void GossipHello(Object* pObject, Player * plr, bool AutoSend)
    {

        GossipMenu *Menu;
        uint32 Team = plr->GetTeam();

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9998, plr);			
		Menu->AddItem( 0, "5,000 Gold", 0);
        
        if(AutoSend)
            Menu->SendTo(plr);
    }

    void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
    {
		Creature * pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?((Creature*)pObject):NULL;
		if(pCreature==NULL)
			return;

		switch(IntId)
		{
			case 0:
				plr->SetUInt32Value(PLAYER_FIELD_COINAGE, 50000000); //Using Set instead of Mod, thats how blizz does it
				break;
		}
    }

	void GossipEnd(Object * pObject, Player* Plr)
	{
		GossipScript::GossipEnd(pObject, Plr);
	}

    void Destroy()
    {
        delete this;
    }
};

void SetupTournament(ScriptMgr * mgr)
{
	GossipScript * teleNpc = (GossipScript*) new Teleporter;
    mgr->register_gossip_script(26367, teleNpc);
    
    GossipScript * payNpc = (GossipScript*) new Paymaster;
    mgr->register_gossip_script(26075, payNpc);

    GossipScript * accVen = (GossipScript*) new AccessoriesVendor;
    mgr->register_gossip_script(26300, accVen);
}