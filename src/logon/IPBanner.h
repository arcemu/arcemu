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

#ifndef IPBANNER_H_
#define IPBANNER_H_

typedef struct
{
	unsigned int Mask;
	unsigned char Bytes;
	uint32 Expire;
	string db_ip;
} IPBan;

enum BAN_STATUS
{
    BAN_STATUS_NOT_BANNED = 0,
    BAN_STATUS_TIME_LEFT_ON_BAN = 1,
    BAN_STATUS_PERMANENT_BAN = 2,
};

class IPBanner : public Singleton< IPBanner >
{
	public:
		void Reload();

		bool Add(const char* ip, uint32 dur);
		bool Remove(const char* ip);

		BAN_STATUS CalculateBanStatus(in_addr ip_address);

	protected:
		Mutex listBusy;
		list<IPBan> banList;
};

#define sIPBanner IPBanner::getSingleton()

#endif
