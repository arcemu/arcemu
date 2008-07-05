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

#ifndef __AUCTIONMGR_H
#define __AUCTIONMGR_H

class AuctionHouse;
class AuctionMgr : public Singleton <AuctionMgr>
{
public:
	AuctionMgr()
	{
		loopcount = 0;
		maxId = 1;
	}

	~AuctionMgr()
	{
		vector<AuctionHouse*>::iterator itr = auctionHouses.begin();
		for(; itr != auctionHouses.end(); ++itr)
			delete (*itr);
	}

	void LoadAuctionHouses();
	void Update();

	AuctionHouse * GetAuctionHouse(uint32 Entry);

	uint32 GenerateAuctionId()
	{ 
		lock.Acquire();
		uint32 id=++maxId;
		lock.Release();
		return id;
	}

private:
	HM_NAMESPACE::hash_map<uint32, AuctionHouse*> auctionHouseEntryMap;
	vector<AuctionHouse*> auctionHouses;
	volatile uint32 maxId;
	Mutex lock;
	uint32 loopcount;
};

#define sAuctionMgr AuctionMgr::getSingleton()

#endif
