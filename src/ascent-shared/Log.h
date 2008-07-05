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

#ifndef WOWSERVER_LOG_H
#define WOWSERVER_LOG_H

#include "Common.h"
#include "Singleton.h"

class WorldPacket;
class WorldSession;

#ifdef WIN32

#define TRED FOREGROUND_RED | FOREGROUND_INTENSITY
#define TGREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define TYELLOW FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define TNORMAL FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE
#define TWHITE TNORMAL | FOREGROUND_INTENSITY
#define TBLUE FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY

#else

#define TRED 1
#define TGREEN 2
#define TYELLOW 3
#define TNORMAL 4
#define TWHITE 5
#define TBLUE 6

#endif
std::string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp);

class SERVER_DECL oLog : public Singleton< oLog > {
public:
  void outString( const char * str, ... );
  void outError( const char * err, ... );
  void outBasic( const char * str, ... );
  void outDetail( const char * str, ... );
  void outDebug( const char * str, ... );
  void outMenu( const char * str, ... );

  void fLogText(const char *text);
  void SetLogging(bool enabled);
  
  void Init(int32 fileLogLevel, int32 screenLogLevel);
  void SetFileLoggingLevel(int32 level);
  void SetScreenLoggingLevel(int32 level);

  void outColor(uint32 colorcode, const char * str, ...);
  
#ifdef WIN32
  HANDLE stdout_handle, stderr_handle;
#endif
  int32 m_fileLogLevel;
  int32 m_screenLogLevel;
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
	ASCENT_INLINE bool IsOpen() { return (m_file != NULL); }
	void Open();
	void Close();
};

extern SessionLogWriter * Anticheat_Log;
extern SessionLogWriter * GMCommand_Log;
extern SessionLogWriter * Player_Log;

#define sLog oLog::getSingleton()
#define sCheatLog (*Anticheat_Log)
#define sGMLog (*GMCommand_Log)
#define sPlrLog (*Player_Log)

class WorldLog : public Singleton<WorldLog>
{
public:
	WorldLog();
	~WorldLog();

	void LogPacket(uint32 len, uint16 opcode, const uint8* data, uint8 direction);
	void Enable();
	void Disable();
private:
	FILE * m_file;
	Mutex mutex;
	bool bEnabled;
};

#define sWorldLog WorldLog::getSingleton()

#endif

