/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2020 <http://www.ArcEmu.org/>
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

#include "ItemPacketHandlers.h"
#include "StdAfx.h"

DEFINE_PACKET_HANDLER_METHOD( ItemRefundInfoPacketHandler )
{
	Player *_player = session.GetPlayer();
	CHECK_INWORLD_RETURN

	LOG_DEBUG("Recieved CMSG_ITEMREFUNDINFO.");

	Arcemu::GamePackets::Items::CItemRefundInfo cRefundInfo;
	cRefundInfo.deserialize( recv_data );

	Item* itm = _player->GetItemInterface()->GetItemByGUID(cRefundInfo.guid);
	if(itm == NULL)
		return;

	if(itm->IsEligibleForRefund())
	{
		std::pair < time_t, uint32 > RefundEntry;

		RefundEntry = _player->GetItemInterface()->LookupRefundable(cRefundInfo.guid);

		if(RefundEntry.first == 0 || RefundEntry.second == 0)
			return;

		ItemExtendedCostEntry* ex =
		    dbcItemExtendedCost.LookupEntryForced(RefundEntry.second);
		if(ex == NULL)
			return;

		ItemPrototype* proto = itm->GetProto();

		Arcemu::GamePackets::Items::SItemRefundInfo sRefundInfo;
		sRefundInfo.guid = uint64(cRefundInfo.guid);
		sRefundInfo.buyprice = uint32(proto->BuyPrice);
		sRefundInfo.honor = uint32(ex->honor);
		sRefundInfo.arena = uint32(ex->arena);

		for(int i = 0; i < 5; ++i)
		{
			sRefundInfo.item[ i ] = uint32(ex->item[i]);
			sRefundInfo.item_count[ i ] = uint32(ex->count[i]);
		}

		sRefundInfo.unk1 = uint32(0);	// always seems to be 0

		uint32* played = _player->GetPlayedtime();

		if(played[1] > (RefundEntry.first + 60 * 60 * 2))
			sRefundInfo.remaining_time = uint32(0);
		else
			sRefundInfo.remaining_time = uint32(RefundEntry.first);

		WorldPacket packet;
		sRefundInfo.serialize( packet );
		session.SendPacket(&packet);

		LOG_DEBUG("Sent SMSG_ITEMREFUNDINFO.");
	}
}
