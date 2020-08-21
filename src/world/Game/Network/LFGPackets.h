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

#ifndef ARCEMU_LFGPACKETS_H
#define ARCEMU_LFGPACKETS_H

namespace Arcemu
{
	namespace GamePackets
	{
		namespace LFG
		{
			/// join queue on raid browser or dungeon finder
			class CLFGAutoJoin : public Arcemu::Shared::Packet
			{
			public:
				struct LFGDungeon
				{
					uint8 dungeon;
					uint8 unk1; /// on some raids this is set to 1
					uint8 unk2;
					uint8 unk3; /// type? 1 = normal, 2 = raid, 5 = heroic, or so it seems
				};
				
				uint32 roles; // flags: 1 = leader, 2 = tank, 4 = healer, 8 = dps
				uint8 unk1;
				uint8 unk2;
				uint8 count;
				std::vector< LFGDungeon > dungeons;
				uint8 unk3;
				uint8 unk4;
				uint8 unk5;
				uint8 unk6;
				std::string comment;

			public:
				CLFGAutoJoin(){}

				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );
			};


			/// Response to joining the dungeon or raid finder queue
			class SLFGJoinResult : public Arcemu::Shared::Packet
			{
			public:
				enum LFGJoinResult
				{
					LFG_JOIN_OK = 0,
					LFG_JOIN_ROLECHECK_FAILED = 1,
					LFG_JOIN_GROUP_IS_FULL = 2,
					LFG_JOIN_INTERNAL_ERROR = 4,
					LFG_JOIN_DO_NOT_MEET_REQUIREMENTS = 5,
					LFG_JOIN_PARTY_DOES_NOT_MEET_REQUIREMENETS = 6,
					LFG_JOIN_CANT_MIX_DUNGEON_RAID_RANDOM = 7,
					LFG_JOIN_DUNGEON_DOESNT_SUPPORT_MULTIREALM = 8,
					LFG_JOIN_DISCONNECTED_OR_PENDING = 9,
					LFG_JOIN_COULDNT_RETRIEVE_PARTY_INFO = 10,
					LFG_JOIN_INVALID_DUNGEON = 11,
					LFG_JOIN_DESERTER_DEBUFF = 12,
					LFG_JOIN_DESERTER_IN_PARTY = 13,
					LFG_JOIN_RANDOM_COOLDOWN = 14,
					LFG_JOIN_RANDOM_COOLDOWN_IN_PARTY = 15,
					LFG_JOIN_TOO_MANY_MEMBERS = 16,
					LFG_JOIN_IN_BG = 17
				};

			public:
				uint32 result;
				uint32 state;

			public:
				SLFGJoinResult(){}

				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );
			};
		}
	}
}

#endif
