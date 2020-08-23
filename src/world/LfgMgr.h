/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2012 <http://www.ArcEmu.org/>
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

class LFGQueueEntry
{
public:
	uint32 guid;
	uint32 roles;
	uint32 team;
};

class LfgMgr : public Singleton < LfgMgr >, EventableObject
{
public:
	enum LFGMgrConstants
	{
		LFGMGR_MAX_DUNGEONS = 294
	};

	enum LFGDungeonTypes
	{
		LFG_DUNGEON_TYPE_RAID = 2,
		LFG_DUNGEON_TYPE_RANDOM = 6
	};

public:
	LfgMgr();
	~LfgMgr();
	
	/// Adds the player to the LFG system
	void addPlayer( uint32 guid, uint32 roles, std::vector< uint32 > dungeons );

	/// Removes the player from the LFG system
	void removePlayer( uint32 guid );

	/// Updates the LFG system.
	void update();

private:
	Mutex lock;

	std::deque< LFGQueueEntry >* queues[ LFGMGR_MAX_DUNGEONS + 1 ];

	HM_NAMESPACE::hash_map< uint32, std::vector< uint32 > > playerToDungeons;
};

#define sLfgMgr LfgMgr::getSingleton()
