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
initialiseSingleton(LogonCommHandler);
#ifndef CLUSTERING
LogonCommHandler::LogonCommHandler()
{
	idhigh = 1;
	next_request = 1;
	pings = !Config.MainConfig.GetBoolDefault("LogonServer", "DisablePings", false);
	string logon_pass = Config.MainConfig.GetStringDefault("LogonServer", "RemotePassword", "r3m0t3");
	
	// sha1 hash it
	Sha1Hash hash;
	hash.UpdateData(logon_pass);
	hash.Finalize();

	memset(sql_passhash,0,20);
	memcpy(sql_passhash, hash.GetDigest(), 20);

	// cleanup
	servers.clear();
	realms.clear();

}

LogonCommHandler::~LogonCommHandler()
{
	for(set<LogonServer*>::iterator i = servers.begin(); i != servers.end(); ++i)
		delete (*i);

	for(set<Realm*>::iterator i = realms.begin(); i != realms.end(); ++i)
		delete (*i);
}

LogonCommClientSocket * LogonCommHandler::ConnectToLogon(string Address, uint32 Port)
{
	LogonCommClientSocket * conn = ConnectTCPSocket<LogonCommClientSocket>(Address.c_str(), Port);	
	return conn;
}

void LogonCommHandler::RequestAddition(LogonCommClientSocket * Socket)
{
	set<Realm*>::iterator itr = realms.begin();

	for(; itr != realms.end(); ++itr)
	{
		WorldPacket data(RCMSG_REGISTER_REALM, 100);

		// Add realm to the packet
		Realm * realm = *itr;
		data << realm->Name;
		data << realm->Address;
		data << uint8(realm->Icon);
		data << uint8(realm->TimeZone);
		data << float(realm->Population);
		data << uint8(realm->Lock);
		Socket->SendPacket(&data,false);
	}
}

class LogonCommWatcherThread : public ThreadBase
{
	bool running;
#ifdef WIN32
	HANDLE hEvent;
#endif
public:

	LogonCommWatcherThread()
	{
#ifdef WIN32
		hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
#endif
		running = true;
	}

	~LogonCommWatcherThread()
	{

	}

	void OnShutdown()
	{
		running = false;
#ifdef WIN32
		SetEvent( hEvent );
#endif
	}

	bool run()
	{
		sLogonCommHandler.ConnectAll();
		while( running )
		{
			sLogonCommHandler.UpdateSockets();
#ifdef WIN32
			WaitForSingleObject( hEvent, 5000 );
#else
			Sleep( 5000 );
#endif
		}

		return true;
	}
};

void LogonCommHandler::Startup()
{
	// Try to connect to all logons.
	LoadRealmConfiguration();

	Log.Notice("LogonCommClient", "Loading forced permission strings...");
	QueryResult * result = CharacterDatabase.Query("SELECT * FROM account_forced_permissions");
	if( result != NULL )
	{
		do 
		{
			string acct = result->Fetch()[0].GetString();
			string perm = result->Fetch()[1].GetString();

			arcemu_TOUPPER(acct);
            forced_permissions.insert(make_pair(acct,perm));

		} while (result->NextRow());
		delete result;
	}

	ThreadPool.ExecuteTask( new LogonCommWatcherThread() );
}

void LogonCommHandler::ConnectAll()
{
	Log.Notice("LogonCommClient", "Attempting to connect to logon server...");
	for(set<LogonServer*>::iterator itr = servers.begin(); itr != servers.end(); ++itr)
		Connect(*itr);
}

const string* LogonCommHandler::GetForcedPermissions(string& username)
{
	ForcedPermissionMap::iterator itr = forced_permissions.find(username);
	if(itr == forced_permissions.end())
		return NULL;

	return &itr->second;
}

void LogonCommHandler::Connect(LogonServer * server)
{
	if(sMaster.m_ShutdownEvent == true && sMaster.m_ShutdownTimer <= 120000) // 2minutes
		return;

	server->RetryTime = (uint32)UNIXTIME + 10;
	server->Registered = false;
	LogonCommClientSocket * conn = ConnectToLogon(server->Address, server->Port);
	logons[server] = conn;
	if(conn == 0)
	{
		Log.Notice("LogonCommClient", "Connection failed. Will try again in 10 seconds.");
		return;
	}
	Log.Notice("LogonCommClient", "Authenticating...");
	uint32 tt = (uint32)UNIXTIME + 10;
	conn->SendChallenge();
	while(!conn->authenticated)
	{
		if((uint32)UNIXTIME >= tt)
		{
			Log.Notice("LogonCommClient", "Authentication timed out.");
			conn->Disconnect();
			logons[server]=NULL;
			return;
		}

		Sleep(50);
	}

	if(conn->authenticated != 1)
	{
		Log.Notice("LogonCommClient","Authentication failed.");
		logons[server] = 0;
		conn->Disconnect();
		return;
	}

	Log.Notice("LogonCommClient","Authentication OK.");
  Log.Notice("LogonCommClient", "Logonserver was connected on [%s:%u].", server->Address.c_str(), server->Port );

	// Send the initial ping
	conn->SendPing();

	Log.Notice("LogonCommClient", "Registering Realms...");
	conn->_id = server->ID;

	RequestAddition(conn);

	uint32 st = (uint32)UNIXTIME + 10;

	// Wait for register ACK
	while(server->Registered == false)
	{
		// Don't wait more than.. like 10 seconds for a registration
		if((uint32)UNIXTIME >= st)
		{
			Log.Notice("LogonCommClient", "Realm registration timed out.");
			logons[server] = 0;
			conn->Disconnect();
			break;
		}
		Sleep(50);
	}

	if(!server->Registered)
		return;

	// Wait for all realms to register
	Sleep(200);

	Log.Notice("LogonCommClient", "Logonserver latency is %ums.", conn->latency);
}

void LogonCommHandler::AdditionAck(uint32 ID, uint32 ServID)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	for(; itr != logons.end(); ++itr)
	{
		if(itr->first->ID == ID)
		{
			itr->first->ServerID = ServID;
			itr->first->Registered = true;
			return;
		}
	}
}

void LogonCommHandler::UpdateSockets()
{
	mapLock.Acquire();

	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	LogonCommClientSocket * cs;
	uint32 t = (uint32)UNIXTIME;
	for(; itr != logons.end(); ++itr)
	{
		cs = itr->second;
		if(cs != 0)
		{
			if(!pings) continue;

            if(cs->IsDeleted() || !cs->IsConnected())
            {
                cs->_id = 0;
                itr->second = 0;
                continue;
            }

			if(cs->last_pong < t && ((t - cs->last_pong) > 60))
			{
				// no pong for 60 seconds -> remove the socket
				printf(" >> realm id %u connection dropped due to pong timeout.\n", (unsigned int)itr->first->ID);
				cs->_id = 0;
				cs->Disconnect();
				itr->second = 0;
				continue;
			}
            
			if( (t - cs->last_ping) > 15 )
			{
				// send a ping packet.
				cs->SendPing();
			}
		}
		else
		{
			// check retry time
			if(t >= itr->first->RetryTime)
			{
				Connect(itr->first);
			}
		}
	}
	mapLock.Release();
}

void LogonCommHandler::ConnectionDropped(uint32 ID)
{
	mapLock.Acquire();
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	for(; itr != logons.end(); ++itr)
	{
		if(itr->first->ID == ID && itr->second != 0)
		{
			sLog.outColor(TNORMAL, " >> realm id %u connection was dropped unexpectedly. reconnecting next loop.", ID);
			sLog.outColor(TNORMAL, "\n");
			itr->second = 0;
			break;
		}
	}
	mapLock.Release();
}

uint32 LogonCommHandler::ClientConnected(string AccountName, WorldSocket * Socket)
{
	uint32 request_id = next_request++;
	size_t i = 0;
	const char * acct = AccountName.c_str();
	sLog.outDebug ( " >> sending request for account information: `%s` (request %u).", AccountName.c_str(), request_id);
	//  sLog.outColor(TNORMAL, "\n");

	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0)
	{
		// No valid logonserver is connected.
		return (uint32)-1;
	}

	LogonCommClientSocket * s = itr->second;
	if( s == NULL )
		return (uint32)-1;

	pendingLock.Acquire();

	WorldPacket data(RCMSG_REQUEST_SESSION, 100);
	data << request_id;

	// strip the shitty hash from it
	for(; acct[i] != '#' && acct[i] != '\0'; ++i )
		data.append( &acct[i], 1 );

	data.append( "\0", 1 );
	s->SendPacket(&data,false);

	pending_logons[request_id] = Socket;
	pendingLock.Release();

	return request_id;
}

void LogonCommHandler::UnauthedSocketClose(uint32 id)
{
	pendingLock.Acquire();
	pending_logons.erase(id);
	pendingLock.Release();
}

void LogonCommHandler::RemoveUnauthedSocket(uint32 id)
{
	pending_logons.erase(id);
}

void LogonCommHandler::LoadRealmConfiguration()
{
	LogonServer * ls = new LogonServer;
	ls->ID = idhigh++;
	ls->Name = Config.RealmConfig.GetStringDefault("LogonServer", "Name", "UnkLogon");
	ls->Address = Config.RealmConfig.GetStringDefault("LogonServer", "Address", "127.0.0.1");
	ls->Port = Config.RealmConfig.GetIntDefault("LogonServer", "Port", 8093);
	servers.insert(ls);

	uint32 realmcount = Config.RealmConfig.GetIntDefault("LogonServer", "RealmCount", 1);
	if(realmcount == 0)
	{
		sLog.outColor(TRED, "\n   >> no realms found. this server will not be online anywhere!\n");
	}
	else
	{
		for(uint32 i = 1; i < realmcount+1; ++i)
		{
			Realm * realm = new Realm;
			realm->Name = Config.RealmConfig.GetStringVA("Name", "SomeRealm", "Realm%u", i);
			realm->Address = Config.RealmConfig.GetStringVA("Address", "127.0.0.1:8129", "Realm%u", i);
			realm->TimeZone = Config.RealmConfig.GetIntVA("TimeZone", 1, "Realm%u", i);
			realm->Population = Config.RealmConfig.GetFloatVA("Population", 0, "Realm%u", i);
			realm->Lock = Config.RealmConfig.GetIntVA("Lock", 0, "Realm%u", i);			
			string rt = Config.RealmConfig.GetStringVA("Icon", "Normal", "Realm%u", i);
			uint32 type;

			// process realm type
			if( stricmp(rt.c_str(), "pvp")==0 )
				type = REALMTYPE_PVP;
			else if( stricmp(rt.c_str(), "rp")==0 )
				type = REALMTYPE_RP;
			else if( stricmp(rt.c_str(), "rppvp")==0 )
				type = REALMTYPE_RPPVP;
			else
				type = REALMTYPE_NORMAL;

			_realmType = type;

			realm->Icon = type;
			realms.insert(realm);
		}
	}
}

void LogonCommHandler::UpdateAccountCount(uint32 account_id, uint8 add)
{
	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}
	itr->second->UpdateAccountCount(account_id, add);
}

void LogonCommHandler::TestConsoleLogon(string& username, string& password, uint32 requestnum)
{
	string newuser = username;
	string newpass = password;
	string srpstr;

	arcemu_TOUPPER(newuser);
	arcemu_TOUPPER(newpass);

	srpstr = newuser + ":" + newpass;

	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	Sha1Hash hash;
	hash.UpdateData(srpstr);
	hash.Finalize();

	WorldPacket data(RCMSG_TEST_CONSOLE_LOGIN, 100);
	data << requestnum;
	data << newuser;
	data.append(hash.GetDigest(), 20);

	itr->second->SendPacket(&data, false);
}

// db funcs
void LogonCommHandler::Account_SetBanned(const char * account, uint32 banned, const char *reason)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 300);
	data << uint32(1);		// 1 = ban
	data << account;
	data << banned;
	data << reason;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetGM(const char * account, const char * flags)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(2);		// 2 = set gm
	data << account;
	data << flags;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::Account_SetMute(const char * account, uint32 muted)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(3);		// 3 = mute
	data << account;
	data << muted;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Add(const char * ip, uint32 duration,const char *reason)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 300);
	data << uint32(4);		// 4 = ipban add
	data << ip;
	data << duration;
	data << reason;
	itr->second->SendPacket(&data, false);
}

void LogonCommHandler::IPBan_Remove(const char * ip)
{
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}

	WorldPacket data(RCMSG_MODIFY_DATABASE, 50);
	data << uint32(5);		// 5 = ipban remove
	data << ip;
	itr->second->SendPacket(&data, false);
}

#endif
