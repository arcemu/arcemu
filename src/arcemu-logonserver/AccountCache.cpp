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

#include "LogonStdAfx.h"
initialiseSingleton(AccountMgr);
initialiseSingleton(IPBanner);
initialiseSingleton(InformationCore);

void AccountMgr::ReloadAccounts(bool silent)
{
	setBusy.Acquire();
	if(!silent) sLog.outString("[AccountMgr] Reloading Accounts...");

	// Load *all* accounts.
	QueryResult * result = sLogonSQL->Query("SELECT acct, login, password, encrypted_password, gm, flags, banned, forceLanguage, muted FROM accounts");
	Field * field;
	string AccountName;
	set<string> account_list;
	Account * acct;

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

		} while(result->NextRow());

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
	Account * acct = new Account;
	Sha1Hash hash;
	string Username     = field[1].GetString();
	string Password	    = field[2].GetString();
	string EncryptedPassword = field[3].GetString();
	string GMFlags		= field[4].GetString();

	acct->AccountId				= field[0].GetUInt32();
	acct->AccountFlags			= field[5].GetUInt8();
	acct->Banned				= field[6].GetUInt32();
	if ( (uint32)UNIXTIME > acct->Banned && acct->Banned != 0 && acct->Banned != 1) //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Banned = 0;
		//me go boom :(
		//printf("Account %s's ban has expired.\n",acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET banned = 0 WHERE acct=%u",acct->AccountId);
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
	if ( (uint32)UNIXTIME > acct->Muted && acct->Muted != 0 && acct->Muted != 1) //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Muted= 0;
		//sLog.outDebug("Account %s's mute has expired.",acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET muted = 0 WHERE acct=%u",acct->AccountId);
	}
	// Convert username/password to uppercase. this is needed ;)
	arcemu_TOUPPER(Username);
	arcemu_TOUPPER(Password);
	
	// prefer encrypted passwords over nonencrypted
	if( EncryptedPassword.size() > 0 )
	{
		if ( EncryptedPassword.size() == 40 )
		{			
			BigNumber bn;
			bn.SetHexStr( EncryptedPassword.c_str() );
			if( bn.GetNumBytes() < 20 )
			{
				// Hacky fix
				memcpy(acct->SrpHash, bn.AsByteArray(), bn.GetNumBytes());
				for (int n=bn.GetNumBytes(); n<=19; n++)
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
			printf("Account `%s` has incorrect number of bytes in encrypted password! Disabling.\n", Username.c_str());
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

void AccountMgr::UpdateAccount(Account * acct, Field * field)
{
	uint32 id = field[0].GetUInt32();
	Sha1Hash hash;
	string Username     = field[1].GetString();
	string Password	    = field[2].GetString();
	string EncryptedPassword = field[3].GetString();
	string GMFlags		= field[4].GetString();

	if(id != acct->AccountId)
	{
		//printf("Account %u `%s` is a duplicate.\n", id, acct->Username.c_str());
		sLog.outColor(TYELLOW, " >> deleting duplicate account %u [%s]...", id, Username.c_str());
		sLog.outColor(TNORMAL, "\n");
		sLogonSQL->Execute("DELETE FROM accounts WHERE acct=%u", id);
		return;
	}

	acct->AccountId				= field[0].GetUInt32();
	acct->AccountFlags			= field[5].GetUInt8();
	acct->Banned				= field[6].GetUInt32();
	if ((uint32)UNIXTIME > acct->Banned && acct->Banned != 0 && acct->Banned != 1) //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Banned = 0;
		sLog.outDebug("Account %s's ban has expired.",acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET banned = 0 WHERE acct=%u",acct->AccountId);
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
	if ((uint32)UNIXTIME > acct->Muted && acct->Muted != 0 && acct->Muted != 1) //1 = perm ban?
	{
		//Accounts should be unbanned once the date is past their set expiry date.
		acct->Muted= 0;
		sLog.outDebug("Account %s's mute has expired.",acct->UsernamePtr->c_str());
		sLogonSQL->Execute("UPDATE accounts SET muted = 0 WHERE acct=%u",acct->AccountId);
	}
	// Convert username/password to uppercase. this is needed ;)
	arcemu_TOUPPER(Username);
	arcemu_TOUPPER(Password);

	// prefer encrypted passwords over nonencrypted
	if( EncryptedPassword.size() > 0 )
	{
		if ( EncryptedPassword.size() == 40 )
		{			
			BigNumber bn;
			bn.SetHexStr( EncryptedPassword.c_str() );
			if( bn.GetNumBytes() < 20 )
			{
				// Hacky fix
				memcpy(acct->SrpHash, bn.AsByteArray(), bn.GetNumBytes());
				for (int n=bn.GetNumBytes(); n<=19; n++)
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
			printf("Account `%s` has incorrect number of bytes in encrypted password! Disabling.\n", Username.c_str());
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

		if( ParseCIDRBan(ip_address.s_addr, itr->Mask, itr->Bytes) )
		{
			// ban hit
			if( itr->Expire == 0 )
				return BAN_STATUS_PERMANENT_BAN;
			
			if( (uint32)UNIXTIME >= itr->Expire )
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

bool IPBanner::Add(const char * ip, uint32 dur)
{
	string sip = string(ip);

	string::size_type i = sip.find("/");
	if( i == string::npos )
		return false;

	string stmp = sip.substr(0, i);
	string smask = sip.substr(i+1);

	unsigned int ipraw = MakeIP(stmp.c_str());
	unsigned int ipmask = atoi(smask.c_str());
	if( ipraw == 0 || ipmask == 0 )
		return false;

	IPBan ipb;
	ipb.db_ip = sip;
	ipb.Bytes = ipmask;
	ipb.Mask = ipraw;
	
	listBusy.Acquire();
	banList.push_back(ipb);
	listBusy.Release();

	return true;
}

InformationCore::~InformationCore()
{
	for( map<uint32, Realm*>::iterator itr = m_realms.begin(); itr != m_realms.end(); ++itr )
		delete itr->second;
}

bool IPBanner::Remove(const char * ip)
{
	listBusy.Acquire();
	for(list<IPBan>::iterator itr = banList.begin(); itr != banList.end(); ++itr)
	{
		if( !strcmp(ip, itr->db_ip.c_str()) )
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
	QueryResult * result = sLogonSQL->Query("SELECT ip, expire FROM ipbans");
	if( result != NULL )
	{
		do 
		{
			IPBan ipb;
			string smask= "32";
			string ip = result->Fetch()[0].GetString();
			string::size_type i = ip.find("/");
			string stmp = ip.substr(0, i);
			if( i == string::npos )
			{
				printf("IP ban \"%s\" netmask not specified. assuming /32 \n", ip.c_str());
			}
			else
				smask = ip.substr(i+1);

			unsigned int ipraw = MakeIP(stmp.c_str());
			unsigned int ipmask = atoi(smask.c_str());
			if( ipraw == 0 || ipmask == 0 )
			{
				printf("IP ban \"%s\" could not be parsed. Ignoring\n", ip.c_str());
				continue;
			}

			ipb.Bytes = ipmask;
			ipb.Mask = ipraw;
			ipb.Expire = result->Fetch()[1].GetUInt32();
			ipb.db_ip = ip;
			banList.push_back(ipb);

		} while (result->NextRow());
		delete result;
	}
	listBusy.Release();
}

Realm * InformationCore::AddRealm(uint32 realm_id, Realm * rlm)
{
	realmLock.Acquire();
	m_realms.insert( make_pair( realm_id, rlm ) );
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	realmLock.Release();
	return rlm;
}

Realm * InformationCore::GetRealm(uint32 realm_id)
{
	Realm * ret = 0;

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
		if (itr->second->Name == Name)
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
		Log.Notice("InfoCore","Removing realm id:(%u) due to socket close.",realm_id);
		delete itr->second;
		m_realms.erase(itr);
	}
	realmLock.Release();
}

void InformationCore::UpdateRealmStatus(uint32 realm_id, uint8 Color)
{
	realmLock.Acquire();
	map<uint32, Realm*>::iterator itr = m_realms.find(realm_id);
	if(itr != m_realms.end())
	{
		itr->second->Colour = Color;
	}
	realmLock.Release();
}

void InformationCore::SendRealms(AuthSocket * Socket)
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
		data << uint8(itr->second->Colour);

		// This part is the same for all.
		data << itr->second->Name;
		data << itr->second->Address;
//		data << uint32(0x3fa1cac1);
		data << float(itr->second->Population);

		/* Get our character count */
		it = itr->second->CharacterMap.find(Socket->GetAccountID());
		data << uint8( (it == itr->second->CharacterMap.end()) ? 0 : it->second );
//		data << uint8(1);   // time zone
//		data << uint8(6);
		data << uint8( itr->second->TimeZone );
		data << uint8( GetRealmIdByName( itr->second->Name ) );    //Realm ID
	}
	data << uint8(0x17);
	data << uint8(0);

	realmLock.Release();

	// Re-calculate size.
#ifdef USING_BIG_ENDIAN
	*(uint16*)&data.contents()[1] = swap16(uint16(data.size() - 3));
#else
	*(uint16*)&data.contents()[1] = uint16(data.size() - 3);
#endif

	// Send to the socket.
	Socket->Send((const uint8*)data.contents(), uint32(data.size()));
}

void InformationCore::TimeoutSockets()
{
	if(!usepings)
		return;

	/* burlex: this is vulnerable to race conditions, adding a mutex to it. */
	serverSocketLock.Acquire();

	uint32 t = uint32(time(NULL));
	// check the ping time
	set<LogonCommServerSocket*>::iterator itr, it2;
	LogonCommServerSocket * s;
	for(itr = m_serverSockets.begin(); itr != m_serverSockets.end();)
	{
		s = *itr;
		it2 = itr;
		++itr;

		if(s->last_ping < t && ((t - s->last_ping) > 300))
		{
			// ping timeout
			printf("Closing socket due to ping timeout.\n");
			s->removed = true;
			set<uint32>::iterator itr = s->server_ids.begin();
			for(; itr != s->server_ids.end(); ++itr)
//				RemoveRealm(*itr);
				UpdateRealmStatus((*itr), 2);
			m_serverSockets.erase(it2);

			s->Disconnect();
		}
	}

	serverSocketLock.Release();
}

void InformationCore::CheckServers()
{
	serverSocketLock.Acquire();

	set<LogonCommServerSocket*>::iterator itr, it2;
	LogonCommServerSocket * s;
	for(itr = m_serverSockets.begin(); itr != m_serverSockets.end();)
	{
		s = *itr;
		it2 = itr;
		++itr;

		if(!IsServerAllowed(s->GetRemoteAddress().s_addr))
		{
			printf("Disconnecting socket: %s due to it no longer being on an allowed IP.\n", s->GetRemoteIP().c_str());
			s->Disconnect();
		}
	}

	serverSocketLock.Release();
}
