#ifndef __STDAFX_H
#define __STDAFX_H

// Windows Header Files:
#include <windows.h>
#include <string>

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

#ifdef WIN32
#define ARCEMU_INLINE __forceinline
#else
#define ARCEMU_INLINE inline
#endif

typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
#define assert(exp)     ((void)0)
#define snprintf _snprintf
#include "math.h"
#include "CellHandler.h"
#include "TerrainMgr.h"
#include "CollideInterface.h"
#include "LocationVector.h"
#include "conio.h"

#define MAXINT (1<<31)
#define COLLISION 1
#define vMapPath "./maps/"
#define ColvMapPath "./vmaps/"
#define vMapOutPath "./maps_out" //no trailing slash requirred
//#define USE_MEMORY_MAPPING_FOR_MAPS

#endif