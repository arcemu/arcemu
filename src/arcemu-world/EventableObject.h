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

#ifndef _EVENTABLE_OBJECT_H
#define _EVENTABLE_OBJECT_H

class EventableObjectHolder;

/**
  * @class EventableObject
  * EventableObject means that the class inheriting this is able to take
  * events. This 'base' class will store and update these events upon
  * receiving the call from the instance thread / WorldRunnable thread.
  */

typedef list<TimedEvent*> EventList;
typedef multimap<uint32, TimedEvent*> EventMap;

#define EVENT_REMOVAL_FLAG_ALL 0xFFFFFFFF
#define WORLD_INSTANCE -1

class SERVER_DECL EventableObject
{
	friend class EventMgr;
	friend class EventableObjectHolder;

protected:
	void event_RemoveEvents();
	void event_RemoveEvents(uint32 EventType);
	void event_ModifyTimeLeft(uint32 EventType, uint32 TimeLeft,bool unconditioned=false);
	void event_ModifyTime(uint32 EventType, uint32 Time);
	void event_ModifyTimeAndTimeLeft(uint32 EventType, uint32 Time);
	bool event_HasEvent(uint32 EventType);
	void event_RemoveByPointer(TimedEvent * ev);
	ARCEMU_INLINE int32 event_GetCurrentInstanceId() { return m_event_Instanceid; }
	bool event_GetTimeLeft(uint32 EventType, uint32 * Time);

public:
	uint32 event_GetEventPeriod(uint32 EventType);
	// Public methods
	EventableObject();
	virtual ~EventableObject();
	void Virtual_Destructor();

	ARCEMU_INLINE bool event_HasEvents() { return m_events.size() > 0 ? true : false; }
	void event_AddEvent(TimedEvent * ptr);
	void event_Relocate();
	
	// this func needs to be implemented by all eventable classes. use it to retreive the instance
	// id that it needs to attach itself to.
	
	virtual int32 event_GetInstanceID() { return -1; }

protected:

	int32 m_event_Instanceid;
	FastMutex m_lock;
	EventMap m_events;
	EventableObjectHolder * m_holder;
	
};

/**
  * @class EventableObjectHolder
  * EventableObjectHolder will store eventable objects, and remove/add them when they change
  * from one holder to another (changing maps / instances).
  *
  * EventableObjectHolder also updates all the timed events in all of its objects when its
  * update function is called.
  *
  */

typedef set<EventableObject*> EventableObjectSet;

class EventableObjectHolder
{
public:
	EventableObjectHolder(int32 instance_id);
	~EventableObjectHolder();

	void Update(uint32 time_difference);

	void AddEvent(TimedEvent * ev);
	void AddObject(EventableObject * obj);

	ARCEMU_INLINE uint32 GetInstanceID() { return mInstanceId; }

protected:
	int32 mInstanceId;
	Mutex m_lock;
	EventList m_events;

	Mutex m_insertPoolLock;
	typedef list<TimedEvent*> InsertableQueue;
	InsertableQueue m_insertPool;
};

#endif
