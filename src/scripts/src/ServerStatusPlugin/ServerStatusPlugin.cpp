/*
 * ArcScript Scripts for Arcemu MMORPG Server
 * Copyright (C) 2008 Arcemu Team
 * Copyright (C) 2007 Moon++ <http://www.moonplusplus.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "../../../arcemu-shared/svn_revision.h"

#define SKIP_ALLOCATOR_SHARING 1
#include <ScriptSetup.h>

extern "C" SCRIPT_DECL uint32 _exp_get_script_type()
{
	return SCRIPT_TYPE_INFODUMPER;
}

#ifdef WIN32
	#pragma pack(push,8)
	#include "PerfCounters.h"
	#pragma pack(pop)
	#include <Psapi.h>

	#define SYSTEM_OBJECT_INDEX					2		// 'System' object
	#define PROCESS_OBJECT_INDEX				230		// 'Process' object
	#define PROCESSOR_OBJECT_INDEX				238		// 'Processor' object
	#define TOTAL_PROCESSOR_TIME_COUNTER_INDEX	240		// '% Total processor time' counter (valid in WinNT under 'System' object)
	#define PROCESSOR_TIME_COUNTER_INDEX		6		// '% processor time' counter (for Win2K/XP)

	#pragma comment(lib, "advapi32")
	#pragma comment(lib, "Psapi")
#endif

#define LOAD_THREAD_SLEEP					180

#ifdef WIN32
	bool m_bFirstTime = true;
	LONGLONG m_lnOldValue = 0;
	LARGE_INTEGER m_OldPerfTime100nSec;
	uint32 number_of_cpus;
#endif

// This is needed because windows is a piece of shit
#ifdef WIN32
	BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
	{
		return TRUE;
	}
#endif

char Filename[MAX_PATH];

// Actual StatDumper Class
class StatDumper
{
public:
    void DumpStats();
};

// Instance of StatDumper
StatDumper dumper;

// Thread Wrapper for StatDumper
struct StatDumperThread : public ThreadBase
{
#ifdef WIN32
	HANDLE hEvent;
#else
	pthread_cond_t cond;
	pthread_mutex_t mutex;
#endif
	bool running;
public:
	StatDumperThread();
	~StatDumperThread();
	void OnShutdown();
	bool run();
};

void StatDumperThread::OnShutdown()
{
	running = false;
#ifdef WIN32
	SetEvent( hEvent );
#else
	pthread_cond_signal( &cond );
#endif
}

StatDumperThread::StatDumperThread()
{
}

StatDumperThread::~StatDumperThread()
{
#ifdef WIN32
	CloseHandle(hEvent);
#else
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
#endif
}

bool StatDumperThread::run()
{
	int delay_ms = Config.MainConfig.GetIntDefault( "StatDumper", "Interval", 120000 );

	if( delay_ms < 1000 )
		delay_ms = 1000;

	int delay_s = delay_ms / 1000;

	if( !Config.MainConfig.GetString( "StatDumper", Filename, "Filename", "stats.xml", MAX_PATH ) )
		strcpy( Filename, "stats.xml" );

#ifdef WIN32
	memset( &m_OldPerfTime100nSec, 0, sizeof( m_OldPerfTime100nSec ) );
	SYSTEM_INFO si;
	GetSystemInfo( &si );
	number_of_cpus = si.dwNumberOfProcessors;
#endif

#ifdef WIN32
	hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
#else
	struct timeval now;
	struct timespec tv;
	pthread_mutex_init( &mutex, NULL );
	pthread_cond_init( &cond, NULL );
#endif

	running = true;
	for(;;)
	{

		dumper.DumpStats();

#ifdef WIN32
		WaitForSingleObject( hEvent, delay_ms );
#else
		gettimeofday( &now, NULL );
		tv.tv_sec = now.tv_sec + delay_s;
		tv.tv_nsec = now.tv_usec * 1000;
		pthread_mutex_lock( &mutex );
		pthread_cond_timedwait( &cond, &mutex, &tv );
		pthread_mutex_unlock( &mutex );
#endif
		if( !running )
			break;
	}

	return true;
}

extern "C" SCRIPT_DECL void _exp_script_register(ScriptMgr* mgr)
{
	ThreadPool.ExecuteTask( new StatDumperThread() );
}

#ifdef WIN32
	/*** GRR ***/
	int __cdecl HandleCrash(PEXCEPTION_POINTERS pExceptPtrs)
	{
		return 0;
	}
#endif

void SetThreadName(const char* format, ...)
{
}

void GenerateUptimeString(char * Dest)
{
    uint32 seconds = sWorld.GetUptime();
    uint32 mins = 0;
    uint32 hours = 0;
    uint32 days = 0;
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
                            hours -= days*24;
                    }
                }
            }
        }
    }
    sprintf(Dest, "%d days, %d hours, %d minutes, %d seconds", (int)days, (int)hours, (int)mins, (int)seconds);
}

#ifdef WIN32

float GetCPUUsageWin32()
{
    CPerfCounters<LONGLONG> PerfCounters;
    DWORD dwObjectIndex = PROCESS_OBJECT_INDEX;
    DWORD dwCpuUsageIndex = PROCESSOR_TIME_COUNTER_INDEX;

    int				CpuUsage = 0;
    LONGLONG		lnNewValue = 0;
    PPERF_DATA_BLOCK pPerfData = NULL;
    LARGE_INTEGER	NewPerfTime100nSec = {0};

    lnNewValue = PerfCounters.GetCounterValueForProcessID(&pPerfData, dwObjectIndex, dwCpuUsageIndex, GetCurrentProcessId());
    NewPerfTime100nSec = pPerfData->PerfTime100nSec;

    if (m_bFirstTime)
    {
        m_bFirstTime = false;
        m_lnOldValue = lnNewValue;
        m_OldPerfTime100nSec = NewPerfTime100nSec;
        return 0;
    }

    LONGLONG lnValueDelta = lnNewValue - m_lnOldValue;
    double DeltaPerfTime100nSec = (double)NewPerfTime100nSec.QuadPart - (double)m_OldPerfTime100nSec.QuadPart;

    m_lnOldValue = lnNewValue;
    m_OldPerfTime100nSec = NewPerfTime100nSec;

    double a = (double)lnValueDelta / DeltaPerfTime100nSec;
    a /= double(number_of_cpus);
    return float(a * 100.0);
}

float GetRAMUsageWin32()
{
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    float ram = (float)pmc.PagefileUsage;
    ram /= 1024.0f;
    ram /= 1024.0f;
    return ram;
}

#endif

float GetCPUUsage()
{
#ifdef WIN32
    return GetCPUUsageWin32();
#else
    return 0.0f;
#endif
}

float GetRAMUsage()
{
#ifdef WIN32
    return GetRAMUsageWin32();
#else
     return 0.0f;
#endif
}

void FillOnlineTime(uint32 Time, char * Dest)
{
    uint32 seconds = Time;
    uint32 mins=0;
    uint32 hours=0;
    uint32 days=0;
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
                            hours -= days*24;
                    }
                }
            }
        }
    }
    sprintf(Dest, "%d hours, %d minutes, %d seconds", (int)hours, (int)mins, (int)seconds);
}

void StatDumper::DumpStats()
{
    if( Filename[0] == NULL )
        return;
    FILE* f = fopen( Filename, "w" );
    if( !f )
        return;

	Log.Debug( "StatDumper", "Writing %s", Filename );

    // Dump Header
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f, "<?xml-stylesheet type=\"text/xsl\" href=\"server_stats.xsl\"?>\n");
    fprintf(f, "<serverpage>\n");
    fprintf(f, "  <status>\n");

	uint32 races[RACE_DRAENEI+1];
	uint32 classes[DRUID+1];
	memset(&races[0], 0, sizeof(uint32)*(RACE_DRAENEI+1));
	memset(&classes[0], 0, sizeof(uint32)*(RACE_DRAENEI+1));
    std::deque<Player*> gms;
    {
        // Dump server information.
#ifdef WIN32
		fprintf(f, "    <platform>ArcEmu %s r%u/%s-Win-%s (www.ArcEmu.org)</platform>\n", BUILD_TAG, BUILD_REVISION, CONFIG, ARCH);		
#else
		fprintf(f, "    <platform>ArcEmu %s r%u/%s-%s (www.ArcEmu.org)</platform>\n", BUILD_TAG, BUILD_REVISION, PLATFORM_TEXT, ARCH);
#endif

        char uptime[80];
        GenerateUptimeString(uptime);
        float AvgLat;
        uint32 GMCount;
        int gm = 0;
        int count = 0;
        int avg = 0;
        // lock players reader
        objmgr._playerslock.AcquireReadLock();

        HM_NAMESPACE::hash_map<uint32, Player*>::const_iterator itr;
        for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
        {
            if(itr->second->GetSession() && itr->second->IsInWorld())
            {
                count++;
                avg += itr->second->GetSession()->GetLatency();
                if(itr->second->GetSession()->GetPermissionCount())
                {
                    gm++;
                    gms.push_back(itr->second);
                }
				classes[itr->second->getClass()]++;
				races[itr->second->getRace()]++;
            }            
        }
        objmgr._playerslock.ReleaseReadLock();

        AvgLat = count ? (float)((float)avg / (float)count) : 0;
        GMCount = gm;

        fprintf(f, "    <uptime>%s</uptime>\n", uptime);
        fprintf(f, "    <oplayers>%u</oplayers>\n", (unsigned int)(sWorld.AlliancePlayers + sWorld.HordePlayers));
        fprintf(f, "    <cpu>%2.2f</cpu>\n", GetCPUUsage());
        fprintf(f, "    <qplayers>%u</qplayers>\n", (unsigned int)sWorld.GetQueueCount());
        fprintf(f, "    <ram>%.3f</ram>\n", GetRAMUsage());
        fprintf(f, "    <avglat>%.3f</avglat>\n", AvgLat);
		fprintf(f, "    <threads>%u</threads>\n", (unsigned int)ThreadPool.GetActiveThreadCount());
		fprintf(f, "    <fthreads>%u</fthreads>\n", (unsigned int)ThreadPool.GetFreeThreadCount());
        time_t t = (time_t)UNIXTIME;
        fprintf(f, "    <gmcount>%u</gmcount>\n", (unsigned int)GMCount);
        fprintf(f, "    <lastupdate>%s</lastupdate>\n", asctime(localtime(&t)));
        fprintf(f, "    <alliance>%u</alliance>\n", (unsigned int)sWorld.AlliancePlayers);
        fprintf(f, "    <horde>%u</horde>\n", (unsigned int)sWorld.HordePlayers);
        fprintf(f, "    <acceptedconns>%u</acceptedconns>\n", (unsigned int)sWorld.mAcceptedConnections);
        fprintf(f, "    <peakcount>%u</peakcount>\n", (unsigned int)sWorld.PeakSessionCount);
		fprintf(f, "    <wdbquerysize>%u</wdbquerysize>\n", WorldDatabase.GetQueueSize());
		fprintf(f, "    <cdbquerysize>%u</cdbquerysize>\n", CharacterDatabase.GetQueueSize());
    }
    fprintf(f, "  </status>\n");
	static const char * race_names[RACE_DRAENEI+1] = {
		NULL,
		"human",
		"orc",
		"dwarf",
		"nightelf",
		"undead",
		"tauren",
		"gnome",
		"troll",
		NULL,
		"bloodelf",
		"draenei",
	};

	static const char * class_names[DRUID+1] = {
		NULL,
		"warrior",
		"paladin",
		"hunter",
		"rogue",
		"priest",
		NULL,
		"shaman",
		"mage",
		"warlock",
		NULL,
		"druid",
	};
	
	fprintf(f, "  <statsummary>\n");
	uint32 i;
	for(i = 0; i <= RACE_DRAENEI; ++i)
	{
		if( race_names[i] != NULL )
			fprintf(f, "    <%s>%u</%s>\n", race_names[i], races[i], race_names[i]);
	}

	for(i = 0; i <= DRUID; ++i)
	{
		if( class_names[i] != NULL )
			fprintf(f, "    <%s>%u</%s>\n", class_names[i], classes[i], class_names[i]);
	}
	fprintf(f, "  </statsummary>\n");

    Player * plr;
    uint32 t = (uint32)time(NULL);
    char otime[100];
    {
        fprintf(f, "  <instances>\n");
        
        // need a big buffer..
        static char buf[500000];
        memset(buf, 0, 500000);

        // Dump Instance Information
        //sWorldCreator.BuildXMLStats(buf);
		sInstanceMgr.BuildXMLStats(buf);
        fprintf(f, buf);
        fprintf(f, "  </instances>\n");
    }
    {
        // GM Information
        fprintf(f, "  <gms>\n");
        while(!gms.empty())
        {
            plr = gms.front();
            gms.pop_front();
			if(!plr->bGMTagOn)
				continue;
            FillOnlineTime(t - plr->OnlineTime, otime);
            fprintf(f, "    <gmplr>\n");
            fprintf(f, "      <name>%s</name>\n", plr->GetName());
            fprintf(f, "      <race>%u</race>\n", plr->getRace());
            fprintf(f, "      <class>%u</class>\n", (unsigned int)plr->getClass());
            fprintf(f, "      <gender>%u</gender>\n", (unsigned int)plr->getGender());
            fprintf(f, "      <pvprank>%u</pvprank>\n", (unsigned int)plr->GetPVPRank());
            fprintf(f, "      <level>%u</level>\n", (unsigned int)plr->GetUInt32Value(UNIT_FIELD_LEVEL));
            fprintf(f, "      <map>%u</map>\n", (unsigned int)plr->GetMapId());
            fprintf(f, "      <areaid>%u</areaid>\n", (unsigned int)plr->GetAreaID());
            fprintf(f, "      <ontime>%s</ontime>\n", otime);
            fprintf(f, "      <latency>%u</latency>\n", (unsigned int)plr->GetSession()->GetLatency());
            fprintf(f, "      <permissions>%s</permissions>\n", plr->GetSession()->GetPermissions());
            fprintf(f, "    </gmplr>\n");
        }

        fprintf(f, "  </gms>\n");
    }

    {
    fprintf(f, "  <sessions>\n");
        // Dump Player Information
        objmgr._playerslock.AcquireReadLock();
        HM_NAMESPACE::hash_map<uint32, Player*>::const_iterator itr;

        for (itr = objmgr._players.begin(); itr != objmgr._players.end(); itr++)
        {
            plr = itr->second;
            if(itr->second->GetSession() && itr->second->IsInWorld())
            {
                FillOnlineTime(t - plr->OnlineTime, otime);

                fprintf(f, "    <plr>\n");
                fprintf(f, "      <name>%s</name>\n", plr->GetName());
                fprintf(f, "      <race>%u</race>\n", (unsigned int)plr->getRace());
                fprintf(f, "      <class>%u</class>\n", (unsigned int)plr->getClass());
				fprintf(f, "      <gender>%u</gender>\n", (unsigned int)plr->getGender());
				fprintf(f, "      <pvprank>%u</pvprank>\n", (unsigned int)plr->GetPVPRank());
                fprintf(f, "      <level>%u</level>\n", (unsigned int)plr->GetUInt32Value(UNIT_FIELD_LEVEL));
                fprintf(f, "      <map>%u</map>\n", (unsigned int)plr->GetMapId());
                fprintf(f, "      <areaid>%u</areaid>\n", (unsigned int)plr->GetAreaID());
				//requested by Zdarkside for he's online map. I hope it does not scre up any parser. If so, then make a better one :P
                fprintf(f, "      <xpos>%f</xpos>\n", plr->GetPositionX ());
                fprintf(f, "      <ypos>%f</ypos>\n", plr->GetPositionY());
                fprintf(f, "      <ontime>%s</ontime>\n", otime);
                fprintf(f, "      <latency>%u</latency>\n", (unsigned int)plr->GetSession()->GetLatency());
                fprintf(f, "    </plr>\n");
                if(plr->GetSession()->GetPermissionCount() > 0)
                    gms.push_back(plr);
            }
        }
        objmgr._playerslock.ReleaseReadLock();
        fprintf(f, "  </sessions>\n");

        
    }

    fprintf(f, "</serverpage>\n");
    fclose(f);
}
