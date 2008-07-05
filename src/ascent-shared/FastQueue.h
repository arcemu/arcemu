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

#ifndef _FASTQUEUE_H
#define _FASTQUEUE_H

/** dummy lock to use a non-locked queue.
 */
class DummyLock
{
public:
	ASCENT_INLINE void Acquire() { }
	ASCENT_INLINE void Release() { }
};

/** linked-list style queue
 */
template<class T, class LOCK>
class FastQueue
{
	struct node
	{
		T element;
		node * next;
	};

	node * last;
	node * first;
	LOCK m_lock;

public:

	FastQueue()
	{
		last = 0;
		first = 0;
	}

	~FastQueue()
	{
		Clear();
	}

	void Clear()
	{
		// clear any elements
		while(last != 0)
			Pop();
	}

	void Push(T elem)
	{
		m_lock.Acquire();
		node * n = new node;
		if(last)
			last->next = n;
		else
			first = n;

		last = n;
		n->next = 0;
		n->element = elem;
		m_lock.Release();
	}

	T Pop()
	{
		m_lock.Acquire();
		if(first == 0)
		{
			m_lock.Release();
			return reinterpret_cast<T>(0);
		}
        
		T ret = first->element;
		node * td = first;
		first = td->next;
		if(!first)
			last = 0;

		delete td;
		m_lock.Release();
		return ret;
	}

	T front()
	{
		m_lock.Acquire();
		if(first == 0)
		{
			m_lock.Release();
			return reinterpret_cast<T>(0);
		}

		T ret = first->element;
		m_lock.Release();
		return ret;
	}

	void pop_front()
	{
		m_lock.Acquire();
		if(first == 0)
		{
			m_lock.Release();
			return;
		}

		node * td = first;
		first = td->next;
		if(!first)
			last = 0;

		delete td;
		m_lock.Release();
	}

	bool HasItems()
	{
		bool ret;
		m_lock.Acquire();
		ret = (first != 0);
		m_lock.Release();
		return ret;
	}
};

#endif
