/*
 * WEmu Scripts for WEmu MMORPG Server
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

#include "StdAfx.h"
#include "Setup.h"
#include "../EAS/EasyFunctions.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \ Menu->SendTo(plr);



class SCRIPT_DECL SpiritScreeches : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature *spirit = (Creature*)(pObject);
		if (spirit == NULL)
			return;

		if(plr->GetQuestLogForEntry(3520))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 2039, plr);
			Menu->AddItem( 0, "Goodbye", 1);

			if(AutoSend)
				Menu->SendTo(plr);
		}



	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature *spirit = (Creature*)(pObject);
		if (spirit == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
				{
					QuestLogEntry *en = plr->GetQuestLogForEntry(3520);
					if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
					{
						en->SetMobCount(0, en->GetMobCount(0) + 1);
						en->SendUpdateAddKill(0);
						en->UpdatePlayerFields();
					}
					if(!spirit)
						return;

					spirit->Despawn(1, 0);
					return;

				}
		}
	}

	void Destroy()
	{
		delete this;
	}
};

class ScreecherSpirit : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ScreecherSpirit);

	ScreecherSpirit(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		if(!_unit)
			return

		_unit->Despawn(60*1000, 0);
		Creature *cialo = _unit->GetMapMgr()->GetInterface()->GetCreatureNearestCoords(_unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 5307);
		if(!cialo)
			return;

		if(!cialo->isAlive())
			cialo->Despawn(1, 6*60*1000);
	}
};

class SCRIPT_DECL StewardOfTime : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* Plr, bool AutoSend)
	{
		GossipMenu *Menu;
		if(Plr->GetQuestLogForEntry(10279))
		{
			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 9978, Plr);
			Menu->AddItem( 0, "Please take me to the mater lair", 1);
			Menu->SendTo(Plr);
		}
	}

	void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
	{
		Creature* creat = static_cast<Creature*>(pObject);
		switch(IntId)
		{
		case 1:
			creat->CastSpell(Plr, dbcSpell.LookupEntry(34891), true);
			break;
		}
	}
}; 

void SetupTanaris(ScriptMgr * mgr)
{
	GossipScript * Screeches = (GossipScript*) new SpiritScreeches();
	mgr->register_gossip_script(8612, Screeches);

	mgr->register_creature_script(8612, &ScreecherSpirit::Create);
	mgr->register_gossip_script(20142, CREATE_GOSSIPSCRIPT(StewardOfTime));
}
