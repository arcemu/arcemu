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

#include "LogonStdAfx.h"
#include "LogonConsole.h"

initialiseSingleton(LogonConsole);
bool Rehash();

void LogonConsole::TranslateRehash(char* str)
{
	sLog.outString("rehashing config file...");
	Rehash();
}

void LogonConsole::Kill()
{
	_thread->kill.SetVal(true);
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
	WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ir, 2, & dwTmp);
#endif
	LOG_BASIC("Waiting for console thread to terminate....");
	while(_thread != NULL)
	{
		Arcemu::Sleep(100);
	}
	LOG_BASIC("Console shut down.");
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

	while(!kill.GetVal())
	{
#ifndef WIN32
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		if(select(1, &fds, NULL, NULL, &tv) <= 0)
		{
			if(!kill.GetVal()) // timeout
				continue;
			else
				break;
		}
#endif
		// Make sure our buffer is clean to avoid Array bounds overflow
		memset(cmd, 0, sizeof(cmd));
		// Read in single line from "stdin"
		fgets(cmd, 80, stdin);

		if(kill.GetVal())
			break;

		len = strlen(cmd);
		for(i = 0; i < len; ++i)
		{
			if(cmd[i] == '\n' || cmd[i] == '\r')
				cmd[i] = '\0';
		}
		sLogonConsole.ProcessCmd(cmd);
	}

	sLogonConsole._thread = NULL;
	return true;
}

//------------------------------------------------------------------------------
// Protected methods:
//------------------------------------------------------------------------------
// Process one command
void LogonConsole::ProcessCmd(char* cmd)
{
	typedef void (LogonConsole::*PTranslater)(char * str);
	struct SCmd
	{
		const char* name;
		PTranslater tr;
	};

	SCmd cmds[] =
	{

		{	"?", &LogonConsole::TranslateHelp},
		{   "help", &LogonConsole::TranslateHelp},
		{   "createaccount", &LogonConsole::CreateAccount },
		{	"reload", &LogonConsole::ReloadAccts},
		{	"rehash", &LogonConsole::TranslateRehash},
		{	"netstatus", &LogonConsole::NetworkStatus},
		{	"shutdown", &LogonConsole::TranslateQuit},
		{	"exit", &LogonConsole::TranslateQuit},
		{	"info", &LogonConsole::Info},
	};

	char cmd2[80];
	strcpy(cmd2, cmd);
	for(size_t i = 0; i < strlen(cmd); ++i)
		cmd2[i] = static_cast<char>(tolower(cmd[i]));

	for(size_t i = 0; i < sizeof(cmds) / sizeof(SCmd); i++)
		if(strncmp(cmd2, cmds[i].name, strlen(cmds[i].name)) == 0)
		{
			(this->*(cmds[i].tr))(cmd + strlen(cmds[i].name));
			return;
		}

	printf("Console: Unknown console command (use \"help\" for help).\n");
}

void LogonConsole::ReloadAccts(char* str)
{
	AccountMgr::getSingleton().ReloadAccounts(false);
	IPBanner::getSingleton().Reload();
}

void LogonConsole::NetworkStatus(char* str)
{
	sSocketMgr.ShowStatus();
}

// quit | exit
void LogonConsole::TranslateQuit(char* str)
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
	mrunning.SetVal(false);
}
//------------------------------------------------------------------------------
// help | ?
void LogonConsole::TranslateHelp(char* str)
{
	ProcessHelp(NULL);
}
void LogonConsole::ProcessHelp(char* command)
{
	if(command == NULL)
	{
		printf("Console:--------help--------\n");
		printf("	Help, ?: Prints this help text.\n");
		printf("	createaccount: Creates new accounts\n");
		printf("	Reload: Reloads accounts.\n");
		printf("	Netstatus: Shows network status.\n");
		printf("	info:  shows some information about the server.\n");
		printf("	Shutdown, exit: Closes the logonserver.\n");
	}
}

void LogonConsole::Info(char* str)
{
	std::cout << "LogonServer information" << std::endl;
	std::cout << "-----------------------" << std::endl;
	std::cout << "CPU Usage: " << LogonServer::getSingleton().perfcounter.GetCurrentCPUUsage() << "%" << std::endl;
	std::cout << "RAM Usage: " << LogonServer::getSingleton().perfcounter.GetCurrentRAMUsage() << "MB" << std::endl;
}

void LogonConsole::CreateAccount(char* str)
{
	char name[ 512 ];
	char password[ 512 ];
	char email[ 512 ];

	int count = sscanf(str, "%s %s %s", name, password, email);
	if(count != 3)
	{
		std::cout << "usage: createaccount <name> <password> <email>" << std::endl;
		std::cout << "example: createaccount ghostcrawler Ih4t3p4l4dins greg.street@blizzard.com" << std::endl;
		return;
	}

	{
		// need to pass uppercase names to check if account exists
		std::string aname(name);

		for(std::string::iterator itr = aname.begin(); itr != aname.end(); ++itr)
			*itr = toupper(*itr);

		if(AccountMgr::getSingleton().GetAccount(aname) != NULL)
		{
			std::cout << "There's already an account with name " << name << std::endl;
			return;
		}
	}

	std::string pass;
	pass.assign(name);
	pass.push_back(':');
	pass.append(password);

	std::stringstream query;
	query << "INSERT INTO `accounts`( `login`,`password`,`encrypted_password`,`gm`,`banned`,`email`,`flags`,`banreason`) VALUES ( '";
	query << name << "','',";
	query << "SHA( UPPER( '" << pass << "' ) ),'0','0','";
	query << email << "','";
	query << 24 << "','' );";

	if(!sLogonSQL->WaitExecuteNA(query.str().c_str()))
	{
		std::cout << "Couldn't save new account to database. Aborting." << std::endl;
		return;
	}

	AccountMgr::getSingleton().ReloadAccounts(true);

	std::cout << "Account created." << std::endl;
}

//------------------------------------------------------------------------------

LogonConsoleThread::LogonConsoleThread()
{
	kill.SetVal(false);
}

LogonConsoleThread::~LogonConsoleThread()
{
}
