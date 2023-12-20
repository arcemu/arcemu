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

#ifndef ARCEMU_PLATFORM_H
#define ARCEMU_PLATFORM_H

#ifdef WIN32
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_DEPRECATE 1
#define _CRT_SECURE_COPP_OVERLOAD_STANDARD_NAMES 1
#pragma warning(disable:4251)		// dll-interface bullshit
#endif

enum TimeVariables
{
    TIME_SECOND = 1,
    TIME_MINUTE = TIME_SECOND * 60,
    TIME_HOUR   = TIME_MINUTE * 60,
    TIME_DAY	= TIME_HOUR * 24,
    TIME_MONTH	= TIME_DAY * 30,
    TIME_YEAR	= TIME_MONTH * 12
};

enum MsTimeVariables
{
    MSTIME_SECOND = 1000,
    MSTIME_6SECONDS = MSTIME_SECOND * 6,
    MSTIME_MINUTE = MSTIME_SECOND * 60,
    MSTIME_HOUR   = MSTIME_MINUTE * 60,
    MSTIME_DAY	  = MSTIME_HOUR * 24
};

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <cstdlib>
#include <cstdio>

#include <cstdarg>
#include <ctime>
#include <cmath>
#include <cerrno>

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
//#  define _WIN32_WINNT 0x0500
#  define NOMINMAX
#  include <windows.h>
#  undef NOMINMAX
#else
#  include <cstring>
#  define MAX_PATH 1024
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifdef CONFIG_USE_SELECT
#undef FD_SETSIZE
#define FD_SETSIZE 2048
#endif

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#endif

#define PLATFORM_WINDOWS 0
#define PLATFORM_UNIX    1
#define PLATFORM_APPLE   2

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
	#define ARCEMU_PLATFORM PLATFORM_WINDOWS
#elif defined( __APPLE_CC__ )
	#define ARCEMU_PLATFORM PLATFORM_APPLE
#else
	#define ARCEMU_PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MSVC 0
#define COMPILER_GCC  1

#ifdef _MSC_VER
	#define ARCEMU_COMPILER COMPILER_MSVC
	#define ARCEMU_COMPILER_VER _MSC_VER
#elif defined( __GNUC__ )
	#define ARCEMU_COMPILER COMPILER_GCC

	#define GCC_VERSION (__GNUC__ * 10000 \
					   + __GNUC_MINOR__ * 100 \
					   + __GNUC_PATCHLEVEL__)

	#define ARCEMU_COMPILER_VER GCC_VERSION
#else
	#error "Unknown compiler"
#endif

#if ARCEMU_COMPILER == COMPILER_MSVC
	#define ARCEMU_FORCEINLINE __forceinline
#else
	#define ARCEMU_FORCEINLINE inline
#endif

#define ARCEMU_INLINE inline

#define UNIX_FLAVOUR_LINUX 1
#define UNIX_FLAVOUR_BSD 2
#define UNIX_FLAVOUR_OTHER 3
#define UNIX_FLAVOUR_OSX 4

#if ARCEMU_PLATFORM == PLATFORM_UNIX || ARCEMU_PLATFORM == PLATFORM_APPLE
	#ifdef HAVE_DARWIN
		#define PLATFORM_TEXT "MacOS"
		#define UNIX_FLAVOUR UNIX_FLAVOUR_OSX
	#else
		#ifdef USE_KQUEUE
			#define PLATFORM_TEXT "BSD"
			#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
		#else
			#define PLATFORM_TEXT "Linux"
			#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX
		#endif
	#endif
#elif ARCEMU_PLATFORM == PLATFORM_WINDOWS
	#define PLATFORM_TEXT "Windows"
#endif

#ifdef _DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif

#ifdef X64
    #if defined( __riscv )
		#define ARCEMU_ARCH_TXT "riscv64"
    #elif defined( __x86_64__ ) || defined( _M_X64 )
		#define ARCEMU_ARCH_TXT "amd64"
	#else
		#error "Unknown architecture"
    #endif
#else
	#if defined( __riscv )
		#error "32 bit RISC V architecture is not supported"
	#elif defined( i386 ) || defined( __i386__ ) || defined( __i386 ) || defined( _M_IX86 )
		#error "32 bit x86 architecure is not supported"
	#else
		#error "Unsupported unknown architecture"
	#endif
#endif


#if ARCEMU_PLATFORM == PLATFORM_WINDOWS
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#if ARCEMU_PLATFORM == PLATFORM_WINDOWS
#define ASYNC_NET
#endif

#ifdef USE_EPOLL
#define CONFIG_USE_EPOLL
#endif
#ifdef USE_KQUEUE
#define CONFIG_USE_KQUEUE
#endif
#ifdef USE_SELECT
#define CONFIG_USE_SELECT
#endif
#ifdef USE_POLL
#define CONFIG_USE_POLL
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include <cstdlib>
#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <climits>
#include <cstdlib>
//#include <iostream>

#if ARCEMU_COMPILER == COMPILER_GCC
#	define LIKELY( _x ) \
		__builtin_expect( ( _x ), 1 )
#	define UNLIKELY( _x ) \
 		__builtin_expect( ( _x ), 0 )
#else
#	define LIKELY( _x ) \
		_x
#	define UNLIKELY( _x ) \
		_x
#endif

#if ARCEMU_PLATFORM != PLATFORM_WINDOWS
#ifndef X64
#  if ARCEMU_COMPILER == COMPILER_GCC
#	if ARCEMU_COMPILER_VER >= 30400
#         ifdef HAVE_DARWIN
#	      define __fastcall
#         else
#    	      define __fastcall __attribute__((__fastcall__))
#         endif
#	else
#	  define __fastcall __attribute__((__regparm__(3)))
#	endif
#  else
#	define __fastcall __attribute__((__fastcall__))
#  endif
#else
#define __fastcall
#endif
#endif

#ifndef HAS_CXX0X
#if ARCEMU_COMPILER == COMPILER_MSVC && ARCEMU_COMPILER_VER > 1600
#define HAS_CXX0X
#endif
#endif

/*
	TEST SUPPORT FOR TR1
	*/
#ifdef HAS_CXX0X
#include <unordered_map>
#include <unordered_set>
#define HM_NAMESPACE ::std
#define HM_HASH_MAP unordered_map
#define HM_HASH_SET unordered_set
#elif ARCEMU_COMPILER == COMPILER_GCC && ARCEMU_COMPILER_VER >= 30400
#include <ext/hash_map>
#include <ext/hash_set>
#define HM_NAMESPACE __gnu_cxx
#define HM_HASH_MAP hash_map
#define HM_HASH_SET hash_set

namespace __gnu_cxx
{
	template<> struct hash<unsigned long long>
	{
		size_t operator()(const unsigned long long & __x) const { return (size_t)__x; }
	};
	template<typename T> struct hash<T*>
	{
		size_t operator()(T* const & __x) const { return (size_t)__x; }
	};
	//support for std::strings as keys to hash maps
	template<> struct hash< ::std::string>
	{
		size_t operator()(const ::std::string & keyval) const
		{
			return hash<const char*>()(keyval.c_str());
		}
	};
};
#else
#define HM_NAMESPACE ::stdext
#define HM_HASH_MAP hash_map
#define HM_HASH_SET hash_set
#include <hash_map>
#include <hash_set>
#endif

/* Use correct types for x64 platforms, too */
#if ARCEMU_COMPILER == COMPILER_MSVC
	typedef signed __int64 int64;
	typedef signed __int32 int32;
	typedef signed __int16 int16;
	typedef signed __int8 int8;
	
	typedef unsigned __int64 uint64;
	typedef unsigned __int32 uint32;
	typedef unsigned __int16 uint16;
	typedef unsigned __int8 uint8;

#elif ARCEMU_COMPILER == COMPILER_GCC
	typedef int64_t int64;
	typedef int32_t int32;
	typedef int16_t int16;
	typedef int8_t int8;
	
	typedef uint64_t uint64;
	typedef uint32_t uint32;
	typedef uint16_t uint16;
	typedef uint8_t uint8;
	//typedef uint32_t DWORD;
#else
	#error "Unknown compiler"
#endif

/*
Scripting system exports/imports
*/

#if ARCEMU_COMPILER == COMPILER_MSVC
#ifndef SCRIPTLIB
#define SERVER_DECL __declspec(dllexport)
#define SCRIPT_DECL __declspec(dllimport)
#else
#define SERVER_DECL __declspec(dllimport)
#define SCRIPT_DECL __declspec(dllexport)
#endif
#define DECL_LOCAL
#elif ARCEMU_COMPILER == COMPILER_GCC && ARCEMU_COMPILER_VER >= 40000
#define SERVER_DECL __attribute__((visibility ("default")))
#define SCRIPT_DECL __attribute__((visibility ("default")))
#define DECL_LOCAL __attribute__((visibility ("hidden")))
#else
#define SERVER_DECL
#define SCRIPT_DECL
#define DECL_LOCAL
#endif

// Include all threading files
#include <cassert>
#include "Threading/Threading.h"

#include "Threading/AtomicULong.h"
#include "Threading/AtomicFloat.h"
#include "Threading/AtomicCounter.h"
#include "Threading/AtomicBoolean.h"
#include "Threading/ConditionVariable.h"

#include "Threading/CRefCounter.h"

#include "Util/MersenneTwister.h"

#if ARCEMU_COMPILER == COMPILER_MSVC

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016lX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#endif

#define atol(a) strtoul( a, NULL, 10)

#define STRINGIZE(a) #a

// fix buggy MSVC's for variable scoping to be reliable =S
#define for if(true) for

#if ARCEMU_COMPILER == COMPILER_MSVC && ARCEMU_COMPILER_VER >= 1400
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

#if ARCEMU_COMPILER == COMPILER_MSVC && ARCEMU_COMPILER_VER >= 1400
#pragma float_control(pop)
#endif

#if ARCEMU_PLATFORM != PLATFORM_WINDOWS
#include <sys/timeb.h>
#endif

#if ARCEMU_PLATFORM != PLATFORM_WINDOWS
#define FALSE   0
#define TRUE	1
#endif

#include "System/DynLib.hpp"
#include "System/FindFiles.hpp"
#include "System/SysInfo.hpp"
#include "System/PerformanceCounter.hpp"

#endif
