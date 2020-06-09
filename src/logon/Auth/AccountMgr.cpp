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

	sIPBanner.Reload();
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
				Arcemu::Shared::Util::reverse_array<uint8>(acct->SrpHash, 20);
			}
			else
			{
				memcpy(acct->SrpHash, bn.AsByteArray(), 20);
				Arcemu::Shared::Util::reverse_array<uint8>(acct->SrpHash, 20);
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
				Arcemu::Shared::Util::reverse_array<uint8>(acct->SrpHash, 20);
			}
			else
			{
				memcpy(acct->SrpHash, bn.AsByteArray(), 20);
				Arcemu::Shared::Util::reverse_array<uint8>(acct->SrpHash, 20);
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


