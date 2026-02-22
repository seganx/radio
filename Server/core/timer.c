#include <time.h>
#include <sys/timeb.h>

#include "platform.h"
#include "timer.h"

SEGAN_LIB_API sx_int sx_localtime(struct tm* out, const time_t* t)
{
    if (!out || !t) return -1;

#if defined(_WIN32)
    // returns 0 on success
    return localtime_s(out, t);
#else
    // returns NULL on failure
    return (localtime_r(t, out) != NULL) ? 0 : -1;
#endif
}


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
    time_t t = timeval / 1000;
    sx_localtime(&timeInfo, &t);
    strftime(dest, destsize, "%Y-%m-%d %H:%M:%S", &timeInfo);
}
