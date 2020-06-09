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

#ifndef __ACCOUNTCACHE_H
#define __ACCOUNTCACHE_H

#include "System/Platform.h"
#include "../shared/Database/DatabaseEnv.h"

struct Account
{
	uint32 AccountId;
	char* GMFlags;
	uint8 AccountFlags;
	uint32 Banned;
	uint8 SrpHash[20]; // the encrypted password field, reversed
	uint8* SessionKey;
	string* UsernamePtr;
	uint32 Muted;

	Account()
	{
		GMFlags = NULL;
		SessionKey = NULL;
	}

	~Account()
	{
		delete [] GMFlags;
		delete [] SessionKey;
	}

	void SetGMFlags(const char* flags)
	{
		delete [] GMFlags;

		size_t len = strlen(flags);
		if(len == 0 || (len == 1 && flags[0] == '0'))
		{
			// no flags
			GMFlags = NULL;
			return;
		}

		GMFlags = new char[len + 1];
		memcpy(GMFlags, flags, len);
		GMFlags[len] = 0;
	}

	void SetSessionKey(const uint8* key)
	{
		if(SessionKey == NULL)
			SessionKey = new uint8[40];
		memcpy(SessionKey, key, 40);
	}

	char Locale[4];
	bool forcedLocale;

};

class AccountMgr : public Singleton < AccountMgr >
{
	public:
		~AccountMgr()
		{

			for(HM_NAMESPACE::HM_HASH_MAP<string, Account*>::iterator itr = AccountDatabase.begin(); itr != AccountDatabase.end(); ++itr)
			{
				delete itr->second;
			}
		}

		void AddAccount(Field* field);

		Account* GetAccount(string Name)
		{
			setBusy.Acquire();
			Account* pAccount = NULL;
			// this should already be uppercase!
			HM_NAMESPACE::HM_HASH_MAP<string, Account*>::iterator itr = AccountDatabase.find(Name);
			if(itr == AccountDatabase.end())	pAccount = NULL;
			else								pAccount = itr->second;

			setBusy.Release();
			return pAccount;
		}

		void UpdateAccount(Account* acct, Field* field);
		void ReloadAccounts(bool silent);
		void ReloadAccountsCallback();

		ARCEMU_INLINE size_t GetCount() { return AccountDatabase.size(); }

	private:
		Account* __GetAccount(string Name)
		{
			// this should already be uppercase!
			HM_NAMESPACE::HM_HASH_MAP<string, Account*>::iterator itr = AccountDatabase.find(Name);
			if(itr == AccountDatabase.end())	return NULL;
			else								return itr->second;
		}

		HM_NAMESPACE::HM_HASH_MAP<string, Account*> AccountDatabase;

	protected:
		Mutex setBusy;
};

#define sAccountMgr AccountMgr::getSingleton()

#endif
