/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
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
