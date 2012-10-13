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

void WorldSession::HandleGuildQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 4);

	uint32 guildId;
	recv_data >> guildId;

	// we can skip some searches here if this is our guild
	if(_player && _player->GetGuildId() == guildId && _player->m_playerInfo->guild)
	{
		_player->m_playerInfo->guild->SendGuildQuery(this);
		return;
	}

	Guild* pGuild = objmgr.GetGuild(guildId);
	if(!pGuild)
		return;

	pGuild->SendGuildQuery(this);
}

void WorldSession::HandleCreateGuild(WorldPacket & recv_data)
{
}

void WorldSession::HandleInviteToGuild(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);

	std::string inviteeName;
	recv_data >> inviteeName;

	Player* plyr = objmgr.GetPlayer(inviteeName.c_str() , false);
	Guild* pGuild = _player->m_playerInfo->guild;

	if(!plyr)
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, inviteeName.c_str(), GUILD_PLAYER_NOT_FOUND);
		return;
	}
	else if(!pGuild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(plyr->GetGuildId())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, plyr->GetName(), ALREADY_IN_GUILD);
		return;
	}
	else if(plyr->GetGuildInvitersGuid())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, plyr->GetName(), ALREADY_INVITED_TO_GUILD);
		return;
	}
	else if(!_player->m_playerInfo->guildRank->CanPerformCommand(GR_RIGHT_INVITE))
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, "", GUILD_PERMISSIONS);
		return;
	}
	else if(plyr->GetTeam() != _player->GetTeam() && _player->GetSession()->GetPermissionCount() == 0 && !sWorld.interfaction_guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, "", GUILD_NOT_ALLIED);
		return;
	}

	pGuild->getLock().Acquire();
	uint32 memberCount = pGuild->GetNumMembers();
	pGuild->getLock().Release();

	if( memberCount >= MAX_GUILD_MEMBERS ){
		SystemMessage( "Your guild is full." );
		return;
	}

	Guild::SendGuildCommandResult(this, GUILD_INVITE_S, inviteeName.c_str(), GUILD_U_HAVE_INVITED);
	//41

	WorldPacket data(SMSG_GUILD_INVITE, 100);
	data << _player->GetName();
	data << pGuild->GetGuildName();
	plyr->GetSession()->SendPacket(&data);

	plyr->SetGuildInvitersGuid(_player->GetLowGUID());
}

void WorldSession::HandleGuildAccept(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	Player* plyr = GetPlayer();

	if(!plyr)
		return;

	Player* inviter = objmgr.GetPlayer(plyr->GetGuildInvitersGuid());
	plyr->UnSetGuildInvitersGuid();

	if(!inviter)
	{
		return;
	}

	Guild* pGuild = inviter->m_playerInfo->guild;
	if(!pGuild)
	{
		return;
	}
	pGuild->getLock().Acquire();
	uint32 memberCount = pGuild->GetNumMembers();
	pGuild->getLock().Release();

	if( memberCount >= MAX_GUILD_MEMBERS ){
		plyr->UnSetGuildInvitersGuid();
		SystemMessage( "That guild is full." );
		return;
	}
	pGuild->AddGuildMember(plyr->m_playerInfo, NULL);
}

void WorldSession::HandleGuildDecline(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WorldPacket data;

	Player* plyr = GetPlayer();

	if(!plyr)
		return;

	Player* inviter = objmgr.GetPlayer(plyr->GetGuildInvitersGuid());
	plyr->UnSetGuildInvitersGuid();

	if(!inviter)
		return;

	data.Initialize(SMSG_GUILD_DECLINE);
	data << plyr->GetName();
	inviter->GetSession()->SendPacket(&data);
}

void WorldSession::HandleSetGuildInformation(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);
	std::string NewGuildInfo;
	recv_data >> NewGuildInfo;

	Guild* pGuild = _player->m_playerInfo->guild;
	if(!pGuild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	pGuild->SetGuildInformation(NewGuildInfo.c_str(), this);
}

void WorldSession::HandleGuildInfo(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->GetGuild() != NULL)
		_player->GetGuild()->SendGuildInfo(this);
}

void WorldSession::HandleGuildRoster(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(!_player->m_playerInfo->guild)
		return;

	_player->m_playerInfo->guild->SendGuildRoster(this);
}

void WorldSession::HandleGuildPromote(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo* dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr == NULL)
		return;

	_player->m_playerInfo->guild->PromoteGuildMember(dstplr, this);
}

void WorldSession::HandleGuildDemote(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo* dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr == NULL)
		return;

	_player->m_playerInfo->guild->DemoteGuildMember(dstplr, this);
}

void WorldSession::HandleGuildLeave(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	_player->m_playerInfo->guild->RemoveGuildMember(_player->m_playerInfo, this);
}

void WorldSession::HandleGuildRemove(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo* dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr == NULL)
		return;

	_player->m_playerInfo->guild->RemoveGuildMember(dstplr, this);
}

void WorldSession::HandleGuildDisband(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(_player->m_playerInfo->guild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, "", GUILD_PERMISSIONS);
		return;
	}

	_player->m_playerInfo->guild->Disband();
}

void WorldSession::HandleGuildLeader(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 1);

	std::string name;
	recv_data >> name;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	PlayerInfo* dstplr = objmgr.GetPlayerInfoByName(name.c_str());
	if(dstplr == NULL)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, name.c_str(), GUILD_PLAYER_NOT_FOUND);
		return;
	}

	_player->m_playerInfo->guild->ChangeGuildMaster(dstplr, this);
}

void WorldSession::HandleGuildMotd(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	std::string motd;
	if(recv_data.size())
		recv_data >> motd;

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	_player->m_playerInfo->guild->SetMOTD(motd.c_str(), this);
}

void WorldSession::HandleGuildRank(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	CHECK_PACKET_SIZE(recv_data, 9);

	if(!_player->m_playerInfo->guild)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(GetPlayer()->GetLowGUID() != _player->m_playerInfo->guild->GetGuildLeader())
	{
		Guild::SendGuildCommandResult(this, GUILD_INVITE_S, "", GUILD_PERMISSIONS);
		return;
	}

	uint32 rankId;
	string newName;
	uint32 i;
	GuildRank* pRank;

	recv_data >> rankId;
	pRank = _player->m_playerInfo->guild->GetGuildRank(rankId);
	if(pRank == NULL)
		return;

	recv_data >> pRank->iRights;
	recv_data >> newName;

	if(newName.length() < 2)
		newName = string(pRank->szRankName);

	if(strcmp(newName.c_str(), pRank->szRankName) != 0)
	{
		// name changed
		char* pTmp = pRank->szRankName;
		pRank->szRankName = strdup(newName.c_str());
		free(pTmp);
	}

	int32 gold_limit;
	recv_data >> gold_limit;

	// do not touch guild masters withdraw limit
	if(pRank->iGoldLimitPerDay != -1 || rankId != 0)
		pRank->iGoldLimitPerDay = gold_limit;


	for(i = 0; i < MAX_GUILD_BANK_TABS; ++i)
	{
		recv_data >> pRank->iTabPermissions[i].iFlags;
		recv_data >> pRank->iTabPermissions[i].iStacksPerDay;
	}

	uint32 guildID = _player->m_playerInfo->guild->GetGuildId();
	uint32 rankID = pRank->iId;

	CharacterDatabase.Execute("DELETE FROM guild_ranks WHERE guildid = %u AND rankid = %u;", guildID, rankID);

	CharacterDatabase.Execute("INSERT INTO guild_ranks VALUES(%u, %u, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
	                          guildID, rankID, CharacterDatabase.EscapeString(newName).c_str(),
	                          pRank->iRights, pRank->iGoldLimitPerDay,
	                          pRank->iTabPermissions[0].iFlags, pRank->iTabPermissions[0].iStacksPerDay,
	                          pRank->iTabPermissions[1].iFlags, pRank->iTabPermissions[1].iStacksPerDay,
	                          pRank->iTabPermissions[2].iFlags, pRank->iTabPermissions[2].iStacksPerDay,
	                          pRank->iTabPermissions[3].iFlags, pRank->iTabPermissions[3].iStacksPerDay,
	                          pRank->iTabPermissions[4].iFlags, pRank->iTabPermissions[4].iStacksPerDay,
	                          pRank->iTabPermissions[5].iFlags, pRank->iTabPermissions[5].iStacksPerDay);

	_player->m_playerInfo->guild->SendGuildRoster(this);
}

void WorldSession::HandleGuildAddRank(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	string rankName;
	Guild* pGuild = _player->GetGuild();

	if(pGuild == NULL)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(pGuild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PERMISSIONS);
		return;
	}

	recv_data >> rankName;
	if(rankName.size() < 2)
		return;

	pGuild->CreateGuildRank(rankName.c_str(), GR_RIGHT_DEFAULT, false);

	// there is probably a command result for this. need to find it.
	pGuild->SendGuildQuery(NULL);
	pGuild->SendGuildRoster(this);
}

void WorldSession::HandleGuildDelRank(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	Guild* pGuild = _player->GetGuild();

	if(pGuild == NULL)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	if(pGuild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PERMISSIONS);
		return;
	}

	pGuild->RemoveGuildRank(this);

	// there is probably a command result for this. need to find it.
	pGuild->SendGuildQuery(NULL);
	pGuild->SendGuildRoster(this);
}

void WorldSession::HandleGuildSetPublicNote(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	string target, newnote;
	recv_data >> target >> newnote;

	PlayerInfo* pTarget = objmgr.GetPlayerInfoByName(target.c_str());
	if(pTarget == NULL)
		return;

	if(!pTarget->guild)
		return;

	pTarget->guild->SetPublicNote(pTarget, newnote.c_str(), this);
}

void WorldSession::HandleGuildSetOfficerNote(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	string target, newnote;
	recv_data >> target >> newnote;

	PlayerInfo* pTarget = objmgr.GetPlayerInfoByName(target.c_str());
	if(pTarget == NULL)
		return;

	if(!pTarget->guild)
		return;

	pTarget->guild->SetOfficerNote(pTarget, newnote.c_str(), this);
}

void WorldSession::HandleSaveGuildEmblem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	Guild* pGuild = _player->GetGuild();
	int32 cost = MONEY_ONE_GOLD * 10;
	uint32 emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor;
	WorldPacket data(MSG_SAVE_GUILD_EMBLEM, 4);
	recv_data >> guid;

	CHECK_PACKET_SIZE(recv_data, 28);
	CHECK_GUID_EXISTS(guid);

	recv_data >> emblemStyle >> emblemColor >> borderStyle >> borderColor >> backgroundColor;
	if(pGuild == NULL)
	{
		data << uint32(ERR_GUILDEMBLEM_NOGUILD);
		SendPacket(&data);
		return;
	}

	if(pGuild->GetGuildLeader() != _player->GetLowGUID())
	{
		data << uint32(ERR_GUILDEMBLEM_NOTGUILDMASTER);
		SendPacket(&data);
		return;
	}

	if(!_player->HasGold((uint32)cost))
	{
		data << uint32(ERR_GUILDEMBLEM_NOTENOUGHMONEY);
		SendPacket(&data);
		return;
	}

	data <<	uint32(ERR_GUILDEMBLEM_SUCCESS);
	SendPacket(&data);

	// set in memory and database
	pGuild->SetTabardInfo(emblemStyle, emblemColor, borderStyle, borderColor, backgroundColor);

	// update all clients (probably is an event for this, again.)
	pGuild->SendGuildQuery(NULL);
}

// Charter part
void WorldSession::HandleCharterBuy(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	/*
	{CLIENT} Packet: (0x01BD) CMSG_PETITION_BUY PacketSize = 85
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|50 91 00 00 6E 13 01 F0 00 00 00 00 00 00 00 00 |P...n...........|
	|00 00 00 00 53 74 6F 72 6D 62 72 69 6E 67 65 72 |....Stormbringer|
	|73 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |s...............|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 01 00 00 |................|
	|00 00 00 00 00								  |.....		   |
	-------------------------------------------------------------------
	*/

	uint8 error;

	// Arena team charters are full of crap
	uint64 creature_guid;
	uint32 crap;
	uint64 crap2;
	string name, UnkString;
	//uint32 crap3, crap4, crap5, crap6, crap7, crap8, crap9;
	uint32 Data[7];
	uint16 crap10;
	uint32 crap11;
	uint32 crap12, PetitionSignerCount;
	string crap13;
	uint32 arena_index;


	recv_data >> creature_guid;
	recv_data >> crap >> crap2;
	recv_data >> name;
	recv_data >> UnkString;
	recv_data >> Data[0] >> Data[1] >> Data[2] >> Data[3] >> Data[4] >> Data[5] >> Data[6];
	recv_data >> crap10;
	recv_data >> crap11 >> crap12 >> PetitionSignerCount;
	for(uint32 s = 0; s < 10; ++s)
	{
		recv_data >> crap13;
	}
	recv_data >> arena_index;

	Creature* crt = _player->GetMapMgr()->GetCreature(GET_LOWGUID_PART(creature_guid));
	if(!crt)
	{
		Disconnect();
		return;
	}

	if(!crt->isTabardDesigner())
	{
		uint32 arena_type = arena_index - 1;
		if(arena_type > 2)
			return;

		if(_player->m_arenaTeams[arena_type])
		{
			SendNotification(_player->GetSession()->LocalizedWorldSrv(71));
			return;
		}

		ArenaTeam* t = objmgr.GetArenaTeamByName(name, arena_type);
		if(t != NULL)
		{
			sChatHandler.SystemMessage(this, _player->GetSession()->LocalizedWorldSrv(72));
			return;
		}

		if(objmgr.GetCharterByName(name, (CharterTypes)arena_index))
		{
			sChatHandler.SystemMessage(this, _player->GetSession()->LocalizedWorldSrv(72));
			return;
		}

		if(_player->m_charters[arena_index])
		{
			SendNotification(_player->GetSession()->LocalizedWorldSrv(73));
			return;
		}

		if(_player->getLevel() < PLAYER_ARENA_MIN_LEVEL)
		{
			//TODO: Replace by LocalizedWorldSrv(..)
			SendNotification("You must be at least level %u to buy Arena charter", PLAYER_ARENA_MIN_LEVEL);
			return;
		}

		static uint32 item_ids[] = {ARENA_TEAM_CHARTER_2v2, ARENA_TEAM_CHARTER_3v3, ARENA_TEAM_CHARTER_5v5};
		static uint32 costs[] = {ARENA_TEAM_CHARTER_2v2_COST, ARENA_TEAM_CHARTER_3v3_COST, ARENA_TEAM_CHARTER_5v5_COST};

		if(!_player->HasGold(costs[arena_type]))
			return;			// error message needed here

		ItemPrototype* ip = ItemPrototypeStorage.LookupEntry(item_ids[arena_type]);
		ARCEMU_ASSERT(ip != NULL);
		SlotResult res = _player->GetItemInterface()->FindFreeInventorySlot(ip);
		if(res.Result == 0)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_INVENTORY_FULL);
			return;
		}

		error = _player->GetItemInterface()->CanReceiveItem(ip, 1);
		if(error)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		}
		else
		{
			// Create the item and charter
			Item* i = objmgr.CreateItem(item_ids[arena_type], _player);
			Charter* c = objmgr.CreateCharter(_player->GetLowGUID(), (CharterTypes)arena_index);
			if(i == NULL || c == NULL)
				return;

			c->GuildName = name;
			c->ItemGuid = i->GetGUID();

			c->UnkString = UnkString;
			c->Unk1 = crap10;
			c->Unk2 = crap11;
			c->Unk3 = crap12;
			c->PetitionSignerCount = PetitionSignerCount;
			memcpy(c->Data, Data, sizeof(Data));

			i->SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
			i->SoulBind();
			i->SetEnchantmentId(0,  c->GetID());
			i->SetItemRandomSuffixFactor(57813883);
			if(!_player->GetItemInterface()->AddItemToFreeSlot(i))
			{
				c->Destroy();
				i->DeleteMe();
				return;
			}

			c->SaveToDB();

			_player->SendItemPushResult(false, true, false, true, _player->GetItemInterface()->LastSearchItemBagSlot(), _player->GetItemInterface()->LastSearchItemSlot(), 1, i->GetEntry(), i->GetItemRandomSuffixFactor(), i->GetItemRandomPropertyId(), i->GetStackCount());

			_player->ModGold(-(int32)costs[arena_type]);
			_player->m_charters[arena_index] = c;
			_player->SaveToDB(false);
		}
	}
	else
	{
		if(!_player->HasGold(1000))
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_NOT_ENOUGH_MONEY);
			return;
		}

		Guild* g = objmgr.GetGuildByGuildName(name);
		Charter* c = objmgr.GetCharterByName(name, CHARTER_TYPE_GUILD);
		if(g != 0 || c != 0)
		{
			SendNotification(_player->GetSession()->LocalizedWorldSrv(74));
			return;
		}

		if(_player->m_charters[CHARTER_TYPE_GUILD])
		{
			SendNotification(_player->GetSession()->LocalizedWorldSrv(75));
			return;
		}

		ItemPrototype* ip = ItemPrototypeStorage.LookupEntry(ITEM_ENTRY_GUILD_CHARTER);
		ARCEMU_ASSERT(ip != NULL);
		SlotResult res = _player->GetItemInterface()->FindFreeInventorySlot(ip);
		if(res.Result == 0)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(0, 0, INV_ERR_INVENTORY_FULL);
			return;
		}

		error = _player->GetItemInterface()->CanReceiveItem(ItemPrototypeStorage.LookupEntry(ITEM_ENTRY_GUILD_CHARTER), 1);
		if(error)
		{
			_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, error);
		}
		else
		{
			// Meh...
			WorldPacket data(SMSG_PLAY_OBJECT_SOUND, 12);
			data << uint32(0x000019C2);
			data << creature_guid;
			SendPacket(&data);

			// Create the item and charter
			Item* i = objmgr.CreateItem(ITEM_ENTRY_GUILD_CHARTER, _player);
			c = objmgr.CreateCharter(_player->GetLowGUID(), CHARTER_TYPE_GUILD);
			if(i == NULL || c == NULL)
				return;

			c->GuildName = name;
			c->ItemGuid = i->GetGUID();

			c->UnkString = UnkString;
			c->Unk1 = crap10;
			c->Unk2 = crap11;
			c->Unk3 = crap12;
			c->PetitionSignerCount = PetitionSignerCount;
			memcpy(c->Data, Data, sizeof(Data));

			i->SetStackCount(1);
			i->SoulBind();
			i->SetEnchantmentId(0, c->GetID());
			i->SetItemRandomSuffixFactor(57813883);
			if(!_player->GetItemInterface()->AddItemToFreeSlot(i))
			{
				c->Destroy();
				i->DeleteMe();
				return;
			}

			c->SaveToDB();

			_player->SendItemPushResult(false, true, false, true, _player->GetItemInterface()->LastSearchItemBagSlot(), _player->GetItemInterface()->LastSearchItemSlot(), 1, i->GetEntry(), i->GetItemRandomSuffixFactor(), i->GetItemRandomPropertyId(), i->GetStackCount());

			_player->m_charters[CHARTER_TYPE_GUILD] = c;
			_player->ModGold(-1000);
			_player->SaveToDB(false);
		}
	}
}

void SendShowSignatures(Charter* c, uint64 i, Player* p)
{
	WorldPacket data(SMSG_PETITION_SHOW_SIGNATURES, 100);
	data << i;
	data << (uint64)c->GetLeader();
	data << c->GetID();
	data << uint8(c->SignatureCount);
	for(uint32 j = 0; j < c->Slots; ++j)
	{
		if(c->Signatures[j] == 0) continue;
		data << uint64(c->Signatures[j]) << uint32(1);
	}
	data << uint8(0);
	p->GetSession()->SendPacket(&data);
}

void WorldSession::HandleCharterShowSignatures(WorldPacket & recv_data)
{
	Charter* pCharter;
	uint64 item_guid;
	recv_data >> item_guid;
	pCharter = objmgr.GetCharterByItemGuid(item_guid);

	if(pCharter)
		SendShowSignatures(pCharter, item_guid, _player);
}

void WorldSession::HandleCharterQuery(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	/*
	{SERVER} Packet: (0x01C7) SMSG_PETITION_QUERY_RESPONSE PacketSize = 77
	|------------------------------------------------|----------------|
	|00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	|------------------------------------------------|----------------|
	|20 08 00 00 28 32 01 00 00 00 00 00 53 74 6F 72 | ...(2......Stor|
	|6D 62 72 69 6E 67 65 72 73 00 00 09 00 00 00 09 |mbringers.......|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
	|00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
	|00 00 00 00 00 00 00 00 00 00 00 00 00		  |.............   |
	-------------------------------------------------------------------

	uint32 charter_id
	uint64 leader_guid
	string guild_name
	uint8  0			<-- maybe subname? or some shit.. who knows
	uint32 9
	uint32 9
	uint32[9] signatures
	uint8  0
	uint8  0
	*/
//this is wrong there are 42 bytes after 9 9, 4*9+2=38 not 42,
	//moreover it can't signature, blizz uses always fullguid so it must be uin64
	//moreover this does not show people who signed this, for this purpose there is another opcde
	uint32 charter_id;
	uint64 item_guid;
	recv_data >> charter_id;
	recv_data >> item_guid;
	/*Charter * c = objmgr.GetCharter(charter_id,CHARTER_TYPE_GUILD);
	if(c == 0)
		c = objmgr.GetCharter(charter_id, CHARTER_TYPE_ARENA_2V2);
	if(c == 0)
		c = objmgr.GetCharter(charter_id, CHARTER_TYPE_ARENA_3V3);
	if(c == 0)
		c = objmgr.GetCharter(charter_id, CHARTER_TYPE_ARENA_5V5);*/

	Charter* c = objmgr.GetCharterByItemGuid(item_guid);
	if(c == 0)
		return;

	WorldPacket data(SMSG_PETITION_QUERY_RESPONSE, 100);
	data << charter_id;
	data << (uint64)c->LeaderGuid;
	data << c->GuildName << uint8(0);
	if(c->CharterType == CHARTER_TYPE_GUILD)
	{
		data << uint32(9) << uint32(9);
	}
	else
	{
		/*uint32 v = c->CharterType;
		if(c->CharterType == CHARTER_TYPE_ARENA_3V3)
			v=2;
		else if(c->CharterType == CHARTER_TYPE_ARENA_5V5)
			v=4;

		data << v << v;*/
		data << uint32(c->Slots) << uint32(c->Slots);
	}

	data << uint32(0);                                      // 4
	data << uint32(0);                                      // 5
	data << uint32(0);                                      // 6
	data << uint32(0);                                      // 7
	data << uint32(0);                                      // 8
	data << uint16(0);                                      // 9 2 bytes field

	if(c->CharterType == CHARTER_TYPE_GUILD)
	{
		data << uint32(1);                                  // 10 min level to sign a guild charter
		data << uint32(80);									// 11 max level to sign a guild charter
	}
	else
	{
		data << uint32(80);                                 // 10 min level to sign an arena charter
		data << uint32(80);									// 11 max level to sign an arena charter
	}

	data << uint32(0);                                      // 12
	data << uint32(0);                                      // 13 count of next strings?
	data << uint32(0);                                      // 14
	data << uint32(0);
	data << uint16(0);

	if(c->CharterType == CHARTER_TYPE_GUILD)
	{
		data << uint32(0);
	}
	else
	{
		data << uint32(1);
	}

	SendPacket(&data);
}

void WorldSession::HandleCharterOffer(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint32 shit;
	uint64 item_guid, target_guid;
	Charter* pCharter;
	recv_data >> shit >> item_guid >> target_guid;

	Player* pTarget = _player->GetMapMgr()->GetPlayer((uint32)target_guid);
	pCharter = objmgr.GetCharterByItemGuid(item_guid);

	if(!pCharter)
	{
		SendNotification(_player->GetSession()->LocalizedWorldSrv(76));
		return;
	}

	if(pTarget == 0 || pTarget->GetTeam() != _player->GetTeam() || (pTarget == _player && !sWorld.interfaction_guild))
	{
		SendNotification(_player->GetSession()->LocalizedWorldSrv(77));
		return;
	}

	if(!pTarget->CanSignCharter(pCharter, _player))
	{
		SendNotification(_player->GetSession()->LocalizedWorldSrv(78));
		return;
	}

	SendShowSignatures(pCharter, item_guid, pTarget);
}

void WorldSession::HandleCharterSign(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 item_guid;
	recv_data >> item_guid;

	Charter* c = objmgr.GetCharterByItemGuid(item_guid);
	if(c == NULL)
		return;

	for(uint32 i = 0; i < c->SignatureCount; ++i)
	{
		if(c->Signatures[i] == _player->GetGUID())
		{
			SendNotification(_player->GetSession()->LocalizedWorldSrv(79));
			return;
		}
	}

	if(c->IsFull())
		return;

	c->AddSignature(_player->GetLowGUID());
	c->SaveToDB();
	_player->m_charters[c->CharterType] = c;
	_player->SaveToDB(false);

	Player* l = _player->GetMapMgr()->GetPlayer(c->GetLeader());
	if(l == 0)
		return;

	WorldPacket data(SMSG_PETITION_SIGN_RESULTS, 100);
	data << item_guid << _player->GetGUID() << uint32(0);
	l->GetSession()->SendPacket(&data);
	data.clear();
	data << item_guid << (uint64)c->GetLeader() << uint32(0);
	SendPacket(&data);
}

void WorldSession::HandleCharterDecline(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 item_guid;
	recv_data >> item_guid;

	Charter* c = objmgr.GetCharterByItemGuid(item_guid);
	if(!c)
		return;

	Player* owner = objmgr.GetPlayer(c->GetLeader());
	if(owner)
	{
		WorldPacket data(MSG_PETITION_DECLINE, 8);
		data << _player->GetGUID();
		owner->GetSession()->SendPacket(&data);
	}
}

void WorldSession::HandleCharterTurnInCharter(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 mooguid;
	recv_data >> mooguid;
	Charter* pCharter = objmgr.GetCharterByItemGuid(mooguid);
	if(!pCharter) return;

	if(pCharter->CharterType == CHARTER_TYPE_GUILD)
	{
		Charter* gc = _player->m_charters[CHARTER_TYPE_GUILD];
		if(gc == 0)
			return;
		if(gc->SignatureCount < 9 && Config.MainConfig.GetBoolDefault("Server", "RequireAllSignatures", false))
		{
			Guild::SendTurnInPetitionResult(this, ERR_PETITION_NOT_ENOUGH_SIGNATURES);
			return;
		}

		// don't know hacky or not but only solution for now
		// If everything is fine create guild

		Guild* pGuild = Guild::Create();
		pGuild->CreateFromCharter(gc, this);

		// Destroy the charter
		_player->m_charters[CHARTER_TYPE_GUILD] = 0;
		gc->Destroy();

		_player->GetItemInterface()->RemoveItemAmt(ITEM_ENTRY_GUILD_CHARTER, 1);
		sHookInterface.OnGuildCreate(_player, pGuild);
	}
	else
	{
		/* Arena charter - TODO: Replace with correct messages */
		ArenaTeam* team;
		uint32 type;
		uint32 i;
		uint32 icon, iconcolor, bordercolor, border, background;
		recv_data >> iconcolor >> icon >> bordercolor >> border >> background;

		switch(pCharter->CharterType)
		{
			case CHARTER_TYPE_ARENA_2V2:
				type = ARENA_TEAM_TYPE_2V2;
				break;

			case CHARTER_TYPE_ARENA_3V3:
				type = ARENA_TEAM_TYPE_3V3;
				break;

			case CHARTER_TYPE_ARENA_5V5:
				type = ARENA_TEAM_TYPE_5V5;
				break;

			default:
				SendNotification("Internal Error");
				return;
		}

		if(_player->m_arenaTeams[pCharter->CharterType - 1] != NULL)
		{
			sChatHandler.SystemMessage(this, "You are already in an arena team.");
			return;
		}

		if(pCharter->SignatureCount < pCharter->GetNumberOfSlotsByType() && Config.MainConfig.GetBoolDefault("Server", "RequireAllSignatures", false))
		{
			Guild::SendTurnInPetitionResult(this, ERR_PETITION_NOT_ENOUGH_SIGNATURES);
			return;
		}

		team = new ArenaTeam(type, objmgr.GenerateArenaTeamId());
		team->m_name = pCharter->GuildName;
		team->m_emblemColour = iconcolor;
		team->m_emblemStyle = icon;
		team->m_borderColour = bordercolor;
		team->m_borderStyle = border;
		team->m_backgroundColour = background;
		team->m_leader = _player->GetLowGUID();
		team->m_stat_rating = 1500;

		objmgr.AddArenaTeam(team);
		objmgr.UpdateArenaTeamRankings();
		team->AddMember(_player->m_playerInfo);


		/* Add the members */
		for(i = 0; i < pCharter->SignatureCount; ++i)
		{
			PlayerInfo* info = objmgr.GetPlayerInfo(pCharter->Signatures[i]);
			if(info)
			{
				team->AddMember(info);
			}
		}

		_player->GetItemInterface()->SafeFullRemoveItemByGuid(mooguid);
		_player->m_charters[pCharter->CharterType] = NULL;
		pCharter->Destroy();
	}

	Guild::SendTurnInPetitionResult(this, ERR_PETITION_OK);
}

void WorldSession::HandleCharterRename(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	string name;
	recv_data >> guid >> name;

	Charter* pCharter = objmgr.GetCharterByItemGuid(guid);
	if(pCharter == 0)
		return;

	Guild* g = objmgr.GetGuildByGuildName(name);
	Charter* c = objmgr.GetCharterByName(name, (CharterTypes)pCharter->CharterType);
	if(c || g)
	{
		SendNotification("That name is in use by another guild.");
		return;
	}

	c = pCharter;
	c->GuildName = name;
	c->SaveToDB();
	WorldPacket data(MSG_PETITION_RENAME, 100);
	data << guid << name;
	SendPacket(&data);
}

void WorldSession::HandleGuildLog(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(!_player->m_playerInfo->guild)
		return;

	_player->m_playerInfo->guild->SendGuildLog(this);
}

void WorldSession::HandleGuildBankBuyTab(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	recv_data >> guid;

	if(!_player->IsInWorld())
		return;

	if(!_player->m_playerInfo->guild)
		return;

	if(_player->m_playerInfo->guild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_MEMBER_S, "", GUILD_PERMISSIONS);
		return;
	}

	if(_player->m_playerInfo->guild->GetBankTabCount() < MAX_GUILD_BANK_TABS)
	{
		//                                        tab1, tab2, tab3, tab4, tab5, tab6
		const static int32 GuildBankPrices[6] = { 100, 250,  500,  1000, 2500, 5000 };
		int32 cost = MONEY_ONE_GOLD * GuildBankPrices[_player->m_playerInfo->guild->GetBankTabCount()];

		if(!_player->HasGold((uint32)cost))
			return;

		_player->ModGold(-cost);
		_player->m_playerInfo->guild->BuyBankTab(this);
		_player->m_playerInfo->guild->LogGuildEvent(GUILD_EVENT_BANKTABBOUGHT, 1, "");
	}
}

void WorldSession::HandleGuildBankGetAvailableAmount(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	// calculate using the last withdrawl blablabla
	if(_player->m_playerInfo->guildMember == NULL)
		return;

	uint64 money = _player->m_playerInfo->guild->GetBankBalance();
	uint32 avail = _player->m_playerInfo->guildMember->CalculateAvailableAmount();

	/* pls gm mi hero poor give 1 gold coin pl0x */
	WorldPacket data(MSG_GUILD_BANK_MONEY_WITHDRAWN, 4);
	data << uint32(money > avail ? avail : money);
	SendPacket(&data);
}

void WorldSession::HandleGuildBankModifyTab(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	GuildBankTab* pTab;
	uint64 guid;
	uint8 slot;
	string tabname;
	string tabicon;
	char* ptmp;

	recv_data >> guid;
	recv_data >> slot;
	recv_data >> tabname;
	recv_data >> tabicon;

	if(_player->m_playerInfo->guild == NULL)
		return;

	pTab = _player->m_playerInfo->guild->GetBankTab(slot);
	if(pTab == NULL)
		return;

	if(_player->m_playerInfo->guild->GetGuildLeader() != _player->GetLowGUID())
	{
		Guild::SendGuildCommandResult(this, GUILD_MEMBER_S, "", GUILD_PERMISSIONS);
		return;
	}

	if(tabname.size())
	{
		if(!(pTab->szTabName && strcmp(pTab->szTabName, tabname.c_str()) == 0))
		{
			ptmp = pTab->szTabName;
			pTab->szTabName = strdup(tabname.c_str());
			if(ptmp)
				free(ptmp);

			CharacterDatabase.Execute("UPDATE guild_banktabs SET tabName = \'%s\' WHERE guildId = %u AND tabId = %u",
			                          CharacterDatabase.EscapeString(tabname).c_str(), _player->m_playerInfo->guild->GetGuildId(), (uint32)slot);
		}
	}
	else
	{
		if(pTab->szTabName)
		{
			ptmp = pTab->szTabName;
			pTab->szTabName = NULL;
			if(ptmp)
				free(ptmp);

			CharacterDatabase.Execute("UPDATE guild_banktabs SET tabName = '' WHERE guildId = %u AND tabId = %u",
			                          _player->m_playerInfo->guild->GetGuildId(), (uint32)slot);
		}
	}

	if(tabicon.size())
	{
		if(!(pTab->szTabIcon && strcmp(pTab->szTabIcon, tabicon.c_str()) == 0))
		{
			ptmp = pTab->szTabIcon;
			pTab->szTabIcon = strdup(tabicon.c_str());
			if(ptmp)
				free(ptmp);

			CharacterDatabase.Execute("UPDATE guild_banktabs SET tabIcon = \'%s\' WHERE guildId = %u AND tabId = %u",
			                          CharacterDatabase.EscapeString(tabicon).c_str(), _player->m_playerInfo->guild->GetGuildId(), (uint32)slot);
		}
	}
	else
	{
		if(pTab->szTabIcon)
		{
			ptmp = pTab->szTabIcon;
			pTab->szTabIcon = NULL;
			if(ptmp)
				free(ptmp);

			CharacterDatabase.Execute("UPDATE guild_banktabs SET tabIcon = '' WHERE guildId = %u AND tabId = %u",
			                          _player->m_playerInfo->guild->GetGuildId(), (uint32)slot);
		}
	}

	// update the client
	_player->m_playerInfo->guild->SendGuildBankInfo(this);
}

void WorldSession::HandleGuildBankWithdrawMoney(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint32 money;

	recv_data >> guid;
	recv_data >> money;

	if(_player->m_playerInfo->guild == NULL)
		return;

	_player->m_playerInfo->guild->WithdrawMoney(this, money);
}

void WorldSession::HandleGuildBankDepositMoney(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint32 money;

	recv_data >> guid;
	recv_data >> money;

	if(_player->m_playerInfo->guild == NULL)
		return;

	_player->m_playerInfo->guild->DepositMoney(this, money);
}

void WorldSession::HandleGuildBankDepositItem(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint8 source_isfrombank;
	uint32 wtf;
	uint8 wtf2;
	uint32 i;

	Guild* pGuild = _player->m_playerInfo->guild;
	GuildMember* pMember = _player->m_playerInfo->guildMember;

	if(pGuild == NULL || pMember == NULL)
		return;

	recv_data >> guid >> source_isfrombank;
	if(source_isfrombank)
	{
		GuildBankTab* pSourceTab;
		GuildBankTab* pDestTab;
		Item* pSourceItem;
		Item* pDestItem;
		uint8 dest_bank;
		uint8 dest_bankslot;
		uint8 source_bank;
		uint8 source_bankslot;

		/* read packet */
		recv_data >> dest_bank;
		recv_data >> dest_bankslot;
		recv_data >> wtf;
		recv_data >> source_bank;
		recv_data >> source_bankslot;

		/* sanity checks to avoid overflows */
		if(source_bankslot >= MAX_GUILD_BANK_SLOTS ||
		        dest_bankslot >= MAX_GUILD_BANK_SLOTS ||
		        source_bank >= MAX_GUILD_BANK_TABS ||
		        dest_bank >= MAX_GUILD_BANK_TABS)
		{
			return;
		}

		/* make sure we have permissions */
		if(!pMember->pRank->CanPerformBankCommand(GR_RIGHT_GUILD_BANK_DEPOSIT_ITEMS, dest_bank) ||
		        !pMember->pRank->CanPerformBankCommand(GR_RIGHT_GUILD_BANK_DEPOSIT_ITEMS, source_bank))
			return;

		/* locate the tabs */
		pSourceTab = pGuild->GetBankTab(source_bank);
		pDestTab = pGuild->GetBankTab(dest_bank);
		if(pSourceTab == NULL || pDestTab == NULL)
			return;

		pSourceItem = pSourceTab->pSlots[source_bankslot];
		pDestItem = pDestTab->pSlots[dest_bankslot];

		if(pSourceItem == NULL && pDestItem == NULL)
			return;

		/* perform the actual swap */
		pSourceTab->pSlots[source_bankslot] = pDestItem;
		pDestTab->pSlots[dest_bankslot] = pSourceItem;

		/* update the client */
		if(pSourceTab == pDestTab)
		{
			/* send both slots in the packet */
			pGuild->SendGuildBank(this, pSourceTab, source_bankslot, dest_bankslot);
		}
		else
		{
			/* send a packet for each different bag */
			pGuild->SendGuildBank(this, pSourceTab, source_bankslot, -1);
			pGuild->SendGuildBank(this, pDestTab, dest_bankslot, -1);
		}

		/* update in sql */
		if(pDestItem == NULL)
		{
			/* this means the source slot is no longer being used. */
			CharacterDatabase.Execute("DELETE FROM guild_bankitems WHERE guildId = %u AND tabId = %u AND slotId = %u",
			                          pGuild->GetGuildId(), (uint32)pSourceTab->iTabId, (uint32)source_bankslot);
		}
		else
		{
			/* insert the new item */
			CharacterDatabase.Execute("DELETE FROM guild_bankitems WHERE guildId = %u AND tabId = %u AND slotId = %u",
			                          pGuild->GetGuildId(), pSourceTab->iTabId, source_bankslot);
			CharacterDatabase.Execute("INSERT INTO guild_bankitems VALUES(%u, %u, %u, %u)",
			                          pGuild->GetGuildId(), (uint32)pSourceTab->iTabId, (uint32)source_bankslot, pDestItem->GetLowGUID());
		}

		if(pSourceItem == NULL)
		{
			/* this means the destination slot is no longer being used. */
			CharacterDatabase.Execute("DELETE FROM guild_bankitems WHERE guildId = %u AND tabId = %u AND slotId = %u",
			                          pGuild->GetGuildId(), (uint32)pDestTab->iTabId, (uint32)dest_bankslot);
		}
		else
		{
			/* insert the new item */
			CharacterDatabase.Execute("DELETE FROM guild_bankitems WHERE guildId = %u AND tabId = %u AND slotId = %u",
			                          pGuild->GetGuildId(), pDestTab->iTabId, dest_bankslot);
			CharacterDatabase.Execute("INSERT INTO guild_bankitems VALUES(%u, %u, %u, %u)",
			                          pGuild->GetGuildId(), (uint32)pDestTab->iTabId, (uint32)dest_bankslot, pSourceItem->GetLowGUID());
		}
	}
	else
	{
		uint8 source_bagslot;
		uint8 source_slot;
		uint8 dest_bank;
		uint8 dest_bankslot;
		uint8 withdraw_stack = 0;
		uint8 deposit_stack = 0;
		GuildBankTab* pTab;
		Item* pSourceItem;
		Item* pDestItem;
		Item* pSourceItem2;

		/* read packet */
		recv_data >> dest_bank;
		recv_data >> dest_bankslot;
		recv_data >> wtf;
		recv_data >> wtf2;
		if(wtf2)
			recv_data >> withdraw_stack;

		recv_data >> source_bagslot;
		recv_data >> source_slot;

		if(!(source_bagslot == 1 && source_slot == 0))
			recv_data >> wtf2 >> deposit_stack;

		/* sanity checks to avoid overflows */
		if(dest_bank >= MAX_GUILD_BANK_TABS)
		{
			return;
		}

		/* make sure we have permissions */
		if(!pMember->pRank->CanPerformBankCommand(GR_RIGHT_GUILD_BANK_DEPOSIT_ITEMS, dest_bank))
			return;

		/* get tab */
		pTab = pGuild->GetBankTab(dest_bank);
		if(pTab == NULL)
			return;

		/* check if we are autoassigning */
		if(dest_bankslot == 0xff)
		{
			for(i = 0; i < MAX_GUILD_BANK_SLOTS; ++i)
			{
				if(pTab->pSlots[i] == NULL)
				{
					dest_bankslot = (uint8)i;
					break;
				}
			}

			if(dest_bankslot == 0xff)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_BAG_FULL);
				return;
			}
		}

		/* another check here */
		if(dest_bankslot >= MAX_GUILD_BANK_SLOTS)
			return;

		/* check if we're pulling an item from the bank, make sure we're not cheating. */
		pDestItem = pTab->pSlots[dest_bankslot];

		/* grab the source/destination item */
		if(source_bagslot == 1 && source_slot == 0)
		{
			// find a free bag slot
			if(pDestItem == NULL)
			{
				// dis is fucked up mate
				return;
			}

			SlotResult sr = _player->GetItemInterface()->FindFreeInventorySlot(pDestItem->GetProto());
			if(!sr.Result)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_BAG_FULL);
				return;
			}

			source_bagslot = sr.ContainerSlot;
			source_slot = sr.Slot;
		}

		if(source_bagslot == 0xff && source_slot < INVENTORY_SLOT_ITEM_START && pDestItem != NULL)
		{
			sCheatLog.writefromsession(this, "Tried to equip an item from the guild bank (WPE HACK)");
			SystemMessage("You don't have permission to do that.");
			return;
		}

		if(pDestItem != NULL)
		{
			if(pMember->pRank->iTabPermissions[dest_bank].iStacksPerDay == 0)
			{
				SystemMessage("You don't have permission to do that.");
				return;
			}

			if(pMember->pRank->iTabPermissions[dest_bank].iStacksPerDay > 0)
			{
				if(pMember->CalculateAllowedItemWithdraws(dest_bank) == 0)
				{
					// a "no permissions" notice would probably be better here
					SystemMessage("You have withdrawn the maximum amount for today.");
					return;
				}

				/* reduce his count by one */
				pMember->OnItemWithdraw(dest_bank);
			}
		}

		pSourceItem = _player->GetItemInterface()->GetInventoryItem(source_bagslot, source_slot);

		/* make sure that both aren't null - wtf ? */
		if(pSourceItem == NULL && pDestItem == NULL)
			return;

		if(pSourceItem != NULL)
		{
			// make sure its not a soulbound item
			if(pSourceItem->IsSoulbound() || pSourceItem->GetProto()->Class == ITEM_CLASS_QUEST)
			{
				_player->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_CANT_DROP_SOULBOUND);
				return;
			}

			// pull the item from the slot
			if(deposit_stack && pSourceItem->GetStackCount() > deposit_stack)
			{
				pSourceItem2 = pSourceItem;
				pSourceItem = objmgr.CreateItem(pSourceItem2->GetEntry(), _player);
				if(pSourceItem == NULL)
					return;

				pSourceItem->SetStackCount(deposit_stack);
				pSourceItem->SetCreatorGUID(pSourceItem2->GetCreatorGUID());
				pSourceItem2->ModStackCount(-(int32)deposit_stack);
				pSourceItem2->m_isDirty = true;
			}
			else
			{
				if(!_player->GetItemInterface()->SafeRemoveAndRetreiveItemFromSlot(source_bagslot, source_slot, false))
					return;

				pSourceItem->RemoveFromWorld();
			}
		}

		/* perform the swap. */
		/* pSourceItem = Source item from players backpack coming into guild bank */
		if(pSourceItem == NULL)
		{
			/* splitting */
			if(pDestItem != NULL && deposit_stack > 0 && pDestItem->GetStackCount() > deposit_stack)
			{
				pSourceItem2 = pDestItem;

				pSourceItem2->ModStackCount(-(int32)deposit_stack);
				pSourceItem2->SaveToDB(0, 0, true, NULL);

				pDestItem = objmgr.CreateItem(pSourceItem2->GetEntry(), _player);
				if(pDestItem == NULL)
					return;

				pDestItem->SetStackCount(deposit_stack);
				pDestItem->SetCreatorGUID(pSourceItem2->GetCreatorGUID());
			}
			else
			{
				/* that slot in the bank is now empty. */
				pTab->pSlots[dest_bankslot] = NULL;
				CharacterDatabase.Execute("DELETE FROM guild_bankitems WHERE guildId = %u AND tabId = %u AND slotId = %u",
				                          pGuild->GetGuildId(), (uint32)pTab->iTabId, (uint32)dest_bankslot);
			}
		}
		else
		{
			/* there is a new item in that slot. */
			pTab->pSlots[dest_bankslot] = pSourceItem;

			CharacterDatabase.Execute("DELETE FROM guild_bankitems WHERE guildId = %u AND tabId = %u AND slotId = %u",
			                          pGuild->GetGuildId(), pTab->iTabId, dest_bankslot);
			CharacterDatabase.Execute("INSERT INTO guild_bankitems VALUES(%u, %u, %u, %u)",
			                          pGuild->GetGuildId(), (uint32)pTab->iTabId, (uint32)dest_bankslot, pSourceItem->GetLowGUID());

			/* remove the item's association with the player */
			pSourceItem->SetOwner(NULL);
			pSourceItem->SetOwnerGUID(0);
			pSourceItem->SaveToDB(0, 0, true, NULL);

			/* log it */
			pGuild->LogGuildBankAction(GUILD_BANK_LOG_EVENT_DEPOSIT_ITEM, _player->GetLowGUID(), pSourceItem->GetEntry(),
			                           (uint8)pSourceItem->GetStackCount(), pTab);
		}

		/* pDestItem = Item from bank coming into players backpack */
		if(pDestItem == NULL)
		{
			/* the item has already been removed from the players backpack at this stage,
			there isn't really much to do at this point. */
		}
		else
		{
			/* the guild was robbed by some n00b! :O */
			pDestItem->SetOwner(_player);
			pDestItem->SetOwnerGUID(_player->GetGUID());
			pDestItem->SaveToDB(source_bagslot, source_slot, true, NULL);

			/* add it to him in game */
			if(!_player->GetItemInterface()->SafeAddItem(pDestItem, source_bagslot, source_slot))
			{
				/* this *really* shouldn't happen. */
				if(!_player->GetItemInterface()->AddItemToFreeSlot(pDestItem))
				{
					//pDestItem->DeleteFromDB();
					pDestItem->DeleteMe();
				}
			}
			else
			{
				/* log it */
				pGuild->LogGuildBankAction(GUILD_BANK_LOG_EVENT_WITHDRAW_ITEM, _player->GetLowGUID(), pDestItem->GetEntry(),
				                           (uint8)pDestItem->GetStackCount(), pTab);
			}
		}

		/* update the clients view of the bank tab */
		pGuild->SendGuildBank(this, pTab, dest_bankslot);
	}
}

void WorldSession::HandleGuildBankOpenVault(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	GameObject* pObj;
	uint64 guid;

	if(!_player->IsInWorld() || _player->m_playerInfo->guild == NULL)
	{
		Guild::SendGuildCommandResult(this, GUILD_CREATE_S, "", GUILD_PLAYER_NOT_IN_GUILD);
		return;
	}

	recv_data >> guid;
	pObj = _player->GetMapMgr()->GetGameObject((uint32)guid);
	if(pObj == NULL)
		return;

	_player->m_playerInfo->guild->SendGuildBankInfo(this);
}

void WorldSession::HandleGuildBankViewTab(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	uint64 guid;
	uint8 tabid;
	GuildBankTab* pTab;
	Guild* pGuild = _player->m_playerInfo->guild;

	recv_data >> guid;
	recv_data >> tabid;

	//Log.Warning("HandleGuildBankViewTab", "Tab %u", (uint32)tabid);

	// maybe last uint8 is "show additional info" such as tab names? *shrug*
	if(pGuild == NULL)
		return;

	pTab = pGuild->GetBankTab(tabid);
	if(pTab == NULL)
		return;

	pGuild->SendGuildBank(this, pTab);
}

void Guild::SendGuildBankInfo(WorldSession* pClient)
{
	GuildMember* pMember = pClient->GetPlayer()->getPlayerInfo()->guildMember;

	if(pMember == NULL)
		return;

	WorldPacket data(SMSG_GUILD_BANK_LIST, 500);
	data << uint64(m_bankBalance);
	data << uint8(0);
	data << uint32(0);
	data << uint8(1);
	data << GetBankTabCount();

	for(uint8 i = 0; i < GetBankTabCount(); ++i)
	{
		GuildBankTab* pTab = GetBankTab(i);
		if(pTab == NULL || !pMember->pRank->CanPerformBankCommand(GR_RIGHT_GUILD_BANK_VIEW_TAB, i))
		{
			data << uint16(0);		// shouldn't happen
			continue;
		}

		if(pTab->szTabName)
			data << pTab->szTabName;
		else
			data << uint8(0);

		if(pTab->szTabIcon)
			data << pTab->szTabIcon;
		else
			data << uint8(0);
	}

	data << uint8(0);
	pClient->SendPacket(&data);
}

void Guild::SendGuildBank(WorldSession* pClient, GuildBankTab* pTab, int8 updated_slot1 /* = -1 */, int8 updated_slot2 /* = -1 */)
{
	size_t pos;
	uint32 count = 0;
	WorldPacket data(SMSG_GUILD_BANK_LIST, 1300);
	GuildMember* pMember = pClient->GetPlayer()->getPlayerInfo()->guildMember;

	if(pMember == NULL || !pMember->pRank->CanPerformBankCommand(GR_RIGHT_GUILD_BANK_VIEW_TAB, pTab->iTabId))
		return;

	//Log.Debug("SendGuildBank", "sending tab %u to client.", pTab->iTabId);

	data << uint64(m_bankBalance);	// amount you have deposited
	data << uint8(pTab->iTabId);
	data << uint32(pMember->CalculateAllowedItemWithdraws(pTab->iTabId));		// remaining stacks for this day
	data << uint8(0);	// Packet type: 0-tab content, 1-tab info,

	// no need to send tab names here..

	pos = data.wpos();
	data << uint8(0);	// number of items, will be filled later

	for(int32 j = 0; j < MAX_GUILD_BANK_SLOTS; ++j)
	{
		if(pTab->pSlots[j] != NULL)
		{
			if(updated_slot1 >= 0 && j == updated_slot1)
				updated_slot1 = -1;

			if(updated_slot2 >= 0 && j == updated_slot2)
				updated_slot2 = -1;

			++count;

			data << uint8(j);			// slot
			data << pTab->pSlots[j]->GetEntry();
			data << uint32(0);			// 3.3.0 (0x8000, 0x8020) from MaNGOS
			data << (uint32)pTab->pSlots[j]->GetItemRandomPropertyId();

			if(pTab->pSlots[j]->GetItemRandomPropertyId())
				data << (uint32)pTab->pSlots[j]->GetItemRandomSuffixFactor();

			data << uint32(pTab->pSlots[j]->GetStackCount());
			data << uint32(0);			// unknown value
			data << uint8(0);			// unknown 2.4.2
			uint32 Enchant0 = 0;
			EnchantmentInstance* ei = pTab->pSlots[j]->GetEnchantment(PERM_ENCHANTMENT_SLOT);
			if(ei != NULL)
				Enchant0 = ei->Enchantment->Id;
			if(Enchant0)
			{
				data << uint8(1);			// number of enchants
				data << uint8(0);			// enchantment slot
				data << uint32(Enchant0);	// enchantment id
			}
			else
				data << uint8(0);		// no enchantment
		}
	}

	// send the forced update slots
	if(updated_slot1 >= 0)
	{
		// this should only be hit if the items null though..
		if(pTab->pSlots[updated_slot1] == NULL)
		{
			++count;
			data << uint8(updated_slot1);
			data << uint32(0);
		}
	}

	if(updated_slot2 >= 0)
	{
		// this should only be hit if the items null though..
		if(pTab->pSlots[updated_slot2] == NULL)
		{
			++count;
			data << uint8(updated_slot2);
			data << uint32(0);
		}
	}

	*(uint8*)&data.contents()[pos] = (uint8)count; // push number of items
	pClient->SendPacket(&data);
}

void WorldSession::HandleGuildGetFullPermissions(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	WorldPacket data(MSG_GUILD_PERMISSIONS, 61);
	GuildRank* pRank = _player->GetGuildRankS();

	if(_player->GetGuild() == NULL)
		return;

	data << pRank->iId;
	data << pRank->iRights;
	data << pRank->iGoldLimitPerDay;
	data << _player->GetGuild()->GetBankTabCount();

	for(uint8 i = 0; i < MAX_GUILD_BANK_TABS; ++i)
	{
		data << pRank->iTabPermissions[i].iFlags;
		data << pRank->iTabPermissions[i].iStacksPerDay;
	}

	SendPacket(&data);
}

void WorldSession::HandleGuildBankViewLog(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	/* slot 6 = I'm requesting money log */
	uint8 slotid;
	recv_data >> slotid;

	if(_player->GetGuild() == NULL)
		return;

	_player->GetGuild()->SendGuildBankLog(this, slotid);
}
void WorldSession::HandleGuildBankQueryText(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	if(_player->GetGuild() == NULL)
		return;

	uint8 tabid;
	recv_data >> tabid;

	GuildBankTab* tab = _player->GetGuild()->GetBankTab(tabid);
	if(tab == NULL)
		return;

	uint32 len = tab->szTabInfo != NULL ? (uint32)strlen(tab->szTabInfo) : 1;

	WorldPacket data(MSG_QUERY_GUILD_BANK_TEXT, 1 + len);
	data << tabid;

	if(tab->szTabInfo != NULL)
		data << tab->szTabInfo;
	else
		data << uint8(0);

	SendPacket(&data);
}

void WorldSession::HandleSetGuildBankText(WorldPacket & recv_data)
{
	CHECK_INWORLD_RETURN

	GuildMember* pMember = _player->m_playerInfo->guildMember;

	if(_player->GetGuild() == NULL || pMember == NULL)
		return;

	uint8 tabid;
	string text;

	recv_data >> tabid >> text;

	GuildBankTab* tab = _player->GetGuild()->GetBankTab(tabid);
	if(tab != NULL &&
	        pMember->pRank->CanPerformBankCommand(GR_RIGHT_GUILD_BANK_CHANGE_TABTXT, tabid))
	{
		tab->szTabInfo = strdup(text.c_str());
		WorldPacket data(SMSG_GUILD_EVENT, 4);
		data << uint8(GUILD_EVENT_TABINFO);
		data << uint8(1);
		data << uint16(0x30 + tabid);
		SendPacket(&data);

		CharacterDatabase.Execute("UPDATE guild_banktabs SET tabInfo = \'%s\' WHERE guildId = %u AND tabId = %u",
		                          CharacterDatabase.EscapeString(text).c_str(), _player->m_playerInfo->guild->GetGuildId(), (uint32)tabid);
	}
}
