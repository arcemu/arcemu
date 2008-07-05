<<<<<<< .working
<<<<<<< .working
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

#ifndef RWLOCK_H
#define RWLOCK_H

#include "Condition.h"
#include "Mutex.h"

class RWLock
{
public: 
  
	ASCENT_INLINE void AcquireReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_readers++;
		//_lock.Release();
		_cond.EndSynchronized();
	}
	
	ASCENT_INLINE void ReleaseReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		if(!(--_readers))
			if(_writers)
				_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}

	ASCENT_INLINE void AcquireWriteLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_writers++;
		if(_readers)
			_cond.Wait();
	}

	ASCENT_INLINE void ReleaseWriteLock()
	{
		if(--_writers)
			_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}
	ASCENT_INLINE RWLock() : _cond(&_lock) {_readers=0;_writers=0;}
  
	private:
		Mutex _lock;
		Condition _cond;
		volatile unsigned int _readers;
		volatile unsigned int _writers;
   
}; 

#endif
=======
/*
 * ArcEmu MMORPG Server
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

#ifndef RWLOCK_H
#define RWLOCK_H

#include "Condition.h"
#include "Mutex.h"

class RWLock
{
public: 
  
	ASCENT_INLINE void AcquireReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_readers++;
		//_lock.Release();
		_cond.EndSynchronized();
	}
	
	ASCENT_INLINE void ReleaseReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		if(!(--_readers))
			if(_writers)
				_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}

	ASCENT_INLINE void AcquireWriteLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_writers++;
		if(_readers)
			_cond.Wait();
	}

	ASCENT_INLINE void ReleaseWriteLock()
	{
		if(--_writers)
			_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}
	ASCENT_INLINE RWLock() : _cond(&_lock) {_readers=0;_writers=0;}
  
	private:
		Mutex _lock;
		Condition _cond;
		volatile unsigned int _readers;
		volatile unsigned int _writers;
   
}; 

#endif
>>>>>>> .merge-right.r249
=======
/*
 * ArcEmu MMORPG Server
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

#ifndef RWLOCK_H
#define RWLOCK_H

#include "Condition.h"
#include "Mutex.h"

class RWLock
{
public: 
  
	ASCENT_INLINE void AcquireReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_readers++;
		//_lock.Release();
		_cond.EndSynchronized();
	}
	
	ASCENT_INLINE void ReleaseReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		if(!(--_readers))
			if(_writers)
				_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}

	ASCENT_INLINE void AcquireWriteLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_writers++;
		if(_readers)
			_cond.Wait();
	}

	ASCENT_INLINE void ReleaseWriteLock()
	{
		if(--_writers)
			_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}
	ASCENT_INLINE RWLock() : _cond(&_lock) {_readers=0;_writers=0;}
  
	private:
		Mutex _lock;
		Condition _cond;
		volatile unsigned int _readers;
		volatile unsigned int _writers;
   
}; 

#endif
>>>>>>> .merge-right.r249
