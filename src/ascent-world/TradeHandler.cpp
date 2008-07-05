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

void WorldSession::HandleInitiateTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 guid;
	recv_data >> guid;
	Player * pTarget = _player->GetMapMgr()->GetPlayer((uint32)guid);
	uint32 TradeStatus = TRADE_STATUS_PROPOSED;
	WorldPacket data(SMSG_TRADE_STATUS, 12);

	if(pTarget == 0)
	{
#ifdef USING_BIG_ENDIAN
		TradeStatus = swap32(uint32(TRADE_STATUS_PLAYER_NOT_FOUND));
#else
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
#endif
		OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
		return;
	}

	// Handle possible error outcomes
	if(pTarget->CalcDistance(_player) > 10.0f)		// This needs to be checked
		TradeStatus = TRADE_STATUS_TOO_FAR_AWAY;
	else if(pTarget->isDead())
		TradeStatus = TRADE_STATUS_DEAD;
	else if(pTarget->mTradeTarget != 0)
		TradeStatus = TRADE_STATUS_ALREADY_TRADING;
	else if(pTarget->GetTeam() != _player->GetTeam() && GetPermissionCount() == 0 && !sWorld.interfaction_trade)
		TradeStatus = TRADE_STATUS_WRONG_FACTION;

	data << TradeStatus;

	if(TradeStatus == TRADE_STATUS_PROPOSED)
	{
		_player->ResetTradeVariables();
		pTarget->ResetTradeVariables();

		pTarget->mTradeTarget = _player->GetLowGUID();
		_player->mTradeTarget = pTarget->GetLowGUID();

		pTarget->mTradeStatus = TradeStatus;
		_player->mTradeStatus = TradeStatus;

		data << _player->GetGUID();
	}

	pTarget->m_session->SendPacket(&data);
}

void WorldSession::HandleBeginTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 TradeStatus = TRADE_STATUS_INITIATED;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
#ifdef USING_BIG_ENDIAN
		TradeStatus = swap32(uint32(TRADE_STATUS_PLAYER_NOT_FOUND));
#else
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
#endif
		OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
		return;
	}
	// We're too far from target now?
	if( _player->CalcDistance( objmgr.GetPlayer(_player->mTradeTarget) ) > 10.0f )
	TradeStatus = TRADE_STATUS_TOO_FAR_AWAY;

#ifdef USING_BIG_ENDIAN
	swap32(&TradeStatus);
	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	swap32(&TradeStatus);
#else
/*	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);*/
	WorldPacket data(SMSG_TRADE_STATUS, 8);
	data << TradeStatus << uint32(0x19);
	plr->m_session->SendPacket(&data);
	SendPacket(&data);
#endif

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;
}

void WorldSession::HandleBusyTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 TradeStatus = TRADE_STATUS_PLAYER_BUSY;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
#ifdef USING_BIG_ENDIAN
		TradeStatus = swap32(uint32(TRADE_STATUS_PLAYER_NOT_FOUND));
#else
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
#endif
		OutPacket(TRADE_STATUS_PLAYER_NOT_FOUND, 4, &TradeStatus);
		return;
	}

#ifdef USING_BIG_ENDIAN
	swap32(&TradeStatus);
	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	swap32(&TradeStatus);
#else
	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
#endif

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
}

void WorldSession::HandleIgnoreTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	uint32 TradeStatus = TRADE_STATUS_PLAYER_IGNORED;

	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || plr == 0)
	{
#ifdef USING_BIG_ENDIAN
		TradeStatus = swap32(uint32(TRADE_STATUS_PLAYER_NOT_FOUND));
#else
		TradeStatus = TRADE_STATUS_PLAYER_NOT_FOUND;
#endif
		OutPacket(TRADE_STATUS_PLAYER_NOT_FOUND, 4, &TradeStatus);
		return;
	}

#ifdef USING_BIG_ENDIAN
	swap32(&TradeStatus);
	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	swap32(&TradeStatus);
#else
	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
#endif

	plr->mTradeStatus = TradeStatus;
	_player->mTradeStatus = TradeStatus;

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
}

void WorldSession::HandleCancelTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	if(_player->mTradeTarget == 0 || _player->mTradeStatus == TRADE_STATUS_COMPLETE)
		return;

#ifdef USING_BIG_ENDIAN
	uint32 TradeStatus = swap32(uint32(TRADE_STATUS_CANCELLED));
#else
    uint32 TradeStatus = TRADE_STATUS_CANCELLED;
#endif

    OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	Player * plr = _player->GetTradeTarget();
    if(plr)
    {
        if(plr->m_session && plr->m_session->GetSocket())
		plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	
	    plr->mTradeTarget = 0;
		plr->ResetTradeVariables();
    }
	
	_player->mTradeTarget = 0;
	_player->ResetTradeVariables();
}

void WorldSession::HandleUnacceptTrade(WorldPacket & recv_data)
{
	if(!_player->IsInWorld()) return;
	Player * plr = _player->GetTradeTarget();
	_player->ResetTradeVariables();

	if(_player->mTradeTarget == 0 || plr == 0)
		return;

#ifdef USING_BIG_ENDIAN
	uint32 TradeStatus = swap32(uint32(TRADE_STATUS_UNACCEPTED));
#else
	uint32 TradeStatus = TRADE_STATUS_UNACCEPTED;
#endif

	OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);

	plr->mTradeTarget = 0;
	_player->mTradeTarget = 0;
	plr->ResetTradeVariables();
}

void WorldSession::HandleSetTradeItem(WorldPacket & recv_data)
{
	if(_player->mTradeTarget == 0)
		return;

	CHECK_PACKET_SIZE(recv_data, 3);

	uint8 TradeSlot = recv_data.contents()[0];
	uint8 SourceBag = recv_data.contents()[1];
	uint8 SourceSlot = recv_data.contents()[2];
	Player * pTarget = _player->GetMapMgr()->GetPlayer( _player->mTradeTarget );

	Item * pItem = _player->GetItemInterface()->GetInventoryItem(SourceBag, SourceSlot);
	if( pTarget == NULL || pItem == 0 || TradeSlot > 6 )
		return;

	if( pItem->IsContainer() && ((Container*)pItem)->HasItems() )
	{
		_player->GetItemInterface()->BuildInventoryChangeError(	pItem, NULL, INV_ERR_CANT_TRADE_EQUIP_BAGS);
		return;
	}

	if(TradeSlot < 6 && pItem->IsSoulbound())
	{
		sCheatLog.writefromsession(this, "tried to cheat trade a soulbound item");
		Disconnect();
		return;
	}

	for(uint32 i = 0; i < 8; ++i)
	{
		// duping little shits
		if(_player->mTradeItems[i] == pItem || pTarget->mTradeItems[i] == pItem)
		{
			sCheatLog.writefromsession(this, "tried to dupe an item through trade");
			Disconnect();
			return;
		}
	}

	_player->mTradeItems[TradeSlot] = pItem;
	_player->SendTradeUpdate();
}

void WorldSession::HandleSetTradeGold(WorldPacket & recv_data)
{
	if(_player->mTradeTarget == 0)
		return;

	uint32 Gold;
	recv_data >> Gold;

	if(_player->mTradeGold != Gold)
	{
		_player->mTradeGold = (Gold > _player->GetUInt32Value(PLAYER_FIELD_COINAGE) ? _player->GetUInt32Value(PLAYER_FIELD_COINAGE) : Gold);
		_player->SendTradeUpdate();
	}
}

void WorldSession::HandleClearTradeItem(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 1);
	if(_player->mTradeTarget == 0)
		return;

	uint8 TradeSlot = recv_data.contents()[0];
	if(TradeSlot > 6)
		return;

	_player->mTradeItems[TradeSlot] = 0;
	_player->SendTradeUpdate();
}

void WorldSession::HandleAcceptTrade(WorldPacket & recv_data)
{
	Player * plr = _player->GetTradeTarget();
	if(_player->mTradeTarget == 0 || !plr)
		return;

	uint32 TradeStatus = TRADE_STATUS_ACCEPTED;
	
	// Tell the other player we're green.
#ifdef USING_BIG_ENDIAN
	swap32(&TradeStatus);
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	_player->mTradeStatus = TradeStatus;
	swap32(&TradeStatus);
#else
	plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
	_player->mTradeStatus = TradeStatus;
#endif

	if(plr->mTradeStatus == TRADE_STATUS_ACCEPTED)
	{
		// Ready!
		uint32 ItemCount = 0;
		uint32 TargetItemCount = 0;
		Player * pTarget = plr;

		// Calculate Item Count
		for(uint32 Index = 0; Index < 7; ++Index)
		{
			if(_player->mTradeItems[Index] != 0)	++ItemCount;
			if(pTarget->mTradeItems[Index] != 0)	++TargetItemCount;
		}

		if( (_player->m_ItemInterface->CalculateFreeSlots(NULL) + ItemCount) < TargetItemCount ||
			(pTarget->m_ItemInterface->CalculateFreeSlots(NULL) + TargetItemCount) < ItemCount )
		{
			// Not enough slots on one end.
			TradeStatus = TRADE_STATUS_CANCELLED;
		}
		else
		{
			TradeStatus = TRADE_STATUS_COMPLETE;
			uint64 Guid;
			Item * pItem;
			
			// Remove all items from the players inventory
			for(uint32 Index = 0; Index < 6; ++Index)
			{
				Guid = _player->mTradeItems[Index] ? _player->mTradeItems[Index]->GetGUID() : 0;
				if(Guid != 0)
				{
					if( _player->mTradeItems[Index]->GetProto()->Bonding == ITEM_BIND_ON_PICKUP ||
						_player->mTradeItems[Index]->GetProto()->Bonding  >=  ITEM_BIND_QUEST  )
					{
						_player->mTradeItems[Index] = NULL;
					}
					else
					{
						if(GetPermissionCount()>0)
						{
							sGMLog.writefromsession(this, "traded item %s to %s", _player->mTradeItems[Index]->GetProto()->Name1, pTarget->GetName());
						}
						pItem = _player->m_ItemInterface->SafeRemoveAndRetreiveItemByGuid(Guid, true);
					}
				}

				Guid = pTarget->mTradeItems[Index] ? pTarget->mTradeItems[Index]->GetGUID() : 0;
				if(Guid != 0)
				{
					if( pTarget->mTradeItems[Index]->GetProto()->Bonding == ITEM_BIND_ON_PICKUP ||  
						pTarget->mTradeItems[Index]->GetProto()->Bonding  >=  ITEM_BIND_QUEST )
					{
						pTarget->mTradeItems[Index] = NULL;
					}
					else
					{
						pTarget->m_ItemInterface->SafeRemoveAndRetreiveItemByGuid(Guid, true);
					}
				}
			}

			// Dump all items back into the opposite players inventory
			for(uint32 Index = 0; Index < 6; ++Index)
			{
				pItem = _player->mTradeItems[Index];
				if(pItem != 0)
				{
					pItem->SetOwner(pTarget);
					if( !pTarget->m_ItemInterface->AddItemToFreeSlot(pItem) )
						delete pItem;
				}

				pItem = pTarget->mTradeItems[Index];
				if(pItem != 0)
				{
					pItem->SetOwner(_player);
					if( !_player->m_ItemInterface->AddItemToFreeSlot(pItem) )
						delete pItem;
				}
			}

			// Trade Gold
			if(pTarget->mTradeGold)
			{
				_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, pTarget->mTradeGold);
				pTarget->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)pTarget->mTradeGold);
			}

			if(_player->mTradeGold)
			{
				pTarget->ModUnsigned32Value(PLAYER_FIELD_COINAGE, _player->mTradeGold);
				_player->ModUnsigned32Value(PLAYER_FIELD_COINAGE, -(int32)_player->mTradeGold);
			}

			// Close Window
			TradeStatus = TRADE_STATUS_COMPLETE;
#ifdef USING_BIG_ENDIAN
            swap32(&TradeStatus);
			OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			swap32(&TradeStatus);
#else
			OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
			plr->m_session->OutPacket(SMSG_TRADE_STATUS, 4, &TradeStatus);
#endif

			_player->mTradeStatus = TRADE_STATUS_COMPLETE;
			plr->mTradeStatus = TRADE_STATUS_COMPLETE;

			// Reset Trade Vars
			_player->ResetTradeVariables();
			pTarget->ResetTradeVariables();
			
			plr->mTradeTarget = 0;
			_player->mTradeTarget = 0;
		}
	}
}
