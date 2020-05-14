#include "SystemUtils.h"

void sleepSecs(int secs)
{
#ifdef WIN32
    Sleep(secs / 1000);
#else
    sleep(secs);
#endif
}

void sleepMillis(int millis)
{
#ifdef WIN32
    Sleep(millis);
#else
    usleep(millis * 1000);
#endif
}

int64_t getPosixTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}