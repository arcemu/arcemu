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

#include "StdAfx.h"

initialiseSingleton( AuctionMgr );

void AuctionMgr::LoadAuctionHouses()
{
	Log.Notice("AuctionMgr", "Loading Auction Houses...");

	QueryResult * res = CharacterDatabase.Query("SELECT MAX(auctionId) FROM auctions");
	if(res)
	{
		maxId = res->Fetch()[0].GetUInt32();
		delete res;
	}

	res = WorldDatabase.Query("SELECT DISTINCT `group` FROM auctionhouse");
	AuctionHouse * ah;
	map<uint32, AuctionHouse*> tempmap;
	if(res)
	{
		uint32 period = (res->GetRowCount() / 20) + 1;
		uint32 c = 0;
		do
		{
			ah = new AuctionHouse(res->Fetch()[0].GetUInt32());
			ah->LoadAuctions();
			auctionHouses.push_back(ah);
			tempmap.insert( make_pair( res->Fetch()[0].GetUInt32(), ah ) );
			if( !((++c) % period) )
				Log.Notice("AuctionHouse", "Done %u/%u, %u%% complete.", c, res->GetRowCount(), float2int32( (float(c) / float(res->GetRowCount()))*100.0f ));

		}while(res->NextRow());
		delete res;
	}

	res = WorldDatabase.Query("SELECT creature_entry, `group` FROM auctionhouse");
	if(res)
	{
		do 
		{
			auctionHouseEntryMap.insert( make_pair( res->Fetch()[0].GetUInt32(), tempmap[res->Fetch()[1].GetUInt32()] ) );
		} while(res->NextRow());
		delete res;
	}
}

AuctionHouse * AuctionMgr::GetAuctionHouse(uint32 Entry)
{
	HM_NAMESPACE::hash_map<uint32, AuctionHouse*>::iterator itr = auctionHouseEntryMap.find(Entry);
	if(itr == auctionHouseEntryMap.end()) return NULL;
	return itr->second;
}

void AuctionMgr::Update()
{
	if((++loopcount % 100))
		return;
		
	vector<AuctionHouse*>::iterator itr = auctionHouses.begin();
	for(; itr != auctionHouses.end(); ++itr)
	{
		(*itr)->UpdateDeletionQueue();

		// Actual auction loop is on a seperate timer.
		if(!(loopcount % 1200))
			(*itr)->UpdateAuctions();
	}
}
