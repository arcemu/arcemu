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

/* *
   @file CrashHandler.h
   Handles crashes/exceptions on a win32 based platform, writes a dump file,
   for later bug fixing.
*/

#ifndef _CRASH_HANDLER_H
#define _CRASH_HANDLER_H

bool HookCrashReporter(bool logon);
void OutputCrashLogLine(const char* format, ...);

#include "Common.h"

#ifdef WIN32

//#include <Windows.h>
#include <DbgHelp.h>
#include "StackWalker.h"
#include "CircularQueue.h"

extern CircularQueue<uint32, 30> last_spells;

class SERVER_DECL CStackWalker : public StackWalker
{
public:
	void OnOutput(LPCSTR szText);
	void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion);
	void OnCallstackEntry(CallstackEntryType eType, CallstackEntry &entry);
	void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
};

void StartCrashHandler();
void OnCrash(bool Terminate);

typedef struct _EXCEPTION_POINTERS EXCEPTION_POINTERS, *PEXCEPTION_POINTERS;
int __cdecl HandleCrash(PEXCEPTION_POINTERS pExceptPtrs);

#define THREAD_TRY_EXECUTION __try 
#define THREAD_HANDLE_CRASH  __except(HandleCrash(GetExceptionInformation())) {}

#define THREAD_TRY_EXECUTION2 __try {
#define THREAD_HANDLE_CRASH2  } __except(HandleCrash(GetExceptionInformation())) {}

#define THREAD_TRY_EXECUTION_MapMgr __try {
#define THREAD_HANDLE_CRASH_MapMgr  } __except(HandleCrash(GetExceptionInformation())) { KillThreadWithCleanup(); }

#else

// We dont wanna confuse nix ;p
#define THREAD_TRY_EXECUTION 
#define THREAD_HANDLE_CRASH 

#define THREAD_TRY_EXECUTION2 ;
#define THREAD_HANDLE_CRASH2 ;

#ifndef FORCED_SERVER_KEEPALIVE
	#define THREAD_TRY_EXECUTION_MapMgr ;
	#define THREAD_HANDLE_CRASH_MapMgr ;
#else
	#define THREAD_TRY_EXECUTION_MapMgr try {
	#define THREAD_HANDLE_CRASH_MapMgr } catch (int error) { KillThreadWithCleanup(); }
#endif

#endif

#endif

