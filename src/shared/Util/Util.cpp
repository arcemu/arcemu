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

#include "Util.h"

using namespace std;

const char* szDayNames[7] =
{
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

const char* szMonthNames[12] =
{
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

void MakeIntString(char* buf, int num)
{
	if(num < 10)
	{
		buf[0] = '0';
		sprintf(&buf[1], "%u", num);
	}
	else
	{
		sprintf(buf, "%u", num);
	}
}

void MakeIntStringNoZero(char* buf, int num)
{
	sprintf(buf, "%u", num);
}

namespace Arcemu
{
	namespace Shared
	{
		void Util::swap32(uint32 &data)
		{
			 data = ((data >> 24 & 0xff)) | ((data >> 8) & 0xff00) | ((data << 8) & 0xff0000) | (data << 24);
		}

		void Util::SetThreadName(const char* format, ...)
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
				RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info);
		#else
				RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
		#endif
			}
			__except(EXCEPTION_CONTINUE_EXECUTION)
			{

			}

		#endif

			va_end(ap);
		}
		float Util::round(float f)
		{
			return std::floor(f + 0.5f);
		}

		double Util::round(double d)
		{
			return std::floor(d + 0.5);
		}

		long double Util::round(long double ld)
		{
			return std::floor(ld + 0.5);
		}

		void Util::Sleep(unsigned long timems)
		{
	#ifdef WIN32
			::Sleep(timems);
	#else
			timespec tv;

			tv.tv_sec = timems / 1000;
			tv.tv_nsec = (timems % 1000) * 1000 * 1000;

			nanosleep(&tv, NULL);
	#endif
		}

		vector<string> Util::StrSplit(const string & src, const string & sep)
		{
			vector<string> r;
			string s;
			for(string::const_iterator i = src.begin(); i != src.end(); ++i)
			{
				if(sep.find(*i) != string::npos)
				{
					if(s.length()) r.push_back(s);
					s = "";
				}
				else
				{
					s += *i;
				}
			}
			if(s.length()) r.push_back(s);
			return r;
		}

		time_t Util::convTimePeriod(uint32 dLength, char dType)
		{
			time_t rawtime = 0;
			if(dLength == 0)
				return rawtime;
			struct tm* ti = localtime(&rawtime);
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
		int32 Util::GetTimePeriodFromString(const char* str)
		{
			uint32 time_to_ban = 0;
			char* p = (char*)str;
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
				// try to find a letter
				if(*p != 0)
				{
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
				}
				else
					multiplier = TIME_MINUTE; // Defaults to MINUTES, if no letter is given
				multipliee = atoi(number_temp.c_str());
				time_to_ban += (multiplier * multipliee);
			}
			return time_to_ban;
		}

		string Util::ConvertTimeStampToString(uint32 timestamp)
		{
			int seconds = (int)timestamp;
			int mins = 0;
			int hours = 0;
			int days = 0;
			int months = 0;
			int years = 0;
			if(seconds >= 60)
			{
				mins = seconds / 60;
				if(mins)
				{
					seconds -= mins * 60;
					if(mins >= 60)
					{
						hours = mins / 60;
						if(hours)
						{
							mins -= hours * 60;
							if(hours >= 24)
							{
								days = hours / 24;
								if(days)
								{
									hours -= days * 24;
									if(days >= 30)
									{
										months = days / 30;
										if(months)
										{
											days -= months * 30;
											if(months >= 12)
											{
												years = months / 12;
												if(years)
												{
													months -= years * 12;
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

			if(years)
			{
				MakeIntStringNoZero(szTempBuf, years);
				szResult += szTempBuf;
				szResult += " years, ";
			}

			if(months)
			{
				MakeIntStringNoZero(szTempBuf, months);
				szResult += szTempBuf;
				szResult += " months, ";
			}

			if(days)
			{
				MakeIntStringNoZero(szTempBuf, days);
				szResult += szTempBuf;
				szResult += " days, ";
			}

			if(hours)
			{
				MakeIntStringNoZero(szTempBuf, hours);
				szResult += szTempBuf;
				szResult += " hours, ";
			}

			if(mins)
			{
				MakeIntStringNoZero(szTempBuf, mins);
				szResult += szTempBuf;
				szResult += " minutes, ";
			}

			if(seconds)
			{
				MakeIntStringNoZero(szTempBuf, seconds);
				szResult += szTempBuf;
				szResult += " seconds";
			}

			return szResult;
		}

		string Util::ConvertTimeStampToDateTime(uint32 timestamp)
		{
			char szTempBuf[100];
			time_t t = (time_t)timestamp;
			tm* pTM = localtime(&t);

			string szResult;
			szResult += szDayNames[pTM->tm_wday];
			szResult += ", ";

			MakeIntString(szTempBuf, pTM->tm_mday);
			szResult += szTempBuf;
			szResult += " ";

			szResult += szMonthNames[pTM->tm_mon];
			szResult += " ";

			MakeIntString(szTempBuf, pTM->tm_year + 1900);
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

		uint32 Util::DecimalToMask(uint32 dec)
		{
			return ((uint32)1 << (dec - 1));
		}

		void Util::arcemu_TOLOWER(std::string & str)
		{
			for(size_t i = 0; i < str.length(); ++i)
				str[i] = (char)tolower(str[i]);
		}

		void Util::arcemu_TOUPPER(std::string & str)
		{
			for(size_t i = 0; i < str.length(); ++i)
				str[i] = (char)toupper(str[i]);
		}

		void Util::CapitalizeString(std::string & arg)
		{
			if(arg.length() == 0) return;
			arg[0] = static_cast<char>(toupper(arg[0]));
			for(uint32 x = 1; x < arg.size(); ++x)
				arg[x] = static_cast<char>(tolower(arg[x]));
		}

		bool Util::ParseCIDRBan(unsigned int IP, unsigned int Mask, unsigned int MaskBits)
		{
			// CIDR bans are a compacted form of IP / Submask
			// So 192.168.1.0/255.255.255.0 would be 192.168.1.0/24
			// IP's in the 192.168l.1.x range would be hit, others not.
			unsigned char* source_ip = (unsigned char*)&IP;
			unsigned char* mask = (unsigned char*)&Mask;
			int full_bytes = MaskBits / 8;
			int leftover_bits = MaskBits % 8;
			//int byte;

			// sanity checks for the data first
			if(MaskBits > 32)
				return false;

			// this is the table for comparing leftover bits
			static const unsigned char leftover_bits_compare[9] =
			{
				0x00,			// 00000000
				0x80,			// 10000000
				0xC0,			// 11000000
				0xE0,			// 11100000
				0xF0,			// 11110000
				0xF8,			// 11111000
				0xFC,			// 11111100
				0xFE,			// 11111110
				0xFF,			// 11111111 - This one isn't used
			};

			// if we have any full bytes, compare them with memcpy
			if(full_bytes > 0)
			{
				if(memcmp(source_ip, mask, full_bytes) != 0)
					return false;
			}

			// compare the left over bits
			if(leftover_bits > 0)
			{
				if((source_ip[full_bytes] & leftover_bits_compare[leftover_bits]) !=
						(mask[full_bytes] & leftover_bits_compare[leftover_bits]))
				{
					// one of the bits does not match
					return false;
				}
			}

			// all of the bits match that were testable
			return true;
		}

		unsigned int Util::MakeIP(const char* str)
		{
			unsigned int bytes[4];
			unsigned int res;
			if(sscanf(str, "%u.%u.%u.%u", &bytes[0], &bytes[1], &bytes[2], &bytes[3]) != 4)
				return 0;

			res = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
			return res;
		}
	}
}



