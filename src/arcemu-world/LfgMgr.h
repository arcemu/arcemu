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

enum LFGTypes 
{
	LFG_NONE = 0,
	LFG_INSTANCE = 1,
	LFG_RAID = 2,
	LFG_QUEST = 3,
	LFG_ZONE = 4,
	LFG_HEROIC_DUNGEON = 5, // from client
};

#define MAX_DUNGEONS 201+1 // check max entrys +1 on lfgdungeons.dbc
#define MAX_LFG_QUEUE_ID 3
#define LFG_MATCH_TIMEOUT 30		// in seconds

class LfgMatch;
class LfgMgr : public Singleton < LfgMgr >, EventableObject
{
public:	
	
	typedef list<Player*> LfgPlayerList;

	LfgMgr();
	~LfgMgr();
	
	bool AttemptLfgJoin(Player * pl, uint32 LfgDungeonId);
	void SetPlayerInLFGqueue(Player *pl,uint32 LfgDungeonId);
	void SetPlayerInLfmList(Player * pl, uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueue(Player *pl,uint32 LfgDungeonId);
	void RemovePlayerFromLfgQueues(Player * pl);
	void RemovePlayerFromLfmList(Player * pl, uint32 LfmDungeonId);
	void UpdateLfgQueue(uint32 LfgDungeonId);
	void SendLfgList(Player * plr, uint32 Dungeon);
	void EventMatchTimeout(LfgMatch * pMatch);

	int32 event_GetInstanceId() { return -1; }
	
protected:
	
	LfgPlayerList m_lookingForGroup[MAX_DUNGEONS];
	LfgPlayerList m_lookingForMore[MAX_DUNGEONS];
	Mutex m_lock;
	
	
};

class LfgMatch
{
public:
	set<Player*> PendingPlayers;
	set<Player*> AcceptedPlayers;
	Mutex lock;
	uint32 DungeonId;
    Group * pGroup;

	LfgMatch(uint32 did) : DungeonId(did),pGroup(NULL) { }
};

extern uint32 LfgDungeonTypes[MAX_DUNGEONS];

#define sLfgMgr LfgMgr::getSingleton()
