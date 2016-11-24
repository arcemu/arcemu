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

void Auction::DeleteFromDB()
{
	CharacterDatabase.WaitExecute("DELETE FROM auctions WHERE auctionId = %u", Id);
}

void Auction::SaveToDB(uint32 AuctionHouseId)
{
	CharacterDatabase.Execute("INSERT INTO auctions VALUES(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", Id, AuctionHouseId, pItem->GetLowGUID(), Owner, StartingPrice, BuyoutPrice, ExpiryTime, HighestBidder, HighestBid, DepositAmount);
}

void Auction::UpdateInDB()
{
	CharacterDatabase.Execute("UPDATE auctions SET bidder = %u, bid = %u WHERE auctionId = %u", HighestBidder, HighestBid, Id);
}

AuctionHouse::AuctionHouse(uint32 ID)
{
	dbc = dbcAuctionHouse.LookupEntryForced(ID);
	ARCEMU_ASSERT(dbc != NULL);

	cut_percent = dbc->tax / 100.0f ;
	deposit_percent = dbc->fee / 100.0f ;

	enabled = true;
}

AuctionHouse::~AuctionHouse()
{
	for (HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin(); itr != auctions.end(); ++itr)
		delete itr->second;
}

void AuctionHouse::QueueDeletion(Auction* auct, uint32 Reason)
{
	if(auct->Deleted)
		return;

	auct->Deleted = true;
	auct->DeletedReason = Reason;
	removalLock.Acquire();
	removalList.push_back(auct);
	removalLock.Release();
}

void AuctionHouse::UpdateDeletionQueue()
{
	removalLock.Acquire();
	Auction* auct;

	for (list<Auction*>::iterator it = removalList.begin(); it != removalList.end(); ++it)
	{
		auct = *it;
		ARCEMU_ASSERT(auct->Deleted);
		RemoveAuction(auct);
	}

	removalList.clear();
	removalLock.Release();
}

void AuctionHouse::UpdateAuctions()
{
	auctionLock.AcquireReadLock();
	removalLock.Acquire();

	uint32 t = (uint32)UNIXTIME;
	for (HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin(); itr != auctions.end(); ++itr)
	{
		Auction* auct = itr->second;

		if(t >= auct->ExpiryTime)
		{
			if(auct->HighestBidder == 0)
			{
				auct->DeletedReason = AUCTION_REMOVE_EXPIRED;
				this->SendAuctionExpiredNotificationPacket(auct);
			}
			else
				auct->DeletedReason = AUCTION_REMOVE_WON;

			auct->Deleted = true;
			removalList.push_back(auct);
		}
	}

	removalLock.Release();
	auctionLock.ReleaseReadLock();
}

void AuctionHouse::AddAuction(Auction* auct)
{
	// add to the map
	auctionLock.AcquireWriteLock();
	auctions.insert(HM_NAMESPACE::hash_map<uint32, Auction*>::value_type(auct->Id , auct));
	auctionLock.ReleaseWriteLock();

	Log.Debug("AuctionHouse", "%u: Add auction %u, expire@ %u.", dbc->id, auct->Id, auct->ExpiryTime);
}

Auction* AuctionHouse::GetAuction(uint32 Id)
{
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr;
	auctionLock.AcquireReadLock();
	itr = auctions.find(Id);
	Auction* ret = (itr == auctions.end()) ? 0 : itr->second;
	auctionLock.ReleaseReadLock();
	return ret;
}

void AuctionHouse::RemoveAuction(Auction* auct)
{
	Log.Debug("AuctionHouse", "%u: Removing auction %u, reason %u.", dbc->id, auct->Id, auct->DeletedReason);

	char subject[100];
	char body[200];
	switch(auct->DeletedReason)
	{
		case AUCTION_REMOVE_EXPIRED:
			{
				// ItemEntry:0:3
				snprintf(subject, 100, "%u:0:3", (unsigned int)auct->pItem->GetEntry());

				// Auction expired, resend item, no money to owner.
				sMailSystem.SendAutomatedMessage(AUCTION, dbc->id, auct->Owner, subject, "", 0, 0, auct->pItem->GetGUID(), MAIL_STATIONERY_AUCTION, MAIL_CHECK_MASK_COPIED);
			}
			break;

		case AUCTION_REMOVE_WON:
			{
				// ItemEntry:0:1
				snprintf(subject, 100, "%u:0:1", (unsigned int)auct->pItem->GetEntry());

				// <owner player guid>:bid:buyout
				snprintf(body, 200, "%X:%u:%u", (unsigned int)auct->Owner, (unsigned int)auct->HighestBid, (unsigned int)auct->BuyoutPrice);

				// Auction won by highest bidder. He gets the item.
				sMailSystem.SendAutomatedMessage(AUCTION, dbc->id, auct->HighestBidder, subject, body, 0, 0, auct->pItem->GetGUID(), MAIL_STATIONERY_AUCTION, MAIL_CHECK_MASK_COPIED);

				// Send a mail to the owner with his cut of the price.
				uint32 auction_cut = float2int32(cut_percent * auct->HighestBid);
				int32 amount = auct->HighestBid - auction_cut + auct->DepositAmount;
				if(amount < 0)
					amount = 0;

				// ItemEntry:0:2
				snprintf(subject, 100, "%u:0:2", (unsigned int)auct->pItem->GetEntry());

				// <hex player guid>:bid:0:deposit:cut
				if(auct->HighestBid == auct->BuyoutPrice)	   // Buyout
					snprintf(body, 200, "%X:%u:%u:%u:%u", (unsigned int)auct->HighestBidder, (unsigned int)auct->HighestBid, (unsigned int)auct->BuyoutPrice, (unsigned int)auct->DepositAmount, (unsigned int)auction_cut);
				else
					snprintf(body, 200, "%X:%u:0:%u:%u", (unsigned int)auct->HighestBidder, (unsigned int)auct->HighestBid, (unsigned int)auct->DepositAmount, (unsigned int)auction_cut);

				// send message away.
				sMailSystem.SendAutomatedMessage(AUCTION, dbc->id, auct->Owner, subject, body, amount, 0, 0, MAIL_STATIONERY_AUCTION, MAIL_CHECK_MASK_COPIED);

				// If it's not a buyout (otherwise the players has been already notified)
				if(auct->HighestBid < auct->BuyoutPrice || auct->BuyoutPrice == 0)
					SendAuctionBuyOutNotificationPacket(auct);
			}
			break;
		case AUCTION_REMOVE_CANCELLED:
			{
				snprintf(subject, 100, "%u:0:5", (unsigned int)auct->pItem->GetEntry());
				uint32 cut = uint32(cut_percent * auct->HighestBid);
				Player* plr = objmgr.GetPlayer(auct->Owner);
				if(cut && plr && plr->HasGold(cut))
					plr->ModGold(-(int32)cut);

				sMailSystem.SendAutomatedMessage(AUCTION, GetID(), auct->Owner, subject, "", 0, 0, auct->pItem->GetGUID(), MAIL_STATIONERY_AUCTION, MAIL_CHECK_MASK_COPIED);

				// return bidders money
				if(auct->HighestBidder)
					sMailSystem.SendAutomatedMessage(AUCTION, GetID(), auct->HighestBidder, subject, "", auct->HighestBid, 0, 0, MAIL_STATIONERY_AUCTION, MAIL_CHECK_MASK_COPIED);
			}
			break;
		default:
			break;
	}

	// Remove the auction from the hashmap.
	auctionLock.AcquireWriteLock();
	auctions.erase(auct->Id);
	auctionLock.ReleaseWriteLock();

	// Destroy the item from memory (it still remains in the db)
	if(auct->pItem)
		auct->pItem->DeleteMe();

	// Finally destroy the auction instance.
	auct->DeleteFromDB();
	delete auct;
}

void WorldSession::HandleAuctionListBidderItems(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	pCreature->auctionHouse->SendBidListPacket(_player, &recv_data);
}

void Auction::AddToPacket(WorldPacket & data)
{
	data << uint32(Id);
	data << uint32(pItem->GetEntry());

	for(uint8 i = 0; i < MAX_INSPECTED_ENCHANTMENT_SLOT; i++)
	{
		data << uint32(pItem->GetEnchantmentId(i));       // Enchantment ID
		data << uint32(pItem->GetEnchantmentApplytime(i));						     // Unknown / maybe ApplyTime
		data << uint32(pItem->GetEnchantmentCharges(i));      // charges
	}

	data << pItem->GetItemRandomPropertyId();		 // -ItemRandomSuffix / random property	 : If the value is negative its ItemRandomSuffix if its possitive its RandomItemProperty
	data << pItem->GetItemRandomSuffixFactor();			  // when ItemRandomSuffix is used this is the modifier

	/******************** ItemRandomSuffix***************************
	* For what I have seen ItemRandomSuffix is like RandomItemProperty
	* The only difference is has is that it has a modifier.
	* That is the result of jewelcrafting, the effect is that the
	* enchantment is variable. That means that a enchantment can be +1 and
	* with more Jem's +12 or so.
	* Description for lookup: You get the enchantmentSuffixID and search the
	* DBC for the last 1 - 3 value's(depending on the enchantment).
	* That value is what I call EnchantmentValue. You guys might find a
	* better name but for now its good enough. The formula to calculate
	* The ingame "GAIN" is:
	* (Modifier / 10000) * enchantmentvalue = EnchantmentGain;
	*/

	data << pItem->GetStackCount();         // Amount
	data << pItem->GetChargesLeft();        // Charges Left
	data << uint32(0);                                      // Unknown
	data << uint64(Owner);                          // Owner guid
	data << uint32(StartingPrice);          // Starting bid
	// If there's no bid yet, we should start at starting bid
	data << uint32((HighestBid > 0 ? 50 : 0));      // Next bid value modifier, like current bid + this value
	data << uint32(BuyoutPrice);            // Buyout
	data << uint32((ExpiryTime - UNIXTIME) * 1000);         // Time left
	data << uint64(HighestBidder);          // Last bidder
	data << uint32(HighestBid);                     // The bid of the last bidder

}

void AuctionHouse::SendBidListPacket(Player* plr, WorldPacket* packet)
{
	WorldPacket data(SMSG_AUCTION_BIDDER_LIST_RESULT, 4 + 4 + 4);
	data << uint32(0);		// placeholder
	uint32 count = 0;
	auctionLock.AcquireReadLock();
	for(HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin(); itr != auctions.end(); ++itr)
	{
		Auction* auct = itr->second;
		if(auct->HighestBidder == plr->GetGUID())
		{
			if(auct->Deleted)
				continue;

			auct->AddToPacket(data);
			(*(uint32*)&data.contents()[0])++;
			++count;
		}
	}

	data.put<uint32>(0, count);	// add count to a placeholder
	data << count;
	data << uint32(300);		// unk 2.3.0 delay for next isFull request?
	plr->GetSession()->SendPacket(&data);
	auctionLock.ReleaseReadLock();
}

void AuctionHouse::UpdateOwner(uint32 oldGuid, uint32 newGuid)
{
	auctionLock.AcquireWriteLock();

	for (HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin(); itr != auctions.end(); ++itr)
	{
		Auction* auction = itr->second;
		if(auction->Owner == oldGuid)
			auction->Owner = newGuid;
		if(auction->HighestBidder == oldGuid)
		{
			auction->HighestBidder = newGuid;
			auction->UpdateInDB();
		}
	}
	auctionLock.ReleaseWriteLock();
}

void AuctionHouse::SendOwnerListPacket(Player* plr, WorldPacket* packet)
{
	uint32 count = 0;

	WorldPacket data(SMSG_AUCTION_OWNER_LIST_RESULT, 4 + 4 + 4);
	data << uint32(0);										  // Placeholder

	auctionLock.AcquireReadLock();
	for (HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin(); itr != auctions.end(); ++itr)
	{
		Auction* auct = itr->second;
		if(auct->Owner == plr->GetGUID())
		{
			if(auct->Deleted) continue;

			auct->AddToPacket(data);
			(*(uint32*)&data.contents()[0])++;
			++count;
		}
	}
	data.put<uint32>(0, count);
	data << count;
	data << uint32(300);		// 2.3.0 delay for next isFull request?
	auctionLock.ReleaseReadLock();
	plr->GetSession()->SendPacket(&data);
}

void AuctionHouse::SendAuctionOutBidNotificationPacket(Auction* auct, uint64 newBidder, uint32 newHighestBid)
{
	Player* bidder = objmgr.GetPlayer(auct->HighestBidder);
	if(bidder && bidder->IsInWorld())
	{
		uint32 outbid = (auct->HighestBid / 100) * 5;
		if(!outbid)
			outbid = 1;

		//TODO: Check this code, when a user has been bid out by instant buy out
		WorldPacket data(SMSG_AUCTION_BIDDER_NOTIFICATION, (8 * 4));
		data << GetID();					// auction house id
		data << auct->Id;					// auction id
		data << uint64(newBidder);			// bidder guid
		data << uint32(newHighestBid);		// bidder highest id

		data << uint32(auct->DeletedReason == AUCTION_REMOVE_WON ? 0 : auct->HighestBid);
		data << uint32(outbid);				// auction out bid

		data << auct->pItem->GetEntry();
		data << auct->pItem->GetItemRandomPropertyId();
		bidder->GetSession()->SendPacket(&data);
	}
}

void AuctionHouse::SendAuctionBuyOutNotificationPacket(Auction* auct)
{
	Player* bidder = objmgr.GetPlayer((uint32)auct->HighestBidder);
	if(bidder && bidder->IsInWorld())
	{
		uint32 outbid = (auct->HighestBid / 100) * 5;
		if (!outbid)
			outbid = 1;

		WorldPacket data(SMSG_AUCTION_BIDDER_NOTIFICATION, (7 * 4));
		data << GetID();
		data << auct->Id;
		data << uint64(auct->HighestBidder);
		data << uint32(auct->DeletedReason == AUCTION_REMOVE_WON ? 0 : auct->HighestBid);
		data << uint32(outbid);
		data << auct->pItem->GetEntry();
		data << auct->pItem->GetItemRandomPropertyId();
		bidder->GetSession()->SendPacket(&data);
	}
}

void AuctionHouse::SendAuctionExpiredNotificationPacket(Auction* auct)
{

	Player *owner = objmgr.GetPlayer((uint32)auct->Owner);
	if(owner && owner->IsInWorld())
	{
		WorldPacket data(SMSG_AUCTION_REMOVED_NOTIFICATION, (3 * 4));
		data << GetID();
		data << auct->pItem->GetEntry();
		data << auct->pItem->GetItemRandomPropertyId();
		owner->GetSession()->SendPacket(&data);
	}
}

void WorldSession::SendAuctionPlaceBidResultPacket(Auction* auct, uint32 error, INV_ERR invError /*=INV_ERR_OK */)
{
	WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 16);
	data << (auct ? auct->Id : 0);
	data << uint32(AUCTION_BID);
	data << error;
	switch (error)
	{
		case AUCTION_ERROR_NONE:
		{
				uint32 outbid = auct ? ((auct->HighestBid / 100) * 5) : 0;
				data << outbid;
		}break;
		case AUCTION_ERROR_INVENTORY:
			data << invError;
			break;
		case AUCTION_ERROR_HIGHER_BID:
			if (auct)
			{
				uint32 outbid = (auct->HighestBid / 100) * 5;
				data << auct->HighestBidder;
				data << auct->HighestBid;
				data << outbid;
			}
		default:
			break;
	}
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
	if(!auct|| !auct->Owner || !_player)
	{
		SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_INTERNAL, INV_ERR_OK);
		return;
	}

	if(auct->Owner == _player->GetGUID())
	{
		SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_BID_OWN_AUCTION);
		return;
	}

	if(auct->HighestBid > price && price != auct->BuyoutPrice)
	{
		SendAuctionPlaceBidResultPacket(auct, AUCTION_ERROR_HIGHER_BID);
		return;
	}

	if(!_player->HasGold(price))
	{
		SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_MONEY);
		return;
	}

	_player->ModGold(-(int32)price);
	if(auct->HighestBidder)
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

		SendAuctionPlaceBidResultPacket(auct, AUCTION_ERROR_NONE);
		ah->SendAuctionBuyOutNotificationPacket(auct);
	}
	else
	{
		// update most recent bid
		auct->HighestBidder = _player->GetLowGUID();
		auct->HighestBid = price;
		auct->UpdateInDB();

		SendAuctionPlaceBidResultPacket(auct, AUCTION_ERROR_NONE);
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
	if (!pCreature)
	{
		Log.Error("WorldSession", "Cannot find creature %u for auction id %u.", guid, auction_id);
		return;
	}

	if (!pCreature->auctionHouse)
	{
		Log.Error("WordSession", "Cannot find actionhouse for creature %u.", guid);
		return;
	}

	Auction* auct = pCreature->auctionHouse->GetAuction(auction_id);
	if (!auct)
	{
		SendAuctionPlaceBidResultPacket(auct, AUCTION_ERROR_NONE);
		return;
	}

	if (!auct->pItem)
	{
		Log.Error("WordSession", "Auction id %u has nonexistent item (guid: %u).", auction_id, auct->pItem->GetLowGUID());
		return;
	}
	pCreature->auctionHouse->QueueDeletion(auct, AUCTION_REMOVE_CANCELLED);

	// Send response packet.
	SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_NONE);

	// Re-send the owner list.
	pCreature->auctionHouse->SendOwnerListPacket(_player, 0);
}

void WorldSession::HandleAuctionSellItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint32 bid, buyout, etime, itemCount;	// etime is in minutes
	std::vector<uint64> guids;
	std::vector<uint32> stackSizes;

	recv_data >> guid;
	recv_data >> itemCount;

	if (itemCount > MAX_BAG_SIZE * 5)
	{
		recv_data.rpos(recv_data.wpos());
		return;
	}

	guids.resize(itemCount);
	stackSizes.resize(itemCount);

	for (uint32 i = 0; i < itemCount; i++)
	{
		recv_data >> guids[i];
		recv_data >> stackSizes[i];
	}

	recv_data >> bid >> buyout >> etime;

	if (!bid || !etime)
		return;

	Creature* pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if (!pCreature)
	{
		Log.Error("WorldSession", "Cannot find auctioneer creature (guid: %u)", guid);
		return;
	}
	else if (!pCreature->auctionHouse)
	{
		Log.Error("WorldSession", "Creature (guid: %u) marked as auctioneer, but has no auctionhouse template setted.", guid);
		return;
	}

	AuctionHouse* ah = pCreature->auctionHouse;
	if (!ah)
		return;

	etime *= TIME_MINUTE;
	switch (etime)
	{
		case 1*MIN_AUCTION_TIME:
		case 2*MIN_AUCTION_TIME:
		case 4*MIN_AUCTION_TIME:
			break;
		default:
			return;
	}

	// remove fake death
	if (GetPlayer()->IsDead())
		GetPlayer()->removeSpellByHashName(SPELL_HASH_FEIGN_DEATH);

	for (uint32 i = 0; i < itemCount; i++)
	{
		uint64 itemGuid = guids[i];
		if (!itemGuid)
			continue;

		uint32 stackSize = stackSizes[i];

		Item* pItem = GetPlayer()->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(itemGuid, false);
		if (!pItem)
		{
			Log.Error("WordSession", "Player tryed to create auction with non existing Item (guid: %u)", itemGuid);
			SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_ITEM, INV_ERR_ITEM_NOT_FOUND);
			return;
		}

		if (pItem->IsSoulbound() || pItem->IsConjured() || pItem->GetItemExpireTime() || (pItem->GetUInt32Value(ITEM_FIELD_DURABILITY) < ITEM_FIELD_MAXDURABILITY))
		{
			SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_ITEM, INV_ERR_CANNOT_TRADE_THAT);
			return;
		}

		// calculate deposit cost
		float  deposit = float(pItem->GetProto()->SellPrice * stackSize / (etime * MIN_AUCTION_TIME));
		deposit *= ah->deposit_percent * 3.0f / 100.0f;

		if (!GetPlayer()->HasGold(uint32(deposit)))   // player cannot afford deposit
		{
			SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_MONEY);
			return;
		}

		if (stackSize == 0)
			stackSize = 1;

		if (stackSize > pItem->GetStackCount())
			stackSize = pItem->GetStackCount();

		if (!GetPlayer()->HasItemCount(pItem->GetEntry(), stackSize))
			continue;

		if (pItem->IsInWorld())
			pItem->RemoveFromWorld();

		pItem->SetOwner(NULL);
		pItem->m_isDirty = true;
		pItem->SaveToDB(INVENTORY_SLOT_NOT_SET, 0, true, NULL);

		// Create auction
		Auction* auct = new Auction;
		auct->BuyoutPrice = buyout;
		auct->ExpiryTime = etime;
		auct->StartingPrice = bid;
		auct->HighestBid = 0;
		auct->HighestBidder = 0;	// hm
		auct->Id = sAuctionMgr.GenerateAuctionId();
		auct->Owner = _player->GetLowGUID();
		auct->pItem = pItem;
		auct->Deleted = false;
		auct->DeletedReason = 0;
		auct->DepositAmount = uint32(deposit);

		// remove deposit
		GetPlayer()->ModGold(-(int32)deposit);
		ah->AddAuction(auct);
		auct->SaveToDB(ah->GetID());

		if (HasGMPermissions())
			GMCommand_Log->write("GM %s (Account: %u) create auction: %s (Entry: %u Count: %u).", GetAccountName().c_str(), GetAccountId(), pItem->GetProto()->Name1, pItem->GetEntry(), stackSize);

		SendAuctionPlaceBidResultPacket(NULL, AUCTION_ERROR_NONE);
	}
};

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

void AuctionHouse::SendAuctionList(Player* plr, WorldPacket* packet)
{
	uint32 start_index, current_index = 0;
	uint32 counted_items = 0;
	std::string auctionString;
	uint8 levelRange1, levelRange2, usableCheck;
	int32 inventory_type, itemclass, itemsubclass, rarityCheck;

	*packet >> start_index;
	*packet >> auctionString;
	*packet >> levelRange1 >> levelRange2;
	*packet >> inventory_type >> itemclass >> itemsubclass;
	*packet >> rarityCheck >> usableCheck;

	// convert auction string to lowercase for faster parsing.
	if(auctionString.length() > 0)
	{
		for(uint32 j = 0; j < auctionString.length(); ++j)
			auctionString[j] = static_cast<char>(tolower(auctionString[j]));
	}

	WorldPacket data(SMSG_AUCTION_LIST_RESULT, 7000);
	data << uint32(0); // count of items

	auctionLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	ItemPrototype* proto;
	for(; itr != auctions.end(); ++itr)
	{
		if(itr->second->Deleted) continue;
		proto = itr->second->pItem->GetProto();

		// Check the auction for parameters

		// inventory type
		if(inventory_type != -1 && inventory_type != (int32)proto->InventoryType)
			continue;

		// class
		if(itemclass != -1 && itemclass != (int32)proto->Class)
			continue;

		// subclass
		if(itemsubclass != -1 && itemsubclass != (int32)proto->SubClass)
			continue;

		// this is going to hurt. - name
		if(auctionString.length() > 0 && !FindXinYString(auctionString, proto->lowercase_name))
			continue;

		// rarity
		if(rarityCheck != -1 && rarityCheck > (int32)proto->Quality)
			continue;

		// level range check - lower boundary
		if(levelRange1 && proto->RequiredLevel < levelRange1)
			continue;

		// level range check - high boundary
		if(levelRange2 && proto->RequiredLevel > levelRange2)
			continue;

		// usable check - this will hurt too :(
		if(usableCheck)
		{
			// allowed class
			if(proto->AllowableClass && !(plr->getClassMask() & proto->AllowableClass))
				continue;

			if(proto->RequiredLevel && proto->RequiredLevel > plr->getLevel())
				continue;

			if(proto->AllowableRace && !(plr->getRaceMask() & proto->AllowableRace))
				continue;

			if(proto->Class == 4 && proto->SubClass && !(plr->GetArmorProficiency() & (((uint32)(1)) << proto->SubClass)))
				continue;

			if(proto->Class == 2 && proto->SubClass && !(plr->GetWeaponProficiency() & (((uint32)(1)) << proto->SubClass)))
				continue;

			if(proto->RequiredSkill && (!plr->_HasSkillLine(proto->RequiredSkill) || proto->RequiredSkillRank > plr->_GetSkillLineCurrent(proto->RequiredSkill, true)))
				continue;
		}

		// Page system.
		++counted_items;
		if(counted_items >= start_index + 50)
			continue;
		current_index++;
		if(start_index && current_index < start_index) continue;

		// all checks passed -> add to packet.
		itr->second->AddToPacket(data);
		(*(uint32*)&data.contents()[0])++;
	}

	// total count
	data << uint32(1 + counted_items);
	data << uint32(300);

	auctionLock.ReleaseReadLock();
	plr->GetSession()->SendPacket(&data);
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

void AuctionHouse::LoadAuctions()
{
	QueryResult* result = CharacterDatabase.Query("SELECT * FROM auctions WHERE auctionhouse =%u", GetID());

	if(!result)
		return;

	Auction* auct;
	Field* fields;

	do
	{
		fields = result->Fetch();
		auct = new Auction;
		auct->Id = fields[0].GetUInt32();

		Item* pItem = objmgr.LoadItem(fields[2].GetUInt32());
		if(!pItem)
		{
			CharacterDatabase.Execute("DELETE FROM auctions WHERE auctionId=%u", auct->Id);
			delete auct;
			continue;
		}
		auct->pItem = pItem;
		auct->Owner = fields[3].GetUInt32();
		auct->StartingPrice = fields[4].GetUInt32();
		auct->BuyoutPrice = fields[5].GetUInt32();
		auct->ExpiryTime = fields[6].GetUInt32();
		auct->HighestBidder = fields[7].GetUInt32();
		auct->HighestBid = fields[8].GetUInt32();
		auct->DepositAmount = fields[9].GetUInt32();

		auct->DeletedReason = 0;
		auct->Deleted = false;

		auctions.insert(HM_NAMESPACE::hash_map<uint32, Auction*>::value_type(auct->Id, auct));
	}
	while(result->NextRow());
	delete result;
}
