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

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "DynLib.hpp"

#ifdef WIN32

std::string decodeWindowsError( DWORD error )
{
	std::string messageText;

	char *message = NULL;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		error,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		(LPTSTR)&message,
		0,
		NULL
	);

	if( message != NULL )
	{
		messageText.append( message );
		LocalFree( message );
	}
	else
	{
		messageText.append( "No message for this error" );
	}

	return messageText;
}

#endif

namespace Arcemu
{

	DynLib::DynLib(const char* libfilename)
	{
		if(libfilename == NULL)
			throw "Dynlib::Dynlib() encountered NULL library filename";

		filename = libfilename;
		lptr = NULL;
		error = false;
	}

	DynLib::~DynLib()
	{
		Close();
	}

#ifdef WIN32

	bool DynLib::Load()
	{
		lptr = LoadLibrary(filename.c_str());

		if(lptr != NULL)
		{
			return true;
		}
		else
		{
			error = true;
			return false;
		}
	}

	void* DynLib::GetAddressForSymbol(const char* symbol)
	{
		void* address = NULL;

		address = GetProcAddress(reinterpret_cast< HMODULE >(lptr), symbol);

		if(address == NULL)
			error = true;

		return address;
	}

	std::string DynLib::GetLastError()
	{
		std::string error;

		DWORD errorCode = ::GetLastError();
		error = decodeWindowsError( errorCode );

		return error;
	}

	void DynLib::Close()
	{

		if(lptr != NULL)
		{
			int err = 0;

			err = FreeLibrary(reinterpret_cast< HMODULE >(lptr));

			if(err != 0)
				error = true;
		}
	}


#else

	bool DynLib::Load()
	{
		lptr = dlopen(filename.c_str(), RTLD_NOW);

		if(lptr != NULL)
		{
			return true;
		}
		else
		{
			error = true;
			return false;
		}
	}

	void* DynLib::GetAddressForSymbol(const char* symbol)
	{
		void* address = NULL;

		address = dlsym(lptr, symbol);

		if(address == NULL)
			error = true;

		return address;
	}

	std::string DynLib::GetLastError()
	{
		std::string error;
		error.append( dlerror() );
		return error;
	}

	void DynLib::Close()
	{

		if(lptr != NULL)
		{
			int err = 0;

			err = dlclose(lptr);

			if(err != 0)
				error = true;
		}
	}


#endif

}
