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

void reverse_array(uint8* pointer, size_t count)
{
	int i = 0;
	int j = (int)( count - 1 );

	while( j > i )
	{
		uint8 temp = pointer[ i ];
		pointer[ i ] = pointer[ j ];
		pointer[ j ] = temp;
		++i;
		--j;
	}
}

initialiseSingleton(AccountMgr);
initialiseSingleton(IPBanner);

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
			Arcemu::Shared::Util::arcemu_TOUPPER(AccountName);

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
	HM_NAMESPACE::HM_HASH_MAP<string, Account*>::iterator itr = AccountDatabase.begin();
	HM_NAMESPACE::HM_HASH_MAP<string, Account*>::iterator it2;

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
	Arcemu::Shared::Util::arcemu_TOUPPER(Username);
	Arcemu::Shared::Util::arcemu_TOUPPER(Password);

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
	Arcemu::Shared::Util::arcemu_TOUPPER(Username);
	Arcemu::Shared::Util::arcemu_TOUPPER(Password);

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

		if(Arcemu::Shared::Util::ParseCIDRBan(ip_address.s_addr, itr->Mask, itr->Bytes))
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

	unsigned int ipraw = Arcemu::Shared::Util::MakeIP(stmp.c_str());
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

			unsigned int ipraw = Arcemu::Shared::Util::MakeIP(stmp.c_str());
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

