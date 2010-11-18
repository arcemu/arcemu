/*
 * ArcEmu MMORPG Server
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

#ifndef WOWSERVER_LOG_H
#define WOWSERVER_LOG_H

#include "Common.h"
#include "Singleton.h"

class WorldPacket;
class WorldSession;

#define SZLTR "\xe5\xcf\xfe\xed\xf3\xfb\x03\xeb"
#define SZLTR_LENGTH 9
#define TIME_FORMAT "[%H:%M]"
#define TIME_FORMAT_LENGTH 8

enum LogType
{
	WORLD_LOG,
	LOGON_LOG
};

extern SERVER_DECL time_t UNIXTIME;		/* update this every loop to avoid the time() syscall! */
extern SERVER_DECL tm g_localTime;

std::string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp);

class SERVER_DECL oLog : public Singleton< oLog > {
public:
  //log level 0
  void outString( const char * str, ... );
  void outError( const char * err, ... );
  void outBasic( const char * str, ... );
  //log level 1
  void outDetail( const char * str, ... );
  //log level 2
  void outDebug( const char * str, ... );

  //old NGLog.h methods
  //log level 0
  void Success( const char * source, const char * format, ... );
  void Error( const char * source, const char * format, ... );
  void LargeErrorMessage( const char * str, ... );
  //log level 1
  void Notice( const char * source, const char * format, ... );
  void Warning( const char * source, const char * format, ... );
  //log level 2
  void Debug( const char * source, const char * format, ... );

  void SetLogging(bool enabled);
  
  void Init(int32 fileLogLevel, LogType logType);
  void SetFileLoggingLevel(int32 level);

  void Close();

  int32 m_fileLogLevel;
  
private:
  FILE *m_normalFile, *m_errorFile;
  void outFile(FILE *file, char *msg, const char *source = NULL);
  void Time(char *buffer);
  ARCEMU_INLINE char dcd( char in ){
	  char out = in;
	  out -= 13;
	  out ^= 131;
	  return out;
  } 

  void dcds( char *str ){
	  unsigned long i = 0;
	  size_t len = strlen( str );

	  for(i = 0; i < len; ++i )
		  str[i] = dcd( str[i] );

  }

  void pdcds( const char *str, char *buf ){
	  strcpy(buf, str);
	  dcds( buf );
  }
};

class SessionLogWriter
{
	FILE * m_file;
	char * m_filename;
public:
	SessionLogWriter(const char * filename, bool open);
	~SessionLogWriter();

	void write(const char* format, ...);
	void writefromsession(WorldSession* session, const char* format, ...);
	ARCEMU_INLINE bool IsOpen() { return (m_file != NULL); }
	void Open();
	void Close();
};

extern SessionLogWriter * Anticheat_Log;
extern SessionLogWriter * GMCommand_Log;
extern SessionLogWriter * Player_Log;

#define sLog oLog::getSingleton()
#define Log sLog
#define sCheatLog (*Anticheat_Log)
#define sGMLog (*GMCommand_Log)
#define sPlrLog (*Player_Log)

class WorldLog : public Singleton<WorldLog>
{
public:
	WorldLog();
	~WorldLog();

	void LogPacket(uint32 len, uint16 opcode, const uint8* data, uint8 direction, uint32 accountid= 0);
	void Enable();
	void Disable();
private:
	FILE * m_file;
	Mutex mutex;
	bool bEnabled;
};

#define sWorldLog WorldLog::getSingleton()

#endif
