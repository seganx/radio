#include "net.h"
#include "../core/trace.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <errno.h>
    #include <string.h>
#endif

SEGAN_LIB_API bool sx_net_initialize(void)
{
    sx_trace();

#if defined(_WIN32)
    bool inited = true;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        sx_print("Error: Network initialization on Windows failed! error: %s !",
                 sx_net_error_string(WSAGetLastError()));
        inited = false;
    }

    if (inited)
    {
        bool incorrectVersion = (LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2);
        if (incorrectVersion)
        {
            sx_print("Error: Network initialization on Windows failed! Invalid version detected!");
            inited = false;
        }
    }

    if (!inited)
    {
        sx_print("The network system is now disabled.");
        WSACleanup();
    }

    sx_return(inited);
#else
    // POSIX sockets do not need initialization
    sx_return(true);
#endif
}

SEGAN_LIB_API void sx_net_finalize(void)
{
    sx_trace();

#if defined(_WIN32)
    WSACleanup();
    sx_print("Network system Finalized.");
#endif

    sx_return();
}

SEGAN_LIB_API sx_int sx_net_last_error_code(void)
{
#if defined(_WIN32)
    return (sx_int)WSAGetLastError();
#else
    return (sx_int)errno;
#endif
}

SEGAN_LIB_API const char* sx_net_last_error_string(sx_int code)
{
    if (code == 0) code = sx_net_last_error_code();

#if defined(_WIN32)
    return sx_net_error_string(code);
#else
    return strerror((int)code);
#endif
}

SEGAN_LIB_API char* sx_net_error_string(const sx_int code)
{
#if defined(_WIN32)
    switch (code)
    {
        case WSAEINTR:              return (char*)"WSAEINTR";
        case WSAEBADF:              return (char*)"WSAEBADF";
        case WSAEACCES:             return (char*)"WSAEACCES";
        case WSAEDISCON:            return (char*)"WSAEDISCON";
        case WSAEFAULT:             return (char*)"WSAEFAULT";
        case WSAEINVAL:             return (char*)"WSAEINVAL";
        case WSAEMFILE:             return (char*)"WSAEMFILE";
        case WSAEWOULDBLOCK:        return (char*)"WSAEWOULDBLOCK";
        case WSAEINPROGRESS:        return (char*)"WSAEINPROGRESS";
        case WSAEALREADY:           return (char*)"WSAEALREADY";
        case WSAENOTSOCK:           return (char*)"WSAENOTSOCK";
        case WSAEDESTADDRREQ:       return (char*)"WSAEDESTADDRREQ";
        case WSAEMSGSIZE:           return (char*)"WSAEMSGSIZE";
        case WSAEPROTOTYPE:         return (char*)"WSAEPROTOTYPE";
        case WSAENOPROTOOPT:        return (char*)"WSAENOPROTOOPT";
        case WSAEPROTONOSUPPORT:    return (char*)"WSAEPROTONOSUPPORT";
        case WSAESOCKTNOSUPPORT:    return (char*)"WSAESOCKTNOSUPPORT";
        case WSAEOPNOTSUPP:         return (char*)"WSAEOPNOTSUPP";
        case WSAEPFNOSUPPORT:       return (char*)"WSAEPFNOSUPPORT";
        case WSAEAFNOSUPPORT:       return (char*)"WSAEAFNOSUPPORT";
        case WSAEADDRINUSE:         return (char*)"WSAEADDRINUSE";
        case WSAEADDRNOTAVAIL:      return (char*)"WSAEADDRNOTAVAIL";
        case WSAENETDOWN:           return (char*)"WSAENETDOWN";
        case WSAENETUNREACH:        return (char*)"WSAENETUNREACH";
        case WSAENETRESET:          return (char*)"WSAENETRESET";
        case WSAECONNABORTED:       return (char*)"WSAECONNABORTED";
        case WSAECONNRESET:         return (char*)"WSAECONNRESET";
        case WSAENOBUFS:            return (char*)"WSAENOBUFS";
        case WSAEISCONN:            return (char*)"WSAEISCONN";
        case WSAENOTCONN:           return (char*)"WSAENOTCONN";
        case WSAESHUTDOWN:          return (char*)"WSAESHUTDOWN";
        case WSAETOOMANYREFS:       return (char*)"WSAETOOMANYREFS";
        case WSAETIMEDOUT:          return (char*)"WSAETIMEDOUT";
        case WSAECONNREFUSED:       return (char*)"WSAECONNREFUSED";
        case WSAELOOP:              return (char*)"WSAELOOP";
        case WSAENAMETOOLONG:       return (char*)"WSAENAMETOOLONG";
        case WSAEHOSTDOWN:          return (char*)"WSAEHOSTDOWN";
        case WSASYSNOTREADY:        return (char*)"WSASYSNOTREADY";
        case WSAVERNOTSUPPORTED:    return (char*)"WSAVERNOTSUPPORTED";
        case WSANOTINITIALISED:     return (char*)"WSANOTINITIALISED";
        case WSAHOST_NOT_FOUND:     return (char*)"WSAHOST_NOT_FOUND";
        case WSATRY_AGAIN:          return (char*)"WSATRY_AGAIN";
        case WSANO_RECOVERY:        return (char*)"WSANO_RECOVERY";
        case WSANO_DATA:            return (char*)"WSANO_DATA";
        default:                    return (char*)"UNKNOWN";
    }
#else
    return (char*)strerror((int)code);
#endif
}
