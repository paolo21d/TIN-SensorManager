#ifndef SERVER_SYSTEMUTILS_H
#define SERVER_SYSTEMUTILS_H

#ifdef WIN32
#include<windows.h>
#else
#include <unistd.h>
#endif

void sleepSecs(int secs);
void sleepMillis(int millis);

#endif //SERVER_SYSTEMUTILS_H
