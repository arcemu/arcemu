/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2009 <http://www.ArcEmu.org/>
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


#ifndef WIN32
#ifndef UNIXMETRICHPP__
#define UNIXMETRICHPP__

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/resource.h>

using namespace std;

class UnixMetric{
private:
  unsigned long nocpus;
  unsigned long start;
  unsigned long lastupdate;
  unsigned long lastsecusage;
  unsigned long lastusecusage;
  void m_GetCPUCount();
  

public:

  UnixMetric(){ 
    m_GetCPUCount();
    start = time( NULL ); 
    
    lastupdate = 0;
    lastsecusage = 0;
    lastusecusage = 0;
    }
  ~UnixMetric(){ }

unsigned long GetUptime(){
    unsigned long now;
    now = time( NULL );

    return now - start;
  }
  
unsigned long GetCPUCount(){ return nocpus; }
  float GetRAMUsageUnix();
  float GetCPUUsageUnix();
  unsigned long GetLastSecUsage(){ return lastsecusage; }
  unsigned long GetLastUSecUsage(){ return lastusecusage; }
  unsigned long GetLastUpdate(){ return lastupdate; }
  void SetLastSecUsage(unsigned long sec ){ lastsecusage = sec; }
  void SetLastUSecUsage(unsigned long usec ){ lastusecusage = usec; }
  void SetLastUpdate(unsigned long utime ){ lastupdate = utime; }

};

#endif
#endif
