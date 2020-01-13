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

void Mailbox::AddMessage(MailMessage* Message)
{
	Messages[Message->message_id] = *Message;
}

void Mailbox::DeleteMessage(uint32 MessageId, bool sql)
{
	Messages.erase(MessageId);
	if(sql)
		CharacterDatabase.WaitExecute("DELETE FROM mailbox WHERE message_id = %u", MessageId);
}

WorldPacket* Mailbox::BuildMailboxListingPacket()
{
	WorldPacket* data = new WorldPacket(SMSG_MAIL_LIST_RESULT, 500);
	MessageMap::iterator itr;
	uint32 realcount = 0;
	uint32 count = 0;
	uint32 t = (uint32)UNIXTIME;
	*data << uint32(0);	 // realcount - this can be used to tell the client we have more mail than that fits into this packet
	*data << uint8(0);	 // size placeholder

	for(itr = Messages.begin(); itr != Messages.end(); ++itr)
	{
		if(itr->second.expire_time && t > itr->second.expire_time)
			continue;	   // expired mail -> skip it

		if((uint32)UNIXTIME < itr->second.delivery_time)
			continue;		// undelivered

		if(count >= 50) //VLack: We could calculate message sizes instead of this, but the original code did a break at 50, so I won't fix this up if no one felt the need to do so before ;-)
		{
			++realcount;
			continue;
		}

		if(itr->second.AddMessageDataToPacket(*data))
		{
			++count;
			++realcount;
		}
	}

	data->put<uint32>(0, realcount);
	data->put<uint8>(4, static_cast< uint8 >(count));

	// do cleanup on request mail
	CleanupExpiredMessages();
	return data;
}

void Mailbox::CleanupExpiredMessages()
{
	MessageMap::iterator itr, it2;
	uint32 curtime = (uint32)UNIXTIME;

	for(itr = Messages.begin(); itr != Messages.end();)
	{
		it2 = itr++;
		if(it2->second.expire_time && it2->second.expire_time < curtime)
		{
			Messages.erase(it2);
		}
	}
}

void Mailbox::FillTimePacket(WorldPacket & data)
{
	uint32 c = 0;
	MessageMap::iterator iter = Messages.begin();
	data << uint32(0) << uint32(0);

	for(; iter != Messages.end(); ++iter)
	{
		if(iter->second.checked_flag & MAIL_CHECK_MASK_READ)
			continue;

		if(iter->second.deleted_flag == 0  && (uint32)UNIXTIME >= iter->second.delivery_time)
		{
			// unread message, w00t.
			++c;
			data << uint64(iter->second.sender_guid);
			data << uint32(0);
			data << uint32(0);// money or something?
			data << uint32(iter->second.stationery);
			//data << float(UNIXTIME-iter->second.delivery_time);
			data << float(-9.0f);	// maybe the above?
		}
	}

	if(c == 0)
	{

		*(uint32*)(&data.contents()[0]) = 0xc7a8c000;
	}
	else
	{

		*(uint32*)(&data.contents()[4]) = c;
	}
}

void Mailbox::Load(QueryResult* result)
{
	if(!result)
		return;

	Field* fields;
	MailMessage msg;
	uint32 i;
	char* str;
	char* p;
	uint32 itemguid;

	do
	{
		fields = result->Fetch();

		// Create message struct
		i = 0;
		msg.items.clear();
		msg.message_id = fields[i++].GetUInt32();
		msg.message_type = fields[i++].GetUInt32();
		msg.player_guid = fields[i++].GetUInt32();
		msg.sender_guid = fields[i++].GetUInt32();
		msg.subject = fields[i++].GetString();
		msg.body = fields[i++].GetString();
		msg.money = fields[i++].GetUInt32();
		str = (char*)fields[i++].GetString();
		p = strchr(str, ',');
		if(p == NULL)
		{
			itemguid = atoi(str);
			if(itemguid != 0)
				msg.items.push_back(itemguid);
		}
		else
		{
			while(p)
			{
				*p = 0;
				p++;

				itemguid = atoi(str);
				if(itemguid != 0)
					msg.items.push_back(itemguid);

				str = p;
				p = strchr(str, ',');
			}
		}

		msg.cod = fields[i++].GetUInt32();
		msg.stationery = fields[i++].GetUInt32();
		msg.expire_time = fields[i++].GetUInt32();
		msg.delivery_time = fields[i++].GetUInt32();
		msg.checked_flag = fields[i++].GetUInt32();
		msg.deleted_flag = fields[i++].GetBool();

		// Add to the mailbox
		AddMessage(&msg);

	}
	while(result->NextRow());
}
