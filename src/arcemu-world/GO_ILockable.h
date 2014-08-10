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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
*/
namespace Arcemu{
///////////////////////////////////////////////////////
//class GO_ILockable
// Lockable Interface for lockable GameObjects
//
//////////////////////////////////////////////////////
class GO_ILockable{
public:
/////////////////////////////////////////////////////
//void Lock()
// Locks the GameObject
//
//Parameters
// None
//
//Return Value
// None
//
//
////////////////////////////////////////////////////
virtual void Lock() = 0;
/////////////////////////////////////////////////////
//void UnLock()
// UnLocks the GameObject
//
//Parameters
// None
//
//Return Value
// None
//
//
////////////////////////////////////////////////////
virtual void UnLock() = 0;
///////////////////////////////////////////////////
//bool IsLocked()
// Tells if the GameObject is locked.
//
//Parameters
// None
//
//Return Value
// Returns true if it's locked.
// Returns false otherwise.
//
//////////////////////////////////////////////////
virtual bool IsLocked() = 0;
/////////////////////////////////////////////////
//unsigned long GetLockId()
// Returns the Lock ID ( index of Lock.dbc )
//
//Parameters
// None
//
//Return Value
// Returns the Lock ID
//
//
////////////////////////////////////////////////
virtual unsigned long GetLockId() = 0;
};
}