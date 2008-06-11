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