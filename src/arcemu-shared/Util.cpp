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

#include "Common.h"

using namespace std;

vector<string> StrSplit(const string &src, const string &sep)
{
	vector<string> r;
	string s;
	for (string::const_iterator i = src.begin(); i != src.end(); i++) {
		if (sep.find(*i) != string::npos) {
			if (s.length()) r.push_back(s);
			s = "";
		} else {
			s += *i;
		}
	}
	if (s.length()) r.push_back(s);
	return r;
}

void SetThreadName(const char* format, ...)
{
	// This isn't supported on nix?
	va_list ap;
	va_start(ap, format);

#ifdef WIN32

	char thread_name[200];
	vsnprintf(thread_name, 200, format, ap);

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;
	info.szName = thread_name;

	__try
	{
#ifdef _WIN64
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info);
#else
		RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
#endif
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{

	}

#endif

	va_end(ap);
}

time_t convTimePeriod ( uint32 dLength, char dType )
{
	time_t rawtime = 0;
	if (dLength == 0)
		return rawtime;
	struct tm * ti = localtime( &rawtime );
	switch(dType)
	{
		case 'h':		// hours
			ti->tm_hour += dLength;
			break;
		case 'd':		// days
			ti->tm_mday += dLength;
			break;
		case 'w':		// weeks
			ti->tm_mday += 7 * dLength;
			break;
		case 'm':		// months
			ti->tm_mon += dLength;
			break;
		case 'y':		// years
			// are leap years considered ? do we care ?
			ti->tm_year += dLength;
			break;
		default:		// minutes
			ti->tm_min += dLength;
			break;
	}
	return mktime(ti);
}
int32 GetTimePeriodFromString(const char * str)
{
	uint32 time_to_ban = 0;
	char * p = (char*)str;
	uint32 multiplier;
	string number_temp;
	uint32 multipliee;
	number_temp.reserve(10);

	while(*p != 0)
	{
		// always starts with a number.
		if(!isdigit(*p))
			break;

		number_temp.clear();
		while(isdigit(*p) && *p != 0)
		{
			number_temp += *p;
			++p;
		}

		// try and find a letter
		if(*p == 0)
			break;

		// check the type
		switch(tolower(*p))
		{
		case 'y':
			multiplier = TIME_YEAR;		// eek!
			break;

		case 'm':
			multiplier = TIME_MONTH;
			break;

		case 'd':
			multiplier = TIME_DAY;
			break;

		case 'h':
			multiplier = TIME_HOUR;
			break;

		default:
			return -1;
			break;
		}

		++p;
		multipliee = atoi(number_temp.c_str());
		time_to_ban += (multiplier * multipliee);
	}

	return time_to_ban;
}

const char * szDayNames[7] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char * szMonthNames[12] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void MakeIntString(char * buf, int num)
{
	if(num<10)
	{
		buf[0] = '0';
		//itoa(num, &buf[1], 10);
		sprintf(&buf[1], "%u", num);
	}
	else
	{
		//itoa(num,buf,10);
		sprintf(buf,"%u",num);
	}
}

void MakeIntStringNoZero(char * buf, int num)
{
	//itoa(num,buf,10);
	sprintf(buf,"%u",num);
}

string ConvertTimeStampToString(uint32 timestamp)
{
	int seconds = (int)timestamp;
	int mins=0;
	int hours=0;
	int days=0;
	int months=0;
	int years=0;
	if(seconds >= 60)
	{
		mins = seconds / 60;
		if(mins)
		{
			seconds -= mins*60;
			if(mins >= 60)
			{
				hours = mins / 60;
				if(hours)
				{
					mins -= hours*60;
					if(hours >= 24)
					{
						days = hours/24;
						if(days)
						{
							hours -= days*24;
							if(days >= 30)
							{
								months = days / 30;
								if(months)
								{
									days -= months*30;
									if(months >= 12)
									{
										years = months / 12;
										if(years)
										{
											months -= years*12;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	char szTempBuf[100];
	string szResult;

	if(years) {
		MakeIntStringNoZero(szTempBuf, years);
		szResult += szTempBuf;
		szResult += " years, ";
	}

	if(months) {
		MakeIntStringNoZero(szTempBuf, months);
		szResult += szTempBuf;
		szResult += " months, ";
	}

	if(days) {
		MakeIntStringNoZero(szTempBuf, days);
		szResult += szTempBuf;
		szResult += " days, ";
	}

	if(hours) {
		MakeIntStringNoZero(szTempBuf, hours);
		szResult += szTempBuf;
		szResult += " hours, ";
	}

	if(mins) {
		MakeIntStringNoZero(szTempBuf, mins);
		szResult += szTempBuf;
		szResult += " minutes, ";
	}

	if(seconds) {
		MakeIntStringNoZero(szTempBuf, seconds);
		szResult += szTempBuf;
		szResult += " seconds";
	}

	return szResult;
}

string ConvertTimeStampToDataTime(uint32 timestamp)
{
	char szTempBuf[100];
	time_t t = (time_t)timestamp;
	tm * pTM = localtime(&t);

	string szResult;
	szResult += szDayNames[pTM->tm_wday];
	szResult += ", ";

	MakeIntString(szTempBuf, pTM->tm_mday);
	szResult += szTempBuf;
	szResult += " ";

	szResult += szMonthNames[pTM->tm_mon];
	szResult += " ";

	MakeIntString(szTempBuf, pTM->tm_year+1900);
	szResult += szTempBuf;
	szResult += ", ";
	MakeIntString(szTempBuf, pTM->tm_hour);
	szResult += szTempBuf;
	szResult += ":";
	MakeIntString(szTempBuf, pTM->tm_min);
	szResult += szTempBuf;
	szResult += ":";
	MakeIntString(szTempBuf, pTM->tm_sec);
	szResult += szTempBuf;

	return szResult;
}

#ifdef WIN32
static char _StringConversionStorage[2048];
#endif

// win32 only
// cebernic added it
// for multilanguage supports
// --------------------------------------------------------------------------------------------------
SERVER_DECL const char* _StringToUTF8(const char*   pASCIIBuf)
{ 
#ifdef WIN32
	DWORD     UniCodeLen=MultiByteToWideChar(CP_ACP,   0,   pASCIIBuf,   -1,   0,   0); 
  std::vector <wchar_t>   vWCH(UniCodeLen); 
  MultiByteToWideChar(CP_ACP,   0,   pASCIIBuf,   -1,   &vWCH[0],   UniCodeLen); 
  DWORD   dwUtf8Len=WideCharToMultiByte(CP_UTF8,   0,   &vWCH[0],   UniCodeLen   ,   NULL,   NULL,   NULL,   NULL   ); 
	ASSERT( dwUtf8Len+1 < 2048 );
	memset(_StringConversionStorage,0,(sizeof(char)*dwUtf8Len)+1);
  WideCharToMultiByte(CP_UTF8,   0,   &vWCH[0],   UniCodeLen   ,   _StringConversionStorage,   dwUtf8Len,   NULL,   NULL   ); 
	return &_StringConversionStorage[0];
#else
	return &pASCIIBuf[0];
#endif
} 
SERVER_DECL const char* _StringToANSI(const char*   pUtf8Buf)
{ 
#ifdef WIN32
	DWORD   UniCodeLen=MultiByteToWideChar(CP_UTF8,   0,   pUtf8Buf,   -1,   NULL,0   ); 
  std::vector <wchar_t>   vWCH(UniCodeLen); 
  MultiByteToWideChar(CP_UTF8,   0,   pUtf8Buf,   -1,   &vWCH[0]   ,   UniCodeLen   ); 
  DWORD   dwASCIILen=WideCharToMultiByte(CP_ACP,   0,   &vWCH[0],   UniCodeLen   ,   NULL   ,NULL   ,   NULL,   NULL   ); 
	ASSERT( dwASCIILen+1 < 2048 );
	memset(_StringConversionStorage,0,(sizeof(char)*dwASCIILen)+1);
  WideCharToMultiByte(CP_ACP,   0,   &vWCH[0],   UniCodeLen   ,   _StringConversionStorage,   dwASCIILen,   NULL,   NULL   ); 
	return &_StringConversionStorage[0];
#else
	return &pUtf8Buf[0];
#endif
} 

SERVER_DECL bool _IsStringUTF8(const char *str)
{
    int   i;
    unsigned char cOctets;  // octets to go in this UTF-8 encoded character
    unsigned char chr;
    bool  bAllAscii= TRUE;
    long iLen = (long)strlen(str);
 
    cOctets= 0;
    for( i=0; i <iLen; i++ ) {
 
     chr = (unsigned char)str[i];
 
     if( (chr & 0x80) != 0 ) bAllAscii= FALSE;
 
     if( cOctets == 0 ) {
        if( chr>= 0x80 )  {
            do  {
                chr <<= 1;
                cOctets++;
            }
            while( (chr & 0x80) != 0 );
            
            cOctets--;                        
            if( cOctets == 0 ) return FALSE;  
        }
     }
     else  {
        if( (chr & 0xC0) != 0x80 ) 
            return FALSE;
 
        cOctets--;                       
     }
    }
    if( cOctets> 0 ) 
     return FALSE;
    if( bAllAscii ) 
     return FALSE;
    return TRUE;

 } 

