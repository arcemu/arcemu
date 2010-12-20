/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

	Player * plr = objmgr.GetPlayer((uint32)recipent);
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

WorldPacket * Mailbox::BuildMailboxListingPacket()
{
	WorldPacket * data = new WorldPacket(SMSG_MAIL_LIST_RESULT, 500);
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
	data->put<uint8>(4, static_cast< uint8 >( count )); 

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

bool MailMessage::AddMessageDataToPacket(WorldPacket& data)
{
	uint8 i = 0;
	uint32 j;
	vector<uint32>::iterator itr;
	Item * pItem;

	// add stuff
	if(deleted_flag)
		return false;

	uint8 guidsize;
	if( message_type == 0 )
		guidsize = 8;
	else
		guidsize = 4;

	size_t msize = 2 + 4 + 1 + guidsize + 7 * 4 + ( subject.size() + 1 ) + ( body.size() + 1 ) + 1 + ( items.size() * ( 1 + 2*4 + 7 * ( 3*4 ) + 6*4 + 1 ) );

	data << uint16( msize );   // message size
	data << uint32( message_id );
	data << uint8( message_type );
	
	switch( message_type ){
	case NORMAL:
		data << uint64( sender_guid ); 
		break;
	case COD:
	case AUCTION:
	case CREATURE:
	case GAMEOBJECT:
	case ITEM: 
		data << uint32(  Arcemu::Util::GUID_LOPART( sender_guid ) );
		break;
	}

	data << uint32( cod );			// cod
	data << uint32( 0 );
	data << uint32( stationery );
	data << uint32( money );		// money
	data << uint32( 0x10 );         // "checked" flag
	data << float( ( expire_time - uint32( UNIXTIME ) ) / 86400.0f );
	data << uint32( 0 );	// mail template
	data << subject;
    data << body;

	data << uint8( items.size() );		// item count

	if( !items.empty( ) )
	{
		for( itr = items.begin( ); itr != items.end( ); ++itr )
		{
			pItem = objmgr.LoadItem( *itr );
			if( pItem == NULL )
				continue;

			data << uint8( i++ );
            data << uint32( pItem->GetLowGUID() );
			data << uint32( pItem->GetEntry() );

			for( j = 0; j < 7; ++j ){
				data << uint32( pItem->GetEnchantmentId( j ) );
				data << uint32( pItem->GetEnchantmentDuration( j ) );
				data << uint32( 0 );
			}

            data << uint32( pItem->GetItemRandomPropertyId() );
			data << uint32( pItem->GetItemRandomSuffixFactor() );
			data << uint32( pItem->GetStackCount() );
			data << uint32( pItem->GetChargesLeft() );
            data << uint32( pItem->GetDurabilityMax() );
            data << uint32( pItem->GetDurability() );
			data << uint8( 0 ); // unknown

			delete pItem;
		}

	}

	return true;

}

void MailSystem::SaveMessageToSQL(MailMessage * message)
{
	stringstream ss;


    ss << "DELETE FROM mailbox WHERE message_id = ";
    ss << message->message_id;
    ss << ";";

    CharacterDatabase.ExecuteNA( ss.str().c_str() );

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

	for( itr = message->items.begin( ); itr != message->items.end( ); ++itr )
		ss << (*itr) << ",";

	ss << "'," 
		<< message->cod << ","
		<< message->stationery << ","
		<< message->expire_time << ","
		<< message->delivery_time << ","
		<< message->copy_made << ","
		<< message->read_flag << ","
		<< message->deleted_flag << ");";
	
    CharacterDatabase.ExecuteNA(ss.str().c_str());
}

void WorldSession::HandleSendMail(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	MailMessage msg;
	uint64 gameobject;
	uint32 unk2;
	uint8 itemcount;
	uint8 itemslot;
	uint8 i;
	uint64 itemguid;
	vector< Item* > items;
	vector< Item* >::iterator itr;
	string recepient;
	Item * pItem;
	//uint32 err = MAIL_OK;

	recv_data >> gameobject >> recepient;
	recv_data >> msg.subject >> msg.body >> msg.stationery;
	recv_data >> unk2 >> itemcount;

	if( itemcount > 12 || msg.body.find("%") != string::npos || msg.subject.find("%") != string::npos)
	{
		//SystemMessage("Sorry, Ascent does not support sending multiple items at this time. (don't want to lose your item do you) Remove some items, and try again.");
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	// Search for the recipient
	PlayerInfo* player = ObjectMgr::getSingleton().GetPlayerInfoByName(recepient.c_str());
	if( player == NULL )
	{
		SendMailError( MAIL_ERR_RECIPIENT_NOT_FOUND );
		return;
	}

	for( i = 0; i < itemcount; ++i )
	{
		recv_data >> itemslot;
		recv_data >> itemguid;

		pItem = _player->GetItemInterface()->GetItemByGUID( itemguid );
        if( pItem == NULL || pItem->IsSoulbound() || pItem->IsConjured() )
		{
			SendMailError( MAIL_ERR_INTERNAL_ERROR );
			return;
		}
		if(pItem->IsAccountbound() && GetAccountId() !=  player->acct) // don't mail account-bound items to another account
		{
			WorldPacket data(SMSG_SEND_MAIL_RESULT, 16);
			data << uint32(0);
			data << uint32(0); 
			data << uint32(MAIL_ERR_BAG_FULL); 
			data << uint32(INV_ERR_ARTEFACTS_ONLY_FOR_OWN_CHARACTERS); 
			SendPacket(&data);
			return;
		}

		items.push_back( pItem );
	}
	
	recv_data >> msg.money;
	recv_data >> msg.cod;
	// left over: (TODO- FIX ME BURLEX!)
	// uint32
	// uint32
	// uint8
	
	bool interfaction = false;
	if( sMailSystem.MailOption( MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION ) || (HasGMPermissions() && sMailSystem.MailOption( MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION_GM ) ) )
	{
		interfaction = true;
	}

	// Check we're sending to the same faction (disable this for testing)
	if( player->team != _player->GetTeam() && !interfaction )
	{
		SendMailError( MAIL_ERR_NOT_YOUR_ALLIANCE );
		return;
	}

	// Check if we're sending mail to ourselves
	if( strcmp(player->name, _player->GetName()) == 0 && !GetPermissionCount())
	{
		SendMailError(MAIL_ERR_CANNOT_SEND_TO_SELF);
		return;
	}

	if( msg.stationery == MAIL_STATIONERY_GM && !HasGMPermissions())
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	// Instant delivery time by default.
	msg.delivery_time = (uint32)UNIXTIME;

	// Set up the cost
	int32 cost = 0;

	// Check for attached money
	if( msg.money > 0 )
		cost += msg.money;

	if( cost < 0 )
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	if( !sMailSystem.MailOption( MAIL_FLAG_DISABLE_POSTAGE_COSTS ) && !( GetPermissionCount() && sMailSystem.MailOption( MAIL_FLAG_NO_COST_FOR_GM ) ) )
	{
		cost += 30;
		if( cost < 30 )//Overflow prevention for those silly WPE hoez.
		{
			SendMailError(MAIL_ERR_INTERNAL_ERROR);
			return;
		}			
	}	
	
	// check that we have enough in our backpack
	if( !_player->HasGold(cost) )
	{
		SendMailError( MAIL_ERR_NOT_ENOUGH_MONEY );
		return;
	}

	// Check for the item, and required item.
	if( !items.empty( ) )
	{
		for( itr = items.begin(); itr != items.end(); ++itr )
		{
			pItem = *itr;
			if( _player->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(pItem->GetGUID(), false) != pItem )
				continue;		// should never be hit.

			pItem->RemoveFromWorld();
			pItem->SetOwner( NULL );
			pItem->SaveToDB( INVENTORY_SLOT_NOT_SET, 0, true, NULL );
            msg.items.push_back( pItem->GetLowGUID() );

			if( GetPermissionCount() > 0 )
			{
				/* log the message */
				sGMLog.writefromsession(this, "sent mail with item entry %u to %s, with gold %u.", pItem->GetEntry(), player->name, msg.money);
			}

			pItem->DeleteMe();
		}
	}

	if(msg.money != 0 || msg.cod != 0 || ( !msg.items.size() && player->acct != _player->GetSession()->GetAccountId()) )
	{
		if(!sMailSystem.MailOption(MAIL_FLAG_DISABLE_HOUR_DELAY_FOR_ITEMS))
			msg.delivery_time += 3600;  // 1hr
	}

	// take the money
	_player->ModGold( -cost );

	// Fill in the rest of the info
	msg.player_guid = player->guid;
	msg.sender_guid = _player->GetGUID();
	
	// 30 day expiry time for unread mail
	if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
		msg.expire_time = (uint32)UNIXTIME + (TIME_DAY * MAIL_DEFAULT_EXPIRATION_TIME);
	else
		msg.expire_time = 0;

	msg.copy_made = false;
	msg.read_flag = false;
	msg.deleted_flag = false;
	msg.message_type = 0;

	// Great, all our info is filled in. Now we can add it to the other players mailbox.
	sMailSystem.DeliverMessage(player->guid, &msg);
	// Save/Update character's gold if they've received gold that is. This prevents a rollback.
	CharacterDatabase.Execute("UPDATE characters SET gold = %u WHERE guid = %u", _player->GetGold(), _player->m_playerInfo->guid);
	// Success packet :)
	SendMailError(MAIL_OK);
}

void WorldSession::HandleMarkAsRead(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	MailMessage * message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0) return;

	// mark the message as read
	message->read_flag = 1;

	// mail now has a 30 day expiry time
	if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
		message->expire_time = (uint32)UNIXTIME + (TIME_DAY * 30);

	// update it in sql
	CharacterDatabase.WaitExecute("UPDATE mailbox SET read_flag = 1, expiry_time = %u WHERE message_id = %u", message->message_id, message->expire_time);
}

void WorldSession::HandleMailDelete(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_DELETED);

	MailMessage * message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	if(message->copy_made)
	{
		// we have the message as a copy on the item. we can't delete it or this item
		// will no longer function.

		// deleted_flag prevents it from being shown in the mail list.
		message->deleted_flag = 1;

		// update in sql
		CharacterDatabase.WaitExecute("UPDATE mailbox SET deleted_flag = 1 WHERE message_id = %u", message_id);
	}
	else
	{
		// delete the message, there are no other references to it.
		_player->m_mailBox.DeleteMessage(message_id, true);
	}

	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleTakeItem(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	uint32 lowguid;
	vector< uint32 >::iterator itr;

	recv_data >> mailbox >> message_id >> lowguid;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_ITEM_TAKEN);
	
	MailMessage * message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0 || message->items.empty())
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	for( itr = message->items.begin( ); itr != message->items.end( ); ++itr )
	{
		if ( (*itr) == lowguid )
			break;
	}

	if( itr == message->items.end( ) )
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	// check for cod credit
	if(message->cod > 0)
	{
		if( !_player->HasGold(message->cod) )
		{
			data << uint32(MAIL_ERR_NOT_ENOUGH_MONEY);
			SendPacket(&data);
			return;
		}
	}

	// grab the item
	Item * item = objmgr.LoadItem( *itr );
	if(item == 0)
	{
		// doesn't exist
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);
		
		return;
	}

	//Find free slot
	SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot(item->GetProto());
	if(result.Result == 0)
	{
		//End of slots
		data << uint32(MAIL_ERR_BAG_FULL);
		SendPacket(&data);

		item->DeleteMe();
		return;
	}
	item->m_isDirty = true;
	
	if( !_player->GetItemInterface()->SafeAddItem(item, result.ContainerSlot, result.Slot) )
	{
		if( !_player->GetItemInterface()->AddItemToFreeSlot(item) )
		{
			//End of slots
			data << uint32(MAIL_ERR_BAG_FULL);
			SendPacket(&data);
			item->DeleteMe();
			return;
		}
	}
	else
	item->SaveToDB(result.ContainerSlot, result.Slot, true, NULL);

	// send complete packet
	data << uint32(MAIL_OK);
    data << item->GetLowGUID();
	data << item->GetStackCount();

	message->items.erase( itr );

	// re-save (update the items field)
	sMailSystem.SaveMessageToSQL( message);
	SendPacket(&data);
	
	if( message->cod > 0 )
	{
		_player->ModGold( -(int32)message->cod );
		string subject = "COD Payment: ";
		subject += message->subject;
		sMailSystem.SendAutomatedMessage(NORMAL, message->player_guid, message->sender_guid, subject, "", message->cod, 0, 0, MAIL_STATIONERY_TEST1 );

		message->cod = 0;
		CharacterDatabase.Execute("UPDATE mailbox SET cod = 0 WHERE message_id = %u", message->message_id);
	}

	// probably need to send an item push here
}

void WorldSession::HandleTakeMoney(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_MONEY_TAKEN);

	MailMessage * message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0 || !message->money)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	// Check they don't have more than the max gold
	if(sWorld.GoldCapEnabled)
	{
		if( (_player->GetGold() + message->money) > sWorld.GoldLimit )
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
			return;
		}
	}

	// add the money to the player
	_player->ModGold( message->money );

	// message no longer has any money
	message->money = 0;

	// update in sql!
	CharacterDatabase.WaitExecute("UPDATE mailbox SET money = 0 WHERE message_id = %u", message->message_id);

	// send result
	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleReturnToSender(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_RETURNED_TO_SENDER);

	MailMessage * msg = _player->m_mailBox.GetMessage(message_id);
	if(msg == 0)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}
	
	// copy into a new struct
	MailMessage message = *msg;

	// remove the old message
	_player->m_mailBox.DeleteMessage(message_id, true);

	// re-assign the owner/sender
	message.player_guid = message.sender_guid;
	message.sender_guid = _player->GetGUID();

	// turn off the read flag
	message.read_flag = false;
	message.deleted_flag = false;
	message.copy_made = false;

	// null out the cod charges. (the sender doesn't want to have to pay for his own item
	// that he got nothing for.. :p)
	message.cod = 0;

	// assign new delivery time
	message.delivery_time = message.items.empty() ? (uint32)UNIXTIME : (uint32)UNIXTIME + 3600;

	// add to the senders mailbox
	sMailSystem.DeliverMessage(message.player_guid, &message);

	// finish the packet
	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleMailCreateTextItem(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_MADE_PERMANENT);
	

	ItemPrototype * proto = ItemPrototypeStorage.LookupEntry(8383);
	MailMessage * message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0 || !proto)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	SlotResult result = _player->GetItemInterface()->FindFreeInventorySlot(proto);
	if(result.Result == 0)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	Item * pItem = objmgr.CreateItem(8383, _player);
	if (pItem== NULL)
		return;

	//pItem->SetTextId(message_id);
	if( _player->GetItemInterface()->AddItemToFreeSlot(pItem) )
	{
		// mail now has an item after it
		message->copy_made = true;

		// update in sql
		CharacterDatabase.WaitExecute("UPDATE mailbox SET copy_made = 1 WHERE message_id = %u", message_id);

		data << uint32(MAIL_OK);
		SendPacket(&data);
	}
	else
	{
		pItem->DeleteMe();
	}
}

void WorldSession::HandleItemTextQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 itemGuid;
	recv_data >> itemGuid;

	string body = "Internal Error";

	//TODO: Store text in database even after we deleted the mail and access it by item GUID (low guid)
	Item *pItem = _player->GetItemInterface()->GetItemByGUID(itemGuid);
	WorldPacket data(SMSG_ITEM_TEXT_QUERY_RESPONSE, body.length() + 9);
	if(!pItem)
		data << uint8(1);
	else
	{
		data << uint8(0);
		data << uint64(itemGuid);
		data << body;
	}

	SendPacket(&data);
}

void Mailbox::FillTimePacket(WorldPacket& data)
{
	uint32 c = 0;
	MessageMap::iterator iter = Messages.begin();
	data << uint32(0) << uint32(0);

	for(; iter != Messages.end(); ++iter)
	{
		if(iter->second.deleted_flag == 0 && iter->second.read_flag == 0 && (uint32)UNIXTIME >= iter->second.delivery_time)
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

	if(c== 0)
	{

		*(uint32*)(&data.contents()[0])= 0xc7a8c000;
	}
	else
	{

		*(uint32*)(&data.contents()[4])=c;
	}
}

void WorldSession::HandleMailTime(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WorldPacket data(MSG_QUERY_NEXT_MAIL_TIME, 100);
	_player->m_mailBox.FillTimePacket(data);
	SendPacket(&data);
}

void WorldSession::SendMailError(uint32 error)
{
	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << uint32(0);
	data << uint32(MAIL_RES_MAIL_SENT);
	data << error;
	SendPacket(&data);
}

void WorldSession::HandleGetMail(WorldPacket & recv_data )
{
	CHECK_INWORLD_RETURN

	WorldPacket * data = _player->m_mailBox.BuildMailboxListingPacket();
	SendPacket(data);
	delete data;
}

void MailSystem::RemoveMessageIfDeleted(uint32 message_id, Player * plr)
{
	MailMessage * msg = plr->m_mailBox.GetMessage(message_id);
	if(msg == 0) return;

	if(msg->deleted_flag)   // we've deleted from inbox
		plr->m_mailBox.DeleteMessage(message_id, true);   // wipe the message
}

void MailSystem::SendAutomatedMessage(uint32 type, uint64 sender, uint64 receiver, string subject, string body,
									  uint32 money, uint32 cod, uint64 item_guid, uint32 stationery)
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
	if( Arcemu::Util::GUID_LOPART(item_guid) != 0 )
		msg.items.push_back( Arcemu::Util::GUID_LOPART(item_guid) );

	msg.stationery = stationery;
	msg.delivery_time = (uint32)UNIXTIME;
	msg.expire_time = 0;
	msg.read_flag = false;
	msg.copy_made = false;
	msg.deleted_flag = false;

	// Send the message.
	DeliverMessage(receiver, &msg);
}

void Mailbox::Load(QueryResult * result)
{
	if(!result)
		return;

	Field * fields;
	MailMessage msg;
	uint32 i;
	char * str;
	char * p;
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
		if( p == NULL )
		{
			itemguid = atoi(str);
			if( itemguid != 0 )
				msg.items.push_back( itemguid );
		}
		else
		{
			while( p )
			{
				*p = 0;
				p++;

				itemguid = atoi( str );
				if( itemguid != 0 )
					msg.items.push_back( itemguid );

                str = p;
				p = strchr( str, ',' );
			}
		}

		msg.cod = fields[i++].GetUInt32();
		msg.stationery = fields[i++].GetUInt32();
		msg.expire_time = fields[i++].GetUInt32();
		msg.delivery_time = fields[i++].GetUInt32();
		msg.copy_made = fields[i++].GetBool();
		msg.read_flag = fields[i++].GetBool();
		msg.deleted_flag = fields[i++].GetBool();

		// Add to the mailbox
		AddMessage(&msg);

	} while(result->NextRow());
}
