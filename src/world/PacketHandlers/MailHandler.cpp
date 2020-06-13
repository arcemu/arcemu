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

void WorldSession::HandleSendMail(WorldPacket & recv_data)
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
	Item* pItem;
	//uint32 err = MAIL_OK;

	recv_data >> gameobject >> recepient;
	recv_data >> msg.subject >> msg.body >> msg.stationery;
	recv_data >> unk2 >> itemcount;

	if(itemcount > MAIL_MAX_ITEM_SLOT || msg.body.find("%") != string::npos || msg.subject.find("%") != string::npos)
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	// Search for the recipient
	PlayerInfo* player = ObjectMgr::getSingleton().GetPlayerInfoByName(recepient.c_str());
	if(player == NULL)
	{
		SendMailError(MAIL_ERR_RECIPIENT_NOT_FOUND);
		return;
	}

	for(i = 0; i < itemcount; ++i)
	{
		recv_data >> itemslot;
		recv_data >> itemguid;

		pItem = _player->GetItemInterface()->GetItemByGUID(itemguid);
		if(pItem == NULL || pItem->IsSoulbound() || pItem->IsConjured())
		{
			SendMailError(MAIL_ERR_INTERNAL_ERROR);
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

		items.push_back(pItem);
	}

	recv_data >> msg.money;
	recv_data >> msg.cod;
	// left over: (TODO- FIX ME BURLEX!)
	// uint32
	// uint32
	// uint8

	bool interfaction = false;
	if(sMailSystem.MailOption(MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION) || (HasGMPermissions() && sMailSystem.MailOption(MAIL_FLAG_CAN_SEND_TO_OPPOSITE_FACTION_GM)))
	{
		interfaction = true;
	}

	// Check we're sending to the same faction (disable this for testing)
	if(player->team != _player->GetTeam() && !interfaction)
	{
		SendMailError(MAIL_ERR_NOT_YOUR_ALLIANCE);
		return;
	}

	// Check if we're sending mail to ourselves
	if(strcmp(player->name, _player->GetName()) == 0 && !GetPermissionCount())
	{
		SendMailError(MAIL_ERR_CANNOT_SEND_TO_SELF);
		return;
	}

	if(msg.stationery == MAIL_STATIONERY_GM && !HasGMPermissions())
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	// Instant delivery time by default.
	msg.delivery_time = (uint32)UNIXTIME;

	// Set up the cost
	int32 cost = 0;

	// Check for attached money
	if(msg.money > 0)
		cost += msg.money;

	if(cost < 0)
	{
		SendMailError(MAIL_ERR_INTERNAL_ERROR);
		return;
	}

	if(!sMailSystem.MailOption(MAIL_FLAG_DISABLE_POSTAGE_COSTS) && !(GetPermissionCount() && sMailSystem.MailOption(MAIL_FLAG_NO_COST_FOR_GM)))
	{
		cost += 30;
		if(cost < 30)  //Overflow prevention for those silly WPE hoez.
		{
			SendMailError(MAIL_ERR_INTERNAL_ERROR);
			return;
		}
	}

	// check that we have enough in our backpack
	if(!_player->HasGold(cost))
	{
		SendMailError(MAIL_ERR_NOT_ENOUGH_MONEY);
		return;
	}

	// Check for the item, and required item.
	if(!items.empty())
	{
		for(itr = items.begin(); itr != items.end(); ++itr)
		{
			pItem = *itr;
			if(_player->GetItemInterface()->SafeRemoveAndRetreiveItemByGuid(pItem->GetGUID(), false) != pItem)
				continue;		// should never be hit.

			pItem->RemoveFromWorld();
			pItem->SetOwner(NULL);
			pItem->SaveToDB(INVENTORY_SLOT_NOT_SET, 0, true, NULL);
			msg.items.push_back(pItem->GetLowGUID());

			if(GetPermissionCount() > 0)
			{
				/* log the message */
				sGMLog.writefromsession(this, "sent mail with item entry %u to %s, with gold %u.", pItem->GetEntry(), player->name, msg.money);
			}

			pItem->DeleteMe();
		}
	}

	if(msg.money != 0 || msg.cod != 0 || (!msg.items.size() && player->acct != _player->GetSession()->GetAccountId()))
	{
		if(!sMailSystem.MailOption(MAIL_FLAG_DISABLE_HOUR_DELAY_FOR_ITEMS))
			msg.delivery_time += 3600;  // 1hr
	}

	// take the money
	_player->ModGold(-cost);

	// Fill in the rest of the info
	msg.player_guid = player->guid;
	msg.sender_guid = _player->GetGUID();

	// 30 day expiry time for unread mail
	if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
		msg.expire_time = (uint32)UNIXTIME + (TIME_DAY * MAIL_DEFAULT_EXPIRATION_TIME);
	else
		msg.expire_time = 0;

	msg.deleted_flag = false;
	msg.message_type = 0;
	msg.checked_flag = msg.body.empty() ? MAIL_CHECK_MASK_COPIED : MAIL_CHECK_MASK_HAS_BODY;

	// Great, all our info is filled in. Now we can add it to the other players mailbox.
	sMailSystem.DeliverMessage(player->guid, &msg);
	// Save/Update character's gold if they've received gold that is. This prevents a rollback.
	CharacterDatabase.Execute("UPDATE characters SET gold = %u WHERE guid = %u", _player->GetGold(), _player->m_playerInfo->guid);
	// Success packet :)
	SendMailError(MAIL_OK);
}

void WorldSession::HandleMarkAsRead(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	MailMessage* message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0) return;

	// mark the message as read
	message->checked_flag |= MAIL_CHECK_MASK_READ;

	// mail now has a 30 day expiry time
	if(!sMailSystem.MailOption(MAIL_FLAG_NO_EXPIRY))
		message->expire_time = (uint32)UNIXTIME + (TIME_DAY * 30);

	// update it in sql
	CharacterDatabase.WaitExecute("UPDATE mailbox SET checked_flag = %u, expiry_time = %u WHERE message_id = %u",
		message->checked_flag, message->expire_time, message->message_id);
}

void WorldSession::HandleMailDelete(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_DELETED);

	MailMessage* message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	_player->m_mailBox.DeleteMessage(message_id, true);

	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleTakeItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	uint32 lowguid;
	vector< uint32 >::iterator itr;

	recv_data >> mailbox >> message_id >> lowguid;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_ITEM_TAKEN);

	MailMessage* message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0 || message->items.empty())
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	for(itr = message->items.begin(); itr != message->items.end(); ++itr)
	{
		if((*itr) == lowguid)
			break;
	}

	if(itr == message->items.end())
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	// check for cod credit
	if(message->cod > 0)
	{
		if(!_player->HasGold(message->cod))
		{
			data << uint32(MAIL_ERR_NOT_ENOUGH_MONEY);
			SendPacket(&data);
			return;
		}
	}

	// grab the item
	Item* item = objmgr.LoadItem(*itr);
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

	if(!_player->GetItemInterface()->SafeAddItem(item, result.ContainerSlot, result.Slot))
	{
		if(!_player->GetItemInterface()->AddItemToFreeSlot(item))
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

	message->items.erase(itr);

	// re-save (update the items field)
	sMailSystem.SaveMessageToSQL(message);
	SendPacket(&data);

	if(message->cod > 0)
	{
		_player->ModGold(-(int32)message->cod);
		string subject = "COD Payment: ";
		subject += message->subject;
		sMailSystem.SendAutomatedMessage(NORMAL, message->player_guid, message->sender_guid, subject, "", message->cod, 0, 0, MAIL_STATIONERY_TEST1, MAIL_CHECK_MASK_COD_PAYMENT);

		message->cod = 0;
		CharacterDatabase.Execute("UPDATE mailbox SET cod = 0 WHERE message_id = %u", message->message_id);
	}

	// probably need to send an item push here
}

void WorldSession::HandleTakeMoney(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_MONEY_TAKEN);

	MailMessage* message = _player->m_mailBox.GetMessage(message_id);
	if(message == 0 || !message->money)
	{
		data << uint32(MAIL_ERR_INTERNAL_ERROR);
		SendPacket(&data);

		return;
	}

	// Check they don't have more than the max gold
	if(sWorld.GoldCapEnabled)
	{
		if((_player->GetGold() + message->money) > sWorld.GoldLimit)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
			return;
		}
	}

	// add the money to the player
	_player->ModGold(message->money);

	// message no longer has any money
	message->money = 0;

	// update in sql!
	CharacterDatabase.WaitExecute("UPDATE mailbox SET money = 0 WHERE message_id = %u", message->message_id);

	// send result
	data << uint32(MAIL_OK);
	SendPacket(&data);
}

void WorldSession::HandleReturnToSender(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_RETURNED_TO_SENDER);

	MailMessage* msg = _player->m_mailBox.GetMessage(message_id);
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

	message.deleted_flag = false;
	message.checked_flag = MAIL_CHECK_MASK_RETURNED;

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

void WorldSession::HandleMailCreateTextItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mailbox;
	uint32 message_id;
	recv_data >> mailbox >> message_id;

	WorldPacket data(SMSG_SEND_MAIL_RESULT, 12);
	data << message_id << uint32(MAIL_RES_MADE_PERMANENT);


	ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(8383);
	MailMessage* message = _player->m_mailBox.GetMessage(message_id);
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

	Item* pItem = objmgr.CreateItem(8383, _player);
	if(pItem == NULL)
		return;

	pItem->SetFlag( ITEM_FIELD_FLAGS, ITEM_FLAG_WRAP_GIFT ); // the flag is probably misnamed
	pItem->SetText( message->body );

	if(_player->GetItemInterface()->AddItemToFreeSlot(pItem))
	{
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

	Item* pItem = _player->GetItemInterface()->GetItemByGUID(itemGuid);
	WorldPacket data(SMSG_ITEM_TEXT_QUERY_RESPONSE, pItem->GetText().size() + 9 );
	if(!pItem)
		data << uint8(1);
	else
	{
		data << uint8(0);
		data << uint64(itemGuid);
		data << pItem->GetText();
	}

	SendPacket(&data);
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

void WorldSession::HandleGetMail(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WorldPacket* data = _player->m_mailBox.BuildMailboxListingPacket();
	SendPacket(data);
	delete data;
}
