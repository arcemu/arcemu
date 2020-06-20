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

#ifndef ARCEMU_ACCOUNTPACKETS_H_
#define ARCEMU_ACCOUNTPACKETS_H_

namespace Arcemu
{
	namespace GamePackets
	{
		namespace Account
		{
			class SERVER_DECL SAccountDataTimes : public Arcemu::Shared::Packet
			{
			public:
				uint32 time;
				uint8 unk1;
				uint32 mask;
				uint32 times[ NUM_ACCOUNT_DATA_TYPES ];

				SAccountDataTimes();

				virtual void serialize( PacketBuffer &buffer ) const;

				virtual void deserialize( PacketBuffer& buffer );
			};
		}
	}
}

#endif
