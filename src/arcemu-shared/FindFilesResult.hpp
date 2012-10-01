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

#ifndef ARCEMU_FFR_HPP
#define ARCEMU_FFR_HPP

#include <string>
#include <vector>

namespace Arcemu{
	///////////////////////////////////////////////////////
	//class FindFilesResult
	//  Contains the FindFile result.
	//  The found filenames can be retrieved as strings.
	//
	//
	////////////////////////////////////////////////////////
	class FindFilesResult{
	public:
		FindFilesResult(){
			filenames.reserve( 10 );
			pos = 0;
		}

		~FindFilesResult(){}


		//////////////////////////////////////////////////////////
		//std::vector< std::string >::size_type Size()
		//  Returns the size of the result set
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the size of the result set
		//
		//
		//////////////////////////////////////////////////////////
		std::vector< std::string >::size_type Size(){
			return filenames.size();
		}

		//////////////////////////////////////////////////////////
		//bool HasNext() const
		//  Tells if there are more filenames in the result set
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns true if there are more.
		//  Returns false otherwise.
		//
		//
		//////////////////////////////////////////////////////////
		bool HasNext() const{
			if( pos < filenames.size() )
				return true;
			else
				return false;
		}

		//////////////////////////////////////////////////////////
		//const std::string& GetNext()
		//  Returns the next result filename
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  Returns the next result filename
		//
		//
		//////////////////////////////////////////////////////////
		const std::string& GetNext(){
			const std::string &res = filenames[ pos ];
			++pos;

			return res;
		}


		//////////////////////////////////////////////////////////
		//void Reset()
		//  Resets the next result string pointer, to point to
		//  the first result.
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////////
		void Reset(){
			pos = 0;
		}


		//////////////////////////////////////////////////////////
		//void Clear()
		//  Clears the result object, removing all result strings
		//
		//Parameter(s)
		//  None
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////////
		void Clear(){
			filenames.clear();
			pos = 0;
		}


		//////////////////////////////////////////////////////////
		//void Add( const char *s )
		//  Adds a new result string to the result object
		//
		//Parameter(s)
		//  const char *s  -  string to add
		//
		//Return Value
		//  None
		//
		//
		//////////////////////////////////////////////////////////
		void Add( std::string s ){
			if( ( s == "." ) || ( s == ".." ) )
				return;

			filenames.push_back( std::string( s ) );
		}

	private:
		std::vector< std::string > filenames;
		std::vector< std::string >::size_type pos;
	};

}

#endif
