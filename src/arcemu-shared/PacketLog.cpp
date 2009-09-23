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

#include "Common.h"
#include "PacketLog.h"
#include "Config/ConfigEnv.h"

#include <ctype.h>


createFileSingleton( PacketLog );

PacketLog::PacketLog()
{
    // clear realm logfile
    if (sConfig.GetBoolDefault("LogRealm", false))
    {
        FILE *pFile = fopen("realm.log", "w+");
        fclose(pFile);
    }
    // clear world logfile
    if (sConfig.GetBoolDefault("LogWorld", false))
    {
        FILE *pFile = fopen("world.log", "w+");
        fclose(pFile);
    }
}

PacketLog::~PacketLog()
{
}


char PacketLog::makehexchar(int i) {
    return (i<=9) ? '0'+i : 'A'+(i-10);
}

int PacketLog::hextoint(char c) {
    c = toupper(c);
    return (c > '9' ? c - 'A' + 10 : c - '0');
}


void PacketLog::HexDump(const unsigned char* data, size_t length, const char* file) 
{
    FILE *pFile;
    pFile = fopen(file, "a");

    const int char_offset = 16*3 + 2;
    const int line_size = 16*3 + 16 + 3;
    char line[line_size+1];

    fprintf(pFile,"OFFSET  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789ABCDEF\n");
    fprintf(pFile,"--------------------------------------------------------------------------\n");

    line[char_offset - 1] = ' ';
    line[char_offset - 2] = ' ';

    for (size_t i=0; i<length; ) {
        int bi=0;
        int ci=0;

        int start_i = i;

        for (int line_i=0; i < length && line_i < 16; i++, line_i++) {
            line[bi++] = makehexchar(*data>>4);
            line[bi++] = makehexchar(*data & 0x0f);
            line[bi++] = ' ';
            line[char_offset+(ci++)]=(isprint(*data) ? *data : '.');
            data++;
        }

        while (bi<16*3) {
            line[bi++]=' ';
        }

        line[char_offset+(ci++)]='\n';
        line[char_offset+ci]=0;

        fprintf(pFile,"%06X  %s", start_i, line);
    }
    fprintf(pFile, "\n\n");
    fclose(pFile);
}

void PacketLog::HexDump(const char *data, size_t length, const char* file) {
    HexDump((unsigned char *)data, length, file);
}

void PacketLog::HexDumpStr(const char *msg, const char *data, size_t len, const char* file)
{
    FILE *pFile;
    pFile = fopen(file, "a");
    fprintf(pFile,"%s\n", msg);
    fclose(pFile);

    HexDump(data, len, file);
}


void PacketLog::RealmHexDump(RealmPacket* data, uint32 socket, bool direction)
{
    if (!sConfig.GetBoolDefault("LogRealm", false))
        return;

    FILE *pFile;
    pFile = fopen("realm.log", "a");

    uint16 len = data->size() + 2;
    uint8 opcode = data->GetOpcode();
    if (direction)
        fprintf(pFile, "SERVER:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.2X\nDATA:\n", socket, len, opcode);
    else
        fprintf(pFile, "CLIENT:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.2X\nDATA:\n", socket, len, opcode);

    fclose(pFile);
    HexDump((char *)data->contents(), data->size(), "realm.log");

}
void PacketLog::WorldHexDump(WorldPacket* data, uint32 socket, bool direction)
{
    if (!sConfig.GetBoolDefault("LogWorld", false))
        return;

    FILE *pFile;
    pFile = fopen("world.log", "a");

    uint16 len = data->size();// + 4;
    uint16 opcode = data->GetOpcode();
    if (direction)
        fprintf(pFile, "SERVER:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.4X\nDATA:\n", socket, len, opcode);
    else
        fprintf(pFile, "CLIENT:\nSOCKET: %d\nLENGTH: %d\nOPCODE: %.4X\nDATA:\n", socket, len, opcode);

    fclose(pFile);
    HexDump((char *)data->contents(), data->size(), "world.log");

}