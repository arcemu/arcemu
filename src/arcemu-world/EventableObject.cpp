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

#include "StdAfx.h"
#include "EventableObject.h"

EventableObject::~EventableObject()
{
	/* decrement event count on all events */

	EventMap::iterator itr = m_events.begin();
	for(; itr != m_events.end(); ++itr)
	{
		itr->second->deleted = true;
		itr->second->DecRef();
	}

	m_events.clear();
}

EventableObject::EventableObject()
{
	/* commented, these will be allocated when the first event is added. */
	//m_event_Instanceid = event_GetInstanceID();
	//m_holder = sEventMgr.GetEventHolder(m_event_Instanceid);
	m_refs = 1;
	m_holder = 0;
	m_event_Instanceid = -1;

	m_events.clear();
}

void EventableObject::event_AddEvent(TimedEvent * ptr)
{
	m_lock.Acquire();

	if( m_holder == NULL )
	{
		m_event_Instanceid = event_GetInstanceID();
		m_holder = sEventMgr.GetEventHolder(m_event_Instanceid);
        
        if( m_holder == NULL ){

            ///////////////////////////////////////// this is for me for debugging purposes - dfighter ////////////////////////////
            // Arcemu::Util::ARCEMU_ASSERT( false );
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            // We still couldn't find an eventholder for us so let's run in WorldRunnable
            m_event_Instanceid = WORLD_INSTANCE;
            m_holder = sEventMgr.GetEventHolder(m_event_Instanceid);
        }
	}

    // We still couldn't find an event holder for ourselves :(
    Arcemu::Util::ARCEMU_ASSERT(   m_holder != NULL );

    // If we are flagged not to run in WorldRunnable then we won't!
    // This is much better than adding us to the eventholder and removing on an update
    if( m_event_Instanceid == WORLD_INSTANCE && ( ptr->eventFlag & EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT ) ){
        delete ptr->cb;
        delete ptr;

        ///////////////////////////////////////// this is for me for debugging purposes - dfighter ////////////////////////////
        // Arcemu::Util::ARCEMU_ASSERT( false );
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_lock.Release();
        return;
    }

	ptr->IncRef();
	ptr->instanceId = m_event_Instanceid;
	pair<uint32,TimedEvent*> p(ptr->eventType, ptr);
	m_events.insert( p );
	m_lock.Release();

   
    /* Add to event manager */
	m_holder->AddEvent(ptr);
}

void EventableObject::event_RemoveByPointer(TimedEvent * ev)
{
	m_lock.Acquire();
	EventMap::iterator itr = m_events.find(ev->eventType);
	EventMap::iterator it2;
	if(itr != m_events.end())
	{
		do 
		{
			it2 = itr++;

			if(it2->second == ev)
			{
				it2->second->deleted = true;
				it2->second->DecRef();
				m_events.erase(it2);
				m_lock.Release();
				return;
			}

		} while(itr != m_events.upper_bound(ev->eventType));
	}
	m_lock.Release();
}

void EventableObject::event_RemoveEvents(uint32 EventType)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	if(EventType == EVENT_REMOVAL_FLAG_ALL)
	{
		EventMap::iterator itr = m_events.begin();
		for(; itr != m_events.end(); ++itr)
		{
			itr->second->deleted = true;
			itr->second->DecRef();
		}
		m_events.clear();
	}
	else
	{
		EventMap::iterator itr = m_events.find(EventType);
		EventMap::iterator it2;
		if(itr != m_events.end())
		{
			do 
			{
				it2 = itr++;

				it2->second->deleted = true;
				it2->second->DecRef();
				m_events.erase(it2);

			} while(itr != m_events.upper_bound(EventType));
		}
	}

	m_lock.Release();
}

void EventableObject::event_RemoveEvents()
{
	event_RemoveEvents(EVENT_REMOVAL_FLAG_ALL);
}

void EventableObject::event_ModifyTimeLeft(uint32 EventType, time_t TimeLeft,bool unconditioned)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if(unconditioned)
				itr->second->currTime = TimeLeft;
			else itr->second->currTime = (TimeLeft > itr->second->msTime) ? itr->second->msTime : TimeLeft;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
}

bool EventableObject::event_GetTimeLeft(uint32 EventType, time_t * Time)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return false;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if( itr->second->deleted )
			{
				++itr;
				continue;
			}

			*Time = (uint32)itr->second->currTime;
			m_lock.Release();
			return true;

		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
	return false;
}

void EventableObject::event_ModifyTime(uint32 EventType, time_t Time)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			itr->second->msTime = Time;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
}

void EventableObject::event_ModifyTimeAndTimeLeft(uint32 EventType, time_t Time)
{
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return;
	}

	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			itr->second->currTime = itr->second->msTime = Time;
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
}


bool EventableObject::event_HasEvent(uint32 EventType)
{
	bool ret = false;
	m_lock.Acquire();
	if(!m_events.size())
	{
		m_lock.Release();
		return false;
	}

	//ret = m_events.find(EventType) == m_events.end() ? false : true;
	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
	{
		do 
		{
			if(!itr->second->deleted)
			{
				ret = true;
				break;
			}
			++itr;
		} while(itr != m_events.upper_bound(EventType));
	}

	m_lock.Release();
	return ret;
}

EventableObjectHolder::EventableObjectHolder(int32 instance_id) : mInstanceId(instance_id)
{
	m_insertPool.clear();
	sEventMgr.AddEventHolder(this, instance_id);
}

EventableObjectHolder::~EventableObjectHolder()
{
	sEventMgr.RemoveEventHolder(this);

	/* decrement events reference count */
	m_lock.Acquire();
	EventList::iterator itr = m_events.begin();
	for(; itr != m_events.end(); ++itr)
		(*itr)->DecRef();
	m_lock.Release();
}

void EventableObjectHolder::Update(time_t time_difference)
{
	m_lock.Acquire();			// <<<<

	/* Insert any pending objects in the insert pool. */
	m_insertPoolLock.Acquire();
	InsertableQueue::iterator iqi;
	InsertableQueue::iterator iq2 = m_insertPool.begin();
	while(iq2 != m_insertPool.end())
	{
		iqi = iq2++;
		if((*iqi)->deleted || (*iqi)->instanceId != mInstanceId)
			(*iqi)->DecRef();
		else
			m_events.push_back( (*iqi) );

		m_insertPool.erase(iqi);
	}
	m_insertPoolLock.Release();

	/* Now we can proceed normally. */
	EventList::iterator itr = m_events.begin();
	EventList::iterator it2;
	TimedEvent * ev;

	while(itr != m_events.end())
	{
		it2 = itr++;

		if((*it2)->instanceId != mInstanceId || (*it2)->deleted ){

			(*it2)->DecRef();
			
			// remove from this list.
			m_events.erase(it2);
			
			continue;
		}

		// Event Update Procedure
		ev = *it2;

		if(ev->currTime <= time_difference)
		{
			// execute the callback
			if(ev->eventFlag & EVENT_FLAG_DELETES_OBJECT)
			{
				m_events.erase(it2);
				ev->deleted = true;
				ev->cb->execute();
				ev->DecRef();
				continue;
			}
			else
				ev->cb->execute();

			// check if the event is expired now.
            if(ev->repeats && --ev->repeats == 0)
			{
				// Event expired :>
				
				/* remove the event from the object */
				/*obj = (EventableObject*)ev->obj;
				obj->event_RemoveByPointer(ev);*/

				/* remove the event from here */
				ev->deleted = true;
				ev->DecRef();
				m_events.erase(it2);

				continue;
			}
			else if(ev->deleted)
			{
				// event is now deleted
				ev->DecRef(); //this was added on "addevent"
				m_events.erase(it2);
				continue;
			}

			// event has to repeat again, reset the timer
			ev->currTime = ev->msTime;
		}
		else
		{
			// event is still "waiting", subtract time difference
			ev->currTime -= time_difference;
		}
	}

	m_lock.Release();
}

void EventableObject::event_Relocate()
{
	/* prevent any new stuff from getting added */
	m_lock.Acquire();

	EventableObjectHolder * nh = sEventMgr.GetEventHolder(event_GetInstanceID());
	if(nh != m_holder)
	{
		// whee, we changed event holder :>
		// doing this will change the instanceid on all the events, as well as add to the new holder.
		
		//If nh is NULL then we were removed from world. There's no reason to be added to WORLD_INSTANCE EventMgr, let's just wait till something will add us again to world.
		if( nh == NULL )
		{
			//set instaceId to 0 to each event of this EventableObject, so EventableObjectHolder::Update() will remove them from its EventList.
			for( EventMap::iterator itr = m_events.begin(); itr != m_events.end(); ++itr )
			{
				itr->second->instanceId = 0;
			}
			// reset our instance id. 
			m_event_Instanceid = 0;
		}
		else
		{
			nh->AddObject(this);
			// reset our instance id
			m_event_Instanceid = nh->GetInstanceID();
		}
		// reset our m_holder pointer
		m_holder = nh;
	}

	/* safe again to add */
	m_lock.Release();
}

uint32 EventableObject::event_GetEventPeriod(uint32 EventType)
{
	uint32 ret = 0;
	m_lock.Acquire();
	EventMap::iterator itr = m_events.find(EventType);
	if(itr != m_events.end())
		ret = (uint32)itr->second->msTime;
	
	m_lock.Release();
	return ret;
}

void EventableObjectHolder::AddEvent(TimedEvent * ev)
{
	// m_lock NEEDS TO BE A RECURSIVE MUTEX
	ev->IncRef();
	if(!m_lock.AttemptAcquire())
	{
		m_insertPoolLock.Acquire();
		m_insertPool.push_back( ev );
		m_insertPoolLock.Release();
	}
	else
	{
		m_events.push_back( ev );
		m_lock.Release();
	}
}

void EventableObjectHolder::AddObject(EventableObject * obj)
{
	// transfer all of this objects events into our holder
	if(!m_lock.AttemptAcquire())
	{
		// The other thread is obviously occupied. We have to use an insert pool here, otherwise
		// if 2 threads relocate at once we'll hit a deadlock situation.
		m_insertPoolLock.Acquire();
		EventMap::iterator it2;

		for(EventMap::iterator itr = obj->m_events.begin(); itr != obj->m_events.end(); ++itr)
		{
			// ignore deleted events (shouldn't be any in here, actually)
			if(itr->second->deleted)
			{
				/*it2 = itr++;
				itr->second->DecRef();
				obj->m_events.erase(it2);*/
				continue;
			}

			itr->second->IncRef();	
			itr->second->instanceId = mInstanceId;
			m_insertPool.push_back(itr->second);
		}

		// Release the insert pool.
		m_insertPoolLock.Release();

		// Ignore the rest of this stuff
		return;
	}
	else
	{
		for(EventMap::iterator itr = obj->m_events.begin(); itr != obj->m_events.end(); ++itr)
		{
			// ignore deleted events
			if(itr->second->deleted)
				continue;

			itr->second->IncRef();
			itr->second->instanceId = mInstanceId;
			m_events.push_back( itr->second );
		}
		m_lock.Release();
	}
}
