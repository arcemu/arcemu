#include <StdAfx.h>
#ifndef WIN32

/*
 * Returns the RAM utilization in MB percent.
 */
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
        pos = line.find("VmRSS");
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

/*
 * Returns the CPU utilization in percent, normalized by the number of CPUs/cores.
 */

float UnixMetric::GetCPUUsageUnix(){
  uint32 lastupdate = GetLastUpdate();
  uint32 lastsecusage = GetLastSecUsage();
  uint32 lastusecusage = GetLastUSecUsage();
  float usage = 0.0f;
  uint32 secusage = 0;
  uint32 usecusage = 0;
  struct rusage susage;
  uint32 nocpus = GetCPUCount();
  uint32 unixtime = time( NULL );

  if(getrusage(RUSAGE_SELF, &susage) == 0){
    secusage = susage.ru_utime.tv_sec + susage.ru_stime.tv_sec;
    usecusage = susage.ru_utime.tv_usec + susage.ru_stime.tv_usec;

    if(usecusage / 1000000 >= 1){
      secusage = secusage + (usecusage / 1000000);
      usecusage = usecusage - ( usecusage / 1000000 ) * 1000000;
    }
  }

     uint32 difft = (unixtime - lastupdate);

     if(difft != 0){
          // when we sample the cpu usage below 10 minutes we can be more precise, since the usecs will be representable even in 32bit values!
          if( difft <= 3600 ){
            usage = ( static_cast<float>( (secusage * 1000000 + usecusage) - (lastsecusage * 1000000 + lastusecusage) ) / ( difft * 1000000 ) ); 

          }else
            usage = ( static_cast<float>( secusage - lastsecusage) / difft);
     }else{
          // this case means we are polling the cpu usage data in realtime, which shouldn't ever happen, except on startup
          // since we can't divide by 0
          // We don't care about the cpu usage of the starting time anyway so we can just report 0%
          usage = 0.0f;
     }

  usage *= 100.0f;

  /* I will leave this here for now, so next time if I tinker I don't have to rewrite it heh
  printf("secusage: %lu\n", secusage);
  printf("lastsecusage: %lu\n", lastsecusage);
  printf("usecusage: %lu\n", usecusage);
  printf("lastusecusage: %lu\n", lastusecusage);
  printf("difft: %lu\n", difft);
  printf("usec difference: %lu\n", ( (secusage * 1000000 + usecusage) - (lastsecusage * 1000000 + lastusecusage) ) );
  printf("usage%%: %lf\n", usage);
  */

  SetLastUpdate( unixtime );
  SetLastSecUsage( secusage );
  SetLastUSecUsage( usecusage );

  return usage / nocpus;
}

#endif
