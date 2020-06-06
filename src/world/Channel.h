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

#ifndef __CHANNEL_H
#define __CHANNEL_H

#define CHANNEL_TRADE 2
#define CHANNEL_GUILDREC 25
#define CHANNEL_LFG 26

using namespace std;

enum CHANNEL_FLAGS
{
    CHANNEL_FLAG_NONE				= 0x00,
    CHANNEL_FLAG_OWNER				= 0x01,
    CHANNEL_FLAG_MODERATOR			= 0x02,
    CHANNEL_FLAG_VOICED				= 0x04,
    CHANNEL_FLAG_MUTED				= 0x08,
    CHANNEL_FLAG_CUSTOM				= 0x10,
    CHANNEL_FLAG_MICROPHONE_MUTE	= 0x20,
};

enum CHANNEL_NOTIFY_FLAGS
{
    CHANNEL_NOTIFY_FLAG_JOINED		= 0x00,
    CHANNEL_NOTIFY_FLAG_LEFT		= 0x01,
    CHANNEL_NOTIFY_FLAG_YOUJOINED	= 0x02,
    CHANNEL_NOTIFY_FLAG_YOULEFT		= 0x03,
    CHANNEL_NOTIFY_FLAG_WRONGPASS	= 0x04,
    CHANNEL_NOTIFY_FLAG_NOTON		= 0x05,
    CHANNEL_NOTIFY_FLAG_NOTMOD		= 0x06,
    CHANNEL_NOTIFY_FLAG_SETPASS		= 0x07,
    CHANNEL_NOTIFY_FLAG_CHGOWNER	= 0x08,
    CHANNEL_NOTIFY_FLAG_NOT_ON_2	= 0x09,
    CHANNEL_NOTIFY_FLAG_NOT_OWNER	= 0x0A,
    CHANNEL_NOTIFY_FLAG_WHO_OWNER	= 0x0B,
    CHANNEL_NOTIFY_FLAG_MODE_CHG	= 0x0C,
    CHANNEL_NOTIFY_FLAG_ENABLE_ANN	= 0x0D,
    CHANNEL_NOTIFY_FLAG_DISABLE_ANN	= 0x0E,
    CHANNEL_NOTIFY_FLAG_MODERATED	= 0x0F,
    CHANNEL_NOTIFY_FLAG_UNMODERATED	= 0x10,
    CHANNEL_NOTIFY_FLAG_YOUCANTSPEAK = 0x11,
    CHANNEL_NOTIFY_FLAG_KICKED		= 0x12,
    CHANNEL_NOTIFY_FLAG_YOURBANNED	= 0x13,
    CHANNEL_NOTIFY_FLAG_BANNED		= 0x14,
    CHANNEL_NOTIFY_FLAG_UNBANNED	= 0x15,
    CHANNEL_NOTIFY_FLAG_UNK_1		= 0x16,
    CHANNEL_NOTIFY_FLAG_ALREADY_ON	= 0x17,
    CHANNEL_NOTIFY_FLAG_INVITED		= 0x18,
    CHANNEL_NOTIFY_FLAG_WRONG_FACT	= 0x19,
    CHANNEL_NOTIFY_FLAG_UNK_2		= 0x1A,
    CHANNEL_NOTIFY_FLAG_UNK_3		= 0x1B,
    CHANNEL_NOTIFY_FLAG_UNK_4		= 0x1C,
    CHANNEL_NOTIFY_FLAG_YOU_INVITED	= 0x1D,
    CHANNEL_NOTIFY_FLAG_UNK_5		= 0x1E,
    CHANNEL_NOTIFY_FLAG_UNK_6		= 0x1F,
    CHANNEL_NOTIFY_FLAG_UNK_7		= 0x20,
    CHANNEL_NOTIFY_FLAG_NOT_IN_LFG	= 0x21,
    CHANNEL_NOTIFY_FLAG_VOICE_ON	= 0x22,
    CHANNEL_NOTIFY_FLAG_VOICE_OFF	= 0x23,
};

enum CHANNEL_DBC_FLAGS
{
    CHANNEL_DBC_UNK_1			= 0x01,
    CHANNEL_DBC_HAS_ZONENAME	= 0x02,
    CHANNEL_DBC_MUTED_DELAYED	= 0x04,
    CHANNEL_DBC_ALLOW_LINKS		= 0x08,
    CHANNEL_DBC_CITY_ONLY_1		= 0x10,
    CHANNEL_DBC_CITY_ONLY_2		= 0x20, //2 identical columns, who knows?
    CHANNEL_DBC_UNUSED_1		= 0x40,
    CHANNEL_DBC_UNUSED_2		= 0x80,
    CHANNEL_DBC_UNUSED_3		= 0x0100,
    CHANNEL_DBC_UNUSED_4		= 0x0200,
    CHANNEL_DBC_UNUSED_5		= 0x0400,
    CHANNEL_DBC_UNUSED_6		= 0x0800,
    CHANNEL_DBC_UNUSED_7		= 0x1000,
    CHANNEL_DBC_UNUSED_8		= 0x2000,
    CHANNEL_DBC_UNUSED_9		= 0x4000,
    CHANNEL_DBC_UNUSED_10		= 0x8000,
    CHANNEL_DBC_UNK_2			= 0x010000, //carried by local and worlddefense
    CHANNEL_DBC_UNK_3			= 0x020000, //carried by guildrecruitment. Perhaps a LeaveOnGuildJoin flag?
    CHANNEL_DBC_LFG				= 0x040000,
    CHANNEL_DBC_UNUSED_11		= 0x080000,
};

enum CHANNEL_PACKET_FLAGS
{
    CHANNEL_PACKET_CUSTOM		= 0x01,
    CHANNEL_PACKET_UNK1			= 0x02, //not seen yet, perhaps related to worlddefense
    CHANNEL_PACKET_ALLOWLINKS	= 0x04,
    CHANNEL_PACKET_ZONESPECIFIC	= 0x08, // I'm sure one of these is zone specific and the other is 'system channel' but not sure
    CHANNEL_PACKET_SYSTEMCHAN	= 0x10, // which way round it is. I need a packet log of worlddefense but I'm guessing this order
    CHANNEL_PACKET_CITY			= 0x20,
    CHANNEL_PACKET_LFG			= 0x40,
    CHANNEL_PACKET_VOICE		= 0x80,
};

class SERVER_DECL Channel
{
		Mutex m_lock;
		typedef map<Player*, uint32> MemberMap;
		MemberMap m_members;
		set<uint32> m_bannedMembers;
	public:
		friend class ChannelIterator;
		static void LoadConfSettings();
		string m_name;
		string m_password;
		uint8 m_flags;
		uint32 m_id;
		bool m_general;
		bool m_muted;
		bool m_announce;
		uint32 m_team;
		ARCEMU_INLINE size_t GetNumMembers() { return m_members.size(); }
		uint32 m_minimumLevel;
	public:
		Channel(const char* name, uint32 team, uint32 type_id);
		~Channel();

		void AttemptJoin(Player* plr, const char* password);
		void Part(Player* plr, bool send_packet = true);
		void Kick(Player* plr, Player* die_player, bool ban);
		void Invite(Player* plr, Player* new_player);
		void Moderate(Player* plr);
		void Mute(Player* plr, Player* die_player);
		void Voice(Player* plr, Player* v_player);
		void Unmute(Player* plr, Player* die_player);
		void Devoice(Player* plr, Player* v_player);
		void Say(Player* plr, const char* message, Player* for_gm_client, bool forced);
		void Unban(Player* plr, PlayerInfo* bplr);
		void GiveModerator(Player* plr, Player* new_player);
		void TakeModerator(Player* plr, Player* new_player);
		void Announce(Player* plr);
		void Password(Player* plr, const char* pass);
		void List(Player* plr);
		void GetOwner(Player* plr);

		void SetOwner(Player* oldpl, Player* plr);

		// Packet Forging
		void SendAlreadyOn(Player* plr, Player* plr2);
		void SendYouAreBanned(Player* plr);
		void SendNotOn(Player* plr);
		void SendNotOwner(Player* plr);
		void SendYouCantSpeak(Player* plr);
		void SendModeChange(Player* plr, uint8 old_flags, uint8 new_flags);

		void SendToAll(WorldPacket* data);
		void SendToAll(WorldPacket* data, Player* plr);

		bool HasMember(Player* pPlayer);
};

class ChannelIterator
{
		Channel::MemberMap::iterator m_itr;
		Channel::MemberMap::iterator m_endItr;
		bool m_searchInProgress;
		Channel* m_target;
	public:
		ChannelIterator(Channel* target) : m_searchInProgress(false), m_target(target) {}
		~ChannelIterator() { if(m_searchInProgress) { EndSearch(); } }

		void BeginSearch()
		{
			// iteminterface doesn't use mutexes, maybe it should :P
			ARCEMU_ASSERT(!m_searchInProgress);
			m_target->m_lock.Acquire();
			m_itr = m_target->m_members.begin();
			m_endItr = m_target->m_members.end();
			m_searchInProgress = true;
		}

		void EndSearch()
		{
			// nothing here either
			ARCEMU_ASSERT(m_searchInProgress);
			m_target->m_lock.Release();
			m_searchInProgress = false;
		}

		Player* operator*() const
		{
			return m_itr->first;
		}

		Player* operator->() const
		{
			return m_itr->first;
		}

		void Increment()
		{
			if(!m_searchInProgress)
				BeginSearch();

			if(m_itr == m_endItr)
				return;

			++m_itr;
		}

		ARCEMU_INLINE Player* Grab() { return m_itr->first; }
		ARCEMU_INLINE bool End() { return (m_itr == m_endItr) ? true : false; }
};

#endif
