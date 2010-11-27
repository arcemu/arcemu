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

#include "StdAfx.h"

#define BANNER "ArcEmu %s r%u/%s-%s-%s :: World Server"

#ifndef WIN32
#include <sched.h>
#endif

#include "svn_revision.h"

#include <signal.h>

createFileSingleton( Master );
std::string LogFileName;
bool bLogChat;

volatile bool Master::m_stopEvent = false;

// Database defines.
SERVER_DECL Database* Database_Character;
SERVER_DECL Database* Database_World;

// mainserv defines
SessionLogWriter* GMCommand_Log;
SessionLogWriter* Anticheat_Log;
SessionLogWriter* Player_Log;

// threads
extern DayWatcherThread * dw;
extern CommonScheduleThread * cs;

void Master::_OnSignal(int s)
{
	switch (s)
	{
#ifndef WIN32
	case SIGHUP:
		sWorld.Rehash(true);
		break;
#endif
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
#ifdef _WIN32
	case SIGBREAK:
#endif
		Master::m_stopEvent = true;
		break;
	}

	signal(s, _OnSignal);
}

Master::Master()
{
	m_ShutdownTimer = 0;
	m_ShutdownEvent = false;
	m_restartEvent = false;
}

Master::~Master()
{
}

struct Addr
{
	unsigned short sa_family;
	/* sa_data */
	unsigned short Port;
	unsigned long IP; // inet_addr
	unsigned long unusedA;
	unsigned long unusedB;
};

#define DEF_VALUE_NOT_SET 0xDEADBEEF

#ifdef WIN32
        static const char* default_config_file = "configs/world.conf";
		static const char* default_optional_config_file = "configs/optional.conf";
        static const char* default_realm_config_file = "configs/realms.conf";
#else
        static const char* default_config_file = CONFDIR "/world.conf";
		static const char* default_optional_config_file = CONFDIR "/optional.conf";
        static const char* default_realm_config_file = CONFDIR "/realms.conf";
#endif

bool bServerShutdown = false;
bool StartConsoleListener();
void CloseConsoleListener();
ThreadBase * GetConsoleListener();

bool Master::Run(int argc, char ** argv)
{
	char * config_file = (char*)default_config_file;
	char * optional_config_file = (char*)default_optional_config_file;
	char * realm_config_file = (char*)default_realm_config_file;

	int file_log_level = DEF_VALUE_NOT_SET;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;
	int do_cheater_check = 0;
	int do_database_clean = 0;
	time_t curTime;

	struct arcemu_option longopts[] =
	{
		{ "checkconf",			arcemu_no_argument,				&do_check_conf,			1		},
		{ "screenloglevel",		arcemu_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		arcemu_required_argument,		&file_log_level,		1		},
		{ "version",			arcemu_no_argument,				&do_version,			1		},
		{ "conf",				arcemu_required_argument,		NULL,					'c'		},
		{ "realmconf",			arcemu_required_argument,		NULL,					'r'		},
		{ "databasecleanup",	arcemu_no_argument,				&do_database_clean,		1		},
		{ "cheatercheck",		arcemu_no_argument,				&do_cheater_check,		1		},
		{ 0, 0, 0, 0 }
	};

	char c;
	while ((c = static_cast<char>( arcemu_getopt_long_only(argc, argv, ":f:", longopts, NULL) ) ) != -1)
	{
		switch (c)
		{
		case 'c':
			config_file = new char[strlen(arcemu_optarg)];
			strcpy(config_file, arcemu_optarg);
			break;

		case 'r':
			realm_config_file = new char[strlen(arcemu_optarg)];
			strcpy(realm_config_file, arcemu_optarg);
			break;

		case 0:
			break;
		default:
			sLog.Init(0, WORLD_LOG);
			printf("Usage: %s [--checkconf] [--fileloglevel <level>] [--conf <filename>] [--realmconf <filename>] [--version] [--databasecleanup] [--cheatercheck]\n", argv[0]);
			sLog.Close();
			return true;
		}
	}

	// Startup banner
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);

	sLog.Init(0, WORLD_LOG);

	sLog.outBasic(BANNER, BUILD_TAG, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
	sLog.outError(BANNER, BUILD_TAG, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);

	if(do_version)
	{
		sLog.Close();
		return true;
	}

	if( do_check_conf )
	{
		Log.Notice( "Config", "Checking config file: %s", config_file );
		if( Config.MainConfig.SetSource(config_file, true ) )
			Log.Success( "Config", "Passed without errors." );
		else
			Log.Error( "Config", "Encountered one or more errors." );

		Log.Notice( "Config", "Checking config file: %s", realm_config_file );
		if( Config.RealmConfig.SetSource( realm_config_file, true ) )
			Log.Success( "Config", "Passed without errors." );
		else
			Log.Error( "Config", "Encountered one or more errors." );

		Log.Notice( "Config", "Checking config file:: %s", optional_config_file);
		if(Config.OptionalConfig.SetSource(optional_config_file, true) )
			Log.Success( "Config", "Passed without errors.");
		else
			Log.Error( "Config", "Encountered one or more errors.");

		sLog.Close();
		return true;
	}

	printf( "The key combination <Ctrl-C> will safely shut down the server at any time." );
    
#ifndef WIN32
	if(geteuid() == 0 || getegid() == 0)
		Log.LargeErrorMessage("You are running ArcEmu as root.", "This is not needed, and may be a possible security risk.", "It is advised to hit CTRL+C now and", "start as a non-privileged user.", NULL);
#endif

	InitImplicitTargetFlags();
	InitRandomNumberGenerators();
	Log.Notice( "Rnd", "Initialized Random Number Generators." );

	ThreadPool.Startup();
	uint32 LoadingTime = getMSTime();

	Log.Success( "Config", "Loading Config Files..." );
	if( Config.MainConfig.SetSource( config_file ) )
		Log.Notice( "Config", ">> configs/world.conf loaded" );
	else
	{
		sLog.Error( "Config", ">> error occurred loading configs/world.conf" );
		sLog.Close();
		return false;
	}

	if(Config.OptionalConfig.SetSource(optional_config_file))
		Log.Notice( "Config", ">> configs/optional.conf loaded");
	else
	{
		sLog.Error( "Config", ">> error occurred loading configs/optional.conf");
		sLog.Close();
		return false;
	}

	if(Config.RealmConfig.SetSource(realm_config_file))
		Log.Notice( "Config", ">> configs/realms.conf loaded" );
	else
	{
		sLog.Error( "Config", ">> error occurred loading configs/realms.conf" );
		sLog.Close();
		return false;
	}

#if !defined(WIN32) && defined(__DEBUG__)
	if (Config.MainConfig.GetIntDefault( "LogLevel", "DisableCrashdumpReport", 0) == 0)
	{
		char cmd[1024];
		char banner[1024];
		snprintf(banner, 1024, BANNER, BUILD_TAG, BUILD_REVISION, CONFIG, PLATFORM_TEXT, ARCH);
		snprintf(cmd, 1024, "./arcemu-crashreport -r %d -d \'%s\'", BUILD_REVISION, banner);
		system(cmd);
	}
	unlink("arcemu.uptime");
#endif

	if( !_StartDB() )
	{
		Database::CleanupLibs();
		sLog.Close();
		return false;
	}

	// Checking the DB version. If it's wrong or can't be validated we exit.
	if( !CheckDBVersion() )
	{
		sLog.Close();
		return false;
	}

	if(do_database_clean)
	{
		sLog.outDebug( "Entering database maintenance mode." );
		new DatabaseCleaner;
		DatabaseCleaner::getSingleton().Run();
		delete DatabaseCleaner::getSingletonPtr();
		sLog.outDebug( "Maintenance finished." );
	}

	new EventMgr;
	new World;

	// optional time stamp in logs
	bool useTimeStamp = Config.MainConfig.GetBoolDefault("log", "TimeStamp", false);

	// open cheat log file
	Anticheat_Log = new SessionLogWriter(FormatOutputString( "logs", "cheaters", useTimeStamp).c_str(), false );
	GMCommand_Log = new SessionLogWriter(FormatOutputString( "logs", "gmcommand", useTimeStamp).c_str(), false );
	Player_Log = new SessionLogWriter(FormatOutputString( "logs", "players", useTimeStamp).c_str(), false );

	/* load the config file */
	sWorld.Rehash(false);

	/* set new log levels */
	if( file_log_level != (int)DEF_VALUE_NOT_SET )
		sLog.SetFileLoggingLevel(file_log_level);

	// Initialize Opcode Table
	WorldSession::InitPacketHandlerTable();

	string host = Config.MainConfig.GetStringDefault( "Listen", "Host", DEFAULT_HOST );
	int wsport = Config.MainConfig.GetIntDefault( "Listen", "WorldServerPort", DEFAULT_WORLDSERVER_PORT );

	new ScriptMgr;

	if( !sWorld.SetInitialWorldSettings() )
	{
		Log.Error( "Server", "SetInitialWorldSettings() failed. Something went wrong? Exiting." );
		sLog.Close();
		return false;
	}

	if( do_cheater_check )
		sWorld.CleanupCheaters();

	sWorld.SetStartTime((uint32)UNIXTIME);
	
	WorldRunnable * wr = new WorldRunnable();
	ThreadPool.ExecuteTask(wr);

	_HookSignals();

	ConsoleThread * console = new ConsoleThread();
	ThreadPool.ExecuteTask(console);

	uint32 realCurrTime, realPrevTime;
	realCurrTime = realPrevTime = getMSTime();

	// Socket loop!
	uint32 start;
	uint32 diff;
	uint32 last_time = now();
	uint32 etime;
	uint32 next_printout = getMSTime(), next_send = getMSTime();

	// Start Network Subsystem
	Log.Success( "Network","Starting subsystem..." );
	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	sScriptMgr.LoadScripts();

	if( Config.MainConfig.GetBoolDefault("Startup", "EnableSpellIDDump", false) )
		sScriptMgr.DumpUnimplementedSpells();

	LoadingTime = getMSTime() - LoadingTime;
	Log.Success( "Server","Ready for connections. Startup time: %ums", LoadingTime );

	Log.Notice("RemoteConsole", "Starting...");
	if( StartConsoleListener() )
	{
#ifdef WIN32
		ThreadPool.ExecuteTask( GetConsoleListener() );
#endif
		Log.Notice("RemoteConsole", "Now open.");
	}
	else
	{
		Log.Warning("RemoteConsole", "Not enabled or failed listen.");
	}
	
 
	/* write pid file */
	FILE * fPid = fopen( "arcemu.pid", "w" );
	if( fPid )
	{
		uint32 pid;
#ifdef WIN32
		pid = GetCurrentProcessId();
#else
		pid = getpid();
#endif
		fprintf( fPid, "%u", (unsigned int)pid );
		fclose( fPid );
	}
#ifdef WIN32
	HANDLE hThread = GetCurrentThread();
#endif

	uint32 loopcounter = 0;
	//ThreadPool.Gobble();

	/* Connect to realmlist servers / logon servers */
	new LogonCommHandler();
	sLogonCommHandler.Startup();

	// Create listener
	ListenSocket<WorldSocket> * ls = new ListenSocket<WorldSocket>(host.c_str(), wsport);
    bool listnersockcreate = ls->IsOpen();
#ifdef WIN32
	if( listnersockcreate )
		ThreadPool.ExecuteTask(ls);
#endif

	while( !m_stopEvent && listnersockcreate )
	{
		start = now();
		diff = start - last_time;
		if(! ((++loopcounter) % 10000) )		// 5mins
		{
			ThreadPool.ShowStats();
			ThreadPool.IntegrityCheck();
#if !defined(WIN32) && defined(__DEBUG__)
			FILE * f = fopen( "arcemu.uptime", "w" );
			if( f )
			{
				fprintf(f, "%u %u %u %u", sWorld.GetUptime(), sWorld.GetSessionCount(), sWorld.PeakSessionCount, sWorld.mAcceptedConnections);
				fclose(f);
			}
#endif
		}

		/* since time() is an expensive system call, we only update it once per server loop */
		curTime = time(NULL);
		if( UNIXTIME != curTime )
		{
			UNIXTIME = time(NULL);
			g_localTime = *localtime(&curTime);
		}

		sSocketGarbageCollector.Update();

		/* UPDATE */
		last_time = now();
		etime = last_time - start;
		if( m_ShutdownEvent )
		{
			if( getMSTime() >= next_printout )
			{
				if(m_ShutdownTimer > 60000.0f)
				{
					if( !( (int)(m_ShutdownTimer) % 60000 ) )
						Log.Notice( "Server", "Shutdown in %i minutes.", (int)(m_ShutdownTimer / 60000.0f ) );
				}
				else
					Log.Notice( "Server","Shutdown in %i seconds.", (int)(m_ShutdownTimer / 1000.0f ) );
					
				next_printout = getMSTime() + 500;
			}

			if( getMSTime() >= next_send )
			{
				int time = m_ShutdownTimer / 1000;
				if( ( time % 30 == 0 ) || time < 10 )
				{
					// broadcast packet.
					WorldPacket data( 20 );
					data.SetOpcode( SMSG_SERVER_MESSAGE );
					if(m_restartEvent)
						data << uint32( SERVER_MSG_RESTART_TIME );
					else
						data << uint32( SERVER_MSG_SHUTDOWN_TIME );
					
					if( time > 0 )
					{
						int mins = 0, secs = 0;
						if(time > 60)
							mins = time / 60;
						if(mins)
							time -= (mins * 60);
						secs = time;
						char str[20];
						snprintf( str, 20, "%02u:%02u", mins, secs );
						data << str;
						sWorld.SendGlobalMessage( &data, NULL );
					}
				}
				next_send = getMSTime() + 1000;
			}
			if( diff >= m_ShutdownTimer )
				break;
			else
				m_ShutdownTimer -= diff;
		}

		if( 50 > etime )
		{
#ifdef WIN32
			WaitForSingleObject( hThread, 50 - etime );
#else
			Sleep( 50 - etime );
#endif
		}
	}
	_UnhookSignals();

    wr->SetThreadState( THREADSTATE_TERMINATE );
	ThreadPool.ShowStats();
	/* Shut down console system */
	console->terminate();
	delete console;

	// begin server shutdown
	Log.Success( "Shutdown", "Initiated at %s", ConvertTimeStampToDataTime( (uint32)UNIXTIME).c_str() );

	if( lootmgr.is_loading )
	{
		Log.Notice( "Shutdown", "Waiting for loot to finish loading..." );
		while( lootmgr.is_loading )
			Sleep( 100 );
	}

	// send a query to wake it up if its inactive
	Log.Notice( "Database", "Clearing all pending queries..." );

	// kill the database thread first so we don't lose any queries/data
	CharacterDatabase.EndThreads();
	WorldDatabase.EndThreads();

	Log.Notice( "DayWatcherThread", "Exiting..." );
	dw->terminate();
	dw = NULL;

	Log.Notice( "CommonScheduleThread", "Exiting..." );
	cs->terminate();
	cs = NULL;

	ls->Close();

	CloseConsoleListener();
	sWorld.SaveAllPlayers();

	Log.Notice( "Network", "Shutting down network subsystem." );
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif
	sSocketMgr.CloseAll();

	bServerShutdown = true;
	ThreadPool.Shutdown();

	delete ls;

	sWorld.LogoutPlayers();

	delete LogonCommHandler::getSingletonPtr();

	sWorld.ShutdownClasses();
	Log.Notice( "World", "~World()" );
	delete World::getSingletonPtr();

	sScriptMgr.UnloadScripts();
	delete ScriptMgr::getSingletonPtr();

	Log.Notice( "ChatHandler", "~ChatHandler()" );
	delete ChatHandler::getSingletonPtr();

	Log.Notice( "EventMgr", "~EventMgr()" );
	delete EventMgr::getSingletonPtr();

	Log.Notice( "Database", "Closing Connections..." );
	_StopDB();

	Log.Notice( "Network", "Deleting Network Subsystem..." );
	delete SocketMgr::getSingletonPtr();
	delete SocketGarbageCollector::getSingletonPtr();

#ifdef ENABLE_LUA_SCRIPTING
	sLog.outString("Deleting Script Engine...");
	LuaEngineMgr::getSingleton().Unload();
#endif

	delete GMCommand_Log;
	delete Anticheat_Log;
	delete Player_Log;

	// remove pid
	remove( "arcemu.pid" );

	Log.Success( "Shutdown", "Shutdown complete." );
	Log.Close();

#ifdef WIN32
	WSACleanup();

	// Terminate Entire Application
	//HANDLE pH = OpenProcess(PROCESS_TERMINATE, TRUE, GetCurrentProcessId());
	//TerminateProcess(pH, 0);
	//CloseHandle(pH);

#endif

	return true;
}

bool Master::CheckDBVersion(){
	const char *versionquery = "SELECT LastUpdate FROM arcemu_db_version;";

	QueryResult *wqr = WorldDatabase.QueryNA( versionquery );
	if( wqr == NULL ){
		Log.Error("Database","World database is missing the table `arcemu_db_version` OR the table doesn't contain any rows. Can't validate database version. Exiting.");
		return false;
	}

	Field *f = wqr->Fetch();
	uint32 WorldDBVersion = f->GetUInt32();

	Log.Notice("Database","World database version: %u", WorldDBVersion );
	if( WorldDBVersion != REQUIRED_WORLD_DB_VERSION ){
		Log.Error("Database","World database version doesn't match the required version which is %u.", REQUIRED_WORLD_DB_VERSION );
		Log.Error("Database","You need to apply the world update queries that start with a larger number than %u. Exiting.", WorldDBVersion );
		delete wqr;
		return false;
	}

	delete wqr;

	QueryResult *cqr = CharacterDatabase.QueryNA( versionquery);
	if( cqr == NULL ){
		Log.Error("Database","Character database is missing the table `arcemu_db_version` OR the table doesn't contain any rows. Can't validate database version. Exiting.");
		return false;
	}

	f = cqr->Fetch();
	uint32 CharDBVersion = f->GetUInt32();

	Log.Notice("Database","Character database version: %u", CharDBVersion );
	if( CharDBVersion != REQUIRED_CHAR_DB_VERSION ){
		Log.Error("Database","Character database version doesn't match the required version which is %u.", REQUIRED_CHAR_DB_VERSION );
		Log.Error("Database","You need to apply the character update queries that start with a larger number than %u. Exiting.", CharDBVersion );
		delete cqr;
		return false;
	}

	delete cqr;

	Log.Success("Database","Database versions successfully validated.");

	return true;
}

bool Master::_StartDB()
{
	Database_World= NULL;
	Database_Character= NULL;
	string hostname, username, password, database;
	int port = 0;
	
	bool result = Config.MainConfig.GetString( "WorldDatabase", "Username", &username );
	Config.MainConfig.GetString( "WorldDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "WorldDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "WorldDatabase", "Port", &port );
	
	Database_World = Database::CreateDatabaseInterface();

	if(result == false)
	{
		Log.Error( "sql","One or more parameters were missing from WorldDatabase directive." );
		return false;
	}

	// Initialize it
	if( !WorldDatabase.Initialize(hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "WorldDatabase", "ConnectionCount", 3 ), 16384 ) )
	{
		Log.Error( "sql","Main database initialization failed. Exiting." );
		return false;
	}

	result = Config.MainConfig.GetString( "CharacterDatabase", "Username", &username );
	Config.MainConfig.GetString( "CharacterDatabase", "Password", &password );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Hostname", &hostname );
	result = !result ? result : Config.MainConfig.GetString( "CharacterDatabase", "Name", &database );
	result = !result ? result : Config.MainConfig.GetInt( "CharacterDatabase", "Port", &port );
	
	Database_Character = Database::CreateDatabaseInterface();

	if(result == false)
	{
		Log.Error( "sql","One or more parameters were missing from Database directive." );
		return false;
	}

	// Initialize it
	if( !CharacterDatabase.Initialize( hostname.c_str(), (unsigned int)port, username.c_str(),
		password.c_str(), database.c_str(), Config.MainConfig.GetIntDefault( "CharacterDatabase", "ConnectionCount", 5 ), 16384 ) )
	{
		Log.Error( "sql","Main database initialization failed. Exiting." );
		return false;
	}

	return true;
}

void Master::_StopDB()
{
	if (Database_World != NULL)
	delete Database_World;
	if (Database_Character != NULL)
	delete Database_Character;
	Database::CleanupLibs();
}

void Master::_HookSignals()
{
	signal( SIGINT, _OnSignal );
	signal( SIGTERM, _OnSignal );
	signal( SIGABRT, _OnSignal );
#ifdef _WIN32
	signal( SIGBREAK, _OnSignal );
#else
	signal( SIGHUP, _OnSignal );
	signal(SIGUSR1, _OnSignal);
#endif
}

void Master::_UnhookSignals()
{
	signal( SIGINT, 0 );
	signal( SIGTERM, 0 );
	signal( SIGABRT, 0 );
#ifdef _WIN32
	signal( SIGBREAK, 0 );
#else
	signal( SIGHUP, 0 );
#endif

}

#ifdef WIN32

Mutex m_crashedMutex;

// Crash Handler
void OnCrash( bool Terminate )
{
		Log.Error( "Crash Handler","Advanced crash handler initialized." );

	if( !m_crashedMutex.AttemptAcquire() )
		TerminateThread( GetCurrentThread(), 0 );

	try
	{
		if( World::getSingletonPtr() != 0 )
		{
			Log.Notice( "sql","Waiting for all database queries to finish..." );
			WorldDatabase.EndThreads();
			CharacterDatabase.EndThreads();
			Log.Notice( "sql","All pending database operations cleared." );
			sWorld.SaveAllPlayers();
			Log.Notice( "sql","Data saved." );
		}
	}
	catch(...)
	{
		Log.Error( "sql","Threw an exception while attempting to save all data." );
	}

	Log.Notice( "Server","Closing." );
	
	// beep
	//printf("\x7");
	
	// Terminate Entire Application
	if( Terminate )
	{
		HANDLE pH = OpenProcess( PROCESS_TERMINATE, TRUE, GetCurrentProcessId() );
		TerminateProcess( pH, 1 );
		CloseHandle( pH );
	}
}

#endif