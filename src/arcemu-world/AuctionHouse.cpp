/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
	CharacterDatabase.Execute("INSERT INTO auctions VALUES(%u, %u, "I64FMTD", %u, %u, %u, %u, %u, %u)",
		Id, AuctionHouseId, pItem->GetGUID(), Owner, BuyoutPrice, ExpiryTime, HighestBidder, HighestBid,
		DepositAmount);
}

void Auction::UpdateInDB()
{
	CharacterDatabase.Execute("UPDATE auctions SET bidder = %u, bid = %u WHERE auctionId = %u", HighestBidder, HighestBid, Id);
}

AuctionHouse::AuctionHouse(uint32 ID)
{
	dbc = dbcAuctionHouse.LookupEntry(ID);
	assert(dbc);

	cut_percent = float( float(dbc->tax) / 100.0f );
	deposit_percent = float( float(dbc->fee ) / 100.0f );
}

AuctionHouse::~AuctionHouse()
{
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	for(; itr != auctions.end(); ++itr)
		delete itr->second;
}

void AuctionHouse::QueueDeletion(Auction * auct, uint32 Reason)
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
	Auction * auct;

	list<Auction*>::iterator it = removalList.begin();
	for(; it != removalList.end(); ++it)
	{
		auct = *it;
		assert(auct->Deleted);
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
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	Auction * auct;
	for(; itr != auctions.end();)
	{
		auct = itr->second;
		++itr;

		if(t >= auct->ExpiryTime)
		{
			if(auct->HighestBidder == 0)
			{
				auct->DeletedReason = AUCTION_REMOVE_EXPIRED;
				this->SendAuctionExpiredNotificationPacket(auct);
			}
			else
			{
				auct->DeletedReason = AUCTION_REMOVE_WON;
			}

			auct->Deleted = true;
			removalList.push_back(auct);
		}
	}

	removalLock.Release();
	auctionLock.ReleaseReadLock();
}

void AuctionHouse::AddAuction(Auction * auct)
{
	// add to the map
	auctionLock.AcquireWriteLock();
	auctions.insert( HM_NAMESPACE::hash_map<uint32, Auction*>::value_type( auct->Id , auct ) );
	auctionLock.ReleaseWriteLock();

	// add the item
	itemLock.AcquireWriteLock();
	auctionedItems.insert( HM_NAMESPACE::hash_map<uint64, Item*>::value_type( auct->pItem->GetGUID(), auct->pItem ) );
	itemLock.ReleaseWriteLock();

	Log.Debug("AuctionHouse", "%u: Add auction %u, expire@ %u.", dbc->id, auct->Id, auct->ExpiryTime);
}

Auction * AuctionHouse::GetAuction(uint32 Id)
{
	Auction * ret;
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr;
	auctionLock.AcquireReadLock();
	itr = auctions.find(Id);
	ret = (itr == auctions.end()) ? 0 : itr->second;
	auctionLock.ReleaseReadLock();
	return ret;
}

void AuctionHouse::RemoveAuction(Auction * auct)
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
			sMailSystem.SendAutomatedMessage(AUCTION, dbc->id, auct->Owner, subject, "", 0, 0, auct->pItem->GetGUID(), 62);
		}break;

	case AUCTION_REMOVE_WON:
		{
			// ItemEntry:0:1
			snprintf(subject, 100, "%u:0:1", (unsigned int)auct->pItem->GetEntry());

			// <owner player guid>:bid:buyout
			snprintf(body, 200, "%X:%u:%u", (unsigned int)auct->Owner, (unsigned int)auct->HighestBid, (unsigned int)auct->BuyoutPrice);

			// Auction won by highest bidder. He gets the item.
			sMailSystem.SendAutomatedMessage(AUCTION, dbc->id, auct->HighestBidder, subject, body, 0, 0, auct->pItem->GetGUID(), 62);

			// Send a mail to the owner with his cut of the price.
			uint32 auction_cut = FL2UINT(float(cut_percent * float(auct->HighestBid)));
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
			sMailSystem.SendAutomatedMessage(AUCTION, dbc->id, auct->Owner, subject, body, amount, 0, 0, 62);

			// If it's not a buyout (otherwise the players has been already notified)
			if(auct->HighestBid < auct->BuyoutPrice || auct->BuyoutPrice == 0)
			{
				this->SendAuctionBuyOutNotificationPacket(auct);
			}
		}break;
	case AUCTION_REMOVE_CANCELLED:
		{
			snprintf(subject, 100, "%u:0:5", (unsigned int)auct->pItem->GetEntry());
			uint32 cut = uint32(float(cut_percent * auct->HighestBid));
			Player * plr = objmgr.GetPlayer(auct->Owner);
			if(cut && plr && plr->GetUInt32Value(PLAYER_FIELD_COINAGE) >= cut)
				plr->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -((int32)cut));

			sMailSystem.SendAutomatedMessage(AUCTION, GetID(), auct->Owner, subject, "", 0, 0, auct->pItem->GetGUID(), 62);

			// return bidders money
			if(auct->HighestBidder)
			{
				sMailSystem.SendAutomatedMessage(AUCTION, GetID(), auct->HighestBidder, subject, "", auct->HighestBid, 0, 0, 62);
			}

		}break;
	}

	// Remove the auction from the hashmap.
	auctionLock.AcquireWriteLock();
	itemLock.AcquireWriteLock();

	auctions.erase(auct->Id);
	auctionedItems.erase(auct->pItem->GetGUID());

	auctionLock.ReleaseWriteLock();
	itemLock.ReleaseWriteLock();

	// Destroy the item from memory (it still remains in the db)
	auct->pItem->DeleteMe();

	// Finally destroy the auction instance.
	auct->DeleteFromDB();
	delete auct;
}

void WorldSession::HandleAuctionListBidderItems( WorldPacket & recv_data )
{
	if(!_player->IsInWorld())
		return;

	uint64 guid;
	recv_data >> guid;

	Creature * pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	pCreature->auctionHouse->SendBidListPacket(_player, &recv_data);
}

void Auction::AddToPacket(WorldPacket & data)
{
	data << Id;
	data << pItem->GetEntry();

	for (uint32 i = 0; i < 6; i++)
	{
		data << pItem->GetUInt32Value(ITEM_FIELD_ENCHANTMENT + (3 * i));   // Enchantment ID
		data << uint32(pItem->GetEnchantmentApplytime(i));						 // Unknown / maybe ApplyTime
		data << pItem->GetUInt32Value(ITEM_FIELD_SPELL_CHARGES + i);	   // Charges
	}

	data << pItem->GetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID);		 // -ItemRandomSuffix / random property	 : If the value is negative its ItemRandomSuffix if its possitive its RandomItemProperty
	data << pItem->GetUInt32Value(ITEM_FIELD_PROPERTY_SEED);			  // when ItemRandomSuffix is used this is the modifier

	/******************** ItemRandomSuffix***************************
	* For what I have seen ItemRandomSuffix is like RandomItemProperty
	* The only difference is has is that it has a modifier.
	* That is the result of jewelcrafting, the effect is that the
	* enchantment is variable. That means that a enchantment can be +1 and 
	* with more Jem's +12 or so.
	* Decription for lookup: You get the enchantmentSuffixID and search the
	* DBC for the last 1 - 3 value's(depending on the enchantment).
	* That value is what I call EnchantmentValue. You guys might find a 
	* better name but for now its good enough. The formula to calculate
	* The ingame "GAIN" is:
	* (Modifier / 10000) * enchantmentvalue = EnchantmentGain;	
	*/

	data << pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT); // Amount
	data << uint32(0);				  // Unknown
	data << uint32(0);				  // Unknown
	data << uint64(Owner);			  // Owner guid
	data << HighestBid;				 // Current prize
	// hehe I know its evil, this creates a nice trough put of money
	data << uint32(50);				 // Next bid value modifier, like current bid + this value
	data << BuyoutPrice;				// Buyout
	data << uint32((ExpiryTime - UNIXTIME) * 1000); // Time left
	data << uint64(HighestBidder);	  // Last bidder
	data << HighestBid;				 // The bid of the last bidder
}

void AuctionHouse::SendBidListPacket(Player * plr, WorldPacket * packet)
{
	uint32 count = 0;

	WorldPacket data(SMSG_AUCTION_BIDDER_LIST_RESULT, 1024);
	data << uint32(0);										  // Placeholder

	Auction * auct;
	auctionLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	for(; itr != auctions.end(); ++itr)
	{
		auct = itr->second;
		if(auct->HighestBidder == plr->GetGUID())
		{
			if(auct->Deleted) continue;

			auct->AddToPacket(data);
			(*(uint32*)&data.contents()[0])++;
			++count;
		}			
	}
#ifdef USING_BIG_ENDIAN
	swap32((uint32*)&data.contents()[0]);
#endif
	data << count;
	auctionLock.ReleaseReadLock();
	plr->GetSession()->SendPacket(&data);
}

void AuctionHouse::UpdateOwner(uint32 oldGuid, uint32 newGuid)
{
	auctionLock.AcquireWriteLock();
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	Auction * auction;
	for(; itr != auctions.end(); ++itr)
	{
		auction = itr->second;
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

void AuctionHouse::SendOwnerListPacket(Player * plr, WorldPacket * packet)
{
	uint32 count = 0;

	WorldPacket data(SMSG_AUCTION_OWNER_LIST_RESULT, 1024);
	data << uint32(0);										  // Placeholder

	Auction * auct;
	auctionLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	for(; itr != auctions.end(); ++itr)
	{
		auct = itr->second;
		if(auct->Owner == plr->GetGUID())
		{
			if(auct->Deleted) continue;

			auct->AddToPacket(data);
			(*(uint32*)&data.contents()[0])++;
			++count;
		}			
	}
	data << count;
#ifdef USING_BIG_ENDIAN
	swap32((uint32*)&data.contents()[0]);
#endif
	auctionLock.ReleaseReadLock();
	plr->GetSession()->SendPacket(&data);
}

void AuctionHouse::SendAuctionOutBidNotificationPacket(Auction * auct, uint64 newBidder, uint32 newHighestBid)
{
	Player *bidder = objmgr.GetPlayer(auct->HighestBidder);
	if(bidder != NULL && bidder->IsInWorld())
	{
		uint32 outbid = (auct->HighestBid / 100) * 5;
		if(!outbid)
			outbid = 1;

		//TODO: Check this code, when a user has been bid out by instant buy out
		WorldPacket data(SMSG_AUCTION_BIDDER_NOTIFICATION, 32);
		data << GetID();
		data << auct->Id;
		data << uint64(newBidder);
		data << uint32(newHighestBid);
		data << uint32(outbid);
		data << auct->pItem->GetEntry();
		data << uint32(0);
		bidder->GetSession()->SendPacket(&data);
	}
}

void AuctionHouse::SendAuctionBuyOutNotificationPacket(Auction * auct)
{
	Player *bidder = objmgr.GetPlayer((uint32)auct->HighestBidder);
	if(bidder != NULL && bidder->IsInWorld())
	{
		uint32 outbid = (auct->HighestBid / 100) * 5;
		if(!outbid)
			outbid = 1;

		WorldPacket data(SMSG_AUCTION_BIDDER_NOTIFICATION, 32);
		data << GetID();
		data << auct->Id;
		data << uint64(auct->HighestBidder);
		data << uint32(0);
		data << uint32(outbid);
		data << auct->pItem->GetEntry();
		data << uint32(0);
		bidder->GetSession()->SendPacket(&data);
	}

	Player *owner = objmgr.GetPlayer((uint32)auct->Owner);
	if(owner != NULL && owner->IsInWorld())
	{
		WorldPacket ownerData(SMSG_AUCTION_OWNER_NOTIFICATION, 28);
		ownerData << GetID(); 
		ownerData << auct->Id;
		ownerData << uint32(0);
		ownerData << uint32(0);
		ownerData << uint32(0);
		ownerData << auct->pItem->GetEntry();
		ownerData << uint32(0);
		owner->GetSession()->SendPacket(&ownerData);
	}
}

void AuctionHouse::SendAuctionExpiredNotificationPacket(Auction * auct)
{
	//TODO: I don't know the net code... so: TODO ;-)

	//Player *owner = objmgr.GetPlayer((uint32)auct->Owner);
	//if(owner != NULL && owner->IsInWorld())
	//{
	//  WorldPacket data(SMSG_AUCTION_REMOVED_NOTIFICATION, ??);
	//  data << GetID();     
	//  data << auct->Id;    
	//  data << uint32(0);   // I don't have an active blizz account..so I can't get the netcode by myself.
	//  data << uint32(0);
	//  data << uint32(0);
	//  data << auct->pItem->GetEntry();
	//  data << uint32(0);
	//  owner->GetSession()->SendPacket(&data);
	//}
}

void WorldSession::SendAuctionPlaceBidResultPacket(uint32 itemId, uint32 error)
{
	WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 16);
	data << itemId;
	data << uint32(AUCTION_BID);
	data << error;
	data << uint32(0);
	SendPacket(&data);
}

void WorldSession::HandleAuctionPlaceBid( WorldPacket & recv_data )
{
	if(!_player->IsInWorld())
		return;

	uint64 guid;
	recv_data >> guid;

	uint32 auction_id, price;
	recv_data >> auction_id >> price;

	Creature * pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	// Find Item
	AuctionHouse * ah = pCreature->auctionHouse;
	Auction * auct = ah->GetAuction(auction_id);
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

	if(_player->GetUInt32Value(PLAYER_FIELD_COINAGE) < price)
	{
		SendAuctionPlaceBidResultPacket(0, AUCTION_ERROR_MONEY);
		return;
	}

	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -((int32)price));
	if(auct->HighestBidder != 0)
	{
		// Return the money to the last highest bidder.
		char subject[100];
		snprintf(subject, 100, "%u:0:0", (int)auct->pItem->GetEntry());
		sMailSystem.SendAutomatedMessage(AUCTION, ah->GetID(), auct->HighestBidder, subject, "", auct->HighestBid, 0, 0, 62);

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

void WorldSession::HandleCancelAuction( WorldPacket & recv_data)
{
	if(!_player->IsInWorld())
		return;

	uint64 guid;
	recv_data >> guid;

	uint32 auction_id;
	recv_data >> auction_id;

	Creature * pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	// Find Item
	Auction * auct = pCreature->auctionHouse->GetAuction(auction_id);
	if(auct == 0) return;

	pCreature->auctionHouse->QueueDeletion(auct, AUCTION_REMOVE_CANCELLED);

	// Send response packet.
	WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
	data << auction_id << uint32(AUCTION_CANCEL) << uint32(0);
	SendPacket(&data);

	// Re-send the owner list.
	pCreature->auctionHouse->SendOwnerListPacket(_player, 0);
}

void WorldSession::HandleAuctionSellItem( WorldPacket & recv_data )
{
	if (!_player->IsInWorld())
		return;

	uint64 guid,item;
	uint32 bid,buyout,etime;	// etime is in minutes

	recv_data >> guid >> item;
	recv_data >> bid >> buyout >> etime;

	Creature * pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(  !pCreature || !pCreature->auctionHouse )
		return;		// NPC doesnt exist or isnt an auctioneer

	// Get item
	Item * pItem = _player->GetItemInterface()->GetItemByGUID(item);
	if( !pItem || pItem->IsSoulbound() || pItem->HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_CONJURED ) )
	{
		WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
		data << uint32(0);
		data << uint32(AUCTION_CREATE);
		data << uint32(AUCTION_ERROR_ITEM);
		SendPacket(&data);
		return;
	};

	AuctionHouse * ah = pCreature->auctionHouse;

	uint32 item_worth = pItem->GetProto()->SellPrice * pItem->GetUInt32Value(ITEM_FIELD_STACK_COUNT);
	uint32 item_deposit = (uint32)(item_worth * ah->deposit_percent) * (uint32)(etime / 240.0f); // deposit is per 4 hours

	if (_player->GetUInt32Value(PLAYER_FIELD_COINAGE) < item_deposit)	// player cannot afford deposit
	{
		WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
		data << uint32(0);
		data << uint32(AUCTION_CREATE);
		data << uint32(AUCTION_ERROR_MONEY);
		SendPacket(&data);
		return;
	}

	pItem = _player->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(item, false);
	if (!pItem){
		WorldPacket data(SMSG_AUCTION_COMMAND_RESULT, 8);
		data << uint32(0);
		data << uint32(AUCTION_CREATE);
		data << uint32(AUCTION_ERROR_ITEM);
		SendPacket(&data);
		return;
	};

	if( pItem->IsInWorld() )
	{
		pItem->RemoveFromWorld();
	}

	pItem->SetOwner(NULL);
	pItem->m_isDirty = true;
	pItem->SaveToDB(INVENTORY_SLOT_NOT_SET, 0, true, NULL);

	// Create auction
	Auction * auct = new Auction;
	auct->BuyoutPrice = buyout;
	auct->ExpiryTime = (uint32)UNIXTIME + (etime * 60);
	auct->HighestBid = bid;
	auct->HighestBidder = 0;	// hm
	auct->Id = sAuctionMgr.GenerateAuctionId();
	auct->Owner = _player->GetLowGUID();
	auct->pItem = pItem;
	auct->Deleted = false;
	auct->DeletedReason = 0;
	auct->DepositAmount = item_deposit;

	// remove deposit
	_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)item_deposit);

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

void WorldSession::HandleAuctionListOwnerItems( WorldPacket & recv_data )
{
	if(!_player->IsInWorld())
		return;

	uint64 guid;
	recv_data >> guid;

	Creature * pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	pCreature->auctionHouse->SendOwnerListPacket(_player, &recv_data);
}

void AuctionHouse::SendAuctionList(Player * plr, WorldPacket * packet)
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
			auctionString[j] = tolower(auctionString[j]);
	}

	WorldPacket data(SMSG_AUCTION_LIST_RESULT, 7000);
	data << uint32(0);

	auctionLock.AcquireReadLock();
	HM_NAMESPACE::hash_map<uint32, Auction*>::iterator itr = auctions.begin();
	ItemPrototype * proto;
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
		if(rarityCheck != -1 && rarityCheck != (int32)proto->Quality)
			continue;

		// level range check - lower boundary
		if(levelRange1 && proto->ItemLevel < levelRange1)
			continue;

		// level range check - high boundary
		if(levelRange2 && proto->ItemLevel > levelRange2)
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

			if(proto->Class == 4 && proto->SubClass && !(plr->GetArmorProficiency()&(((uint32)(1))<<proto->SubClass)))
				continue;

			if(proto->Class == 2 && proto->SubClass && !(plr->GetWeaponProficiency()&(((uint32)(1))<<proto->SubClass)))
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
#ifdef USING_BIG_ENDIAN
	swap32((uint32*)&data.contents()[0]);
#endif

	auctionLock.ReleaseReadLock();
	plr->GetSession()->SendPacket(&data);
}

void WorldSession::HandleAuctionListItems( WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN
		uint64 guid;
	recv_data >> guid;

	Creature * pCreature = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(guid));
	if(!pCreature || !pCreature->auctionHouse)
		return;

	pCreature->auctionHouse->SendAuctionList(_player, &recv_data);
}

void AuctionHouse::LoadAuctions()
{
	QueryResult *result = CharacterDatabase.Query("SELECT * FROM auctions WHERE auctionhouse =%u", GetID());

	if( !result )
		return;

	Auction * auct;
	Field * fields;

	do
	{
		fields = result->Fetch();
		auct = new Auction;
		auct->Id = fields[0].GetUInt32();

		Item * pItem = objmgr.LoadItem(fields[2].GetUInt64());
		if(!pItem)
		{
			CharacterDatabase.Execute("DELETE FROM auctions WHERE auctionId=%u",auct->Id);
			delete auct;
			continue;
		}
		auct->pItem = pItem;
		auct->Owner = fields[3].GetUInt32();
		auct->BuyoutPrice = fields[4].GetUInt32();
		auct->ExpiryTime = fields[5].GetUInt32();
		auct->HighestBidder = fields[6].GetUInt32();
		auct->HighestBid = fields[7].GetUInt32();
		auct->DepositAmount = fields[8].GetUInt32();
		auct->DeletedReason = 0;
		auct->Deleted = false;

		auctions.insert( HM_NAMESPACE::hash_map<uint32, Auction*>::value_type( auct->Id, auct ) );
	} while (result->NextRow());
	delete result;
}
