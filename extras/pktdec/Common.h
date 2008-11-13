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

#ifndef WOWSERVER_COMMON_H
#define WOWSERVER_COMMON_H

/* Define these if you are creating a repack */
/*
#define REPACK "Moocow's Repack"
#define REPACK_AUTHOR "Trelorn"
#define REPACK_WEBSITE "www.google.com"*/

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
	TIME_YEAR	= TIME_MONTH * 12,
};

enum MsTimeVariables
{
	MSTIME_SECOND = 1000,
	MSTIME_MINUTE = MSTIME_SECOND * 60,
	MSTIME_HOUR   = MSTIME_MINUTE * 60,
	MSTIME_DAY	= MSTIME_HOUR * 24,
};

#ifdef WIN32
#define ARCEMU_INLINE __forceinline
#else
#define ARCEMU_INLINE inline
#endif

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* Define this if you're using a big-endian machine */
#ifdef USING_BIG_ENDIAN
#include <machine/byte_order.h>
#define bswap_16(x) NXSwapShort(x)
#define bswap_32(x) NXSwapInt(x)
#define bswap_64(x) NXSwapLongLong(x)
#endif

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <errno.h>

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
#  define _WIN32_WINNT 0x0500
#  define NOMINMAX
#  include <windows.h>
#else
#  include <string.h>
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

// current platform and compiler
#define PLATFORM_WIN32 0
#define PLATFORM_UNIX  1
#define PLATFORM_APPLE 2

#define UNIX_FLAVOUR_LINUX 1
#define UNIX_FLAVOUR_BSD 2
#define UNIX_FLAVOUR_OTHER 3
#define UNIX_FLAVOUR_OSX 4

#if defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define PLATFORM PLATFORM_WIN32
#elif defined( __APPLE_CC__ )
#  define PLATFORM PLATFORM_APPLE
#else
#  define PLATFORM PLATFORM_UNIX
#endif

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1
#define COMPILER_BORLAND   2

#ifdef _MSC_VER
#  define COMPILER COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#  define COMPILER COMPILER_BORLAND
#elif defined( __GNUC__ )
#  define COMPILER COMPILER_GNU
#else
#  pragma error "FATAL ERROR: Unknown compiler."
#endif

#if PLATFORM == PLATFORM_UNIX || PLATFORM == PLATFORM_APPLE
#ifdef HAVE_DARWIN
#define PLATFORM_TEXT "MacOSX"
#define UNIX_FLAVOUR UNIX_FLAVOUR_OSX
#else
#ifdef USE_KQUEUE
#define PLATFORM_TEXT "FreeBSD"
#define UNIX_FLAVOUR UNIX_FLAVOUR_BSD
#else
#define PLATFORM_TEXT "Linux"
#define UNIX_FLAVOUR UNIX_FLAVOUR_LINUX
#endif
#endif
#endif

#if PLATFORM == PLATFORM_WIN32
#define PLATFORM_TEXT "Win32"
#endif

#ifdef _DEBUG
#define CONFIG "Debug"
#else
#define CONFIG "Release"
#endif

#ifdef USING_BIG_ENDIAN
#define ARCH "PPC"
#else
#ifdef X64
#define ARCH "X64"
#else
#define ARCH "X86"
#endif
#endif

/*#if COMPILER == COMPILER_MICROSOFT
#  pragma warning( disable : 4267 ) // conversion from 'size_t' to 'int', possible loss of data
#  pragma warning( disable : 4311 ) // 'type cast': pointer truncation from HMODULE to uint32
#  pragma warning( disable : 4786 ) // identifier was truncated to '255' characters in the debug information
#  pragma warning( disable : 4146 )
#  pragma warning( disable : 4800 )
#endif*/

#if PLATFORM == PLATFORM_WIN32
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

#if PLATFORM == PLATFORM_WIN32
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

#if defined (__GNUC__)
#  define GCC_VERSION (__GNUC__ * 10000 \
					   + __GNUC_MINOR__ * 100 \
					   + __GNUC_PATCHLEVEL__)
#endif


#ifndef WIN32
#ifndef X64
#  if defined (__GNUC__)
#	if GCC_VERSION >= 30400
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

#if COMPILER == COMPILER_GNU && __GNUC__ >= 3
#include <ext/hash_map>
#include <ext/hash_set>
#else
#include <hash_map>
#include <hash_set>
#endif



#ifdef _STLPORT_VERSION
#define HM_NAMESPACE std
using std::hash_map;
using std::hash_set;
#elif COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1300
#define HM_NAMESPACE stdext
using stdext::hash_map;
using stdext::hash_set;
#define ENABLE_SHITTY_STL_HACKS 1

// hacky stuff for vc++
#define snprintf _snprintf
#define vsnprintf _vsnprintf
//#define strlen lstrlen

#ifdef WIN32
#define __T(x) x
#endif

// cebernic added it
#define __utf8(x) _StringToUTF8(x)
#define __ansi(x) _StringToANSI(x)
#define __isutf8(x) _IsStringUTF8(x)


#elif COMPILER == COMPILER_INTEL
#define HM_NAMESPACE std
using std::hash_map;
using std::hash_set;
#elif COMPILER == COMPILER_GNU && __GNUC__ >= 3
#define HM_NAMESPACE __gnu_cxx
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

namespace __gnu_cxx
{
	template<> struct hash<unsigned long long>
	{
		size_t operator()(const unsigned long long &__x) const { return (size_t)__x; }
	};
	template<typename T> struct hash<T *>
	{
		size_t operator()(T * const &__x) const { return (size_t)__x; }
	};

};

#else
#define HM_NAMESPACE std
using std::hash_map;
#endif

/* Use correct types for x64 platforms, too */
#if COMPILER != COMPILER_GNU
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
#else

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef uint32_t DWORD;

#endif

/* these can be optimized into assembly */
#ifdef USING_BIG_ENDIAN

/*ARCEMU_INLINE static void swap16(uint16* p) { *p = ((*p >> 8) & 0xff) | (*p << 8); }
ARCEMU_INLINE static void swap32(uint32* p) { *p = ((*p >> 24 & 0xff)) | ((*p >> 8) & 0xff00) | ((*p << 8) & 0xff0000) | (*p << 24); }
ARCEMU_INLINE static void swap64(uint64* p) { *p = ((*p >> 56)) | ((*p >> 40) & 0x000000000000ff00ULL) | ((*p >> 24) & 0x0000000000ff0000ULL) | ((*p >> 8 ) & 0x00000000ff000000ULL) |
								((*p << 8 ) & 0x000000ff00000000ULL) | ((*p << 24) & 0x0000ff0000000000ULL) | ((*p << 40) & 0x00ff000000000000ULL) | ((*p << 56)); }*/

ARCEMU_INLINE static void swap16(uint16* p) { *p = bswap_16((uint16_t)*p); }
ARCEMU_INLINE static void swap32(uint32* p) { *p = bswap_32((uint32_t)*p); }
ARCEMU_INLINE static void swap64(uint64* p) { *p = bswap_64((uint64_t)*p);; }

ARCEMU_INLINE static float swapfloat(float p)
{
	union { float asfloat; uint8 asbytes[4]; } u1, u2;
	u1.asfloat = p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
    
	return u2.asfloat;
}

ARCEMU_INLINE static double swapdouble(double p)
{
	union { double asfloat; uint8 asbytes[8]; } u1, u2;
	u1.asfloat = p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];

	return u2.asfloat;
}

ARCEMU_INLINE static void swapfloat(float * p)
{
	union { float asfloat; uint8 asbytes[4]; } u1, u2;
	u1.asfloat = *p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
	*p = u2.asfloat;
}

ARCEMU_INLINE static void swapdouble(double * p)
{
	union { double asfloat; uint8 asbytes[8]; } u1, u2;
	u1.asfloat = *p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];
	*p = u2.asfloat;
}

/*ARCEMU_INLINE static uint16 swap16(uint16 p) { return ((p >> 8) & 0xff) | (p << 8); }
ARCEMU_INLINE static uint32 swap32(uint32 p) { return ((p >> 24) & 0xff) | ((p >> 8) & 0xff00) | ((p << 8) & 0xff0000) | (p << 24); }
ARCEMU_INLINE static uint64 swap64(uint64 p)  { p = (((p >> 56) & 0xff)) | ((p >> 40) & 0x000000000000ff00ULL) | ((p >> 24) & 0x0000000000ff0000ULL) | ((p >> 8 ) & 0x00000000ff000000ULL) |
								((p << 8 ) & 0x000000ff00000000ULL) | ((p << 24) & 0x0000ff0000000000ULL) | ((p << 40) & 0x00ff000000000000ULL) | ((p << 56)); }

ARCEMU_INLINE static void swap16(int16* p) { *p = ((*p >> 8) & 0xff) | (*p << 8); }
ARCEMU_INLINE static void swap32(int32* p) { *p = ((*p >> 24) & 0xff) | ((*p >> 8) & 0xff00) | ((*p << 8) & 0xff0000) | (*p << 24); }
ARCEMU_INLINE static void swap64(int64* p) { *p = ((*p >> 56) & 0xff) | ((*p >> 40) & 0x000000000000ff00ULL) | ((*p >> 24) & 0x0000000000ff0000ULL) | ((*p >> 8 ) & 0x00000000ff000000ULL) |
								((*p << 8 ) & 0x000000ff00000000ULL) | ((*p << 24) & 0x0000ff0000000000ULL) | ((*p << 40) & 0x00ff000000000000ULL) | ((*p << 56)); }

ARCEMU_INLINE static int16 swap16(int16 p) { return ((p >> 8) & 0xff) | (p << 8); }
ARCEMU_INLINE static int32 swap32(int32 p) { return ((p >> 24) & 0xff) | ((p >> 8) & 0xff00) | ((p << 8) & 0xff0000) | (p << 24); }
ARCEMU_INLINE static int64 swap64(int64 p)  { return ((((p >> 56) & 0xff)) | ((p >> 40) & 0x000000000000ff00ULL) | ((p >> 24) & 0x0000000000ff0000ULL) | ((p >> 8 ) & 0x00000000ff000000ULL) |
								((p << 8 ) & 0x000000ff00000000ULL) | ((p << 24) & 0x0000ff0000000000ULL) | ((p << 40) & 0x00ff000000000000ULL) | ((p << 56))); }*/

ARCEMU_INLINE static uint16 swap16(uint16 p) { return bswap_16((uint16_t)p); }
ARCEMU_INLINE static uint32 swap32(uint32 p) { return bswap_32((uint32_t)p); }
ARCEMU_INLINE static uint64 swap64(uint64 p)  { return bswap_64((uint64_t)p); }

ARCEMU_INLINE static void swap16(int16* p) { *p = bswap_16((uint16_t)*p); }
ARCEMU_INLINE static void swap32(int32* p) { *p = bswap_32((uint32_t)*p); }
ARCEMU_INLINE static void swap64(int64* p) { *p = bswap_64((uint64_t)*p); }

ARCEMU_INLINE static int16 swap16(int16 p) { return bswap_16((uint16_t)p); }
ARCEMU_INLINE static int32 swap32(int32 p) { return bswap_32((uint32_t)p); }
ARCEMU_INLINE static int64 swap64(int64 p)  { return bswap_64((uint64_t)p); }

#endif
/* 
Scripting system exports/imports
*/

#ifdef WIN32
	#ifndef SCRIPTLIB
		#define SERVER_DECL __declspec(dllexport)
		#define SCRIPT_DECL __declspec(dllimport)
	#else
		#define SERVER_DECL __declspec(dllimport)
		#define SCRIPT_DECL __declspec(dllexport)
	#endif
#else
	#define SERVER_DECL 
	#define SCRIPT_DECL 
#endif

// Include all threading files
#include <assert.h>

#if COMPILER == COMPILER_MICROSOFT

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#endif

#ifndef WIN32
#ifdef USING_BIG_ENDIAN
//#define GUID_HIPART(x) (*((uint32*)&(x)))
//#define GUID_LOPART(x) (*(((uint32*)&(x))+1))
#define GUID_LOPART(x) ( ( x >> 32 ) )
#define GUID_HIPART(x) ( ( x & 0x00000000ffffffff ) )
#else
#define GUID_HIPART(x) ( ( x >> 32 ) )
#define GUID_LOPART(x) ( ( x & 0x00000000ffffffff ) )
#endif
#else
#define GUID_HIPART(x) (*(((uint32*)&(x))+1))
#define GUID_LOPART(x) (*((uint32*)&(x)))
#endif

#define atol(a) strtoul( a, NULL, 10)

#define STRINGIZE(a) #a

// fix buggy MSVC's for variable scoping to be reliable =S
#define for if(true) for

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1400
#pragma float_control(push)
#pragma float_control(precise, on)
#endif

// fast int abs
static inline int int32abs( const int value )
{
	return (value ^ (value >> 31)) - (value >> 31);
}

// fast int abs and recast to unsigned
static inline uint32 int32abs2uint32( const int value )
{
	return (uint32)(value ^ (value >> 31)) - (value >> 31);
}

/// Fastest Method of float2int32
static inline int float2int32(const float value)
{
#if !defined(X64) && COMPILER == COMPILER_MICROSOFT && !defined(USING_BIG_ENDIAN)
	int i;
	__asm {
		fld value
		frndint
		fistp i
	}
	return i;
#else
	union { int asInt[2]; double asDouble; } n;
	n.asDouble = value + 6755399441055744.0;

#if USING_BIG_ENDIAN
	return n.asInt [1];
#else
	return n.asInt [0];
#endif
#endif
}

/// Fastest Method of long2int32
static inline int long2int32(const double value)
{
#if !defined(X64) && COMPILER == COMPILER_MICROSOFT && !defined(USING_BIG_ENDIAN)
	int i;
	__asm {
		fld value
		frndint
		fistp i
	}
	return i;
#else
  union { int asInt[2]; double asDouble; } n;
  n.asDouble = value + 6755399441055744.0;

#if USING_BIG_ENDIAN
  return n.asInt [1];
#else
  return n.asInt [0];
#endif
#endif
}

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1400
#pragma float_control(pop)
#endif

#ifndef WIN32
#include <sys/timeb.h>
#endif

ARCEMU_INLINE uint32 now()
{	
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif
}

#ifndef WIN32
#define FALSE   0
#define TRUE	1
#endif

#ifndef WIN32
#define Sleep(ms) usleep(1000*ms)
#endif

/*#ifdef WIN32
#ifndef __SHOW_STUPID_WARNINGS__
#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4305) 
#pragma warning(disable:4748)
#pragma warning(disable:4800) 
#pragma warning(disable:4996)
#pragma warning(disable:4251)
#endif	  
#endif

#undef INTEL_COMPILER
#ifdef INTEL_COMPILER
#pragma warning(disable:279)
#pragma warning(disable:1744)
#pragma warning(disable:1740)
#endif*/


#endif
