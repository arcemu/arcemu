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
