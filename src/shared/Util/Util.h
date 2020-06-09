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

#ifndef _UTIL_H
#define _UTIL_H

#include "System/Platform.h"

#ifdef WIN32

typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // must be 0x1000
	LPCSTR szName; // pointer to name (in user addr space)
	DWORD dwThreadID; // thread ID (-1=caller thread)
	DWORD dwFlags; // reserved for future use, must be zero
} THREADNAME_INFO;

#endif

namespace Arcemu
{
	namespace Shared
	{
		class SERVER_DECL Util
		{
		public:
			template< typename T >
			static void reverse_array(T* pointer, size_t count)
			{
				size_t i = 0;
				size_t j = (int)( count - 1 );

				while( j > i )
				{
					T temp = pointer[ i ];
					pointer[ i ] = pointer[ j ];
					pointer[ j ] = temp;
					++i;
					--j;
				}
			}

			static void swap32(uint32 &data);

			inline static uint32 secsToTimeBitFields(time_t secs)
			{
				tm* lt = localtime(&secs);
				return (lt->tm_year - 100) << 24 | lt->tm_mon  << 20 | (lt->tm_mday - 1) << 14 | lt->tm_wday << 11 | lt->tm_hour << 6 | lt->tm_min;
			}

			// This HAS to be called outside the threads __try / __except block!
			static void SetThreadName(const char* format, ...);

			static float round(float f);
			
			static double round(double d);
			
			static long double round(long double ld);

			/////////////////////////////////////////////////////////////////////////
			//void Sleep( unsigned long timems );
			//  Puts the calling thread to sleep for the specified miliseconds
			//
			//Parameter(s)
			//  unsigned long timemes  -  time interval to put the thread to sleep for
			//
			//Return Value
			//  None
			//
			//
			/////////////////////////////////////////////////////////////////////////
			void static Sleep(unsigned long timems);

			static std::vector<std::string> StrSplit(const std::string & src, const std::string & sep);

			static time_t convTimePeriod(uint32 dLength, char dType);

			static int32 GetTimePeriodFromString(const char* str);
			
			static std::string ConvertTimeStampToString(uint32 timestamp);

			static std::string ConvertTimeStampToDateTime(uint32 timestamp);

			static uint32 DecimalToMask(uint32 dec);

			static void arcemu_TOLOWER(std::string & str);

			static void arcemu_TOUPPER(std::string & str);

			static void CapitalizeString(std::string & arg);

			// returns true if the ip hits the mask, otherwise false
			static bool ParseCIDRBan(unsigned int IP, unsigned int Mask, unsigned int MaskBits);

			static unsigned int MakeIP(const char* str);

			/////////////////////////////////////////////////////////
			//uint32 getMSTime()
			//  Returns the time elapsed in milliseconds
			//
			//Parameter(s)
			//  None
			//
			//Return Value
			//  Returns the time elapsed in milliseconds
			//
			//
			/////////////////////////////////////////////////////////
			ARCEMU_INLINE static uint32 getMSTime()
			{
				uint32 MSTime = 0;
			#ifdef WIN32
				MSTime = GetTickCount();
			#else
				timeval tv;
				gettimeofday(&tv, NULL);
				MSTime = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
			#endif
				return MSTime;
			}
		};
	}
}

#endif
