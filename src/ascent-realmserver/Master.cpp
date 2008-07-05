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

#include "RStdAfx.h"
#include "../ascent-shared/svn_revision.h"

Database * Database_Character;
Database * Database_World;

int main(int argc, char *argv[])
{
	sLog.Init(-1, -1);
	sLog.outString("TexT");
	/* Initialize global timestamp */
	UNIXTIME = time(NULL);

    /* Print Banner */
	Log.Notice("Server", "==============================================================");
	Log.Notice("Server", "| Codename Ascent Cluster - Realm Server                     |");
	Log.Notice("Server", "| Version 0.1, Revision %04u                                 |", BUILD_REVISION);
	Log.Notice("Server", "==============================================================");
	Log.Line();

	Database_Character = Database::CreateDatabaseInterface( 1 );
	Database_World = Database::CreateDatabaseInterface( 1 );
	Log.Success("Database", "Interface Created.");

	new ClusterMgr;
	new ClientMgr;
	ThreadPool.Startup();
	Log.Line();
	Log.Notice("Database", "Connecting to database...");
	if(Database_Character->Initialize("localhost", 3306, "moocow", "moo", "antrix-character", 1, 1000) &&
		Database_World->Initialize("localhost", 3306, "moocow", "moo", "antrix-world", 1, 1000))
	{
		Log.Success("Database", "Connections established.");
	}
	else
	{
		Log.Error("Database", "One or more errors occured while connecting to databases.");
		exit(-1);
	}

	Log.Line();

	Log.Success("Storage", "DBC Files Loaded...");
	Storage_Load();

	Log.Line();
	//CreateSocketEngine();
	//sSocketEngine.SpawnThreads();
	new SocketMgr;
	new SocketGarbageCollector;
	sSocketMgr.SpawnWorkerThreads();

	Log.Success("Network", "Network Subsystem Started.");

	Log.Notice("Network", "Opening Client Port...");
	ListenSocket<WorldSocket> * wsl = new ListenSocket<WorldSocket>("0.0.0.0", 8129);
	bool lsc = wsl->IsOpen();
	
	Log.Notice("Network", "Opening Server Port...");
	ListenSocket<WSSocket> * isl = new ListenSocket<WSSocket>("0.0.0.0", 11010);
	bool ssc = isl->IsOpen();

	Config.MainConfig.SetSource("ascent.conf");
	Config.RealmConfig.SetSource("realms.conf");

	if(!lsc || !ssc)
	{
		Log.Error("Network", "Could not open one of the sockets.");
		return 1;
	}

	/* connect to LS */
	new LogonCommHandler;
	sLogonCommHandler.Startup();

	/* main loop */
	for(;;)
	{
        sLogonCommHandler.UpdateSockets();
		//wsl->Update();
		//isl->Update();
		sClientMgr.Update();
		sClusterMgr.Update();
		Sleep(50);
	}

}

void OnCrash(bool Terminate)
{

}


