#include "MeasureReader.h"
#include <SystemUtils.h>

#ifdef WIN32
#include "windows.h"
#include <iostream>
MEMORYSTATUSEX memInfo;
//#else
//#include "sys/types.h"
//#include "sys/sysinfo.h"
//struct sysinfo memInfo;
#endif

using namespace std;

MeasureReader &MeasureReader::getInstance()
{
    static MeasureReader instance;
    return instance;
}

std::pair<int, int64_t> MeasureReader::getMeasure()
{
#ifdef WIN32
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    double totalPhysMem = memInfo.ullTotalPhys / 1024 / 1024;
    double physMemUsed = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024 / 1024;
    int val = ceil(physMemUsed * 100 / totalPhysMem);
    return make_pair<int, long> (val, getPosixTime() + timeOffset);
#else
    //Commented lines get RAM usage in Linux
//    sysinfo (&memInfo);
//    double totalPhysMem  = memInfo.totalram / 1024 / 1024;
//    double physMemUsed  = memInfo.totalram - memInfo.freeram / 1024 / 1024;
//    return ceil(physMemUsed * 100 / totalPhysMem);
    return make_pair<int, long> (0, getPosixTime() + timeOffset);
#endif
}

void MeasureReader::setCurTime(long serverTime)
{
    timeOffset = serverTime - getPosixTime();
    cout << "Sync clock: " << timeOffset << "ms offset" << endl;
}

MeasureReader::MeasureReader() : timeOffset(0)
{

}