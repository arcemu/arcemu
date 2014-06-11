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

initialiseSingleton(AuctionMgr);

void AuctionMgr::LoadAuctionHouses()
{
	Log.Notice("AuctionMgr", "Loading Auction Houses...");

	QueryResult* res = CharacterDatabase.Query("SELECT MAX(auctionId) FROM auctions");
	if(res)
	{
		maxId.SetVal(res->Fetch()[0].GetUInt32());
		delete res;
	}

	res = WorldDatabase.Query("SELECT DISTINCT ahgroup FROM auctionhouse");
	map<uint32, AuctionHouse*> tempmap;
	if(res)
	{
		uint32 period = (res->GetRowCount() / 20) + 1;
		uint32 c = 0;
		do
		{
            AuctionHouse* ah = new AuctionHouse(res->Fetch()[0].GetUInt32());
			ah->LoadAuctions();
			auctionHouses.push_back(ah);
			tempmap.insert(make_pair(res->Fetch()[0].GetUInt32(), ah));
			if(!((++c) % period))
				Log.Notice("AuctionHouse", "Done %u/%u, %u%% complete.", c, res->GetRowCount(), c * 100 / res->GetRowCount());

		}
		while(res->NextRow());
		delete res;
	}

	res = WorldDatabase.Query("SELECT creature_entry, ahgroup FROM auctionhouse");
	if(res)
	{
		do
		{
            uint32 entry = res->Fetch()[0].GetUInt32();
            uint32 ah_entry = res->Fetch()[0].GetUInt32();
            if (!CreatureNameStorage.LookupEntry(entry))
            {
                Log.Error("AuctionMgr", "Creature (entry: %u) was not found for auction (entry: %u).", entry, ah_entry);
                continue;
            }

            auctionHouseEntryMap.insert(make_pair(entry, tempmap[ah_entry]));
		}
		while(res->NextRow());
		delete res;
	}
    Log.Success("AuctionMgr", "Loaded %u auction house entries.", auctionHouseEntryMap.size());
}

AuctionHouse* AuctionMgr::GetAuctionHouse(uint32 Entry)
{
	HM_NAMESPACE::hash_map<uint32, AuctionHouse*>::iterator itr = auctionHouseEntryMap.find(Entry);
    if(itr == auctionHouseEntryMap.end())
        return NULL;
	return itr->second;
}

void AuctionMgr::Update()
{
	if((++loopcount % 100))
		return;

    for(vector<AuctionHouse*>::iterator itr = auctionHouses.begin(); itr != auctionHouses.end(); ++itr)
	{
		(*itr)->UpdateDeletionQueue();

		// Actual auction loop is on a separate timer.
		if(!(loopcount % 1200))
			(*itr)->UpdateAuctions();
	}
}
