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

#ifndef ARCEMU_ITEMPACKETS_H_
#define ARCEMU_ITEMPACKETS_H_

namespace Arcemu
{
	namespace GamePackets
	{
		namespace Items
		{
			/// The client asks for refund information on the item with the provided GUID
			class CItemRefundInfo : public Arcemu::Shared::Packet
			{
			public:
				uint64 guid;

			public:
				CItemRefundInfo(){}
				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );
			};

			/// Server sends the refund information on the item with the provided GUID
			class SItemRefundInfo : public Arcemu::Shared::Packet
			{
			public:
				static const uint32 MAX_ITEM_COUNT = 5;

			public:
				uint64 guid;
				uint32 buyprice;
				uint32 honor;
				uint32 arena;
				uint32 item[ MAX_ITEM_COUNT ];
				uint32 item_count[ MAX_ITEM_COUNT ];
				uint32 unk1;
				uint32 remaining_time;

			public:
				SItemRefundInfo(){}
				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );
			};

			/// Client requests the refund of the item
			class CItemRefundRequest : public Arcemu::Shared::Packet
			{
			public:
				uint64 guid;

			public:
				CItemRefundRequest(){}
				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );
			};

			/// Server responds to refund request
			class SItemRefundRequest : public Arcemu::Shared::Packet
			{
			public:
				static const uint32 MAX_ITEM_COUNT = 5;

			public:
				uint64 guid;
				uint32 error;
				uint32 buyprice;
				uint32 honor;
				uint32 arena;
				uint32 item[ MAX_ITEM_COUNT ];
				uint32 item_count[ MAX_ITEM_COUNT ];

			public:
				SItemRefundRequest(){}
				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );
			};

			class SSellItem : public Arcemu::Shared::Packet
			{
			public:
				uint64 vendorGuid;
				uint64 itemGuid;
				uint8 result;

			public:
				SSellItem(){}
				void serialize( PacketBuffer &buffer ) const;
				void deserialize( PacketBuffer& buffer );

				size_t maxSize() const{ return 17U; }
			};
		}
	}
}

#endif
