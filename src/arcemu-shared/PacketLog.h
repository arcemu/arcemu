/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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

#ifndef WOWSERVER_PACKETLOG_H
#define WOWSERVER_PACKETLOG_H

#include "Common.h"
#include "Singleton.h"
#include "RealmPacket.h"
#include "WorldPacket.h"

class PacketLog : public Singleton< PacketLog > {
public:
    PacketLog();
    ~PacketLog();
    //utility functions
    int hextoint(char c);
    char makehexchar(int i);
    //general log functions
    void HexDump(const unsigned char* data, size_t length, const char* file);
    void HexDump(const char *data, size_t length, const char* file);
    void HexDumpStr(const char *msg, const char *data, size_t len, const char* file);
    //realm packet log
    void RealmHexDump(RealmPacket * data, uint32 socket, bool direction);
    //world packet log
    void WorldHexDump(WorldPacket * data, uint32 socket, bool direction);
};

#define sPacketLog PacketLog::getSingleton()

#endif