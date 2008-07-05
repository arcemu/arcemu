/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef AUCTIONHOUSE_H
#define AUCTIONHOUSE_H

enum AuctionRemoveType
{
	AUCTION_REMOVE_EXPIRED,
	AUCTION_REMOVE_WON,
	AUCTION_REMOVE_CANCELLED,
};
enum AUCTIONRESULT
{
	AUCTION_CREATE,
	AUCTION_CANCEL,
	AUCTION_BID,
	AUCTION_BUYOUT,
};
enum AUCTIONRESULTERROR
{
	AUCTION_ERROR_NONE = 0,
	AUCTION_ERROR_INTERNAL = 2,
	AUCTION_ERROR_MONEY = 3,
	AUCTION_ERROR_ITEM = 4,
	AUCTION_ERROR_BID_OWN_AUCTION = 10
};
enum AuctionMailResult
{
	AUCTION_OUTBID,
	AUCTION_WON,
	AUCTION_SOLD,
	AUCTION_EXPIRED,
	AUCTION_EXPIRED2,
	AUCTION_CANCELLED,
};

struct Auction
{
	uint32 Id;

	uint32 Owner;
	uint32 HighestBidder;
	uint32 HighestBid;

	uint32 BuyoutPrice;
	uint32 DepositAmount;

	uint32 ExpiryTime;
	Item * pItem;

	void DeleteFromDB();
	void SaveToDB(uint32 AuctionHouseId);
	void UpdateInDB();
	void AddToPacket(WorldPacket & data);
	bool Deleted;
	uint32 DeletedReason;
};

class AuctionHouse
{
public:
	AuctionHouse(uint32 ID);
	~AuctionHouse();

	ASCENT_INLINE uint32 GetID() { return dbc->id; }
	void LoadAuctions();

	void UpdateAuctions();
	void UpdateDeletionQueue();

	void RemoveAuction(Auction * auct);
	void AddAuction(Auction * auct);
	Auction * GetAuction(uint32 Id);
	void QueueDeletion(Auction * auct, uint32 Reason);

	void SendOwnerListPacket(Player * plr, WorldPacket * packet);
	void SendBidListPacket(Player * plr, WorldPacket * packet);
	void SendAuctionBuyOutNotificationPacket(Auction * auct);
	void SendAuctionOutBidNotificationPacket(Auction * auct, uint64 newBidder, uint32 newHighestBid);
	void SendAuctionExpiredNotificationPacket(Auction * auct);
	void SendAuctionList(Player * plr, WorldPacket * packet);

private:
	RWLock itemLock;
	HM_NAMESPACE::hash_map<uint64, Item*> auctionedItems;

	RWLock auctionLock;
	HM_NAMESPACE::hash_map<uint32, Auction*> auctions;

	Mutex removalLock;
	list<Auction*> removalList;

	AuctionHouseDBC * dbc;

public:
	float cut_percent;
	float deposit_percent;
};
#endif

