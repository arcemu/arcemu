/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef __ADDONMGR_H
#define __ADDONMGR_H

struct AddonEntry
{
	std::string name;
	uint64 crc;
	bool banned;
	bool isNew;
	bool showinlist;
};

class AddonMgr :  public Singleton < AddonMgr >
{
public:
	AddonMgr();
	~AddonMgr();

	void LoadFromDB();
	void SaveToDB();

	void SendAddonInfoPacket(WorldPacket *source, uint32 pos, WorldSession *m_session);
	bool AppendPublicKey(WorldPacket& data, string AddonName, uint32 CRC);

private:
	std::map<std::string, AddonEntry*> KnownAddons;
	map<string, ByteBuffer> AddonData;

	bool IsAddonBanned(uint64 crc, std::string name = "");
	bool IsAddonBanned(std::string name, uint64 crc = 0);
	bool ShouldShowInList(std::string name);
};

#define sAddonMgr AddonMgr::getSingleton()

#endif
