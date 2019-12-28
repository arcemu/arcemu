/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2019 <http://www.ArcEmu.org/>
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

#ifndef _UPDATEBUILDER_H
#define _UPDATEBUILDER_H

class Object;
class Player;
class ByteBuffer;
class UpdateMask;

class UpdateBuilder
{
public:
    static void _BuildValuesUpdate( ByteBuffer* data, UpdateMask* updateMask, Object *object, Player* target );

    static uint32 BuildCreateUpdateBlockForPlayer( ByteBuffer* data, Object *object, Player* target );

    static void _BuildMovementUpdate( ByteBuffer* data, uint16 flags, uint32 flags2, Object *object, Player* target );

    static WorldPacket* BuildFieldUpdatePacket(Object* object, uint32 index, uint32 value);

    static void SendFieldUpdatePacket(Player* Target, Object* object, uint32 Index, uint32 Value);

    static void BuildFieldUpdatePacket(ByteBuffer* buf, Object* object, uint32 Index, uint32 Value);

    static uint32 BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, Object* object, Player* target);

    static uint32 BuildValuesUpdateBlockForPlayer(ByteBuffer* buf, UpdateMask* mask, Object* object);
};

#endif
