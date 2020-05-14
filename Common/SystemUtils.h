#ifndef SERVER_SYSTEMUTILS_H
#define SERVER_SYSTEMUTILS_H

#ifdef WIN32
#include<windows.h>
#else
#include <unistd.h>
#endif

#include <chrono>

void sleepSecs(int secs);
void sleepMillis(int millis);
int64_t getPosixTime();

#endif //SERVER_SYSTEMUTILS_H
