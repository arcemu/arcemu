/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

#include "StdAfx.h"

void WorldSession::HandleInitiateTrade(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;
	Player * pTarget = _player->GetMapMgr()->GetPlayer((uint32)guid);
	uint32 TradeStatus = TRADE_STATUS_PROPOSED;
	WorldPacket data(SMSG_TRADE_STATUS, 12);

	if(pTarget == 0)
	{

		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;

		OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
		return;
	}

	// Handle possible error outcomes
	if(pTarget->CalcDistance(_player) > 10.0f)		// This needs to be checked
		TradeStatus = TRADE_STATUS_TOO_FAR_AWAY;
	else if(pTarget->IsDead())
		TradeStatus = TRADE_STATUS_DEAD;
	else if(pTarget->mTradeTarget != 0)
		TradeStatus = TRADE_STATUS_ALREADY_TRADING;
	else if(pTarget->GetTeam() != _player->GetTeam() && GetPermissionCount() == 0 && !sWorld.interfaction_trade)
		TradeStatus = TRADE_STATUS_WRONG_FACTION;

	data << TradeStatus;

	if(TradeStatus == TRADE_STATUS_PROPOSED)
	{
		_player->ResetTradeVariables();
		pTarget->ResetTradeVariables();

		pTarget->mTradeTarget = _player->GetLowGUID();
		_player->mTradeTarget = pTarget->GetLowGUID();

		pTarget->mTradeStatus = TradeStatus;
		_player->mTradeStatus = TradeStatus;

		data << _player->GetGUID();
	}

	pTarget->m_session->SendPacket(&data);
}

void WorldSession::HandleBeginTrade(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 TradeStatus = TRADE_STATUS_INITIATED;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{

		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;

		OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
		return;
	}
	// We're too far from target now?
	if( _player->CalcDistance( objmgr.GetPlayer(_player->mTradeTarget) ) > 10.0f )
	TradeStatus = TRADE_STATUS_TOO_FAR_AWAY;

	WorldPacket data(SMSG_TRADE_STATUS, 8);

	data << uint32( TradeStatus );
    data << uint32( 0x19 );
	
    plr->m_session->SendPacket(&data);

	SendPacket(&data);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;
}

void WorldSession::HandleBusyTrade(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 TradeStatus = TRADE_STATUS_PLAYER_BUSY;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{

		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;

		OutPacket(TRADE_STATUS_PLAYER_NOT_FOUND, 4, &TradeStatus);
		return;
	}


	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
}

void WorldSession::HandleIgnoreTrade(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 TradeStatus = TRADE_STATUS_PLAYER_IGNORED;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;

		OutPacket(TRADE_STATUS_PLAYER_NOT_FOUND, 4, &TradeStatus);
		return;
	}

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
}

void WorldSession::HandleCancelTrade(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->mTradeTarget == 0 || _player->mTradeStatus == TRADE_STATUS_COMPLETE)
		return;

    uint32 TradeStatus = TRADE_STATUS_CANCELLED;

    OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	Player * plr = _player->GetTradeTarget();
    if(plr)
    {
        if(plr->m_session && plr->m_session->GetSocket())
		plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	
		plr->ResetTradeVariables();
    }
	
	_player->ResetTradeVariables();
}

void WorldSession::HandleUnacceptTrade(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	Player * plr = _player->GetTradeTarget();
	//_player->ResetTradeVariables();

	if(_player->mTradeTarget == 0 || plr == 0)
		return;

	uint32 TradeStatus = TRADE_STATUS_UNACCEPTED;

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);



	TradeStatus = TRADE_STATUS_STATE_CHANGED;

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

}

void WorldSession::HandleSetTradeItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->mTradeTarget == 0)
		return;

	CHECK_PACKET_SIZE(recv_data, 3);

	uint8 TradeSlot = recv_data.contents()[0];
	uint8 SourceBag = recv_data.contents()[1];
	uint8 SourceSlot = recv_data.contents()[2];
	Player * pTarget = _player->GetMapMgr()->GetPlayer( _player->mTradeTarget );

	Item * pItem = _player->GetItemInterface()->GetInventoryItem(SourceBag, SourceSlot);

	if( pTarget == NULL || pItem == NULL || TradeSlot > 6 )
		return;
	if( TradeSlot < 6 )
	{
		if( pItem->IsAccountbound() )
			return;//dual accounting is not allowed so noone can trade Accountbound items. Btw the client doesn't send any client-side notification
		if( pItem->IsSoulbound() )
		{
			sCheatLog.writefromsession(this, "tried to cheat trade a soulbound item");
			Disconnect();
			return;
		}
	}

	uint32 TradeStatus = TRADE_STATUS_STATE_CHANGED;
	Player * plr = _player->GetTradeTarget();
	if(!plr) return;

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;


	if( pItem->IsContainer() )
	{
		if( TO< Container* >(pItem)->HasItems() )
		{
			_player->GetItemInterface()->BuildInventoryChangeError(pItem,0, INV_ERR_CAN_ONLY_DO_WITH_EMPTY_BAGS);

			//--trade cancel

			TradeStatus = TRADE_STATUS_CANCELLED;

			OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			_player->ResetTradeVariables();

			plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			plr->ResetTradeVariables();

			return;
		}
	}			
		
	for(uint32 i = 0; i < 8; ++i)
	{
		// duping little shits
		if(_player->mTradeItems[i] == pItem || pTarget->mTradeItems[i] == pItem)
		{
			sCheatLog.writefromsession(this, "tried to dupe an item through trade");
			Disconnect();
			return;
		}
	}

	if( SourceSlot >= INVENTORY_SLOT_BAG_START && SourceSlot < INVENTORY_SLOT_BAG_END )
	{
		Item * itm =  _player->GetItemInterface()->GetInventoryItem(SourceBag);//NULL if it's the backpack or the item is equipped
		if( itm == NULL || SourceSlot >= itm->GetProto()->ContainerSlots)//Required as there are bags with SourceSlot > INVENTORY_SLOT_BAG_START
		{
			//More duping woohoo
			sCheatLog.writefromsession(this, "tried to cheat trade a soulbound item");
			Disconnect();
		}
	}

	_player->mTradeItems[TradeSlot] = pItem;
	_player->SendTradeUpdate();
}

void WorldSession::HandleSetTradeGold(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->mTradeTarget == 0)
		return;
  // cebernic: TradeGold sameway.
	uint32 TradeStatus = TRADE_STATUS_STATE_CHANGED;
	Player * plr = _player->GetTradeTarget();
	if(!plr) return;

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;


	uint32 Gold;
	recv_data >> Gold;

	if(_player->mTradeGold != Gold)
	{
		_player->mTradeGold = (Gold > _player->GetGold() ? _player->GetGold() : Gold);
		_player->SendTradeUpdate();
	}
}

void WorldSession::HandleClearTradeItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);
	if(_player->mTradeTarget == 0)
		return;

	uint8 TradeSlot = recv_data.contents()[0];
	if(TradeSlot > 6)
		return;

  // clean status
	Player * plr = _player->GetTradeTarget();
	if ( !plr ) return;

	uint32 TradeStatus = TRADE_STATUS_STATE_CHANGED;

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;


	_player->mTradeItems[TradeSlot] = 0;
	_player->SendTradeUpdate();
}

void WorldSession::HandleAcceptTrade(WorldPacket & recv_data) 
{
	CHECK_INWORLD_RETURN

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || !plr)
		return;

	uint32 TradeStatus = TRADE_STATUS_ACCEPTED;
	
	// Tell the other player we're green.
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	_player->mTradeStatus = TradeStatus;

	if(plr->mTradeStatus == TRADE_STATUS_ACCEPTED)
	{
		// Ready!
		uint32 ItemCount = 0;
		uint32 TargetItemCount = 0;
		Player * pTarget = plr;

/*		// Calculate Item Count
		for(uint32 Index = 0; Index < 7; ++Index)
		{
			if(_player->mTradeItems[Index] != 0)	++ItemCount;
			if(pTarget->mTradeItems[Index] != 0)	++TargetItemCount;
		}*/
		

		// Calculate Count
		for(uint32 Index = 0; Index < 6; ++Index) // cebernic: checking for 6items ,untradable item check via others func.
		{
			Item * pItem;

			// safely trade checking
			pItem = _player->mTradeItems[Index];
			if( pItem )
			{
				if( ( pItem->IsContainer() && TO< Container* >(pItem)->HasItems() )   || ( pItem->GetProto()->Bonding==ITEM_BIND_ON_PICKUP) )
				{
					ItemCount = 0;
					TargetItemCount = 0;
					break;
				}
				else ++ItemCount;
			}					
			
			pItem = pTarget->mTradeItems[Index];
			if( pItem )
			{
				if( ( pItem->IsContainer() && TO< Container* >(pItem)->HasItems() )   || ( pItem->GetProto()->Bonding==ITEM_BIND_ON_PICKUP) )
				{
					ItemCount = 0;
					TargetItemCount = 0;
					break;
				}
				else ++TargetItemCount;
			}					

			//if(_player->mTradeItems[Index] != 0)	++ItemCount;
			//if(pTarget->mTradeItems[Index] != 0)	++TargetItemCount;
		}

		

		if( (_player->m_ItemInterface->CalculateFreeSlots(NULL) + ItemCount) < TargetItemCount ||
			(pTarget->m_ItemInterface->CalculateFreeSlots(NULL) + TargetItemCount) < ItemCount ||
			(ItemCount== 0 && TargetItemCount== 0 && !pTarget->mTradeGold && !_player->mTradeGold) )	// cebernic added it
		{
			// Not enough slots on one end.
			TradeStatus = TRADE_STATUS_CANCELLED;
		}
		else
		{
			TradeStatus = TRADE_STATUS_COMPLETE;
			uint64 Guid;
			Item * pItem;
			
			// Remove all items from the players inventory
			for(uint32 Index = 0; Index < 6; ++Index)
			{
				Guid = _player->mTradeItems[Index] ? _player->mTradeItems[Index]->GetGUID() : 0;
				if(Guid != 0)
				{
					if( _player->mTradeItems[Index]->GetProto()->Bonding == ITEM_BIND_ON_PICKUP ||
						_player->mTradeItems[Index]->GetProto()->Bonding  >=  ITEM_BIND_QUEST  )
					{
						_player->mTradeItems[Index] = NULL;
					}
					else
					{
						if(GetPermissionCount()>0)
						{
							sGMLog.writefromsession(this, "traded item %s to %s", _player->mTradeItems[Index]->GetProto()->Name1, pTarget->GetName());
						}
						pItem = _player->m_ItemInterface->SafeRemoveAndRetreiveItemByGuid(Guid, true);
					}
				}

				Guid = pTarget->mTradeItems[Index] ? pTarget->mTradeItems[Index]->GetGUID() : 0;
				if(Guid != 0)
				{
					if( pTarget->mTradeItems[Index]->GetProto()->Bonding == ITEM_BIND_ON_PICKUP ||  
						pTarget->mTradeItems[Index]->GetProto()->Bonding  >=  ITEM_BIND_QUEST )
					{
						pTarget->mTradeItems[Index] = NULL;
					}
					else
					{
						pTarget->m_ItemInterface->SafeRemoveAndRetreiveItemByGuid(Guid, true);
					}
				}
			}

			// Dump all items back into the opposite players inventory
			for(uint32 Index = 0; Index < 6; ++Index)
			{
				pItem = _player->mTradeItems[Index];
				if( pItem != NULL )
				{
					pItem->SetOwner(pTarget); // crash fixed.
					if( !pTarget->m_ItemInterface->AddItemToFreeSlot(pItem) )
						pItem->DeleteMe();
				}

				pItem = pTarget->mTradeItems[Index];
				if( pItem != NULL )
				{
					pItem->SetOwner(_player);
					if( !_player->m_ItemInterface->AddItemToFreeSlot(pItem) )
						pItem->DeleteMe();
				}
			}

			// Trade Gold
			if(pTarget->mTradeGold)
			{
				// Check they don't have more than the max gold
				if(sWorld.GoldCapEnabled && (_player->GetGold() + pTarget->mTradeGold) > sWorld.GoldLimit)
				{
					_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
				}
				else
				{
					_player->ModGold( pTarget->mTradeGold );
					pTarget->ModGold( -(int32)pTarget->mTradeGold );
				}
			}

			if(_player->mTradeGold)
			{
				// Check they don't have more than the max gold
				if(sWorld.GoldCapEnabled && (pTarget->GetGold() + _player->mTradeGold) > sWorld.GoldLimit)
				{
					pTarget->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
				}
				else
				{
					pTarget->ModGold( _player->mTradeGold );
					_player->ModGold( -(int32)_player->mTradeGold );
				}
			}

			// Close Window
			TradeStatus = TRADE_STATUS_COMPLETE;
			
		}
					
			OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

			_player->mTradeStatus = TRADE_STATUS_COMPLETE;
			plr->mTradeStatus = TRADE_STATUS_COMPLETE;

			// Reset Trade Vars
			_player->ResetTradeVariables();
			pTarget->ResetTradeVariables();
			
			// Save for each other
			plr->SaveToDB(false);
			_player->SaveToDB(false);
	}
}
