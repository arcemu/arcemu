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

#include "Config/ConfigEnv.h"
#include "Log.h"
#include <cstdarg>

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
initialiseSingleton( WorldLog );

SERVER_DECL time_t UNIXTIME;
SERVER_DECL tm g_localTime;

void oLog::outFile(FILE *file, char *msg, const char *source)
{
	char time_buffer[TIME_FORMAT_LENGTH];
	char szltr_buffer[SZLTR_LENGTH];
	Time(time_buffer);
	pdcds(SZLTR, szltr_buffer);

	if(source != NULL){
		fprintf(file, "%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
		printf( "%s%s%s: %s\n", time_buffer, szltr_buffer, source, msg);
	}else{
		fprintf(file, "%s%s%s\n", time_buffer, szltr_buffer, msg);
		printf( "%s%s%s\n", time_buffer, szltr_buffer, msg);
	}
}

void oLog::Time(char *buffer)
{
	time_t now;
	struct tm * timeinfo = NULL;
	
	time( &now );
	timeinfo = localtime( &now );

	if( timeinfo != NULL )
	{
		strftime(buffer,TIME_FORMAT_LENGTH,TIME_FORMAT,timeinfo);
	}
	else
	{
		buffer[0] = '\0';
	}
}

void oLog::outString( const char * str, ... )
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outError( const char * err, ... )
{
	if(m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, err);
	vsnprintf(buf, 32768, err, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::outBasic( const char * str, ... )
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outDetail( const char * str, ... )
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outDebug( const char * str, ... )
{
	if(m_fileLogLevel < 2 || m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

//old NGLog.h methods
void oLog::Notice(const char * source, const char * format, ...)
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_normalFile, buf, source);
}

void oLog::Warning(const char * source, const char * format, ...)
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_normalFile, buf, source);
}

void oLog::Success(const char * source, const char * format, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_normalFile, buf, source);
}

void oLog::Error(const char * source, const char * format, ...)
{
	if(m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_errorFile, buf, source);
}

void oLog::Debug(const char * source, const char * format, ...)
{
	if(m_fileLogLevel < 2 || m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_errorFile, buf, source);
}

void oLog::LargeErrorMessage(const char * source, ...)
{
	std::vector<char*> lines;
	char * pointer;
	va_list ap;
	va_start(ap, source);

	pointer = const_cast<char*>(source);
	lines.push_back(pointer);

	size_t i,j,k;
	pointer = va_arg(ap, char*);
	while( pointer != NULL )
	{
		lines.push_back( pointer );
		pointer = va_arg(ap, char*);
	}
	
	outError("*********************************************************************");
	outError("*                        MAJOR ERROR/WARNING                        *");
	outError("*                        ===================                        *");

	for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
	{
		stringstream sstext;
		i = strlen(*itr);
		j = (i<=65) ? 65 - i : 0;
		sstext << "* " << *itr;
		for( k = 0; k < j; ++k )
		{
			sstext << " ";
		}

		sstext << " *";
		outError(sstext.str().c_str());
	}

	outError("*********************************************************************");
}

void oLog::Init(int32 fileLogLevel, LogType logType)
{
	SetFileLoggingLevel(fileLogLevel);

	const char *logNormalFilename = NULL, *logErrorFilename = NULL;
	switch(logType)
	{
		case LOGON_LOG:
			{
				logNormalFilename = "logon-normal.log";
				logErrorFilename = "logon-error.log";
				break;
			}
		case WORLD_LOG:
			{
				logNormalFilename = "world-normal.log";
				logErrorFilename = "world-error.log";
				break;
			}
	}

	m_normalFile = fopen(logNormalFilename, "a");
	if (m_normalFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logNormalFilename, strerror(errno));
	else
	{
		tm* aTm = localtime(&UNIXTIME);
		outBasic("[%-4d-%02d-%02d %02d:%02d:%02d] ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	}

	m_errorFile = fopen(logErrorFilename, "a");
	if (m_errorFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logErrorFilename, strerror(errno));
	else
	{
		tm* aTm = localtime(&UNIXTIME);
		outError("[%-4d-%02d-%02d %02d:%02d:%02d] ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	}
}

void oLog::Close()
{
	if(m_normalFile != NULL)
	{
		fflush(m_normalFile);
		fclose(m_normalFile);
		m_normalFile = NULL;
	}

	if(m_errorFile != NULL)
	{
		fflush(m_errorFile);
		fclose(m_errorFile);
		m_errorFile = NULL;
	}
}

void oLog::SetFileLoggingLevel(int32 level)
{
	//log level -1 is no more allowed
	if(level >= 0)
		m_fileLogLevel = level;
}

void SessionLogWriter::write(const char* format, ...)
{
	if(!m_file)
		return;

	char out[32768];
	va_list ap;

	va_start(ap, format);
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
	m_file= NULL;

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
	m_file = fopen("world.log", "a");
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
	m_file= NULL;
}

WorldLog::~WorldLog()
{
if (m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}
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
	m_file= NULL;
}

SessionLogWriter::SessionLogWriter(const char * filename, bool open)
{
	m_filename = strdup(filename);
	m_file= NULL;
	if(open)
		Open();
}

SessionLogWriter::~SessionLogWriter()
{
	if(m_file)
		Close();

	free(m_filename);
}