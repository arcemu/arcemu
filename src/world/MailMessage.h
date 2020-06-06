/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#ifndef _MAILMESSAGE_H
#define _MAILMESSAGE_H

struct MailMessage
{
	uint32 message_id;
	uint32 message_type;
	uint64 player_guid;
	uint64 sender_guid;
	string subject;
	string body;
	uint32 money;
	vector<uint32> items;
	uint32 cod;
	uint32 stationery;
	uint32 expire_time;
	uint32 delivery_time;
	uint32 checked_flag;
	bool deleted_flag;

	bool AddMessageDataToPacket(WorldPacket & data);
};

#endif
