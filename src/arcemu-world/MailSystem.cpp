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
initialiseSingleton(MailSystem);

void MailSystem::StartMailSystem()
{

}

MailError MailSystem::DeliverMessage(uint64 recipent, MailMessage* message)
{
	// assign a new id
	message->message_id = objmgr.GenerateMailID();

	Player* plr = objmgr.GetPlayer((uint32)recipent);
	if(plr != NULL)
	{
		plr->m_mailBox.AddMessage(message);
		if((uint32)UNIXTIME >= message->delivery_time)
		{
			uint32 v = 0;
			plr->GetSession()->OutPacket(SMSG_RECEIVED_MAIL, 4, &v);
		}
	}

	SaveMessageToSQL(message);
	return MAIL_OK;
}

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

void MailSystem::SaveMessageToSQL(MailMessage* message)
{
	stringstream ss;


	ss << "DELETE FROM mailbox WHERE message_id = ";
	ss << message->message_id;
	ss << ";";

	CharacterDatabase.ExecuteNA(ss.str().c_str());

	ss.rdbuf()->str("");

	vector< uint32 >::iterator itr;
	ss << "INSERT INTO mailbox VALUES("
	   << message->message_id << ","
	   << message->message_type << ","
	   << message->player_guid << ","
	   << message->sender_guid << ",\'"
	   << CharacterDatabase.EscapeString(message->subject) << "\',\'"
	   << CharacterDatabase.EscapeString(message->body) << "\',"
	   << message->money << ",'";

	for(itr = message->items.begin(); itr != message->items.end(); ++itr)
		ss << (*itr) << ",";

	ss << "',"
	   << message->cod << ","
	   << message->stationery << ","
	   << message->expire_time << ","
	   << message->delivery_time << ","
	   << message->checked_flag << ","
	   << message->deleted_flag << ");";

	CharacterDatabase.ExecuteNA(ss.str().c_str());
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

void MailSystem::RemoveMessageIfDeleted(uint32 message_id, Player* plr)
{
	MailMessage* msg = plr->m_mailBox.GetMessage(message_id);
	if(msg == 0) return;

	if(msg->deleted_flag)   // we've deleted from inbox
		plr->m_mailBox.DeleteMessage(message_id, true);   // wipe the message
}

void MailSystem::SendAutomatedMessage(uint32 type, uint64 sender, uint64 receiver, string subject, string body,
                                      uint32 money, uint32 cod, vector<uint64> &item_guids, uint32 stationery, MailCheckMask checked, uint32 deliverdelay)
{
	// This is for sending automated messages, for example from an auction house.
	MailMessage msg;
	msg.message_type = type;
	msg.sender_guid = sender;
	msg.player_guid = receiver;
	msg.subject = subject;
	msg.body = body;
	msg.money = money;
	msg.cod = cod;
	for(vector<uint64>::iterator itr = item_guids.begin(); itr != item_guids.end(); ++itr)
		msg.items.push_back(Arcemu::Util::GUID_LOPART(*itr));

	msg.stationery = stationery;
	msg.delivery_time = (uint32)UNIXTIME + deliverdelay;

	// 30 days expiration time for unread mail + possible delivery delay.
	if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
		msg.expire_time = (uint32)UNIXTIME + deliverdelay + (TIME_DAY * MAIL_DEFAULT_EXPIRATION_TIME);
	else
		msg.expire_time = 0;

	msg.deleted_flag = false;
	msg.checked_flag = checked;

	// Send the message.
	DeliverMessage(receiver, &msg);
}

//overload to keep backward compatibility (passing just 1 item guid instead of a vector)
void MailSystem::SendAutomatedMessage(uint32 type, uint64 sender, uint64 receiver, string subject, string body, uint32 money,
                                      uint32 cod, uint64 item_guid, uint32 stationery, MailCheckMask checked, uint32 deliverdelay)
{
	vector<uint64> item_guids;
	if(item_guid != 0)
		item_guids.push_back(item_guid);
	SendAutomatedMessage(type, sender, receiver, subject, body, money, cod, item_guids, stationery, checked, deliverdelay);
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
