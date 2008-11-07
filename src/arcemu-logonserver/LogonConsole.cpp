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

#include "LogonStdAfx.h"
#include "LogonConsole.h"
#include "Log.h"

initialiseSingleton(LogonConsole);
bool Rehash();

void LogonConsole::TranslateRehash(char* str)
{
	sLog.outString("rehashing config file...");
	Rehash();
}

void LogonConsole::Kill()
{
	_thread->kill=true;
#ifdef WIN32
	/* write the return keydown/keyup event */
	DWORD dwTmp;
	INPUT_RECORD ir[2];
	ir[0].EventType = KEY_EVENT;
	ir[0].Event.KeyEvent.bKeyDown = TRUE;
	ir[0].Event.KeyEvent.dwControlKeyState = 288;
	ir[0].Event.KeyEvent.uChar.AsciiChar = 13;
	ir[0].Event.KeyEvent.wRepeatCount = 1;
	ir[0].Event.KeyEvent.wVirtualKeyCode = 13;
	ir[0].Event.KeyEvent.wVirtualScanCode = 28;
	ir[1].EventType = KEY_EVENT;
	ir[1].Event.KeyEvent.bKeyDown = FALSE;
	ir[1].Event.KeyEvent.dwControlKeyState = 288;
	ir[1].Event.KeyEvent.uChar.AsciiChar = 13;
	ir[1].Event.KeyEvent.wRepeatCount = 1;
	ir[1].Event.KeyEvent.wVirtualKeyCode = 13;
	ir[1].Event.KeyEvent.wVirtualScanCode = 28;
	WriteConsoleInput (GetStdHandle(STD_INPUT_HANDLE), ir, 2, & dwTmp);
#endif
	printf("Waiting for console thread to terminate....\n");
	while(_thread != NULL)
	{
		Sleep(100);
	}
	printf("Console shut down.\n");
}

bool LogonConsoleThread::run()
{
	new LogonConsole;

	SetThreadName("Console Interpreter");
	sLogonConsole._thread = this;
	size_t i = 0, len = 0;
	char cmd[96];

#ifndef WIN32
	fd_set fds;
	struct timeval tv;
#endif

	while (!kill)
	{
#ifndef WIN32
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO( &fds );
		FD_SET( STDIN_FILENO, &fds );
		if ( select( 1, &fds, NULL, NULL, &tv ) <= 0 )
		{
			if(!kill) // timeout
				continue;
			else
				break;
		}
#endif
		// Make sure our buffer is clean to avoid Array bounds overflow
		memset( cmd,0,sizeof( cmd ) );
		// Read in single line from "stdin"
		fgets( cmd, 80, stdin );

		if ( kill )
			break;

		len = strlen( cmd );
		for ( i = 0; i < len; ++i )
		{
			if ( cmd[i] == '\n' || cmd[i] == '\r' )
				cmd[i] = '\0';
		}
		sLogonConsole.ProcessCmd( cmd );
	}

	sLogonConsole._thread=NULL;
	return true;
}

//------------------------------------------------------------------------------
// Protected methods:
//------------------------------------------------------------------------------
// Process one command
void LogonConsole::ProcessCmd(char *cmd)
{
	typedef void (LogonConsole::*PTranslater)(char *str);
	struct SCmd
	{
		const char *name;
		PTranslater tr;
	};

	SCmd cmds[] =
	{

		{"?", &LogonConsole::TranslateHelp}, {"help", &LogonConsole::TranslateHelp},
		{ "reload", &LogonConsole::ReloadAccts},
		{ "rehash", &LogonConsole::TranslateRehash},
		{ "netstatus", &LogonConsole::NetworkStatus},
		{"shutdown", &LogonConsole::TranslateQuit}, {"exit", &LogonConsole::TranslateQuit}, 
	};

	char cmd2[80];
	strcpy(cmd2, cmd);
	for(size_t i = 0; i < strlen(cmd); ++i)
		cmd2[i] = tolower(cmd[i]);

	for (size_t i = 0; i < sizeof(cmds)/sizeof(SCmd); i++)
		if (strncmp(cmd2, cmds[i].name, strlen(cmds[i].name)) == 0)
		{
			(this->*(cmds[i].tr)) (cmd + strlen(cmds[i].name));
			return;
		}

		printf("Console:Unknown console command (use \"help\" for help).\n");
}

void LogonConsole::ReloadAccts(char *str)
{
	AccountMgr::getSingleton().ReloadAccounts(false);
	IPBanner::getSingleton().Reload();
}

void LogonConsole::NetworkStatus(char *str)
{
	return;
	//sSocketMgr.ShowStatus();
}

// quit | exit
void LogonConsole::TranslateQuit(char *str)
{
	int delay = str != NULL ? atoi(str) : 5000;
	if(!delay)
		delay = 5000;
	else
		delay *= 1000;

	ProcessQuit(delay);
}
void LogonConsole::ProcessQuit(int delay)
{
	mrunning = false;
}
//------------------------------------------------------------------------------
// help | ?
void LogonConsole::TranslateHelp(char *str)
{
	ProcessHelp(NULL);
}
void LogonConsole::ProcessHelp(char *command)
{
	if (command == NULL)
	{
		sLog.outString("Console:--------help--------");
		sLog.outString("   help, ?: print this text");
		sLog.outString("   reload: reloads accounts");
		sLog.outString("   netstatus: shows network status");
		sLog.outString("   shutdown, exit: close program");
	}
}
//------------------------------------------------------------------------------

LogonConsoleThread::LogonConsoleThread()
{
	kill=false;
}

LogonConsoleThread::~LogonConsoleThread()
{
}
