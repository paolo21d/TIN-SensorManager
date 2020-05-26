#include <SystemUtils.h>
#include "MeasureReader.h"

int getMeasure()
{
#ifdef WIN32
    return 0;
#else
    int64_t timestamp = getPosixTime();
    return timestamp % 1000 * (timestamp % 100000 / 1000);
#endif
}