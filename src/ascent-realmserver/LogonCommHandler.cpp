/*
 * ArcEmu MMORPG Server
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

#include "RStdAfx.h"
initialiseSingleton(LogonCommHandler);

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
	memcpy(sql_passhash, hash.GetDigest(), 20);
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
	WorldPacket data(RCMSG_REGISTER_REALM, 100);
	for(; itr != realms.end(); ++itr)
	{
		data.clear();

		// Add realm to the packet
		Realm * realm = *itr;
		data << realm->Name;
		data << realm->Address;
//		data << realm->Colour;
		data << realm->Icon;
		data << realm->TimeZone;
		data << realm->Population;
		data << realm->Lock;
		Socket->SendPacket(&data);
	}
}

void LogonCommHandler::Startup()
{
	// Try to connect to all logons.
	sLog.outColor(TNORMAL, "\n >> loading realms and logon server definitions... ");
	LoadRealmConfiguration();

	sLog.outColor(TNORMAL, " >> attempting to connect to all logon servers... \n");
	for(set<LogonServer*>::iterator itr = servers.begin(); itr != servers.end(); ++itr)
		Connect(*itr);

	sLog.outColor(TNORMAL, "\n");
}

void LogonCommHandler::Connect(LogonServer * server)
{
	sLog.outColor(TNORMAL, "	>> connecting to `%s` on `%s:%u`...", server->Name.c_str(), server->Address.c_str(), server->Port);
	server->RetryTime = uint32(time(NULL) + 10);
	server->Registered = false;
	LogonCommClientSocket * conn = ConnectToLogon(server->Address, server->Port);
	logons[server] = conn;
	if(conn == 0)
	{
		sLog.outColor(TRED, " fail!\n	   server connection failed. I will try again later.");
		sLog.outColor(TNORMAL, "\n");
		return;
	}
	sLog.outColor(TGREEN, " ok!\n");
	sLog.outColor(TNORMAL, "        >> authenticating...\n");
	sLog.outColor(TNORMAL, "        >> ");
	uint32 tt = uint32(time(NULL) + 10);
	conn->SendChallenge();
	sLog.outColor(TNORMAL, "        >> result:");
	while(!conn->authenticated)
	{
		if((uint32)time(NULL) >= tt)
		{
			sLog.outColor(TYELLOW, " timeout.\n");
			return;
		}

		Sleep(50);
	}

	if(conn->authenticated != 1)
	{
		sLog.outColor(TRED, " failure.\n");
		logons[server] = 0;
		conn->Disconnect();
		return;
	}
	else
		sLog.outColor(TGREEN, " ok!\n");

	// Send the initial ping
	conn->SendPing();

	sLog.outColor(TNORMAL, "        >> registering realms... ");
	conn->_id = server->ID;

	RequestAddition(conn);

	uint32 st = uint32(time(NULL) + 10);

	// Wait for register ACK
	while(server->Registered == false)
	{
		// Don't wait more than.. like 10 seconds for a registration
		if((uint32)time(NULL) >= st)
		{
			sLog.outColor(TYELLOW, "timeout.");
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

	sLog.outColor(TNORMAL, "\n        >> ping test: ");
	sLog.outColor(TYELLOW, "%ums", conn->latency);
	sLog.outColor(TNORMAL, "\n");
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
	uint32 t = uint32(time(NULL));
	for(; itr != logons.end(); ++itr)
	{
		cs = itr->second;
		if(cs != 0)
		{
			if(!pings) continue;

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
	sLog.outDebug ( " >> sending request for account information: `%s` (request %u).", AccountName.c_str(), request_id);
  //  sLog.outColor(TNORMAL, "\n");
	
	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return (uint32)-1;
	}
	pendingLock.Acquire();

	WorldPacket data(RCMSG_REQUEST_SESSION, 100);
	data << request_id;
	data << AccountName;
	itr->second->SendPacket(&data);

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

void LogonCommHandler::LogonDatabaseSQLExecute(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	char query[1024];
	vsnprintf(query, 1024, str, ap);
	va_end(ap);
	string q = string(query);

	WorldPacket data(RCMSG_SQL_EXECUTE, q.size()+1);
	data << q;
	
	// Send request packet to server.
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}
	itr->second->SendPacket(&data);
}

void LogonCommHandler::LogonDatabaseReloadAccounts()
{
	WorldPacket data(RCMSG_RELOAD_ACCOUNTS, 1);
	map<LogonServer*, LogonCommClientSocket*>::iterator itr = logons.begin();
	if(logons.size() == 0 || itr->second == 0)
	{
		// No valid logonserver is connected.
		return;
	}
	itr->second->SendPacket(&data);
}

void LogonCommHandler::LoadRealmConfiguration()
{
	LogonServer * ls = new LogonServer;
	ls->ID = idhigh++;
	ls->Name = Config.RealmConfig.GetStringDefault("LogonServer", "Name", "UnkLogon");
	ls->Address = Config.RealmConfig.GetStringDefault("LogonServer", "Address", "127.0.0.1");
	ls->Port = Config.RealmConfig.GetIntDefault("LogonServer", "Port", 8093);
	servers.insert(ls);
	sLog.outColor(TYELLOW, "1 servers, ");

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
//			realm->Colour = Config.RealmConfig.GetIntVA("Colour", 1, "Realm%u", i);
			realm->TimeZone = Config.RealmConfig.GetIntVA("TimeZone", 1, "Realm%u", i);
			realm->Population = Config.RealmConfig.GetFloatVA("Population", 0, "Realm%u", i);
			string rt = Config.RealmConfig.GetStringVA("Icon", "Normal", "Realm%u", i);
			uint32 type;

			// process realm type
			if(!stricmp(rt.c_str(), "pvp"))
				type = REALMTYPE_PVP;
			else if(!stricmp(rt.c_str(), "rp"))
				type = REALMTYPE_RP;
			else if(!stricmp(rt.c_str(), "rppvp"))
				type = REALMTYPE_RPPVP;
			else
				type = REALMTYPE_NORMAL;

			_realmType = type;

			realm->Icon = type;
			realms.insert(realm);
		}
		sLog.outColor(TBLUE, "%u realms.\n", realmcount);
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
