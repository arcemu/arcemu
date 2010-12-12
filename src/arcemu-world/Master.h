/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
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

#ifndef _MASTER_H
#define _MASTER_H

#include "Common.h"
//#include "Singleton.h"
#include "Config/ConfigEnv.h"
#include "Database/DatabaseEnv.h"
#include "MainServerDefines.h"

#ifndef _VERSION
# define _VERSION "3.3.5a"
#endif

#if PLATFORM == PLATFORM_WIN32
# define _FULLVERSION _VERSION "-SVN (Win32)"
#else
# define _FULLVERSION _VERSION "-SVN (Unix)"
#endif

#ifdef _DEBUG
#define BUILDTYPE "Debug"
#else
#define BUILDTYPE "Release"
#endif

#define REQUIRED_WORLD_DB_VERSION 3955
#define REQUIRED_CHAR_DB_VERSION 3800

#define DEFAULT_LOOP_TIME 0 /* 0 milliseconds - instant */
#define DEFAULT_LOG_LEVEL 0
#define DEFAULT_PLAYER_LIMIT 100
#define DEFAULT_WORLDSERVER_PORT 8129
#define DEFAULT_REALMSERVER_PORT 3724
#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_REGEN_RATE 0.15
#define DEFAULT_XP_RATE 1
#define DEFAULT_DROP_RATE 1
#define DEFAULT_REST_XP_RATE 1
#define DEFAULT_QUEST_XP_RATE 1
#define DEFAULT_SAVE_RATE 300000	// 5mins

class Master : public Singleton<Master>
{
public:
	Master();
	~Master();
	bool Run(int argc, char ** argv);
	bool m_ShutdownEvent;
	uint32 m_ShutdownTimer;

	static volatile bool m_stopEvent;
	bool m_restartEvent;

private:
	bool _StartDB();
	void _StopDB();
	bool CheckDBVersion();

	void _HookSignals();
	void _UnhookSignals();

	static void _OnSignal(int s);
};

#define sMaster Master::getSingleton()

#endif
