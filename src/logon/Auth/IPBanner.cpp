/*
 * ArcEmu MMORPG Server
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

#include "LogonStdAfx.h"

initialiseSingleton(IPBanner);

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
