/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"

bool MailMessage::AddMessageDataToPacket(WorldPacket & data)
{
	uint8 i = 0;
	uint32 j;
	vector<uint32>::iterator itr;
	Item* pItem;

	// add stuff
	if(deleted_flag)
		return false;

	uint8 guidsize;
	if(message_type == 0)
		guidsize = 8;
	else
		guidsize = 4;

	size_t msize = 2 + 4 + 1 + guidsize + 7 * 4 + (subject.size() + 1) + (body.size() + 1) + 1 + (items.size() * (1 + 2 * 4 + 7 * (3 * 4) + 6 * 4 + 1));

	data << uint16(msize);     // message size
	data << uint32(message_id);
	data << uint8(message_type);

	switch(message_type)
	{
		case NORMAL:
			data << uint64(sender_guid);
			break;
		case COD:
		case AUCTION:
		case GAMEOBJECT:
		case ITEM:
			data << uint32(Arcemu::Util::GUID_LOPART(sender_guid));
			break;
		case CREATURE:
			data << uint32( Arcemu::Util::GET_CREATURE_ENTRY_FROM_GUID(sender_guid));
			break;
	}

	data << uint32(cod);			// cod
	data << uint32(0);
	data << uint32(stationery);
	data << uint32(money);		// money
	data << uint32(checked_flag);           // "checked" flag
	data << float((expire_time - uint32(UNIXTIME)) / 86400.0f);
	data << uint32(0);	// mail template
	data << subject;
	data << body;

	data << uint8(items.size());		// item count

	if(!items.empty())
	{
		for(itr = items.begin(); itr != items.end(); ++itr)
		{
			pItem = objmgr.LoadItem(*itr);
			if(pItem == NULL)
				continue;

			data << uint8(i++);
			data << uint32(pItem->GetLowGUID());
			data << uint32(pItem->GetEntry());

			for(j = 0; j < 7; ++j)
			{
				data << uint32(pItem->GetEnchantmentId(j));
				data << uint32(pItem->GetEnchantmentDuration(j));
				data << uint32(0);
			}

			data << uint32(pItem->GetItemRandomPropertyId());
			data << uint32(pItem->GetItemRandomSuffixFactor());
			data << uint32(pItem->GetStackCount());
			data << uint32(pItem->GetChargesLeft());
			data << uint32(pItem->GetDurabilityMax());
			data << uint32(pItem->GetDurability());
			data << uint8(0);   // unknown

			delete pItem;
		}

	}

	return true;

}

