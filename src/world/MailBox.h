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

#ifndef _MAILBOX_H
#define _MAILBOX_H

typedef map<uint32, MailMessage> MessageMap;

class Mailbox
{
	protected:
		uint64 owner;
		MessageMap Messages;

	public:
		Mailbox(uint64 owner_) : owner(owner_) {}

		void AddMessage(MailMessage* Message);
		void DeleteMessage(uint32 MessageId, bool sql);
		MailMessage* GetMessage(uint32 message_id)
		{
			MessageMap::iterator iter = Messages.find(message_id);
			if(iter == Messages.end())
				return NULL;
			return &(iter->second);
		}

		WorldPacket* BuildMailboxListingPacket();
		void CleanupExpiredMessages();
		ARCEMU_INLINE size_t MessageCount() { return Messages.size(); }
		void FillTimePacket(WorldPacket & data);
		ARCEMU_INLINE uint64 GetOwner() { return owner; }
		void Load(QueryResult* result);
};

#endif
