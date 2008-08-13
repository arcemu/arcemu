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

#ifndef WOWSERVER_SINGLETON_H
#define WOWSERVER_SINGLETON_H

#include "Errors.h"

/// Should be placed in the appropriate .cpp file somewhere
#define initialiseSingleton( type ) \
  template <> type * Singleton < type > :: mSingleton = 0

#define initialiseTemplateSingleton( temp, type ) \
  template <> temp< type > * Singleton < temp< type > > :: mSingleton = 0

/// To be used as a replacement for initialiseSingleton( )
///  Creates a file-scoped Singleton object, to be retrieved with getSingleton
#define createFileSingleton( type ) \
  initialiseSingleton( type ); \
  type the##type

template < class type > class SERVER_DECL Singleton {
public:
  /// Constructor
  Singleton( ) {
	/// If you hit this assert, this singleton already exists -- you can't create another one!
	WPAssert( this->mSingleton == 0 );
	this->mSingleton = static_cast<type *>(this);
  }
  /// Destructor
  virtual ~Singleton( ) {
	this->mSingleton = 0;
  }

  ARCEMU_INLINE static type & getSingleton( ) { WPAssert( mSingleton ); return *mSingleton; }
  ARCEMU_INLINE static type * getSingletonPtr( ) { return mSingleton; }

protected:

  /// Singleton pointer, must be set to 0 prior to creating the object
  static type * mSingleton;
};
#endif
