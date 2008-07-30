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

class ThePathoftheAdept : public GameObjectAIScript
{
public:
	ThePathoftheAdept(GameObject* goinstance) : GameObjectAIScript(goinstance) {}
	static GameObjectAIScript *Create(GameObject * GO) { return new ThePathoftheAdept(GO); }

	void OnActivate(Player * pPlayer)
	{
		if(!pPlayer)
			return;

		QuestLogEntry *qle = pPlayer->GetQuestLogForEntry(9692);
		if(qle == NULL)
			return;

		Creature *zelemar = sEAS.SpawnCreature(pPlayer, 17830, pPlayer->GetPositionX()+5, pPlayer->GetPositionY()+5, pPlayer->GetPositionZ(), pPlayer->GetOrientation(), 0);
		zelemar->Despawn(6*60*1000, 0);
	}
};


class SCRIPT_DECL PathoftheAdept : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature *lord = (Creature*)(pObject);
		if (lord == NULL)
			return;

			objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
			if(plr->GetQuestLogForEntry(9692))
			Menu->AddItem( 0, "Take Insignia", 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature *lord = (Creature*)(pObject);
		if (lord == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				sEAS.AddItem(24226, plr);
				return;
			}break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};


class LordDawnstar : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(LordDawnstar);
	LordDawnstar(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->SetUInt32Value(UNIT_NPC_FLAGS, 1);
		_unit->SetStandState(7);
		_unit->setDeathState(CORPSE);
		_unit->GetAIInterface()->m_canMove = false;
	}
};


void SetupSilvermoonCity(ScriptMgr * mgr)
{
	mgr->register_gameobject_script(182024, &ThePathoftheAdept::Create);

	GossipScript *LordGossip = (GossipScript*) new PathoftheAdept();
	mgr->register_gossip_script(17832, LordGossip);

	mgr->register_creature_script(17832, &LordDawnstar::Create);
}
