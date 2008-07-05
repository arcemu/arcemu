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

//
// MapCell.h
//

#ifndef __MAP_CELL_H
#define __MAP_CELL_H

class Map;

#define MAKE_CELL_EVENT(x,y) ( ((x) * 1000) + 200 + y )
#define DECODE_CELL_EVENT(dest_x, dest_y, ev) (dest_x) = ((ev-200)/1000); (dest_y) = ((ev-200)%1000);

class MapCell
{
	friend class MapMgr;
public:
	MapCell() {};
	~MapCell();

	typedef std::set<Object*> ObjectSet;

	//Init
	void Init(uint32 x, uint32 y, uint32 mapid, MapMgr *mapmgr);

	//Object Managing
	void AddObject(Object *obj); 
	void RemoveObject(Object *obj);
	bool HasObject(Object *obj) { return (_objects.find(obj) != _objects.end()); }
	bool HasPlayers() { return ((_playerCount > 0) ? true : false); }
	ASCENT_INLINE size_t GetObjectCount() { return _objects.size(); }
	void RemoveObjects();
	ASCENT_INLINE ObjectSet::iterator Begin() { return _objects.begin(); }
	ASCENT_INLINE ObjectSet::iterator End() { return _objects.end(); }

	//State Related
	void SetActivity(bool state);

	ASCENT_INLINE bool IsActive() { return _active; }
	ASCENT_INLINE bool IsLoaded() { return _loaded; }

	//Object Loading Managing
	void LoadObjects(CellSpawns * sp);
	ASCENT_INLINE uint32 GetPlayerCount() { return _playerCount; }

	ASCENT_INLINE bool IsUnloadPending() { return _unloadpending; }
	ASCENT_INLINE void SetUnloadPending(bool up) { _unloadpending = up; }
	void QueueUnloadPending();
	void CancelPendingUnload();
	void Unload();

	ObjectSet _respawnObjects;

private:
	uint16 _x,_y;
	ObjectSet _objects;
	bool _active, _loaded;
	bool _unloadpending;

	uint16 _playerCount;
	MapMgr* _mapmgr;
};

#endif
