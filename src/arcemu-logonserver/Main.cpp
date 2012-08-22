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
#include <signal.h>
#include "git_version.h"
#ifndef WIN32
#include <sys/resource.h>
#endif
#include "../arcemu-shared/arcemu_getopt.h"

#define BANNER "ArcEmu %s %s/%s-%s (%s) :: Logon Server"

#ifndef WIN32
#include <sched.h>
#endif

#include "LogonConfig.h"
#include "LogonConfigParser.h"

// Database impl
Database* sLogonSQL;
initialiseSingleton(LogonServer);
Arcemu::Threading::AtomicBoolean mrunning(true);
Mutex _authSocketLock;
set<AuthSocket*> _authSockets;

LogonConfigData configFile;

/*** Signal Handler ***/
void _OnSignal(int s)
{
	switch(s)
	{
#ifndef WIN32
		case SIGHUP:
			{
				LOG_DETAIL("Received SIGHUP signal, reloading accounts.");
				AccountMgr::getSingleton().ReloadAccounts(true);
			}
			break;
#endif
		case SIGINT:
		case SIGTERM:
		case SIGABRT:
#ifdef _WIN32
		case SIGBREAK:
#endif
			mrunning.SetVal(false);
			break;
	}

	signal(s, _OnSignal);
}

int main(int argc, char** argv)
{
#ifndef WIN32
	rlimit rl;
	if(getrlimit(RLIMIT_CORE, &rl) == -1)
		printf("getrlimit failed. This could be problem.\n");
	else
	{
		rl.rlim_cur = rl.rlim_max;
		if(setrlimit(RLIMIT_CORE, &rl) == -1)
			printf("setrlimit failed. Server may not save core.dump files.\n");
	}
#endif

	new LogonServer;

	// Run!
	LogonServer::getSingleton().Run(argc, argv);
	delete LogonServer::getSingletonPtr();
}


/**
  * Initialises the logon database
  *
  * Reads the configs\logon.conf file and parses the <LogonDatabase> tag
  *
  * Any errors in this file, such as a missing parameter should be caught
  * and the user notified in an intelligent way
  *
  * If no errors are found, the database is initialized
  **/
bool startdb()
{
	std::string lhostname, lusername, lpassword, ldatabase;
	int lport = 0;

	sLogonSQL = Database::CreateDatabaseInterface();

	// Initialize it
	if( !sLogonSQL->Initialize( configFile.logondb.host.c_str(),
								configFile.logondb.port,
								configFile.logondb.username.c_str(),
								configFile.logondb.password.c_str(),
								configFile.logondb.database.c_str(),
								5,
								16384 
								) )
	{
		LOG_ERROR("sql: Logon database initialization failed. Exiting.");
		return false;
	}

	return true;
}

#define DEF_VALUE_NOT_SET 0xDEADBEEF


Mutex m_allowedIpLock;
vector<AllowedIP> m_allowedIps;
vector<AllowedIP> m_allowedModIps;

bool IsServerAllowed(unsigned int IP)
{
	m_allowedIpLock.Acquire();
	for(vector<AllowedIP>::iterator itr = m_allowedIps.begin(); itr != m_allowedIps.end(); ++itr)
	{
		if(ParseCIDRBan(IP, itr->IP, itr->Bytes))
		{
			m_allowedIpLock.Release();
			return true;
		}
	}
	m_allowedIpLock.Release();
	return false;
}

bool IsServerAllowedMod(unsigned int IP)
{
	m_allowedIpLock.Acquire();
	for(vector<AllowedIP>::iterator itr = m_allowedModIps.begin(); itr != m_allowedModIps.end(); ++itr)
	{
		if(ParseCIDRBan(IP, itr->IP, itr->Bytes))
		{
			m_allowedIpLock.Release();
			return true;
		}
	}
	m_allowedIpLock.Release();
	return false;
}

bool rehash2()
{
	char* config_file = (char*)CONFDIR "/logon.conf.xml";
	LogonConfigParser configParser;
	if( !configParser.parseFile( config_file ) )
	{
		LOG_ERROR("Config file could not be loaded.");
		return false;
	}
	configFile = configParser.getData();

	std::string ips = configFile.security.allowed_ip_ranges; 
	std::string ipsmod = configFile.security.allowed_mod_ip_ranges;

	std::vector< std::string > vips = StrSplit(ips, " ");
	std::vector< std::string > vipsmod = StrSplit(ips, " ");

	m_allowedIpLock.Acquire();
	m_allowedIps.clear();
	m_allowedModIps.clear();
	
	std::vector< std::string >::iterator itr;
	for(itr = vips.begin(); itr != vips.end(); ++itr)
	{
		string::size_type i = itr->find("/");
		if(i == string::npos)
		{
			LOG_ERROR("IP: %s could not be parsed. Ignoring", itr->c_str());
			continue;
		}

		string stmp = itr->substr(0, i);
		string smask = itr->substr(i + 1);

		unsigned int ipraw = MakeIP(stmp.c_str());
		unsigned char ipmask = (char)atoi(smask.c_str());
		if(ipraw == 0 || ipmask == 0)
		{
			LOG_ERROR("IP: %s could not be parsed. Ignoring", itr->c_str());
			continue;
		}

		AllowedIP tmp;
		tmp.Bytes = ipmask;
		tmp.IP = ipraw;
		m_allowedIps.push_back(tmp);
	}

	for(itr = vipsmod.begin(); itr != vipsmod.end(); ++itr)
	{
		string::size_type i = itr->find("/");
		if(i == string::npos)
		{
			LOG_ERROR("IP: %s could not be parsed. Ignoring", itr->c_str());
			continue;
		}

		string stmp = itr->substr(0, i);
		string smask = itr->substr(i + 1);

		unsigned int ipraw = MakeIP(stmp.c_str());
		unsigned char ipmask = (char)atoi(smask.c_str());
		if(ipraw == 0 || ipmask == 0)
		{
			LOG_ERROR("IP: %s could not be parsed. Ignoring", itr->c_str());
			continue;
		}

		AllowedIP tmp;
		tmp.Bytes = ipmask;
		tmp.IP = ipraw;
		m_allowedModIps.push_back(tmp);
	}

	if(InformationCore::getSingletonPtr() != NULL)
		sInfoCore.CheckServers();

	m_allowedIpLock.Release();


	return true;
}


void LogonServer::Run(int argc, char** argv)
{
	UNIXTIME = time(NULL);
	g_localTime = *localtime(&UNIXTIME);
	char* config_file = (char*)CONFDIR "/logon.conf";
	int file_log_level = DEF_VALUE_NOT_SET;
	int screen_log_level = DEF_VALUE_NOT_SET;
	int do_check_conf = 0;
	int do_version = 0;

	struct arcemu_option longopts[] =
	{
		{ "checkconf",			arcemu_no_argument,				&do_check_conf,			1		},
		{ "screenloglevel",		arcemu_required_argument,		&screen_log_level,		1		},
		{ "fileloglevel",		arcemu_required_argument,		&file_log_level,		1		},
		{ "version",			arcemu_no_argument,				&do_version,			1		},
		{ "conf",				arcemu_required_argument,		NULL,					'c'		},
		{ 0, 0, 0, 0 }
	};

	int c;
	while((c = arcemu_getopt_long_only(argc, argv, ":f:", longopts, NULL)) != -1)
	{
		switch(c)
		{
			case 'c':
				/* Log filename was set */
				config_file = new char[strlen(arcemu_optarg)];
				strcpy(config_file, arcemu_optarg);
				break;
			case 0:
				break;
			default:
				sLog.Init(0, LOGON_LOG);
				sLog.outBasic("Usage: %s [--checkconf] [--fileloglevel <level>] [--conf <filename>] [--version]", argv[0]);
				return;
		}
	}

	sLog.Init(0, LOGON_LOG);
	
	sLog.outBasic(BANNER, BUILD_TAG, BUILD_HASH_STR, CONFIG, PLATFORM_TEXT, ARCH);
	sLog.outErrorSilent(BANNER, BUILD_TAG, BUILD_HASH_STR, CONFIG, PLATFORM_TEXT, ARCH); // Echo off.

	if(do_version)
	{
		sLog.Close();
		return;
	}

	if( do_check_conf )
	{
		LOG_BASIC("Checking config file: %s", config_file);
		
		LogonConfigParser parser;		
		if( parser.parseFile( "configs/logon.conf.xml" ) )
			LOG_BASIC("  Passed without errors.");
		else
			LOG_BASIC("  Encountered one or more errors.");

		sLog.Close();
		return;
	}

	/* set new log levels */
	if(file_log_level != (int)DEF_VALUE_NOT_SET)
		sLog.SetFileLoggingLevel(file_log_level);

	printf("The key combination <Ctrl-C> will safely shut down the server at any time.\n");
	Log.Success("System", "Initializing Random Number Generators...");

	Log.Success("Config", "Loading Config Files...");
	if(!rehash2())
	{
		sLog.Close();
		return;
	}

	Log.Success("ThreadMgr", "Starting...");
	sLog.SetFileLoggingLevel(  configFile.log.level );

	ThreadPool.Startup();

	if(!startdb())
	{
		sLog.Close();
		return;
	}

	Log.Success("AccountMgr", "Starting...");
	new AccountMgr;
	new IPBanner;

	Log.Success("InfoCore", "Starting...");
	new InformationCore;

	new PatchMgr;
	Log.Notice("AccountMgr", "Precaching accounts...");
	sAccountMgr.ReloadAccounts(true);
	Log.Success("AccountMgr", "%u accounts are loaded and ready.", sAccountMgr.GetCount());

	// Spawn periodic function caller thread for account reload every 10mins
	uint32 atime = configFile.rates.account_refresh;
	atime *= 1000;
	//SpawnPeriodicCallThread(AccountMgr, AccountMgr::getSingletonPtr(), &AccountMgr::ReloadAccountsCallback, time);
	PeriodicFunctionCaller<AccountMgr> * pfc = new PeriodicFunctionCaller<AccountMgr>(AccountMgr::getSingletonPtr(), &AccountMgr::ReloadAccountsCallback, atime);
	ThreadPool.ExecuteTask(pfc);

	// Load conf settings..
	uint32 cport = configFile.host.logon_port;
	uint32 sport = configFile.host.is_port;
	std::string host = configFile.host.logon_address;
	std::string shost = configFile.host.is_address;

	min_build = LOGON_MINBUILD;
	max_build = LOGON_MAXBUILD;

	std::string logon_pass = configFile.security.remote_password;
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();
	memcpy(sql_hash, hash.GetDigest(), 20);

	ThreadPool.ExecuteTask(new LogonConsoleThread);

	new SocketMgr;
	new SocketGarbageCollector;

	ListenSocket<AuthSocket> * cl = new ListenSocket<AuthSocket>(host.c_str(), cport);
	ListenSocket<LogonCommServerSocket> * sl = new ListenSocket<LogonCommServerSocket>(shost.c_str(), sport);

	sSocketMgr.SpawnWorkerThreads();

	// Spawn auth listener
	// Spawn interserver listener
	bool authsockcreated = cl->IsOpen();
	bool intersockcreated = sl->IsOpen();
	if(authsockcreated && intersockcreated)
	{
#ifdef WIN32
		ThreadPool.ExecuteTask(cl);
		ThreadPool.ExecuteTask(sl);
#endif
		// hook signals
		sLog.outString("Hooking signals...");
		signal(SIGINT, _OnSignal);
		signal(SIGTERM, _OnSignal);
		signal(SIGABRT, _OnSignal);
#ifdef _WIN32
		signal(SIGBREAK, _OnSignal);
#else
		signal(SIGHUP, _OnSignal);
#endif

		/* write pid file */
		FILE* fPid = fopen("logonserver.pid", "w");
		if(fPid)
		{
			uint32 pid;
#ifdef WIN32
			pid = GetCurrentProcessId();
#else
			pid = getpid();
#endif
			fprintf(fPid, "%u", (unsigned int)pid);
			fclose(fPid);
		}
		uint32 loop_counter = 0;
		//ThreadPool.Gobble();
		sLog.outString("Success! Ready for connections");
		while(mrunning.GetVal())
		{
			if(!(++loop_counter % 20))	 // 20 seconds
				CheckForDeadSockets();

			if(!(loop_counter % 300))	// 5mins
				ThreadPool.IntegrityCheck();

			if(!(loop_counter % 5))
			{
				sInfoCore.TimeoutSockets();
				sSocketGarbageCollector.Update();
				CheckForDeadSockets();			  // Flood Protection
				UNIXTIME = time(NULL);
				g_localTime = *localtime(&UNIXTIME);
			}

			PatchMgr::getSingleton().UpdateJobs();
			Arcemu::Sleep(1000);
		}

		sLog.outString("Shutting down...");
		signal(SIGINT, 0);
		signal(SIGTERM, 0);
		signal(SIGABRT, 0);
#ifdef _WIN32
		signal(SIGBREAK, 0);
#else
		signal(SIGHUP, 0);
#endif
	}
	else
	{
		LOG_ERROR("Error creating sockets. Shutting down...");
	}

	pfc->kill();

	cl->Close();
	sl->Close();
	sSocketMgr.CloseAll();
#ifdef WIN32
	sSocketMgr.ShutdownThreads();
#endif
	sLogonConsole.Kill();
	delete LogonConsole::getSingletonPtr();

	// kill db
	sLog.outString("Waiting for database to close..");
	sLogonSQL->EndThreads();
	sLogonSQL->Shutdown();
	delete sLogonSQL;

	ThreadPool.Shutdown();

	// delete pid file
	remove("logonserver.pid");

	delete AccountMgr::getSingletonPtr();
	delete InformationCore::getSingletonPtr();
	delete PatchMgr::getSingletonPtr();
	delete IPBanner::getSingletonPtr();
	delete SocketMgr::getSingletonPtr();
	delete SocketGarbageCollector::getSingletonPtr();
	delete pfc;
	delete cl;
	delete sl;
	LOG_BASIC("Shutdown complete.");
	sLog.Close();
}

void OnCrash(bool Terminate)
{

}

void LogonServer::CheckForDeadSockets()
{
	_authSocketLock.Acquire();
	time_t t = time(NULL);
	time_t diff;
	set<AuthSocket*>::iterator itr = _authSockets.begin();
	set<AuthSocket*>::iterator it2;
	AuthSocket* s;

	for(itr = _authSockets.begin(); itr != _authSockets.end();)
	{
		it2 = itr;
		s = (*it2);
		++itr;

		diff = t - s->GetLastRecv();
		if(diff > 300)		   // More than 5mins
		{
			_authSockets.erase(it2);
			s->removedFromSet = true;
			s->Disconnect();
		}
	}
	_authSocketLock.Release();
}