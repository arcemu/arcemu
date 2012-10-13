/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
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

#include "StdAfx.h"
#include <git_version.h>
#include "ConsoleCommands.h"

bool HandleTimeDateCommand( BaseConsole *console, int argc, const char *argv[] )
{
	time_t unixTime = UNIXTIME;

	std::stringstream ss;
	ss << "Date and time according to localtime() (american style):" << std::endl;
	ss << localtime( &unixTime )->tm_mon << "-";
	ss << localtime( &unixTime )->tm_mday << "-";
	ss << ( localtime( &unixTime )->tm_year + 1900 ) << "  ";
	ss << localtime( &unixTime )->tm_hour << ":";
	ss << localtime( &unixTime )->tm_min << std::endl << std::endl;

	console->Write( ss.str().c_str() );

	return true;
}

bool HandleInfoCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	uint32 clientsNum = (uint32)sWorld.GetSessionCount();
	int gm = 0;
	int count = 0;
	int avg = 0;
	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
	{
		if(itr->second->GetSession())
		{
			count++;
			avg += itr->second->GetSession()->GetLatency();
			if(itr->second->GetSession()->GetPermissionCount())
				gm++;
		}
	}
	objmgr._playerslock.ReleaseReadLock();

	pConsole->Write("======================================================================\r\n");
	pConsole->Write("Server Information: \r\n");
	pConsole->Write("======================================================================\r\n");
	pConsole->Write("Server Revision: ArcEmu %s/%s-%s-%s (www.arcemu.org)\r\n", BUILD_HASH_STR, CONFIG, PLATFORM_TEXT, ARCH);
	pConsole->Write("Server Uptime: %s\r\n", sWorld.GetUptimeString().c_str());
	pConsole->Write("Current Players: %d (%d GMs, %d queued)\r\n", clientsNum, gm,  0);
	pConsole->Write("Active Thread Count: %u\r\n", ThreadPool.GetActiveThreadCount());
	pConsole->Write("Free Thread Count: %u\r\n", ThreadPool.GetFreeThreadCount());
	pConsole->Write("Average Latency: %.3fms\r\n", count ? ((float)((float)avg / (float)count)) : 0.0f);
	pConsole->Write("CPU Usage: %3.2f %%\r\n", sWorld.GetCPUUsage());
	pConsole->Write("RAM Usage: %4.2f MB\r\n", sWorld.GetRAMUsage());
	pConsole->Write("SQL Query Cache Size (World): %u queries delayed\r\n", WorldDatabase.GetQueueSize());
	pConsole->Write("SQL Query Cache Size (Character): %u queries delayed\r\n", CharacterDatabase.GetQueueSize());

	return true;
}

bool HandleNetworkStatusCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	sSocketMgr.ShowStatus();
	return true;
}

bool HandleGMsCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	WorldPacket data;
	//bool first = true;

	pConsole->Write("There are the following GM's online on this server: \r\n");
	pConsole->Write("======================================================\r\n");
	pConsole->Write("| %21s | %15s | % 03s  |\r\n" , "Name", "Permissions", "Latency");
	pConsole->Write("======================================================\r\n");

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
	{
		if(itr->second->GetSession()->GetPermissionCount())
		{
			pConsole->Write("| %21s | %15s | %03u ms |\r\n" , itr->second->GetName(), itr->second->GetSession()->GetPermissions(), itr->second->GetSession()->GetLatency());
		}
	}
	objmgr._playerslock.ReleaseReadLock();

	pConsole->Write("======================================================\r\n\r\n");
	return true;
}


bool HandleOnlinePlayersCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	WorldPacket data;
	//bool first = true;

	pConsole->Write("There following players online on this server: \r\n");
	pConsole->Write("======================================================\r\n");
	pConsole->Write("| %21s | %15s | % 03s  |\r\n" , "Name", "Level", "Latency");
	pConsole->Write("======================================================\r\n");

	PlayerStorageMap::const_iterator itr;
	objmgr._playerslock.AcquireReadLock();
	for(itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
	{
		pConsole->Write("| %21s | %15u | %03u ms |\r\n" , itr->second->GetName(), itr->second->GetSession()->GetPlayer()->getLevel(), itr->second->GetSession()->GetLatency());
	}
	objmgr._playerslock.ReleaseReadLock();

	pConsole->Write("======================================================\r\n\r\n");
	return true;
}

void ConcatArgs(string & outstr, int argc, int startoffset, const char* argv[])
{
	for(int i = startoffset + 1; i < argc; ++i)
	{
		outstr += argv[i];
		if((i + 1) != (argc))
			outstr += " ";
	}
}
bool HandleAnnounceCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	char pAnnounce[1024];
	string outstr;
	if(argc < 2)
		return false;

	ConcatArgs(outstr, argc, 0, argv);
	snprintf(pAnnounce, 1024, "%sConsole: |r%s", MSG_COLOR_LIGHTBLUE, outstr.c_str());
	sWorld.SendWorldText(pAnnounce); // send message
	pConsole->Write("Message sent.\r\n");
	return true;
}

bool HandleWAnnounceCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	char pAnnounce[1024];
	string outstr;

	if(argc < 2)
		return false;

	ConcatArgs(outstr, argc, 0, argv);
	snprintf(pAnnounce, 1024, "%sConsole: |r%s", MSG_COLOR_LIGHTBLUE, outstr.c_str());
	sWorld.SendWorldWideScreenText(pAnnounce); // send message
	pConsole->Write("Message sent.\r\n");
	return true;
}
bool HandleWhisperCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	char pAnnounce[1024];
	Player* pPlayer;
	string outstr;

	if(argc < 3)
		return false;

	pPlayer = objmgr.GetPlayer(argv[1]);

	if(pPlayer == NULL)
	{
		pConsole->Write("Could not find player, %s.\r\n", argv[1]);
		return true;
	}

	ConcatArgs(outstr, argc, 1, argv);
	snprintf(pAnnounce, 1024, "%sConsole whisper: |r%s", MSG_COLOR_MAGENTA, outstr.c_str());

	pPlayer->BroadcastMessage(pAnnounce);
	pConsole->Write("Message sent to %s.\r\n", pPlayer->GetName());
	return true;
}

bool HandleKickCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(argc < 3)
		return false;

	char pAnnounce[1024];
	Player* pPlayer;
	string outstr;

	pPlayer = objmgr.GetPlayer(argv[1]);
	if(pPlayer == NULL)
	{
		pConsole->Write("Could not find player, %s.\r\n", argv[1]);
		return true;
	}
	ConcatArgs(outstr, argc, 1, argv);
	snprintf(pAnnounce, 1024, "%sConsole:|r %s was removed from the server. Reason: %s.", MSG_COLOR_LIGHTBLUE, pPlayer->GetName(), outstr.c_str());
	pPlayer->BroadcastMessage("You are now being removed by the game by an administrator via the console. Reason: %s", outstr.c_str());
	sWorld.SendWorldText(pAnnounce, NULL);
	pPlayer->Kick(5000);
	pConsole->Write("Kicked player %s.\r\n", pPlayer->GetName());
	return true;
}

bool HandleShutDownCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	uint32 delay = 5;
	if(argc >= 2)
	{
		if(stricmp(argv[1], "fast") == 0)
		{
			PlayerStorageMap::const_iterator itr;
			objmgr._playerslock.AcquireReadLock();
			for(itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
			{
				if(itr->second->GetSession())
					itr->second->SaveToDB(false);
			}
			objmgr._playerslock.ReleaseReadLock();

			exit(0);
		}
		else
		{
			delay = atoi(argv[1]);
		}
	}

	sMaster.m_ShutdownTimer = delay * 1000;
	sMaster.m_ShutdownEvent = true;
	pConsole->Write("Shutdown has initiated.\r\n");
	return true;
}

bool HandleCancelCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	pConsole->Write("Shutdown has been canceled.\r\n");
	sMaster.m_ShutdownTimer = 5000;
	sMaster.m_ShutdownEvent = false;
	return true;
}

bool HandleBanAccountCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(argc < 3)
		return false;

	int32 timeperiod = GetTimePeriodFromString(argv[2]);
	if(timeperiod < 0)
		return false;

	uint32 banned = (timeperiod ? (uint32)UNIXTIME + timeperiod : 1);

	char emptystring = 0;
	char* pReason;
	if(argc == 4)
		pReason = (char*)argv[3];
	else
		pReason = &emptystring;

	/// apply instantly in db
	sLogonCommHandler.Account_SetBanned(argv[1], banned, pReason);

	pConsole->Write("Account '%s' has been banned %s%s. The change will be effective immediately.\r\n", argv[1],
	                timeperiod ? "until " : "forever", timeperiod ? ConvertTimeStampToDataTime(timeperiod + (uint32)UNIXTIME).c_str() : "");

	return true;
}

bool HandleUnbanAccountCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(argc < 2)
		return false;

	sLogonCommHandler.Account_SetBanned(argv[1], 0, "");

	pConsole->Write("Account '%s' has been unbanned.\r\n", argv[1]);
	return true;
}

bool HandleMOTDCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(argc < 2)
	{
		pConsole->Write("The current message of the day is: '%s'.\r\n", sWorld.GetMotd());
	}
	else
	{
		char set_motd[1024];
		string outstr;
		ConcatArgs(outstr, argc, 0, argv);
		snprintf(set_motd, 1024, "%s", outstr.c_str());

		sWorld.SetMotd(set_motd);
		pConsole->Write("The message of the day has been set to: '%s'.\r\n", sWorld.GetMotd());
	}
	return true;
}

bool HandlePlayerInfoCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(argc < 2)
		return false;

	Player* plr = objmgr.GetPlayer(argv[1]);
	if(plr == NULL)
	{
		pConsole->Write("Player not found.\r\n");
		return true;
	}

	pConsole->Write("Player: %s\r\n", plr->GetName());
	pConsole->Write("Race: %s\r\n", plr->myRace->name1);
	pConsole->Write("Class: %s\r\n", plr->myClass->name);
	pConsole->Write("IP: %s\r\n", plr->GetSession()->GetSocket() ? plr->GetSession()->GetSocket()->GetRemoteIP().c_str() : "disconnected");
	pConsole->Write("Level: %u\r\n", plr->getLevel());
	pConsole->Write("Account: %s\r\n", plr->GetSession()->GetAccountNameS());
	return true;
}

void TestConsoleLogin(string & username, string & password, uint32 requestno)
{
	sLogonCommHandler.TestConsoleLogon(username, password, requestno);
}

bool HandleRevivePlayer(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(!argc)
		return false;

	Player* plr = objmgr.GetPlayer(argv[1], false);
	if(!plr)
	{
		pConsole->Write("Could not find player %s.\r\n", argv[1]);
		return true;
	}

	if(plr->IsDead())
	{
		plr->RemoteRevive();
		pConsole->Write("Revived player %s.\r\n", argv[1]);
	}
	else
	{
		pConsole->Write("Player %s is not dead.\r\n", argv[1]);
	}
	return true;
}


bool HandleRehashCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	pConsole->Write("Config file re-parsed.");
	sWorld.Rehash(true);
	return true;
}

bool HandleNameHashCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	if(!argc)
		return false;
	string spname;
	ConcatArgs(spname, argc, 0, argv);
	pConsole->Write("Name Hash for %s is 0x%X" , spname.c_str() , crc32((const unsigned char*)spname.c_str(), (unsigned int)spname.length()));
	sWorld.Rehash(true);
	return true;
}

bool HandleClearConsoleCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	system("cls");
	pConsole->Write("Out of the ashes, Chuck Norris appears! With a roundhouse kick, your console shall now be cleaned!");
	return true;
}

bool HandleReloadConsoleCommand(BaseConsole* pConsole, int argc, const char* argv[])
{
	sWorld.SendWorldText("Support for reloading tables on the fly was disabled in Arcemu revision 3621. You are seeing this message because apparently reading SVN changelog or using forums search is way over the head of some of our users.", 0);
	return true;

	/*
	if( argc < 2 || strlen(argv[1]) < 3 )
		return false;

	char str[200];
	int ret = 0;
	uint32 mstime = getMSTime();

	snprintf(str, 200, "%sConsole initiated server-side reload of table `%s`. The server may experience some lag while this occurs.",
		MSG_COLOR_LIGHTRED, argv[1]);
	sWorld.SendWorldText(str, 0);

	if( !stricmp(argv[1], "spell_disable") )
	{
		objmgr.ReloadDisabledSpells();
		ret = 1;
	}
	else if( !stricmp(argv[1], "vendors") )
	{
		objmgr.ReloadVendors();
		ret = 1;
	}
	else
		ret = Storage_ReloadTable(argv[1]);


	if (ret == 0)
	{
		pConsole->Write( "Database reload failed.\r\n" );
		snprintf(str, 200, "%sDatabase reload failed.", MSG_COLOR_LIGHTRED);
	}
	else
	{
		uint32 timediff = (unsigned int)(getMSTime() - mstime);
		pConsole->Write( "Database reload completed in %u ms.\r\n", timediff );
		snprintf( str, 200, "%sDatabase reload completed in %u ms.", MSG_COLOR_LIGHTBLUE, timediff );
	}
	sWorld.SendWorldText( str, 0 );

	return true;

	*/

}
bool HandleScriptEngineReloadCommand(BaseConsole*, int, const char* [])
{
	sScriptMgr.ReloadScriptEngines();
	return true;
}
