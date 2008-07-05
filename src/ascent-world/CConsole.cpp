/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#include "StdAfx.h"
LocalConsole g_localConsole;

#ifndef WIN32
    #include <termios.h>
#endif

void ConsoleThread::terminate()
{
	m_killSwitch = true;
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
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, 2, & dwTmp );
#endif
	printf( "Waiting for console thread to terminate....\n" );
	while( m_isRunning )
	{
		Sleep( 100 );
	}
	printf( "Console shut down.\n" );
}

bool ConsoleThread::run()
{
	SetThreadName("Console Interpreter");
	size_t i = 0;
	size_t len;
	char cmd[300];
#ifndef WIN32
	fd_set fds;
	struct timeval tv;
#endif

	m_killSwitch = false;
	m_isRunning = true;
	while( m_killSwitch != true )
	{
#ifdef WIN32

		// Read in single line from "stdin"
		memset( cmd, 0, sizeof( cmd ) ); 
		if( fgets( cmd, 300, stdin ) == NULL )
			continue;

		if( m_killSwitch )
			break;

#else
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO( &fds );
		FD_SET( STDIN_FILENO, &fds );
		if( select( 1, &fds, NULL, NULL, &tv ) <= 0 )
		{
			if(!m_killSwitch)	// timeout
				continue;
			else
				break;
		}

		// Read in single line from "stdin"
		memset( cmd, 0, sizeof( cmd ) ); 
		if( fgets( cmd, 300, stdin ) == NULL )
			continue;
#endif

		len = strlen(cmd);
		for( i = 0; i < len; ++i )
		{
			if(cmd[i] == '\n' || cmd[i] == '\r')
				cmd[i] = '\0';
		}

		HandleConsoleInput(&g_localConsole, cmd);
	}
	m_isRunning = false;
	return false;
}
