/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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

#include "RStdAfx.h"
#include "../ascent-shared/AuthCodes.h"

void Session::HandleCharacterEnum(WorldPacket & pck)
{
	struct player_item
	{
		uint32 displayid;
		uint8 invtype;
	};

	player_item items[20];
	uint32 slot;
	uint32 i;
	ItemPrototype * proto;

	uint32 start_time = getMSTime();

	// loading characters
	QueryResult* result = CharacterDatabase.Query("SELECT guid, level, race, class, gender, bytes, bytes2, guildid, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending FROM characters WHERE acct=%u ORDER BY guid", GetAccountId());
	QueryResult * res;
	CreatureInfo *info = NULL;
	uint8 num = 0;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data((result ? result->GetRowCount() * 200 : 1));	

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
	data.SetOpcode(SMSG_CHAR_ENUM);

	data << num;
	if( result )
	{
		uint64 guid;
		uint8 Class;
		uint32 bytes2;
		Field *fields;
		do
		{
			fields = result->Fetch();
			guid = fields[0].GetUInt32();
			bytes2 = fields[6].GetUInt32();
			Class = fields[3].GetUInt8();			

			/* build character enum, w0000t :p */
			data << fields[0].GetUInt64();		// guid
			data << fields[8].GetString();		// name
			data << fields[2].GetUInt8();		// race
			data << fields[3].GetUInt8();		// class
			data << fields[4].GetUInt8();		// gender
			data << fields[5].GetUInt32();		// PLAYER_BYTES
			data << uint8(bytes2 & 0xFF);		// facial hair
			data << fields[1].GetUInt8();		// Level
			data << fields[13].GetUInt32();		// zoneid
			data << fields[12].GetUInt32();		// Mapid
			data << fields[11].GetFloat();		// X
			data << fields[10].GetFloat();		// Y
			data << fields[9].GetFloat();		// Z
			data << fields[7].GetUInt32();		// GuildID

			if(fields[14].GetBool())
				data << (uint32)7;	// Banned (cannot login)
			else if(fields[17].GetBool())
				data << uint32(0x00A04342);  // wtf blizz? :P (rename pending)
			else if(fields[16].GetUInt32() != 0)
				data << (uint32)8704; // Dead (displaying as Ghost)
			else
				data << uint32(1);		// alive

			data << fields[15].GetUInt8();		// Rest State

			if(Class==9 || Class==3)
			{
				res = CharacterDatabase.Query("SELECT entryid FROM playerpets WHERE ownerguid=%u AND active=1", guid);

				if(res)
				{
					info = CreatureNameStorage.LookupEntry(res->Fetch()[0].GetUInt32());
					delete res;
				}
			}

			if(info)  //PET INFO uint32 displayid,	uint32 level,		 uint32 familyid
				data << uint32(info->DisplayID) << uint32(10) << uint32(info->Family);
			else
				data << uint32(0) << uint32(0) << uint32(0);

			res = CharacterDatabase.Query("SELECT slot, entry FROM playeritems WHERE ownerguid=%u and containerslot=-1 and slot < 19 and slot >= 0", guid);
			
			memset(items, 0, sizeof(player_item) * 20);
			if(res)
			{
				do 
				{
					proto = ItemPrototypeStorage.LookupEntry(res->Fetch()[1].GetUInt32());
					if(proto)
					{
						slot = res->Fetch()[0].GetUInt32();
						items[slot].displayid = proto->DisplayInfoID;
						items[slot].invtype = proto->InventoryType;
					}
				} while(res->NextRow());
				delete res;
			}

			for( i = 0; i < 20; ++i )
				data << items[i].displayid << items[i].invtype;

			num++;
		}
		while( result->NextRow() );

		delete result;
	}

	data.put<uint8>(0, num);

	Log.Debug("Character Enum", "Built in %u ms.", getMSTime() - start_time);
	SendPacket( &data );
}


void Session::HandlePlayerLogin(WorldPacket & pck)
{
	WorldPacket data(SMSG_CHARACTER_LOGIN_FAILED, 30);
	LocationVector LoginCoord;
	Instance * dest;
	ASSERT(!m_currentPlayer);
	uint64 guid;
	pck >> guid;

	if(sClientMgr.GetRPlayer((uint32)guid) != NULL)
	{
		data << uint8(CHAR_LOGIN_DUPLICATE_CHARACTER);
		SendPacket(&data);
		return;
	}

	m_currentPlayer = sClientMgr.CreateRPlayer((uint32)guid);
	RPlayerInfo * p = m_currentPlayer;

	/* Load player data */
	QueryResult * result = CharacterDatabase.Query("SELECT acct, name, level, guildid, positionX, positionY, zoneId, mapId, race, class, gender, instance_id, entrypointmap, entrypointx, entrypointy, entrypointz, entrypointo FROM characters WHERE guid = %u", guid);
	if(result)
	{
		Field * f = result->Fetch();
		p->AccountId = f[0].GetUInt32();
		p->Name = f[1].GetString();
		p->Level = f[2].GetUInt32();
		p->GuildId = f[3].GetUInt32();
		p->PositionX = f[4].GetFloat();
		p->PositionY = f[5].GetFloat();
		p->ZoneId = f[6].GetUInt32();
		p->MapId = f[7].GetUInt32();
		p->Race = f[8].GetUInt8();
		p->Class = f[9].GetUInt8();
		p->Gender = f[10].GetUInt8();
		p->Latency = m_latency;
		p->GMPermissions = m_GMPermissions;
		p->Account_Flags = m_accountFlags;
		p->InstanceId = f[11].GetUInt32();
		p->RecoveryMapId = f[12].GetUInt32();
		p->RecoveryPosition.ChangeCoords(f[13].GetFloat(), f[14].GetFloat(), f[15].GetFloat(), f[16].GetFloat());
		delete result;
	}
	else
	{
		data << uint8(CHAR_LOGIN_NO_CHARACTER);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	if(IS_MAIN_MAP(m_currentPlayer->MapId))
	{
		/* we're on a continent, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);		
	}
	else
	{
		/* we're in an instanced map, try to find the world server we're going to */
		dest = sClusterMgr.GetInstanceByInstanceId(m_currentPlayer->InstanceId);

		if(!dest)
		{
			/* our instance has been deleted or no longer valid */
			m_currentPlayer->MapId = m_currentPlayer->RecoveryMapId;
			LoginCoord = m_currentPlayer->RecoveryPosition;

			/* obtain instance */
			dest = sClusterMgr.GetInstanceByMapId(m_currentPlayer->MapId);
			if(dest)
			{
				data.SetOpcode(SMSG_NEW_WORLD);
                data << m_currentPlayer->MapId << m_currentPlayer->RecoveryPosition << float(0);
				SendPacket(&data);
				data.clear();
			}
		}
	}

	if(!dest ||
		!dest->Server)		// Shouldn't happen
	{
		/* world server is down */
		data << uint8(CHAR_LOGIN_NO_WORLD);
		SendPacket(&data);
		sClientMgr.DestroyRPlayerInfo((uint32)guid);
		m_currentPlayer = NULL;
		return;
	}

	/* log the player into that WS */
	data.SetOpcode(ISMSG_PLAYER_LOGIN);

	/* append info */
	data << uint32(guid) << uint32(dest->MapId) << uint32(dest->InstanceId);

	/* append the account information */
	data << uint32(m_accountId) << uint32(m_accountFlags) << uint32(m_sessionId)
		<< m_GMPermissions << m_accountName;

	dest->Server->SendPacket(&data);
	m_nextServer = dest->Server;
}

