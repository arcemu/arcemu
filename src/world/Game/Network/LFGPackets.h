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
		}
	}
}

#endif
