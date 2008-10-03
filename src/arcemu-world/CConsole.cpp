/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"
LocalConsole g_localConsole;

#ifndef WIN32
    #include <poll.h>
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
	struct pollfd input;

	input.fd      = 0;
	input.events  = POLLIN | POLLPRI;
	input.revents = 0;
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
		int ret = poll(&input, 1, 1000);
		if (ret < 0)
		{
			break;
		}
		else if( ret == 0 )
		{
			if(!m_killSwitch)	// timeout
				continue;
			else
				break;
		}

		ret = read(0, cmd, sizeof(cmd));
		if (ret <= 0)
		{
			break;
		}
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
