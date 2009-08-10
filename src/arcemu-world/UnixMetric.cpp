#include <StdAfx.h>
#ifndef WIN32

float UnixMetric::GetRAMUsageUnix(){
    std::ifstream memf;
    std::string line;
    std::string value;
    std::stringstream fname;
    char line2[100];
    float memusage;
    unsigned long pos = 0;
    unsigned long pid = getpid();

    fname << "/proc/" << pid << "/status";

    memf.open(fname.str().c_str());
    do{
        pos = std::string::npos;
        memf.getline(line2,100);
        line.assign(line2);
        pos = line.find("VmData");
    }while(pos == std::string::npos);
    pos = line.find(" ");
    ++pos;

    value = line.substr(pos,std::string::npos);

    memusage = static_cast<float>( atof(value.c_str()) );

    memf.close();

    memusage /= 1024.0f;
    return memusage;
}


void UnixMetric::m_GetCPUCount(){
  char line[500];
  ifstream is;
  unsigned long nocpu = 0;
  
  is.open("/proc/cpuinfo");
  do{
    is.getline(line,500);
    if(strstr(line,"processor"))
      ++nocpu;        
  }while( !is.eof() );
  
  nocpus = nocpu;    
}

float UnixMetric::GetCPUUsageUnix(){
  uint32 lastupdate = GetLastUpdate();
  uint32 lastsecusage = GetLastSecUsage();
  float usage = 0.0f;
  uint32 secusage = 0;
  uint32 usecusage = 0;
  struct rusage susage;
  uint32 nocpus = GetCPUCount();
  uint32 uptime = GetUptime();
  uint32 unixtime = time( NULL );

  if(getrusage(RUSAGE_SELF, &susage) == 0){
    secusage = susage.ru_utime.tv_sec + susage.ru_stime.tv_sec;
    usecusage = susage.ru_utime.tv_usec + susage.ru_stime.tv_usec;

    if(usecusage / 1000000 >= 1)
      secusage = secusage + (usecusage / 1000000);
  }

  if(lastupdate == 0){
     if(uptime != 0)
        usage = ( static_cast<float>( secusage ) / uptime );
     else
        usage = 0.0f;
  }else{
     uint32 difft = (unixtime - lastupdate);
     if(difft != 0)
          usage = ( static_cast<float>( secusage - lastsecusage) / difft);
     else
          usage = 0.0f;
   
  }
  
  usage *= 100.0f;

  SetLastUpdate( unixtime );
  SetLastSecUsage( secusage );

  return usage / nocpus;
}

#endif
