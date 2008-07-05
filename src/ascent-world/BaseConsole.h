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

#ifndef _BASECONSOLE_H
#define _BASECONSOLE_H

class ConsoleSocket;

class BaseConsole
{
public:
	virtual ~BaseConsole() {}
	virtual void Write(const char * Format, ...) = 0;
	virtual void WriteNA(const char * Format) = 0;
};

class RemoteConsole : public BaseConsole
{
	ConsoleSocket * m_pSocket;
public:
	RemoteConsole(ConsoleSocket* pSocket);
	void Write(const char * Format, ...);
	void WriteNA(const char * Format);
};

class LocalConsole : public BaseConsole
{
public:
	void Write(const char * Format, ...);
	void WriteNA(const char * Format);
};

void HandleConsoleInput(BaseConsole * pConsole, const char * szInput);

#endif
