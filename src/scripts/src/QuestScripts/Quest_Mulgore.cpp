/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2008 WEmu Team
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

class The_Plains_Vision : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(The_Plains_Vision);
		The_Plains_Vision(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnReachWP(uint32 iWaypointId, bool bForwards)
		{
			if(iWaypointId == 2)
				_unit->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "You follow me.");

			if(iWaypointId == 22)
			{
				sEAS.DeleteWaypoints(_unit);
				_unit->Despawn(500, 0);
			}
		}
};

// Quest: 925
class CairneBloodhoof_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), Plr, menu);
			if(plr->HasQuest(925))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "I know this is rather silly but a young ward who is a bit shy would like your hoofprint.", 0);
			
			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 523, plr);
			TO_CREATURE(pObject)->CastSpell(plr, 23123, false);
		}
};

// Quest: 770
class SkornWhitecloud_Gossip : public Arcemu::Gossip::Script
{
	public:
		void OnHello(Object* pObject, Player* plr)
		{
			Arcemu::Gossip::Menu menu(pObject->GetGUID(), objmgr.GetGossipTextForNpc(pObject->GetEntry()), plr->GetSession()->language);
			sQuestMgr.FillQuestMenu(TO_CREATURE(pObject), Plr, menu);
			if(plr->HasQuest(770))
				menu.AddItem(Arcemu::Gossip::ICON_CHAT, "Tell me a story, Skorn.", 0);

			menu.Send(plr);
		}

		void OnSelectOption(Object* pObject, Player* plr, uint32 Id, const char* Code)
		{
			Arcemu::Gossip::Menu::SendSimpleMenu(pObject->GetGUID(), 523, plr);
		}
};

void SetupMulgore(ScriptMgr* mgr)
{
	mgr->register_creature_script(2983, &The_Plains_Vision::Create);
	mgr->register_creature_gossip(3057, new CairneBloodhoof_Gossip);
	mgr->register_creature_gossip(3052, new SkornWhitecloud_Gossip);
}
