/*
 * ArcEmu MMORPG Server
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

#ifndef ARCEMU_DYNLIB_HPP
#define ARCEMU_DYNLIB_HPP

#include <string>

namespace Arcemu{
	//////////////////////////////////////////////////////
	//class DynLib
	//  Dynamic Library ( dll / so / dylib ) handler class
	//
	//
	//////////////////////////////////////////////////////
	class DynLib{

	public:
		//////////////////////////////////////////////////////
		//DynLib( const char *libfilename = "" )
		//  Constructor of the class
		//
		//Parameter(s)
		//  const char *libfilename  - filename with path
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////
		DynLib( const char *libfilename = "" );

		~DynLib();


		//////////////////////////////////////////////////////
		//bool Load()
		//  Loads the library if possible.
		//  Sets the error state of the class to true on error
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns true on success.
		//  Returns false on failure.
		//
		//
		//////////////////////////////////////////////////////
		bool Load();


		//////////////////////////////////////////////////////
		//void Close()
		//  Closes the library if possible.
		//  Sets the error state of the class on error.
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////
		void Close();


		//////////////////////////////////////////////////////
		//void* GetAddressForSymbol( const char *symbol )
		//  Returns the address of the symbol.
		//  Sets the error state of the class on error
		//
		//Parameter(s)
		//  const char *symbol  -  identifier of the symbol
		//                         to be looked up.
		//
		//Return Value
		//  Returns the address on success.
		//  Returns NULL on failure.
		//
		//
		//////////////////////////////////////////////////////
		void* GetAddressForSymbol( const char *symbol );


		//////////////////////////////////////////////////////
		//bool Error()
		//  Returns the last error state of the class, and
		//  resets it to false.
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the last error state
		//
		//
		//////////////////////////////////////////////////////
		bool Error(){
			bool lasterror = error;
			
			error = false;
			
			return lasterror;
		}


		//////////////////////////////////////////////////////
		//std::string GetName()
		//  Returns the name of the library loaded
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the name of the library loaded
		//
		//
		//////////////////////////////////////////////////////
		std::string GetName(){
			return filename;
		}

	private:
		std::string filename;  // filename of the library file
		void *lptr;            // pointer to the opened library
		bool error;            // last error state
	};
}

#endif

