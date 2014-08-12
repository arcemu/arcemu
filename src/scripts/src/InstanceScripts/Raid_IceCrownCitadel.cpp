/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2014 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "Setup.h"
#include "Raid_IceCrownCitadel.h"

#define MAP_ICECROWNCITADEL 631
////////////////////////////////////////////////////////
//ICC zone: 4812
//Prepared creature entry:
//
//#define CN_LORD_MARROWGAR           36612
//#define CN_LADY_DEATHWHISPER        36855
//#define CN_DEATHBRINGER_SAURFANG    37813
//#define CN_FESTERGURT               36626
//#define CN_ROTFACE                  36627
//#define CN_PROFESSOR_PUTRICIDE      36678
//#define CN_PRINCE_VALANAR           37970
//#define CN_BLOOD_QUEEN_LANATHEL     37955
//#define CN_VALITHRIA_DREAMWALKER    36789
//#define CN_SINDRAGOSA               36853
//#define CN_THE_LICHKING             36597
//
//ToDo: start boss scripts
////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//IceCrownCitadel Instance

class IceCrownCitadelScript : public MoonInstanceScript
{
public:
	MOONSCRIPT_INSTANCE_FACTORY_FUNCTION(IceCrownCitadelScript, MoonInstanceScript);
	IceCrownCitadelScript(MapMgr* pMapMgr) : MoonInstanceScript(pMapMgr) {};
	
	/*bool NPCData
	Creature.h looks good to realize teleport intrusion
	CREATURE_STATE_ALIVE = 0,			// no special death state
    CREATURE_STATE_APPEAR_DEAD = 1,		// these creatures are actually alive but appears as dead for client
    CREATURE_STATE_DEAD = 2				// these creatures are dead*/

};

// IceCrown Teleporter
static float ICCTeleCoords[6][5] =
{	// Maybe moving this to a clean mysql table with all teleports in it... maybe as spell or event?
	{ 631, -17.856115f, 2211.640137f, 30.115812f, 0.0f },     //1   Teleport to Light's Hammer 
	{ 631, -503.632599f, 2211.219971f, 62.823246f, 0.0f },    //2   Teleport to Oratory of The Damned
	{ 631, -615.098267f, 2211.509766f, 199.973083f, 0.0f },   //3   Teleport to Rampart of Skulls 
	{ 631, -549.151001f, 2211.463967f, 539.290222f, 0.0f },   //4   Teleport to Deathbringer's Rise 
	{ 631, 4356.780273f, 2863.636230f, 349.337982f, 0.0f },   //5   Teleport to the Upper Spire.
	{ 631, 4453.248535f, 2769.325684f, 349.347473f, 0.0f }    //6   Teleport to Sindragosa's Lair
};

class ICCTeleporterAI : public GameObjectAIScript
{
public:

	ICCTeleporterAI(GameObject* go) : GameObjectAIScript(go){}

	~ICCTeleporterAI() {}

	static GameObjectAIScript* Create(GameObject* go) { return new ICCTeleporterAI(go); }

	void OnActivate(Player* player)
	{
		GossipMenu* menu = NULL;
		objmgr.CreateGossipMenuForPlayer(&menu, _gameobject->GetGUID(), 1/*Its not one need to be checked*/, player);

		menu->AddItem(Arcemu::Gossip::ICON_CHAT, "Teleport to Light's Hammer.", 0);

		// need to be checked, if npc is death...
		// CN_LORD_MARROWGAR killed?
		menu->AddItem(Arcemu::Gossip::ICON_CHAT, "Teleport to Oratory of The Damned.", 1);
		// CN_LADY_DEATHWHISPER  killed?
		menu->AddItem(Arcemu::Gossip::ICON_CHAT, "Teleport to Rampart of Skulls.", 2);
		// Who has to be killed?
		menu->AddItem(Arcemu::Gossip::ICON_CHAT, "Teleport to Deathbringer's Rise.", 3);
		// Who has to be killed?
		menu->AddItem(Arcemu::Gossip::ICON_CHAT, "Teleport to the Upper Spire.", 4);
		// CN_VALITHRIA_DREAMWALKER killed?
		menu->AddItem(Arcemu::Gossip::ICON_CHAT, "Teleport to Sindragosa's Lair.", 5);

		menu->SendTo(player);
	}
};

class ICCTeleporterGossip : public GossipScript
{
public:
	ICCTeleporterGossip() : GossipScript(){}

	void OnSelectOption(Object* object, Player* player, uint32 Id, const char* enteredcode)
	{
		Arcemu::Gossip::Menu::Complete(player);

		if(Id >= 6) //if menuitem bigger or 6 then somethin went completly wrong...
			return;
		else
			player->SafeTeleport(ICCTeleCoords[Id][0], player->GetInstanceID(), ICCTeleCoords[Id][1], ICCTeleCoords[Id][2], ICCTeleCoords[Id][3], ICCTeleCoords[Id][4]);
	}
};

void SetupICC(ScriptMgr* mgr)
{
	//Bosses
	mgr->register_instance_script(MAP_ICECROWNCITADEL, &IceCrownCitadelScript::Create);
	//Teleporters
	mgr->register_gameobject_script(202242, &ICCTeleporterAI::Create);
	mgr->register_go_gossip_script(202242, new ICCTeleporterGossip());

	mgr->register_gameobject_script(202243, &ICCTeleporterAI::Create);
	mgr->register_go_gossip_script(202243, new ICCTeleporterGossip());

	mgr->register_gameobject_script(202244, &ICCTeleporterAI::Create);
	mgr->register_go_gossip_script(202244, new ICCTeleporterGossip());

	mgr->register_gameobject_script(202245, &ICCTeleporterAI::Create);
	mgr->register_go_gossip_script(202245, new ICCTeleporterGossip());

	mgr->register_gameobject_script(202246, &ICCTeleporterAI::Create);
	mgr->register_go_gossip_script(202246, new ICCTeleporterGossip());
};