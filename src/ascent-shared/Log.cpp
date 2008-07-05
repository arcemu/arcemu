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

#include "Common.h"
#include "Config/ConfigEnv.h"
#include "Log.h"
#include "NGLog.h"
#include <stdarg.h>

string FormatOutputString(const char * Prefix, const char * Description, bool useTimeStamp)
{

	char p[MAX_PATH];
	p[0] = 0;
	time_t t = time(NULL);
	tm * a = gmtime(&t);
	strcat(p, Prefix);
	strcat(p, "/");
	strcat(p, Description);
	if(useTimeStamp)
	{
		char ftime[100];
		snprintf(ftime, 100, "-%-4d-%02d-%02d %02d-%02d-%02d", a->tm_year+1900, a->tm_mon+1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec);
		strcat(p, ftime);
	}

	strcat(p, ".log");
	return string(p);
}

createFileSingleton( oLog );
createFileSingleton(CLog);
initialiseSingleton( WorldLog );

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;
#ifndef WIN32
static const char* colorstrings[TBLUE+1] = {
"",
"\033[22;31m",
"\033[22;32m",
"\033[01;33m",
//"\033[22;37m",
"\033[0m",
"\033[01;37m",
"\033[22;34m",
};
#endif

void oLog::outString( const char * str, ... )
{
	if(m_fileLogLevel < 0 && m_screenLogLevel < 0)
		return;

	va_list ap;
	va_start(ap, str);
	
	if(m_screenLogLevel >= 0)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
}

void oLog::outError( const char * err, ... )
{
	if(m_fileLogLevel < 1 && m_screenLogLevel < 1)
		return;

	va_list ap;
	va_start(ap, err);

	if(m_screenLogLevel >= 1)
	{
#ifdef WIN32
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
		puts(colorstrings[TRED]);
#endif
		vfprintf(stderr, err, ap);
		putc('\n', stderr);
#ifdef WIN32
		SetConsoleTextAttribute(stderr_handle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
#else
		puts(colorstrings[TNORMAL]);
#endif
	}

	va_end(ap);
}

void oLog::outBasic( const char * str, ... )
{
	if(m_fileLogLevel < 1 && m_screenLogLevel < 1)
		return;

	va_list ap;
	va_start(ap, str);

	if(m_screenLogLevel >= 1)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
}

void oLog::outDetail( const char * str, ... )
{
	if(m_fileLogLevel < 2 && m_screenLogLevel < 2)
		return;

	va_list ap;
	va_start(ap, str);

	if(m_screenLogLevel >= 2)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
}

void oLog::outDebug( const char * str, ... )
{
	if(m_fileLogLevel < 3 && m_screenLogLevel < 3)
		return;

	va_list ap;
	va_start(ap, str);

	if(m_screenLogLevel >= 3)
	{
		vprintf(str, ap);
		putc('\n', stdout);
	}

	va_end(ap);
}

void oLog::outMenu( const char * str, ... )
{
	va_list ap;
	va_start(ap, str);
	vprintf( str, ap );
	va_end(ap);
	fflush(stdout);
}

void oLog::Init(int32 fileLogLevel, int32 screenLogLevel)
{
	m_screenLogLevel = screenLogLevel;
	m_fileLogLevel = fileLogLevel;

	// get error handle
#ifdef WIN32
	stderr_handle = GetStdHandle(STD_ERROR_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void oLog::SetScreenLoggingLevel(int32 level)
{
	m_screenLogLevel = level;
}

void oLog::SetFileLoggingLevel(int32 level)
{
	m_fileLogLevel = level;
}

void SessionLogWriter::write(const char* format, ...)
{
	if(!m_file)
		return;

	va_list ap;
	va_start(ap, format);
	char out[32768];

	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	sprintf(out, "[%-4d-%02d-%02d %02d:%02d:%02d] ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	size_t l = strlen(out);
	vsnprintf(&out[l], 32768 - l, format, ap);

	fprintf(m_file, "%s\n", out);
	va_end(ap);
}

WorldLog::WorldLog()
{
	bEnabled = false;
	m_file=NULL;

	if (Config.MainConfig.GetBoolDefault("LogLevel", "World", false))
	{
		Log.Notice("WorldLog", "Enabling packetlog output to \"world.log\"");
		Enable();
	} else {
		Disable();
	}
}

void WorldLog::Enable()
{
	if(bEnabled)
		return;

	bEnabled = true;
	if(m_file != NULL)
	{
		Disable();
		bEnabled=true;
	}
	m_file = fopen("world.log", "w");
}

void WorldLog::Disable()
{
	if(!bEnabled)
		return;

	bEnabled = false;
	if(!m_file)
		return;

	fflush(m_file);
	fclose(m_file);
	m_file=NULL;
}

WorldLog::~WorldLog()
{

}

void oLog::outColor(uint32 colorcode, const char * str, ...)
{
	if( !str ) return;
	va_list ap;
	va_start(ap, str);
#ifdef WIN32
	SetConsoleTextAttribute(stdout_handle, colorcode);
#else
	printf(colorstrings[colorcode]);
#endif
	vprintf( str, ap );
	fflush(stdout);
	va_end(ap);
}

void SessionLogWriter::Open()
{
	m_file = fopen(m_filename, "a");
}

void SessionLogWriter::Close()
{
	if(!m_file) return;
	fflush(m_file);
	fclose(m_file);
	m_file=NULL;
}

SessionLogWriter::SessionLogWriter(const char * filename, bool open)
{
	m_filename = strdup(filename);
	m_file=NULL;
	if(open)
		Open();
}

SessionLogWriter::~SessionLogWriter()
{
	if(m_file)
		Close();

	free(m_filename);
}
