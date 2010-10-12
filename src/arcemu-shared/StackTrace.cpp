/*
 * Copyright (c) 2001 Jani Kajala
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Jani Kajala makes no representations about the suitability 
 * of this software for any purpose. It is provided "as is" 
 * without express or implied warranty.
 */

#include "StackTrace.h"
#include "MapFile.h"
#include "MapFileEntry.h"
#include <string.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

#define MAX_DEPTH 32

static long getCaller( int index )
{
#if defined(_DEBUG) && defined(_MSC_VER) && defined(_M_IX86)

	long caller = 0;
	__asm
	{
		mov ebx, ebp
		mov ecx, index
		inc ecx
		xor eax, eax
StackTrace_getCaller_next:
		mov eax, [ebx+4]
		mov ebx, [ebx]
		dec ecx
		jnz StackTrace_getCaller_next
		mov caller, eax
	}
	return caller;

#else

	return 0;

#endif
}

int StackTrace::printStackTrace( MapFile** map, int maps,
	int initLevel, int maxDepth,
	char* buffer, int bufferSize )
{
	if ( maxDepth > MAX_DEPTH )
		maxDepth = MAX_DEPTH;

	// list callers
	long callersAddr[MAX_DEPTH];
	int callers = 0;
	int i;
	for ( i = initLevel ; i < maxDepth ; ++i )
	{
		long addr = getCaller( i );
		callersAddr[callers++] = addr;

		// end tracing here if the entry is not in a map file
		int entry = -1;
		for ( int j = 0 ; j < maps ; ++j )
		{
			entry = map[j]->findEntry( addr );
			if ( -1 != entry )
				break;
		}
		if ( -1 == entry )
			break;
	}

	// output call stack
	if ( bufferSize > 0 )
		*buffer = 0;
	int needed = 0;
	for ( i = initLevel ; i < callers ; ++i )
	{
		long addr = callersAddr[callers-i-1];

		// find entry info
		int entry = -1;
		const MapFile* entryMap = 0;
		for ( int j = 0 ; j < maps ; ++j )
		{
			entry = map[j]->findEntry( addr );
			if ( -1 != entry )
			{
				entryMap = map[j];
				break;
			}
		}

		// format entry to tempory buf
		char buf[MapFileEntry::MAX_NAME+MAX_DEPTH+20];	// name + margin + hex number
		buf[0] = 0;
		for ( int k = initLevel-1 ; k < i ; ++k )
			strcat( buf, " " );
		if ( !entryMap )
			sprintf( buf+strlen(buf), "0x%x\n", addr );
		else
			sprintf( buf+strlen(buf), "%s (%x)\n", entryMap->getEntry(entry).name(), addr );

		// append temporary buf to output buffer if space left
		needed += (int)strlen( buf );
		if ( needed < bufferSize )
			strcat( buffer, buf );
	}

	// terminate output buffer
	if ( needed < bufferSize )
		buffer[needed] = 0;
	else if ( bufferSize > 0 )
		buffer[bufferSize-1] = 0;
	return needed;
}
