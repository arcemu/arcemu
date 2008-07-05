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

#ifndef FQUEUE_H
#define FQUEUE_H

#include "Condition.h"
#include "Mutex.h"
 
template<class T> 
class FQueue 
{
public:
	ASCENT_INLINE FQueue() : cond(&lock) {first=last=NULL;size=0;}
	volatile unsigned int size;

	uint32 get_size()
	{
		uint32 ret;
		cond.BeginSynchronized();
		ret = size;
		cond.EndSynchronized();
		return ret;
	}

	void push(T &item)
	{
		h*p=new h;
		p->value=item;
		p->pNext=NULL;
		
		//lock.Acquire();
		cond.BeginSynchronized();
		if(last)//have some items
		{
			last->pNext=p;
			last=p;
			size++;
		}
		else//first item
		{
			last=first=p;
			size=1;
			cond.Signal();
		}
		//lock.Release();
		cond.EndSynchronized();
	}

	T pop_nowait()
	{
		//lock.Acquire();
		cond.BeginSynchronized();
		if(size==0)
		{
			cond.EndSynchronized();
			return NULL;
		}

		h*tmp=first;
		if(tmp == NULL)
		{
			cond.EndSynchronized();
			return NULL;
		}

		if(--size)//more than 1 item
		{
			first=(h*)first->pNext;
		}
		else//last item
		{
			first=last=NULL;
		}
		//lock.Release();
		cond.EndSynchronized();

		T returnVal = tmp->value;
		delete tmp;

		return returnVal;
	}

	T pop()
	{
		//lock.Acquire();
		cond.BeginSynchronized();
		if(size==0)
		cond.Wait();

		h*tmp=first;
		if(tmp == NULL)
		{
			cond.EndSynchronized();
			return NULL;
		}

		if(--size)//more than 1 item
		{
			first=(h*)first->pNext;
		}
		else//last item
		{
			first=last=NULL;
		}
		//lock.Release();
		cond.EndSynchronized();

		T returnVal = tmp->value;
		delete tmp;
		
		return returnVal;
	}	

	ASCENT_INLINE Condition& GetCond() { return cond; }
	
private:
	struct h
	{
		T value;
		void *pNext;
	};

	h*first;
	h*last;
	
	Mutex lock;
	Condition cond;

};

#endif 


