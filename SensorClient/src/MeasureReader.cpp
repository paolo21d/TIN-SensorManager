#include "MeasureReader.h"
#include <math.h>

#ifdef WIN32
#include "windows.h"
#include <iostream>
MEMORYSTATUSEX memInfo;
#else
#include "sys/types.h"
#include "sys/sysinfo.h"
struct sysinfo memInfo;
#endif

int getMeasure()
{
#ifdef WIN32
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    double totalPhysMem = memInfo.ullTotalPhys / 1024 / 1024;
    double physMemUsed = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024 / 1024;
    return ceil(physMemUsed * 100 / totalPhysMem);
#else
    sysinfo (&memInfo);
    double totalPhysMem  = memInfo.totalram / 1024 / 1024;
    double physMemUsed  = memInfo.totalram - memInfo.freeram / 1024 / 1024;
    return ceil(physMemUsed * 100 / totalPhysMem);
#endif
}