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
#include "git_version.h"

LoginErrorCode VerifyName(const char* name, size_t nlen)
{
	const char* p;
	size_t i;

	static const char* bannedCharacters = "\t\v\b\f\a\n\r\\\"\'\? <>[](){}_=+-|/!@#$%^&*~`.,0123456789\0";
	static const char* allowedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if(sWorld.m_limitedNames)
	{
		if(nlen == 0)
			return E_CHAR_NAME_NO_NAME;
		else if(nlen < 2)
			return E_CHAR_NAME_TOO_SHORT;
		else if(nlen > 12)
			return E_CHAR_NAME_TOO_LONG;

		for(i = 0; i < nlen; ++i)
		{
			p = allowedCharacters;
			for(; *p != 0; ++p)
			{
				if(name[i] == *p)
					goto cont;
			}
			return E_CHAR_NAME_INVALID_CHARACTER;
		cont:
			continue;
		}
	}
	else
	{
		for(i = 0; i < nlen; ++i)
		{
			p = bannedCharacters;
			while(*p != 0 && name[i] != *p && name[i] != 0)
				++p;

			if(*p != 0)
				return E_CHAR_NAME_INVALID_CHARACTER;
		}
	}

	return E_CHAR_NAME_SUCCESS;
}

bool ChatHandler::HandleRenameAllCharacter(const char* args, WorldSession* m_session)
{
	uint32 uCount = 0;
	uint32 ts = getMSTime();
	QueryResult* result = CharacterDatabase.Query("SELECT guid, name FROM characters");
	if(result)
	{
		do
		{
			uint32 uGuid = result->Fetch()[0].GetUInt32();
			const char* pName = result->Fetch()[1].GetString();
			size_t szLen = strlen(pName);

			if(VerifyName(pName, szLen) != E_CHAR_NAME_SUCCESS)
			{
				LOG_DEBUG("renaming character %s, %u", pName, uGuid);
				Player* pPlayer = objmgr.GetPlayer(uGuid);
				if(pPlayer != NULL)
				{
					pPlayer->rename_pending = true;
					pPlayer->GetSession()->SystemMessage("Your character has had a force rename set, you will be prompted to rename your character at next login in conformance with server rules.");
				}

				CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 1 WHERE guid = %u", uGuid);
				++uCount;
			}

		}
		while(result->NextRow());
		delete result;
	}

	SystemMessage(m_session, "Procedure completed in %u ms. %u character(s) forced to rename.", getMSTime() - ts, uCount);
	return true;
}

void CapitalizeString(string & arg)
{
	if(arg.length() == 0) return;
	arg[0] = static_cast<char>(toupper(arg[0]));
	for(uint32 x = 1; x < arg.size(); ++x)
		arg[x] = static_cast<char>(tolower(arg[x]));
}

void WorldSession::CharacterEnumProc(QueryResult* result)
{
	struct player_item
	{
		uint32 displayid;
		uint8 invtype;
		uint32 enchantment; // added in 2.4
	};

	uint32 start_time = getMSTime();

	player_item items[23];
	int8 slot;
	uint32 i;
	ItemPrototype* proto;
	QueryResult* res;
	CreatureInfo* info = NULL;
	uint8 race;
	has_dk = false;
	_side = -1; // side should be set on every enumeration for safety

	uint32 numchar;

	if(result)
		numchar = result->GetRowCount();
	else
		numchar = 0;

	// should be more than enough.. 200 bytes per char..
	WorldPacket data(SMSG_CHAR_ENUM, 1 + numchar * 200);

	// parse m_characters and build a mighty packet of
	// characters to send to the client.
	data << uint8(numchar);

	if(result)
	{
		uint64 guid;
		uint8 Class;
		uint32 bytes2;
		uint32 flags;
		uint32 banned;
		Field* fields;
		uint32 petLevel = 0;
		do
		{
			fields = result->Fetch();

			guid = fields[0].GetUInt64();
			bytes2 = fields[6].GetUInt32();
			Class = fields[3].GetUInt8();
			flags = fields[17].GetUInt32();
			race = fields[2].GetUInt8();

			if(_side < 0)
			{
				// work out the side
				static uint8 sides[RACE_DRAENEI + 1] = {0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0};
				_side = sides[race];
			}

			// Death Knight starting information
			// Note: To change what level is required to make a dk change the >= 55 to something.
			// For example >=80 would require a level 80 to create a DK
			has_level_55_char = has_level_55_char || (fields[1].GetUInt8() >= 55);
			has_dk = has_dk || (Class == 6);

			/* build character enum, w0000t :p */
			data << uint64(guid);						//guid
			data << fields[7].GetString();				//name
			data << uint8(race);						//race
			data << uint8(Class);						//class
			data << uint8(fields[4].GetUInt8());		//gender
			data << uint32(fields[5].GetUInt32());		//PLAYER_BYTES
			data << uint8(bytes2 & 0xFF);				//facial hair
			data << uint8(fields[1].GetUInt8());		//Level
			data << uint32(fields[12].GetUInt32());		//zoneid
			data << uint32(fields[11].GetUInt32());		//Mapid
			data << float(fields[8].GetFloat());		//X
			data << float(fields[9].GetFloat());		//Y
			data << float(fields[10].GetFloat());		//Z
			data << uint32(fields[18].GetUInt32());		//GuildID

			banned = fields[13].GetUInt32();
			uint32 char_flags = 0;

			if(banned && (banned < 10 || banned > (uint32)UNIXTIME))
				char_flags |= 0x01000000;	//Character is banned
			if(fields[15].GetUInt32() != 0)
				char_flags |= 0x00002000;	//Character is dead
			if(flags & PLAYER_FLAG_NOHELM)
				char_flags |= 0x00000400;	//Helm not displayed
			if(flags & PLAYER_FLAG_NOCLOAK)
				char_flags |= 0x00000800;	//Cloak not displayed
			if(fields[16].GetUInt32() != 0)
				char_flags |= 0x00004000;	//Character has to be renamed before logging in

			data << uint32(char_flags);
			data << uint32(0);				//Character recustomization flags
			data << uint8(0);				//Unknown 3.2.0

			if(Class == WARLOCK || Class == HUNTER)
			{
				res = CharacterDatabase.Query("SELECT entry, level FROM playerpets WHERE ownerguid = %u AND MOD( active, 10 ) = 1 AND alive = TRUE;", Arcemu::Util::GUID_LOPART(guid));

				if(res)
				{
					petLevel = res->Fetch()[1].GetUInt32();
					info = CreatureNameStorage.LookupEntry(res->Fetch()[0].GetUInt32());
					delete res;
				}
				else
					info = NULL;
			}
			else
				info = NULL;

			if(info)
			{
				data << uint32(info->Male_DisplayID);
				data << uint32(petLevel);
				data << uint32(info->Family);
			}
			else
			{
				data << uint32(0);
				data << uint32(0);
				data << uint32(0);
			}

			res = CharacterDatabase.Query("SELECT slot, entry, enchantments FROM playeritems WHERE ownerguid=%u AND containerslot = '-1' AND slot BETWEEN '0' AND '22'", Arcemu::Util::GUID_LOPART(guid));

			memset(items, 0, sizeof(player_item) * 23);
			uint32 enchantid;
			EnchantEntry* enc;
			if(res)
			{
				do
				{
					slot = res->Fetch()[0].GetInt8();
					proto = ItemPrototypeStorage.LookupEntry(res->Fetch()[1].GetUInt32());
					if(proto)
					{
						items[slot].displayid = proto->DisplayInfoID;
						items[slot].invtype = static_cast<uint8>(proto->InventoryType);

						// weapon glows
						if(slot == EQUIPMENT_SLOT_MAINHAND || slot == EQUIPMENT_SLOT_OFFHAND)
						{
							// get enchant visual ID
							const char* enchant_field = res->Fetch()[2].GetString();
							if(sscanf(enchant_field , "%u,0,0;" , (unsigned int*)&enchantid) == 1 && enchantid > 0)
							{
								enc = dbcEnchant.LookupEntryForced(enchantid);
								if(enc != NULL)
									items[slot].enchantment = enc->visual;
							}
						}
					}
				}
				while(res->NextRow());
				delete res;
			}

			for(i = 0; i < INVENTORY_SLOT_BAG_END; ++i)
			{
				data << uint32(items[i].displayid);
				data << uint8(items[i].invtype);
				data << uint32(items[i].enchantment);
			}
		}
		while(result->NextRow());
	}

	Log.Debug("Character Enum", "Built in %u ms.", getMSTime() - start_time);
	SendPacket(&data);
}

void WorldSession::HandleCharEnumOpcode(WorldPacket & recv_data)
{
	AsyncQuery* q = new AsyncQuery(new SQLClassCallbackP1<World, uint32>(World::getSingletonPtr(), &World::CharacterEnumProc, GetAccountId()));
	q->AddQuery("SELECT guid, level, race, class, gender, bytes, bytes2, name, positionX, positionY, positionZ, mapId, zoneId, banned, restState, deathstate, forced_rename_pending, player_flags, guild_data.guildid FROM characters LEFT JOIN guild_data ON characters.guid = guild_data.playerid WHERE acct=%u ORDER BY guid LIMIT 10", GetAccountId());
	CharacterDatabase.QueueAsyncQuery(q);
}

void WorldSession::LoadAccountDataProc(QueryResult* result)
{
	size_t len;
	const char* data;
	char* d;

	if(!result)
	{
		CharacterDatabase.Execute("INSERT INTO account_data VALUES(%u, '', '', '', '', '', '', '', '', '')", _accountId);
		return;
	}

	for(uint32 i = 0; i < 7; ++i)
	{
		data = result->Fetch()[1 + i].GetString();
		len = data ? strlen(data) : 0;
		if(len > 1)
		{
			d = new char[len + 1];
			memcpy(d, data, len + 1);
			SetAccountData(i, d, true, (uint32)len);
		}
	}
}

void WorldSession::HandleCharCreateOpcode(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 10);
	std::string name;
	uint8 race, class_;

	recv_data >> name >> race >> class_;
	recv_data.rpos(0);

	LoginErrorCode res = VerifyName(name.c_str(), name.length());
	if(res != E_CHAR_NAME_SUCCESS)
	{
		OutPacket(SMSG_CHAR_CREATE, 1, &res);
		return;
	}

	res = g_characterNameFilter->Parse(name, false) ? E_CHAR_NAME_PROFANE : E_CHAR_NAME_SUCCESS;
	if(res != E_CHAR_NAME_SUCCESS)
	{
		OutPacket(SMSG_CHAR_CREATE, 1, &res);
		return;
	}

	res = objmgr.GetPlayerInfoByName(name.c_str()) == NULL ? E_CHAR_CREATE_SUCCESS : E_CHAR_CREATE_NAME_IN_USE;
	if(res != E_CHAR_CREATE_SUCCESS)
	{
		OutPacket(SMSG_CHAR_CREATE, 1, &res);
		return;
	}

	res = sHookInterface.OnNewCharacter(race, class_, this, name.c_str()) ? E_CHAR_CREATE_SUCCESS : E_CHAR_CREATE_ERROR;
	if(res != E_CHAR_CREATE_SUCCESS)
	{
		OutPacket(SMSG_CHAR_CREATE, 1, &res);
		return;
	}

	QueryResult* result = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			OutPacket(SMSG_CHAR_CREATE, 1, CHAR_NAME_PROFANE);
			delete result;
			return;
		}
		delete result;
	}

	// Check if player got Death Knight already on this realm.
	if(Config.OptionalConfig.GetBoolDefault("ClassOptions" , "DeathKnightLimit" , true) && has_dk
	        && (class_ == DEATHKNIGHT))
	{
		OutPacket(SMSG_CHAR_CREATE, 1, CHAR_CREATE_UNIQUE_CLASS_LIMIT);
		return;
	}

	// loading characters

	// Check the number of characters, so we can't make over 10.
	// They're able to manage to create >10 sometimes, not exactly sure how ..

	result = CharacterDatabase.Query("SELECT COUNT(*) FROM characters WHERE acct = %u", GetAccountId());
	if(result)
	{
		if(result->Fetch()[0].GetUInt32() >= 10)
		{
			// We can't make any more characters.
			OutPacket(SMSG_CHAR_CREATE, 1, CHAR_CREATE_SERVER_LIMIT);
			delete result;
			return;
		}
		delete result;
	}

	Player* pNewChar = objmgr.CreatePlayer(class_);
	pNewChar->SetSession(this);
	if(!pNewChar->Create(recv_data))
	{
		// failed.
		pNewChar->ok_to_remove = true;
		delete pNewChar;
		OutPacket(SMSG_CHAR_CREATE, 1, CHAR_CREATE_FAILED);
		return;
	}

	//Same Faction limitation only applies to PVP and RPPVP realms :)
	uint32 realmType = sLogonCommHandler.GetRealmType();
	if(!HasGMPermissions() && realmType == REALMTYPE_PVP && _side >= 0 && !sWorld.crossover_chars)  // ceberwow fixed bug
	{
		if((pNewChar->IsTeamAlliance() && (_side == 1)) || (pNewChar->IsTeamHorde() && (_side == 0)))
		{
			pNewChar->ok_to_remove = true;
			delete pNewChar;
			OutPacket(SMSG_CHAR_CREATE, 1, CHAR_CREATE_PVP_TEAMS_VIOLATION);
			return;
		}
	}

	//Check if player has a level 55 or higher character on this realm and allow him to create DK.
	//This check can be turned off in optional.conf
	if(Config.OptionalConfig.GetBoolDefault("ClassOptions" , "DeathKnightPreReq" , false) && !has_level_55_char
	        && (class_ == DEATHKNIGHT))
	{
		pNewChar->ok_to_remove = true;
		delete pNewChar;
		/*
		WorldPacket data(1);
		data.SetOpcode(SMSG_CHAR_CREATE);
		data << (uint8)56 + 1; // This errorcode is not the actual one. Need to find a real error code.
		SendPacket( &data );
		*/
		OutPacket(SMSG_CHAR_CREATE, 1, CHAR_CREATE_LEVEL_REQUIREMENT);
		return;
	}

	pNewChar->UnSetBanned();
	pNewChar->addSpell(22027);	  // Remove Insignia

	if(pNewChar->getClass() == WARLOCK)
	{
		pNewChar->AddSummonSpell(416, 3110);		// imp fireball
		pNewChar->AddSummonSpell(417, 19505);
		pNewChar->AddSummonSpell(1860, 3716);
		pNewChar->AddSummonSpell(1863, 7814);
	}

	pNewChar->SaveToDB(true);

	PlayerInfo* pn = new PlayerInfo ;
	pn->guid = pNewChar->GetLowGUID();
	pn->name = strdup(pNewChar->GetName());
	pn->cl = pNewChar->getClass();
	pn->race = pNewChar->getRace();
	pn->gender = pNewChar->getGender();
	pn->acct = GetAccountId();
	pn->m_Group = 0;
	pn->subGroup = 0;
	pn->m_loggedInPlayer = NULL;
	pn->team = pNewChar->GetTeam();
	pn->guild = NULL;
	pn->guildRank = NULL;
	pn->guildMember = NULL;
	pn->lastOnline = UNIXTIME;
	objmgr.AddPlayerInfo(pn);

	pNewChar->ok_to_remove = true;
	delete  pNewChar;

	OutPacket(SMSG_CHAR_CREATE, 1, CHAR_CREATE_SUCCESS);

	sLogonCommHandler.UpdateAccountCount(GetAccountId(), 1);
}

void WorldSession::HandleCharDeleteOpcode(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint8 fail = E_CHAR_DELETE_SUCCESS;

	uint64 guid;
	recv_data >> guid;

	if(objmgr.GetPlayer((uint32)guid) != NULL)
	{
		// "Char deletion failed"
		fail = E_CHAR_DELETE_FAILED;
	}
	else
	{
		fail = DeleteCharacter((uint32)guid);
	}
	OutPacket(SMSG_CHAR_DELETE, 1, &fail);
}

uint8 WorldSession::DeleteCharacter(uint32 guid)
{
	PlayerInfo* inf = objmgr.GetPlayerInfo(guid);
	if(inf != NULL && inf->m_loggedInPlayer == NULL)
	{
		QueryResult* result = CharacterDatabase.Query("SELECT name FROM characters WHERE guid = %u AND acct = %u", (uint32)guid, _accountId);
		if(!result)
			return E_CHAR_DELETE_FAILED;

		string name = result->Fetch()[0].GetString();
		delete result;

		if(inf->guild)
		{
			if(inf->guild->GetGuildLeader() == inf->guid)
				return E_CHAR_DELETE_FAILED_GUILD_LEADER;
			else
				inf->guild->RemoveGuildMember(inf, NULL);
		}

		for(int i = 0; i < NUM_CHARTER_TYPES; ++i)
		{
			Charter* c = objmgr.GetCharterByGuid(guid, (CharterTypes)i);
			if(c != NULL)
				c->RemoveSignature((uint32)guid);
		}


		for(int i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
		{
			ArenaTeam* t = objmgr.GetArenaTeamByGuid((uint32)guid, i);
			if(t != NULL && t->m_leader == guid)
				return E_CHAR_DELETE_FAILED_ARENA_CAPTAIN;
			if(t != NULL)
				t->RemoveMember(inf);
		}

		/*if( _socket != NULL )
			sPlrLog.write("Account: %s | IP: %s >> Deleted player %s", GetAccountName().c_str(), GetSocket()->GetRemoteIP().c_str(), name.c_str());*/

		sPlrLog.writefromsession(this, "deleted character %s (GUID: %u)", name.c_str(), (uint32)guid);

		CharacterDatabase.WaitExecute("DELETE FROM characters WHERE guid = %u", (uint32)guid);

		Corpse* c = objmgr.GetCorpseByOwner((uint32)guid);
		if(c)
			CharacterDatabase.Execute("DELETE FROM corpses WHERE guid = %u", c->GetLowGUID());

		CharacterDatabase.Execute("DELETE FROM playeritems WHERE ownerguid=%u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE playerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM tutorials WHERE playerId = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM mailbox WHERE player_guid = %u", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u OR friend_guid = %u", (uint32)guid, (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u OR ignore_guid = %u", (uint32)guid, (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM character_achievement WHERE guid = '%u' AND achievement NOT IN (457, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 1404, 1405, 1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 1415, 1414, 1416, 1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1463, 1400, 456, 1402)", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM character_achievement_progress WHERE guid = '%u'", (uint32)guid);
		CharacterDatabase.Execute("DELETE FROM playerspells WHERE GUID = '%u'", guid);
		CharacterDatabase.Execute("DELETE FROM playerdeletedspells WHERE GUID = '%u'", guid);
		CharacterDatabase.Execute("DELETE FROM playerreputations WHERE guid = '%u'", guid);
		CharacterDatabase.Execute("DELETE FROM playerskills WHERE GUID = '%u'", guid);

		/* remove player info */
		objmgr.DeletePlayerInfo((uint32)guid);
		return E_CHAR_DELETE_SUCCESS;
	}
	return E_CHAR_DELETE_FAILED;
}

void WorldSession::HandleCharRenameOpcode(WorldPacket & recv_data)
{
	WorldPacket data(SMSG_CHAR_RENAME, recv_data.size() + 1);

	uint64 guid;
	string name;
	recv_data >> guid >> name;

	PlayerInfo* pi = objmgr.GetPlayerInfo((uint32)guid);
	if(pi == 0) return;

	QueryResult* result = CharacterDatabase.Query("SELECT forced_rename_pending FROM characters WHERE guid = %u AND acct = %u",
	                      (uint32)guid, _accountId);
	if(result == 0)
	{
		delete result;
		return;
	}
	delete result;

	// Check name for rule violation.

	LoginErrorCode err = VerifyName(name.c_str(), name.length());
	if(err != E_CHAR_NAME_SUCCESS)
	{
		data << uint8(err);
		data << guid << name;
		SendPacket(&data);
		return;
	}

	QueryResult* result2 = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(name).c_str());
	if(result2)
	{
		if(result2->Fetch()[0].GetUInt32() > 0)
		{
			// That name is banned!
			data << uint8(E_CHAR_NAME_PROFANE);
			data << guid << name;
			SendPacket(&data);
		}
		delete result2;
	}

	// Check if name is in use.
	if(objmgr.GetPlayerInfoByName(name.c_str()) != NULL)
	{
		data << uint8(E_CHAR_CREATE_NAME_IN_USE);
		data << guid << name;
		SendPacket(&data);
		return;
	}

	// correct capitalization
	CapitalizeString(name);
	objmgr.RenamePlayerInfo(pi, pi->name, name.c_str());

	sPlrLog.writefromsession(this, "a rename was pending. renamed character %s (GUID: %u) to %s.", pi->name, pi->guid, name.c_str());

	// If we're here, the name is okay.
	free(pi->name);
	pi->name = strdup(name.c_str());
	CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", name.c_str(), (uint32)guid);
	CharacterDatabase.WaitExecute("UPDATE characters SET forced_rename_pending = 0 WHERE guid = %u", (uint32)guid);

	data << uint8(E_RESPONSE_SUCCESS) << guid << name;
	SendPacket(&data);
}


void WorldSession::HandlePlayerLoginOpcode(WorldPacket & recv_data)
{
	CHECK_PACKET_SIZE(recv_data, 8);
	uint64 playerGuid = 0;

	LOG_DEBUG("WORLD: Recvd Player Logon Message");

	recv_data >> playerGuid; // this is the GUID selected by the player
	if(objmgr.GetPlayer((uint32)playerGuid) != NULL || m_loggingInPlayer || _player)
	{
		// A character with that name already exists 0x3E
		uint8 respons = E_CHAR_LOGIN_DUPLICATE_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}

	AsyncQuery* q = new AsyncQuery(new SQLClassCallbackP0<WorldSession>(this, &WorldSession::LoadPlayerFromDBProc));
	q->AddQuery("SELECT guid,class FROM characters WHERE guid = %u AND forced_rename_pending = 0", playerGuid); // 0
	CharacterDatabase.QueueAsyncQuery(q);
}

void WorldSession::LoadPlayerFromDBProc(QueryResultVector & results)
{
	if(results.size() < 1)
	{
		uint8 respons = E_CHAR_LOGIN_NO_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}

	QueryResult* result = results[0].result;
	if(! result)
	{
		Log.Error("WorldSession::LoadPlayerFromDBProc", "Player login query failed!");
		uint8 respons = E_CHAR_LOGIN_NO_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}

	Field* fields = result->Fetch();

	uint64 playerGuid = fields[0].GetUInt64();
	uint8 _class = fields[1].GetUInt8();

	Player* plr = NULL;

	switch(_class)
	{
		case WARRIOR:
			plr = new Warrior(playerGuid);
			break;
		case PALADIN:
			plr = new Paladin(playerGuid);
			break;
		case HUNTER:
			plr = new Hunter(playerGuid);
			break;
		case ROGUE:
			plr = new Rogue(playerGuid);
			break;
		case PRIEST:
			plr = new Priest(playerGuid);
			break;
		case DEATHKNIGHT:
			plr = new DeathKnight(playerGuid);
			break;
		case SHAMAN:
			plr = new Shaman(playerGuid);
			break;
		case MAGE:
			plr = new Mage(playerGuid);
			break;
		case WARLOCK:
			plr = new Warlock(playerGuid);
			break;
		case DRUID:
			plr = new Druid(playerGuid);
			break;
	}

	if(plr == NULL)
	{
		Log.Error("WorldSession::LoadPlayerFromDBProc", "Class %u unknown!", _class);
		uint8 respons = E_CHAR_LOGIN_NO_CHARACTER;
		OutPacket(SMSG_CHARACTER_LOGIN_FAILED, 1, &respons);
		return;
	}

	plr->SetSession(this);

	m_bIsWLevelSet = false;

	Log.Debug("WorldSession", "Async loading player %u", (uint32)playerGuid);
	m_loggingInPlayer = plr;
	plr->LoadFromDB((uint32)playerGuid);
}

void WorldSession::FullLogin(Player* plr)
{
	Log.Debug("WorldSession", "Fully loading player %u", plr->GetLowGUID());

	SetPlayer(plr);
	m_MoverWoWGuid.Init(plr->GetGUID());

	MapMgr* mgr = sInstanceMgr.GetInstance(plr);
	if(mgr && mgr->m_battleground)
	{
		// Don't allow player to login into a bg that has ended or is full
		if(mgr->m_battleground->HasEnded() == true ||
		        mgr->m_battleground->HasFreeSlots(plr->GetTeamInitial(), mgr->m_battleground->GetType() == false))
		{
			mgr = NULL;
		}
	}

	// Trying to log to an instance that doesn't exists anymore?
	if(!mgr)
	{
		if(!IS_INSTANCE(plr->m_bgEntryPointMap))
		{
			plr->m_position.x = plr->m_bgEntryPointX;
			plr->m_position.y = plr->m_bgEntryPointY;
			plr->m_position.z = plr->m_bgEntryPointZ;
			plr->m_position.o = plr->m_bgEntryPointO;
			plr->m_mapId = plr->m_bgEntryPointMap;
		}
		else
		{
			plr->m_position.x = plr->GetBindPositionX();
			plr->m_position.y = plr->GetBindPositionY();
			plr->m_position.z = plr->GetBindPositionZ();
			plr->m_position.o = 0;
			plr->m_mapId = plr->GetBindMapId();
		}
	}

	// copy to movement array
	movement_packet[0] = m_MoverWoWGuid.GetNewGuidMask();
	memcpy(&movement_packet[1], m_MoverWoWGuid.GetNewGuid(), m_MoverWoWGuid.GetNewGuidLen());

	// world preload
	uint32 VMapId;
	float VO;
	float VX;
	float VY;
	float VZ;

	// GMs should start on GM Island and be bound there
	if(HasGMPermissions() && plr->m_FirstLogin && sWorld.gamemaster_startonGMIsland)
	{
		VMapId = 1;
		VO = 0;
		VX = 16222.6f;
		VY = 16265.9f;
		VZ = 14.2085f;

		plr->m_position.x = VX;
		plr->m_position.y = VY;
		plr->m_position.z = VZ;
		plr->m_position.o = VO;
		plr->m_mapId = VMapId;

		plr->SetBindPoint(plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), plr->GetMapId(), plr->GetZoneId());
	}
	else
	{
		VMapId = plr->GetMapId();
		VO = plr->GetOrientation();
		VX = plr->GetPositionX();
		VY = plr->GetPositionY();
		VZ = plr->GetPositionZ();
	}

	plr->SendLoginVerifyWorld(VMapId, VX, VY, VZ, VO);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// send voicechat state - active/inactive
	//
	// {SERVER} Packet: (0x03C7) UNKNOWN PacketSize = 2
	// |------------------------------------------------|----------------|
	// |00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |0123456789ABCDEF|
	// |------------------------------------------------|----------------|
	// |02 01							                |..              |
	// -------------------------------------------------------------------
	//
	//
	// Old packetdump is OLD. This is probably from 2.2.0 (that was the patch when it was added to wow)!
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////


	StackWorldPacket<20> datab(SMSG_FEATURE_SYSTEM_STATUS);


	datab.Initialize(SMSG_FEATURE_SYSTEM_STATUS);

	datab << uint8(2);
	datab << uint8(0);

	SendPacket(&datab);

	WorldPacket dataldm(SMSG_LEARNED_DANCE_MOVES, 4 + 4);

	dataldm << uint32(0);
	dataldm << uint32(0);

	SendPacket(&dataldm);

	plr->UpdateAttackSpeed();

	// Make sure our name exists (for premade system)
	PlayerInfo* info = objmgr.GetPlayerInfo(plr->GetLowGUID());

	if(info == NULL)
	{
		info = new PlayerInfo;
		info->cl = plr->getClass();
		info->gender = plr->getGender();
		info->guid = plr->GetLowGUID();
		info->name = strdup(plr->GetName());
		info->lastLevel = plr->getLevel();
		info->lastOnline = UNIXTIME;
		info->lastZone = plr->GetZoneId();
		info->race = plr->getRace();
		info->team = plr->GetTeam();
		info->guild = NULL;
		info->guildRank = NULL;
		info->guildMember = NULL;
		info->m_Group = NULL;
		info->subGroup = 0;
		objmgr.AddPlayerInfo(info);
	}
	plr->m_playerInfo = info;
	if(plr->m_playerInfo->guild)
	{
		plr->SetGuildId(plr->m_playerInfo->guild->GetGuildId());
		plr->SetGuildRank(plr->m_playerInfo->guildRank->iId);
	}

	info->m_loggedInPlayer = plr;

	// account data == UI config
	SendAccountDataTimes(PER_CHARACTER_CACHE_MASK);

	// Set TIME OF LOGIN
	CharacterDatabase.Execute("UPDATE characters SET online = 1 WHERE guid = %u" , plr->GetLowGUID());

	bool enter_world = true;

	// Find our transporter and add us if we're on one.
	if(plr->transporter_info.guid != 0)
	{
		Transporter* pTrans = objmgr.GetTransporter(Arcemu::Util::GUID_LOPART(plr->transporter_info.guid));
		if(pTrans)
		{
			if(plr->IsDead())
			{
				plr->ResurrectPlayer();
				plr->SetHealth(plr->GetMaxHealth());
				plr->SetPower(POWER_TYPE_MANA, plr->GetMaxPower(POWER_TYPE_MANA));
			}

			float c_tposx = pTrans->GetPositionX() + plr->transporter_info.x;
			float c_tposy = pTrans->GetPositionY() + plr->transporter_info.y;
			float c_tposz = pTrans->GetPositionZ() + plr->transporter_info.z;

			if(plr->GetMapId() != pTrans->GetMapId())	   // loaded wrong map
			{
				plr->SetMapId(pTrans->GetMapId());

				StackWorldPacket<20> dataw(SMSG_NEW_WORLD);

				dataw << pTrans->GetMapId();
				dataw << c_tposx;
				dataw << c_tposy;
				dataw << c_tposz;
				dataw << plr->GetOrientation();

				SendPacket(&dataw);

				// shit is sent in worldport ack.
				enter_world = false;
			}

			plr->SetPosition(c_tposx, c_tposy, c_tposz, plr->GetOrientation(), false);
			plr->m_CurrentTransporter = pTrans;
			pTrans->AddPlayer(plr);
		}
	}

	Log.Debug("Login", "Player %s logged in.", plr->GetName());

	sWorld.incrementPlayerCount(plr->GetTeam());

	if(plr->m_FirstLogin)
	{
		uint32 introid = plr->info->introid;

		OutPacket(SMSG_TRIGGER_CINEMATIC, 4, &introid);

		if(sWorld.m_AdditionalFun)    //cebernic: tells people who 's newbie :D
		{
			const int classtext[] = {0, 5, 6, 8, 9, 11, 0, 4, 3, 7, 0, 10};
			sWorld.SendLocalizedWorldText(true, "{65}", classtext[(uint32)plr->getClass() ] , plr->GetName() , (plr->IsTeamHorde() ? "{63}" : "{64}"));
		}

	}


	LOG_DETAIL("WORLD: Created new player for existing players (%s)", plr->GetName());

	// Login time, will be used for played time calc
	plr->m_playedtime[2] = uint32(UNIXTIME);

	//Issue a message telling all guild members that this player has signed on
	if(plr->IsInGuild())
	{
		Guild* pGuild = plr->m_playerInfo->guild;
		if(pGuild)
		{
			WorldPacket data(SMSG_GUILD_EVENT, 50);

			data << uint8(GUILD_EVENT_MOTD);
			data << uint8(1);

			if(pGuild->GetMOTD())
				data << pGuild->GetMOTD();
			else
				data << uint8(0);

			SendPacket(&data);

			pGuild->LogGuildEvent(GUILD_EVENT_HASCOMEONLINE, 1, plr->GetName());
		}
	}

	// Send online status to people having this char in friendlist
	_player->Social_TellFriendsOnline();
	// send friend list (for ignores)
	_player->Social_SendFriendList(7);

	plr->SendDungeonDifficulty();
	plr->SendRaidDifficulty();

	plr->SendEquipmentSetList();

#ifndef GM_TICKET_MY_MASTER_COMPATIBLE
	GM_Ticket* ticket = objmgr.GetGMTicketByPlayer(_player->GetGUID());
	if(ticket != NULL)
	{
		//Send status change to gm_sync_channel
		Channel* chn = channelmgr.GetChannel(sWorld.getGmClientChannel().c_str(), _player);
		if(chn)
		{
			std::stringstream ss;
			ss << "GmTicket:" << GM_TICKET_CHAT_OPCODE_ONLINESTATE;
			ss << ":" << ticket->guid;
			ss << ":1";
			chn->Say(_player, ss.str().c_str(), NULL, true);
		}
	}
#endif

	// server Message Of The Day
	SendMOTD();

	//Set current RestState
	if(plr->m_isResting)
		// We are resting at an inn , turn on Zzz
		plr->ApplyPlayerRestState(true);

	//Calculate rest bonus if there is time between lastlogoff and now
	if(plr->m_timeLogoff > 0 && plr->getLevel() < plr->GetMaxLevel())	// if timelogoff = 0 then it's the first login
	{
		uint32 currenttime = uint32(UNIXTIME);
		uint32 timediff = currenttime - plr->m_timeLogoff;

		//Calculate rest bonus
		if(timediff > 0)
			plr->AddCalculatedRestXP(timediff);
	}

	if(info->m_Group)
		info->m_Group->Update();

	if(enter_world && !_player->GetMapMgr())
		plr->AddToWorld();

	sHookInterface.OnFullLogin(_player);

	objmgr.AddPlayer(_player);

}

bool ChatHandler::HandleRenameCommand(const char* args, WorldSession* m_session)
{
	// prevent buffer overflow
	if(strlen(args) > 100)
		return false;

	char name1[100];
	char name2[100];

	if(sscanf(args, "%s %s", name1, name2) != 2)
		return false;

	if(VerifyName(name2, strlen(name2)) != E_CHAR_NAME_SUCCESS)
	{
		RedSystemMessage(m_session, "That name is invalid or contains invalid characters.");
		return true;
	}

	string new_name = name2;
	PlayerInfo* pi = objmgr.GetPlayerInfoByName(name1);
	if(pi == 0)
	{
		RedSystemMessage(m_session, "Player not found with this name.");
		return true;
	}

	if(objmgr.GetPlayerInfoByName(new_name.c_str()) != NULL)
	{
		RedSystemMessage(m_session, "Player found with this name in use already.");
		return true;
	}

	objmgr.RenamePlayerInfo(pi, pi->name, new_name.c_str());

	free(pi->name);
	pi->name = strdup(new_name.c_str());

	// look in world for him
	Player* plr = objmgr.GetPlayer(pi->guid);
	if(plr != 0)
	{
		plr->SetName(new_name);
		BlueSystemMessageToPlr(plr, "%s changed your name to '%s'.", m_session->GetPlayer()->GetName(), new_name.c_str());
		plr->SaveToDB(false);
	}
	else
	{
		CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", CharacterDatabase.EscapeString(new_name).c_str(), (uint32)pi->guid);
	}

	GreenSystemMessage(m_session, "Changed name of '%s' to '%s'.", name1, name2);
	sGMLog.writefromsession(m_session, "renamed character %s (GUID: %u) to %s", name1, pi->guid, name2);
	sPlrLog.writefromsession(m_session, "GM renamed character %s (GUID: %u) to %s", name1, pi->guid, name2);
	return true;
}
