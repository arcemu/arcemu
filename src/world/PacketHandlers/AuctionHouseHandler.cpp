/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

void WorldSession::SendAuctionPlaceBidResultPacket(uint32 itemId, uint32 error)
{
	WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 16);
	data << itemId;
	data << uint32(AUCTION_BID);
	data << error;
	data << uint32(0);
	SendPacket(&data);
}

void WorldSession::HandleAuctionPlaceBid(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	uint32 auction_id, price;
	recv_data >> auction_id >> price;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	// Find Item
	AuctionHouse* ah = pCreature->auctionHouse;
	Auction* auct = ah->GetAuction(auction_id);
	if(auct == 0 || !auct->Owner || !_player)
	{
		SendAuctionPlaceBidResultPacket(0, AUCTION_ERROR_INTERNAL);
		return;
	}

	if(auct->Owner == _player->GetGUID())
	{
		SendAuctionPlaceBidResultPacket(0, AUCTION_ERROR_BID_OWN_AUCTION);
		return;
	}
	if(auct->HighestBid > price && price != auct->BuyoutPrice)
	{
		//HACK: Don't know the correct error code...
		SendAuctionPlaceBidResultPacket(0, AUCTION_ERROR_INTERNAL);
		return;
	}

	if(!_player->HasGold(price))
	{
		SendAuctionPlaceBidResultPacket(0, AUCTION_ERROR_MONEY);
		return;
	}

	_player->ModGold(-(int32)price);
	if(auct->HighestBidder != 0)
	{
		// Return the money to the last highest bidder.
		char subject[100];
		snprintf(subject, 100, "%u:0:0", (int)auct->pItem->GetEntry());
		sMailSystem.SendAutomatedMessage(AUCTION, ah->GetID(), auct->HighestBidder, subject, "", auct->HighestBid, 0, 0, MAIL_STATIONERY_AUCTION);

		// Do not send out bid notification, when current highest bidder and new bidder are the same player..
		if(auct->HighestBidder != (uint32)_player->GetLowGUID())
			ah->SendAuctionOutBidNotificationPacket(auct, _player->GetGUID(), price);
	}

	if(auct->BuyoutPrice == price)
	{
		auct->HighestBidder = _player->GetLowGUID();
		auct->HighestBid = price;

		// we used buyout on the item.
		ah->QueueDeletion(auct, AUCTION_REMOVE_WON);

		SendAuctionPlaceBidResultPacket(auct->Id, AUCTION_ERROR_NONE);
		ah->SendAuctionBuyOutNotificationPacket(auct);
	}
	else
	{
		// update most recent bid
		auct->HighestBidder = _player->GetLowGUID();
		auct->HighestBid = price;
		auct->UpdateInDB();

		SendAuctionPlaceBidResultPacket(auct->Id, AUCTION_ERROR_NONE);
	}
}

void WorldSession::HandleCancelAuction(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	uint32 auction_id;
	recv_data >> auction_id;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	// Find Item
	Auction* auct = pCreature->auctionHouse->GetAuction(auction_id);
	if(auct == 0) return;

	pCreature->auctionHouse->QueueDeletion(auct, AUCTION_REMOVE_CANCELLED);

	// Send response packet.
	WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
	data << auction_id << uint32(AUCTION_CANCEL) << uint32(0);
	SendPacket(&data);

	// Re-send the owner list.
	pCreature->auctionHouse->SendOwnerListPacket(_player, 0);
}

void WorldSession::HandleAuctionSellItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid, item;
	uint32 bid, buyout, etime, unk1, unk2;	// etime is in minutes

	recv_data >> guid >> unk1 >> item;
	recv_data >> unk2;
	recv_data >> bid >> buyout >> etime;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;		// NPC doesn't exist or isn't an auctioneer

	// Get item
	Item* pItem = _player->GetItemInterface()->GetItemByGUID(item);
	if(!pItem || pItem->IsSoulbound() || pItem->IsConjured())
	{
		WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
		data << uint32(0);
		data << uint32(AUCTION_CREATE);
		data << uint32(AUCTION_ERROR_ITEM);
		SendPacket(&data);
		return;
	};

	AuctionHouse* ah = pCreature->auctionHouse;

	uint32 item_worth = pItem->GetProto()->SellPrice * pItem->GetStackCount();
	uint32 item_deposit = (uint32)(item_worth * ah->deposit_percent) * (uint32)(etime / 240.0f); // deposit is per 4 hours

	if(!_player->HasGold(item_deposit))   // player cannot afford deposit
	{
		WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
		data << uint32(0);
		data << uint32(AUCTION_CREATE);
		data << uint32(AUCTION_ERROR_MONEY);
		SendPacket(&data);
		return;
	}

	pItem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(item, false);
	if(!pItem)
	{
		WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
		data << uint32(0);
		data << uint32(AUCTION_CREATE);
		data << uint32(AUCTION_ERROR_ITEM);
		SendPacket(&data);
		return;
	};

	if(pItem->IsInWorld())
	{
		pItem->RemoveFromWorld();
	}

	pItem->SetOwner(NULL);
	pItem->m_isDirty = true;
	pItem->SaveToDB(INVENTORY_SLOT_NOT_SET, 0, true, NULL);

	// Create auction
	Auction* auct = new Auction;
	auct->BuyoutPrice = buyout;
	auct->ExpiryTime = (uint32)UNIXTIME + (etime * 60);
	auct->StartingPrice = bid;
	auct->HighestBid = 0;
	auct->HighestBidder = 0;	// hm
	auct->Id = sAuctionMgr.GenerateAuctionId();
	auct->Owner = _player->GetLowGUID();
	auct->pItem = pItem;
	auct->Deleted = false;
	auct->DeletedReason = 0;
	auct->DepositAmount = item_deposit;

	// remove deposit
	_player->ModGold(-(int32)item_deposit);

	// Add and save auction to DB
	ah->AddAuction(auct);
	auct->SaveToDB(ah->GetID());

	// Send result packet
	WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
	data << auct->Id;
	data << uint32(AUCTION_CREATE);
	data << uint32(AUCTION_ERROR_NONE);
	SendPacket(&data);
}

void WorldSession::HandleAuctionListOwnerItems(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	pCreature->auctionHouse->SendOwnerListPacket(_player, &recv_data);
}

void WorldSession::HandleAuctionListItems(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN;

	uint64 guid;
	recv_data >> guid;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	pCreature->auctionHouse->SendAuctionList(_player, &recv_data);
}

void WorldSession::HandleAuctionListPendingSales(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	LOG_DEBUG("WORLD: Received CMSG_AUCTION_LIST_PENDING_SALES");

	uint32 count = 0;

	WorldPacket data(SMSG_AUCTION_LIST_PENDING_SALES, 4);
	data << uint32(count);                                  // count
	/*for(uint32 i = 0; i < count; ++i)
	{
		data << "";                                         // string
		data << "";                                         // string
		data << uint32(0);
		data << uint32(0);
		data << float(0);
	}*/
	SendPacket(&data);
}

