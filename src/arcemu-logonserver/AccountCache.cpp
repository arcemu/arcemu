/*
 * ArcEmu MMORPG Server
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

#include "LogonStdAfx.h"
initialiseSingleton(AccountMgr);
initialiseSingleton(IPBanner);
initialiseSingleton(InformationCore);

void AccountMgr::ReloadAccounts(bool silent)
{
	setBusy.Acquire();
	if(!silent) sLog.outString("[AccountMgr] Reloading Accounts...");

	// Load *all* accounts.
	QueryResult* result = sLogonSQL->Query("SELECT acct, login, password, encrypted_password, gm, flags, banned, forceLanguage, muted FROM accounts");
	Field* field;
	string AccountName;
	set<string> account_list;
	Account* acct;

	if(result)
	{
		do
		{
			field = result->Fetch();
			AccountName = field[1].GetString();

			// transform to uppercase
			arcemu_TOUPPER(AccountName);

			//Use private __GetAccount, for locks
			acct = __GetAccount(AccountName);
			if(acct == 0)
			{
				// New account.
				AddAccount(field);
			}
			else
			{
				// Update the account with possible changed details.
				UpdateAccount(acct, field);
			}

			// add to our "known" list
			account_list.insert(AccountName);

		}
		while(result->NextRow());

		delete result;
	}

	// check for any purged/deleted accounts
#ifdef WIN32
	HM_NAMESPACE::hash_map<string, Account*>::iterator itr = AccountDatabase.begin();
	HM_NAMESPACE::hash_map<string, Account*>::iterator it2;
#else
	std::map<string, Account*>::iterator itr = AccountDatabase.begin();
	std::map<string, Account*>::iterator it2;
#endif

	for(; itr != AccountDatabase.end();)
	{
		it2 = itr;
		++itr;

		if(account_list.find(it2->first) == account_list.end())
		{
			delete it2->second;
			AccountDatabase.erase(it2);
		}
		else
		{
			it2->second->UsernamePtr = (std::string*)&it2->first;
		}
	}

	if(!silent) sLog.outString("[AccountMgr] Found %u accounts.", AccountDatabase.size());
	setBusy.Release();

	IPBanner::getSingleton().Reload();
}

void AccountMgr::AddAccount(Field* field)
{
	Account* acct = new Account;
	Sha1Hash hash;
	string Username     = field[1].GetString();
	string Password	    = field[2].GetString();
	string EncryptedPassword = field[3].GetString();
	string GMFlags		= field[4].GetString();

	acct->AccountId				= field[0].GetUInt32();
	acct->AccountFlags			= field[5].GetUInt8();
	acct->Banned				= field[6].GetUInt32();
	if((uint32)UNIXTIME > acct->Banned && acct->Banned != 0 && acct->Banned != 1)   //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Banned = 0;
		//me go boom :(
		//printf("Account %s's ban has expired.\n",acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET banned = 0 WHERE acct=%u", acct->AccountId);
	}
	acct->SetGMFlags(GMFlags.c_str());
	acct->Locale[0] = 'e';
	acct->Locale[1] = 'n';
	acct->Locale[2] = 'U';
	acct->Locale[3] = 'S';
	if(strcmp(field[7].GetString(), "enUS"))
	{
		// non-standard language forced
		memcpy(acct->Locale, field[7].GetString(), 4);
		acct->forcedLocale = true;
	}
	else
		acct->forcedLocale = false;

	acct->Muted = field[8].GetUInt32();
	if((uint32)UNIXTIME > acct->Muted && acct->Muted != 0 && acct->Muted != 1)   //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Muted = 0;
		//LOG_DEBUG("Account %s's mute has expired.",acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET muted = 0 WHERE acct=%u", acct->AccountId);
	}
	// Convert username/password to uppercase. this is needed ;)
	arcemu_TOUPPER(Username);
	arcemu_TOUPPER(Password);

	// prefer encrypted passwords over nonencrypted
	if(EncryptedPassword.size() > 0)
	{
		if(EncryptedPassword.size() == 40)
		{
			BigNumber bn;
			bn.SetHexStr(EncryptedPassword.c_str());
			if(bn.GetNumBytes() < 20)
			{
				// Hacky fix
				memcpy(acct->SrpHash, bn.AsByteArray(), bn.GetNumBytes());
				for(int n = bn.GetNumBytes(); n <= 19; n++)
					acct->SrpHash[n] = (uint8)0;
				reverse_array(acct->SrpHash, 20);
			}
			else
			{
				memcpy(acct->SrpHash, bn.AsByteArray(), 20);
				reverse_array(acct->SrpHash, 20);
			}
		}
		else
		{
			LOG_ERROR("Account `%s` has incorrect number of bytes in encrypted password! Disabling.", Username.c_str());
			memset(acct->SrpHash, 0, 20);
		}
	}
	else
	{
		// Prehash the I value.
		hash.UpdateData((Username + ":" + Password));
		hash.Finalize();
		memcpy(acct->SrpHash, hash.GetDigest(), 20);
	}

	AccountDatabase[Username] = acct;
}

void AccountMgr::UpdateAccount(Account* acct, Field* field)
{
	uint32 id = field[0].GetUInt32();
	Sha1Hash hash;
	string Username     = field[1].GetString();
	string Password	    = field[2].GetString();
	string EncryptedPassword = field[3].GetString();
	string GMFlags		= field[4].GetString();

	if(id != acct->AccountId)
	{
		LOG_ERROR(" >> deleting duplicate account %u [%s]...", id, Username.c_str());
		sLogonSQL->Execute("DELETE FROM accounts WHERE acct=%u", id);
		return;
	}

	acct->AccountId				= field[0].GetUInt32();
	acct->AccountFlags			= field[5].GetUInt8();
	acct->Banned				= field[6].GetUInt32();
	if((uint32)UNIXTIME > acct->Banned && acct->Banned != 0 && acct->Banned != 1)  //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Banned = 0;
		LOG_DEBUG("Account %s's ban has expired.", acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET banned = 0 WHERE acct=%u", acct->AccountId);
	}
	acct->SetGMFlags(GMFlags.c_str());
	if(strcmp(field[7].GetString(), "enUS"))
	{
		// non-standard language forced
		memcpy(acct->Locale, field[7].GetString(), 4);
		acct->forcedLocale = true;
	}
	else
		acct->forcedLocale = false;

	acct->Muted = field[8].GetUInt32();
	if((uint32)UNIXTIME > acct->Muted && acct->Muted != 0 && acct->Muted != 1)  //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Muted = 0;
		LOG_DEBUG("Account %s's mute has expired.", acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET muted = 0 WHERE acct=%u", acct->AccountId);
	}
	// Convert username/password to uppercase. this is needed ;)
	arcemu_TOUPPER(Username);
	arcemu_TOUPPER(Password);

	// prefer encrypted passwords over nonencrypted
	if(EncryptedPassword.size() > 0)
	{
		if(EncryptedPassword.size() == 40)
		{
			BigNumber bn;
			bn.SetHexStr(EncryptedPassword.c_str());
			if(bn.GetNumBytes() < 20)
			{
				// Hacky fix
				memcpy(acct->SrpHash, bn.AsByteArray(), bn.GetNumBytes());
				for(int n = bn.GetNumBytes(); n <= 19; n++)
					acct->SrpHash[n] = (uint8)0;
				reverse_array(acct->SrpHash, 20);
			}
			else
			{
				memcpy(acct->SrpHash, bn.AsByteArray(), 20);
				reverse_array(acct->SrpHash, 20);
			}
		}
		else
		{
			LOG_ERROR("Account `%s` has incorrect number of bytes in encrypted password! Disabling.", Username.c_str());
			memset(acct->SrpHash, 0, 20);
		}
	}
	else
	{
		// Prehash the I value.
		hash.UpdateData((Username + ":" + Password));
		hash.Finalize();
		memcpy(acct->SrpHash, hash.GetDigest(), 20);
	}
}

void AccountMgr::ReloadAccountsCallback()
{
	ReloadAccounts(true);
}
BAN_STATUS IPBanner::CalculateBanStatus(in_addr ip_address)
{
	Guard lguard(listBusy);
	list<IPBan>::iterator itr;
	list<IPBan>::iterator itr2 = banList.begin();
	for(; itr2 != banList.end();)
	{
		itr = itr2;
		++itr2;

		if(ParseCIDRBan(ip_address.s_addr, itr->Mask, itr->Bytes))
		{
			// ban hit
			if(itr->Expire == 0)
				return BAN_STATUS_PERMANENT_BAN;

			if((uint32)UNIXTIME >= itr->Expire)
			{
				sLogonSQL->Execute("DELETE FROM ipbans WHERE expire = %u AND ip = \"%s\"", itr->Expire, sLogonSQL->EscapeString(itr->db_ip).c_str());
				banList.erase(itr);
			}
			else
			{
				return BAN_STATUS_TIME_LEFT_ON_BAN;
			}
		}
	}

	return BAN_STATUS_NOT_BANNED;
}

bool IPBanner::Add(const char* ip, uint32 dur)
{
	string sip = string(ip);

	string::size_type i = sip.find("/");
	if(i == string::npos)
		return false;

	string stmp = sip.substr(0, i);
	string smask = sip.substr(i + 1);

	unsigned int ipraw = MakeIP(stmp.c_str());
	unsigned int ipmask = atoi(smask.c_str());
	if(ipraw == 0 || ipmask == 0)
		return false;

	IPBan ipb;
	ipb.db_ip = sip;
	ipb.Bytes = static_cast<unsigned char>(ipmask);
	ipb.Mask = ipraw;
	ipb.Expire = dur;

	listBusy.Acquire();
	banList.push_back(ipb);
	listBusy.Release();

	return true;
}

InformationCore::~InformationCore()
{
	for(map<uint32, Realm*>::iterator itr = m_realms.begin(); itr != m_realms.end(); ++itr)
		delete itr->second;
}

bool IPBanner::Remove(const char* ip)
{
	listBusy.Acquire();
	for(list<IPBan>::iterator itr = banList.begin(); itr != banList.end(); ++itr)
	{
		if(!strcmp(ip, itr->db_ip.c_str()))
		{
			banList.erase(itr);
			listBusy.Release();
			return true;
		}
	}
	listBusy.Release();
	return false;
}

void IPBanner::Reload()
{

	listBusy.Acquire();
	banList.clear();
	QueryResult* result = sLogonSQL->Query("SELECT ip, expire FROM ipbans");
	if(result != NULL)
	{
		do
		{
			IPBan ipb;
			string smask = "32";
			string ip = result->Fetch()[0].GetString();
			string::size_type i = ip.find("/");
			string stmp = ip.substr(0, i);
			if(i == string::npos)
			{
				LOG_DETAIL("IP ban \"%s\" netmask not specified. assuming /32", ip.c_str());
			}
			else
				smask = ip.substr(i + 1);

			unsigned int ipraw = MakeIP(stmp.c_str());
			unsigned int ipmask = atoi(smask.c_str());
			if(ipraw == 0 || ipmask == 0)
			{
				LOG_ERROR("IP ban \"%s\" could not be parsed. Ignoring", ip.c_str());
				continue;
			}

			ipb.Bytes = static_cast<unsigned char>(ipmask);
			ipb.Mask = ipraw;
			ipb.Expire = result->Fetch()[1].GetUInt32();
			ipb.db_ip = ip;
			banList.push_back(ipb);

		}
		while(result->NextRow());
		delete result;
	}
	listBusy.Release();
}

Realm* InformationCore::AddRealm(uint32 realm_id, Realm* rlm)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);

	if(itr == m_realms.end())
		m_realms.insert(make_pair(realm_id, rlm));
	else
	{
		delete itr->second;
		itr->second = rlm;
	}
	realmLock.Release();
	return rlm;
}

Realm* InformationCore::GetRealm(uint32 realm_id)
{
	Realm* ret = NULL;

	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		ret = itr->second;
	}
	realmLock.Release();
	return ret;
}

int32 InformationCore::GetRealmIdByName(string Name)
{
	map<uint32, Realm*>::iterator itr = m_realms.begin();
	for(; itr != m_realms.end(); ++itr)
		if(itr->second->Name == Name)
		{
			return itr->first;
		}
	return -1;
}

void InformationCore::RemoveRealm(uint32 realm_id)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		delete itr->second;
		m_realms.erase(itr);
	}
	realmLock.Release();
}

void InformationCore::UpdateRealmStatus(uint32 realm_id, uint8 flags)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		itr->second->flags = flags;
	}
	realmLock.Release();
}

void InformationCore::UpdateRealmPop(uint32 realm_id, float pop)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		uint8 flags;
		if(pop >= 3)
			flags =  REALM_FLAG_FULL | REALM_FLAG_INVALID; // Full
		else if(pop >= 2)
			flags = REALM_FLAG_INVALID; // Red
		else if(pop >= 0.5)
			flags = 0; // Green
		else
			flags = REALM_FLAG_NEW_PLAYERS; // recommended

		itr->second->Population = (pop > 0) ? (pop >= 1) ? (pop >= 2) ? 2.0f : 1.0f : 0.0f : 0.0f;
		itr->second->flags = flags;
	}
	realmLock.Release();
}
void InformationCore::SendRealms(AuthSocket* Socket)
{
	realmLock.Acquire();

	// packet header
	ByteBuffer data(m_realms.size() * 150 + 20);
	data << uint8(0x10);
	data << uint16(0);	  // Size Placeholder

	// dunno what this is..
	data << uint32(0);

	//sAuthLogonChallenge_C * client = Socket->GetChallenge();
	data << uint16(m_realms.size());

	// loop realms :/
	map<uint32, Realm*>::iterator itr = m_realms.begin();
	HM_NAMESPACE::hash_map<uint32, uint8>::iterator it;
	for(; itr != m_realms.end(); ++itr)
	{
//		data << uint8(itr->second->Icon);
//		data << uint8(0);				   // Locked Flag
//		data << uint8(itr->second->Colour);
		data << uint8(itr->second->Icon);
		data << uint8(itr->second->Lock);		// delete when using data << itr->second->Lock;
		data << uint8(itr->second->flags);

		// This part is the same for all.
		data << itr->second->Name;
		data << itr->second->Address;
//		data << uint32(0x3fa1cac1);
		data << float(itr->second->Population);

		/* Get our character count */
		it = itr->second->CharacterMap.find(Socket->GetAccountID());
		data << uint8((it == itr->second->CharacterMap.end()) ? 0 : it->second);
//		data << uint8(1);   // time zone
//		data << uint8(6);
		data << uint8(itr->second->TimeZone);
		data << uint8(GetRealmIdByName(itr->second->Name));        //Realm ID
	}
	data << uint8(0x17);
	data << uint8(0);

	realmLock.Release();

	// Re-calculate size.

	*(uint16*)&data.contents()[1] = uint16(data.size() - 3);

	// Send to the socket.
	Socket->Send((const uint8*)data.contents(), uint32(data.size()));

	std::list< LogonCommServerSocket* > ss;
	std::list< LogonCommServerSocket* >::iterator SSitr;

	ss.clear();

	serverSocketLock.Acquire();

	if(m_serverSockets.empty())
	{
		serverSocketLock.Release();
		return;
	}

	set<LogonCommServerSocket*>::iterator itr1;

	// We copy the sockets to a list and call RefreshRealmsPop() from there because if the socket is dead,
	//then calling the method deletes the socket and removes it from the set corrupting the iterator and causing a crash!
	for(itr1 = m_serverSockets.begin(); itr1 != m_serverSockets.end(); ++itr1)
	{
		ss.push_back(*itr1);
	}

	serverSocketLock.Release();

	for(SSitr = ss.begin(); SSitr != ss.end(); ++SSitr)
		(*SSitr)->RefreshRealmsPop();

	ss.clear();
}

void InformationCore::TimeoutSockets()
{
	if(!usepings)
		return;

	uint32 now = uint32(time(NULL));

	/* burlex: this is vulnerable to race conditions, adding a mutex to it. */
	serverSocketLock.Acquire();

	for(set< LogonCommServerSocket* >::iterator itr = m_serverSockets.begin(); itr != m_serverSockets.end();)
	{
		LogonCommServerSocket* s = *itr;
		++itr;

		uint32 last_ping = s->last_ping.GetVal();
		if(last_ping < now && ((now - last_ping) > 300))
		{
			for(set< uint32 >::iterator RealmITR = s->server_ids.begin(); RealmITR != s->server_ids.end(); ++RealmITR)
			{
				uint32 RealmID = *RealmITR;

				SetRealmOffline(RealmID);
			}

			s->removed = true;
			m_serverSockets.erase(s);
			s->Disconnect();
		}
	}

	serverSocketLock.Release();
}

void InformationCore::CheckServers()
{
	serverSocketLock.Acquire();

	set<LogonCommServerSocket*>::iterator itr, it2;
	LogonCommServerSocket* s;
	for(itr = m_serverSockets.begin(); itr != m_serverSockets.end();)
	{
		s = *itr;
		it2 = itr;
		++itr;

		if(!IsServerAllowed(s->GetRemoteAddress().s_addr))
		{
			LOG_DETAIL("Disconnecting socket: %s due to it no longer being on an allowed IP.", s->GetRemoteIP().c_str());
			s->Disconnect();
		}
	}

	serverSocketLock.Release();
}

void InformationCore::SetRealmOffline(uint32 realm_id)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		itr->second->flags = REALM_FLAG_OFFLINE | REALM_FLAG_INVALID;
		itr->second->CharacterMap.clear();
		Log.Notice("InfoCore", "Realm %u is now offline (socket close).", realm_id);
	}
	realmLock.Release();
}
