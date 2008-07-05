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

class ChannelMgr :  public Singleton < ChannelMgr >
{
 
public:
	ChannelMgr();
	~ChannelMgr();

	Channel *GetCreateChannel(const char *name, Player * p, uint32 type_id);
	Channel *GetChannel(const char *name, Player * p);
	Channel * GetChannel(const char * name, uint32 team);
#ifdef VOICE_CHAT
	void VoiceDied();
#endif
	void RemoveChannel(Channel * chn);
	bool seperatechannels;

private:
	//team 0: aliance, team 1 horde
	typedef map<string,Channel *> ChannelList;
	ChannelList Channels[2];
	Mutex lock;
};

#define channelmgr ChannelMgr::getSingleton()
