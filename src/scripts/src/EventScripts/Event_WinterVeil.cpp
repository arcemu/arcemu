/*
 * ArcEmu, a MMORPG Server
 * 
 * Copyright (C) 2008-2012 ArcEmu Developers <http://arcemu.org/>
 *
 * See former copyright holders in CREDITS
 * 
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Setup.h"

class PX238WinterWondervolt : public GameObjectAIScript
{
	public:
		PX238WinterWondervolt(GameObject*  goinstance) : GameObjectAIScript(goinstance) {}
		static GameObjectAIScript* Create(GameObject* GO) { return new PX238WinterWondervolt(GO); }

		void OnSpawn()
		{
			RegisterAIUpdateEvent(1);
		}

		void AIUpdate()
		{
			Player* plr = _gameobject->GetMapMgr()->GetInterface()->GetPlayerNearestCoords(_gameobject->GetPositionX(), _gameobject->GetPositionY(), _gameobject->GetPositionZ());
			if(!plr)
				return;

			if(_gameobject->CalcDistance(_gameobject, plr) <= 1.050000f && !plr->HasAura(26274))       // aura given by the PX-238 Winter Wondervolt
			{
				plr->CastSpell(plr, 26275 , true);   // Spell that change into random gnome dispalyid (respect male & female)
			}
		}
};

void WinterReveler(Player* pPlayer, Unit* pUnit)
{
	if(pUnit->GetEntry() == 15760)
	{
		uint32 Winteritem = 0;
		SlotResult slotresult;

		uint32 chance = RandomUInt(2);
		switch(chance)
		{
			case 0:
				{
					ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(21212);
					if(!proto)
						return;

					slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot(proto);
					Winteritem = 21212;
				}
				break;

			case 1:
				{
					ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(21519);
					if(!proto)
						return;

					slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot(proto);
					Winteritem = 21519;
				}
				break;

			case 2:
				{
					ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(34191);
					if(!proto)
						return;

					slotresult = pPlayer->GetItemInterface()->FindFreeInventorySlot(proto);
					Winteritem = 34191;
				}
				break;

		}

		if(!slotresult.Result)
		{
			pPlayer->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
			return;
		}
		else
		{
			Item* itm = objmgr.CreateItem(Winteritem, pPlayer);
			itm->SetStackCount(5);
			pPlayer->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot);
			pUnit->CastSpell(pPlayer, 26218, true);
		}
	}
}

void WinterVeilEmote(Player* pPlayer, uint32 Emote, Unit* pUnit)
{
	pUnit = pPlayer->GetMapMgr()->GetUnit(pPlayer->GetSelection());
	if(!pUnit || !pUnit->isAlive() || pUnit->GetAIInterface()->getNextTarget())
		return;

	if(Emote == EMOTE_ONESHOT_KISS)
	{
		if(!pPlayer->HasAura(26218))
			WinterReveler(pPlayer, pUnit);
	}
}

void SetupWinterVeil(ScriptMgr* mgr)
{
	mgr->register_gameobject_script(180796, &PX238WinterWondervolt::Create);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_EMOTE, (void*)&WinterVeilEmote);
}