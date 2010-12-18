/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

//
// MapCell.h
//

#ifndef __MAP_CELL_H
#define __MAP_CELL_H

class Map;

#define MAP_CELL_DEFAULT_UNLOAD_TIME 300
#define MAKE_CELL_EVENT(x,y) ( ((x) * 1000) + 200 + y )
#define DECODE_CELL_EVENT(dest_x, dest_y, ev) (dest_x) = ((ev-200)/1000); (dest_y) = ((ev-200)%1000);

class SERVER_DECL MapCell
{
	friend class MapMgr;
public:
	MapCell() {};
	~MapCell();

	typedef std::set<Object*> ObjectSet;

	//Init
	void Init(uint32 x, uint32 y, MapMgr *mapmgr);
	
	//Object Managing
	void AddObject(Object *obj); 
	void RemoveObject(Object *obj);
	bool HasObject(Object *obj) { return (_objects.find(obj) != _objects.end()); }
	bool HasPlayers() { return ((_playerCount > 0) ? true : false); }
	ARCEMU_INLINE size_t GetObjectCount() { return _objects.size(); }
	void RemoveObjects();
	ARCEMU_INLINE ObjectSet::iterator Begin() { return _objects.begin(); }
	ARCEMU_INLINE ObjectSet::iterator End() { return _objects.end(); }

	//State Related
	void SetActivity(bool state);

	ARCEMU_INLINE bool IsActive() { return _active; }
	ARCEMU_INLINE bool IsLoaded() { return _loaded; }
	ARCEMU_INLINE void SetLoaded( bool Loaded = true ) { _loaded = Loaded; }

	//Object Loading Managing
	void LoadObjects(CellSpawns * sp);
	ARCEMU_INLINE uint32 GetPlayerCount() { return _playerCount; }

	ARCEMU_INLINE bool IsUnloadPending() { return _unloadpending; }
	ARCEMU_INLINE void SetUnloadPending(bool up) { _unloadpending = up; }
	void QueueUnloadPending();
	void CancelPendingUnload();
	void Unload();
	ARCEMU_INLINE uint16 GetPositionX() { return _x; }
	ARCEMU_INLINE uint16 GetPositionY() { return _y; }

	ObjectSet _respawnObjects;
	ObjectSet::iterator objects_iterator;//required by MapCell::RemoveObjects() removing Creatures which will remove their guardians and corrupt itr.

	//the corpse has no more an owner (like if he resurrected) so it can be despawned and
	//the MapCell can be unloaded(if CanUnload() returns true)
	void CorpseGoneIdle(Object* corpse);

private:
	uint16 _x,_y;
	ObjectSet _objects;
	bool _active, _loaded;
	bool _unloadpending;

	uint16 _playerCount;

	//checks if the MapCell can be unloaded, based on _corpses and if it's in a battleground
	bool CanUnload();
	//checks if the MapCell can be unloaded and if so it queues it for unload.
	//this MUST be called when a corpse goes idle
	void CheckUnload();

	//keep track of active corpses so we don't unload a MapCell with an active corpse (otherwise players will not be able to resurrect)
	std::list< Object* > _corpses;

	MapMgr* _mapmgr;

	Mutex m_objectlock;
};

#endif

