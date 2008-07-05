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

#ifndef _DATABASECLEANER_H
#define _DATABASECLEANER_H

class DatabaseCleaner : public Singleton<DatabaseCleaner>
{
public:
	void Run();

protected:
	void CleanCharacters();
	void CleanWorld();
	void Optimize();
};

#endif

