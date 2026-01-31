#include <time.h>
#include <sys/timeb.h>

#include "platform.h"
#include "timer.h"

SEGAN_LIB_API sx_ulong sx_time_now()
{
#if defined(_WIN32) || defined(_WIN64)
    struct _timeb timebuffer;
    _ftime64_s(&timebuffer);
    return (sx_ulong)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (sx_ulong)(((timebuffer.time * 1000) + timebuffer.millitm));
#endif
}

SEGAN_LIB_API sx_ulong sx_time_diff(const sx_ulong endtime, const sx_ulong starttime)
{
    return endtime - starttime;// difftime(endtime, starttime);
}

SEGAN_LIB_API void sx_time_print(char* dest, const sx_uint destsize, const sx_ulong timeval)
{
    struct tm timeInfo;
    sx_ulong t = timeval / 1000;
    localtime_s(&timeInfo, &t);
    strftime(dest, destsize, "%Y-%m-%d %H:%M:%S", &timeInfo);
}
