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

//Made by ???
//Modified by WigSplitta whole code
//Re-Modified by HalestormXV (Added More Features)
//Random Dice Concept to Kuraria for Helping HalestormXV
//Modified by Neo_mat

#include "StdAfx.h"
#include "Setup.h"

#define TokenID 19712 //set token id here

void onPvpKill(Player* pPlayer)
{
	int die;
	die = rand()%5 + 1;
	if (die <= 1)
	{
		Item *item;
		item = objmgr.CreateItem( TokenID , pPlayer);
		item->SetUInt32Value(ITEM_FIELD_STACK_COUNT,1);
		item->SoulBind();
		if(!pPlayer->GetItemInterface()->AddItemToFreeSlot(item))
		{
			pPlayer->GetSession()->SendNotification("No free slots were found in your inventory!");
			item->DeleteMe();
		}
		else
		{
			SlotResult *lr = pPlayer->GetItemInterface()->LastSearchResult();
			pPlayer->GetSession()->SendItemPushResult(item,false,true,false,true,lr->ContainerSlot,lr->Slot,1);
		}
	}
}

void SetupPvPToken(ScriptMgr * mgr)
{
	mgr->register_hook(SERVER_HOOK_EVENT_ON_KILL_PLAYER, (void*)onPvpKill);
}
