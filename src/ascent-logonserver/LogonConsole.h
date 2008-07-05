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

#ifndef __LOGONCONSOLE_H
#define __LOGONCONSOLE_H

#include "Common.h"
#include "CThreads.h"

class LogonConsoleThread : public ThreadBase
{
public:
	bool kill;
	LogonConsoleThread();
	~LogonConsoleThread();
	bool run();
};

class LogonConsole :  public Singleton < LogonConsole >
{
	friend class LogonConsoleThread;

public:						// Public methods:
	void Kill();

protected:					// Protected methods:
	LogonConsoleThread *_thread;

	// Process one command
	void ProcessCmd(char *cmd);

	// quit | exit
	void TranslateQuit(char *str);
	void ProcessQuit(int delay);
	void CancelShutdown(char *str);

	// help | ?
	void TranslateHelp(char *str);
	void ProcessHelp(char *command);

	void ReloadAccts(char *str);
	void TranslateRehash(char* str);
};

#define sLogonConsole LogonConsole::getSingleton()

#endif
