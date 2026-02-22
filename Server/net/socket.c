#include "socket.h"
#include "core/trace.h"
#include "core/platform.h"

#if defined(_WIN32)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <string.h>
#endif


static void sx_socket_platform_close(sx_socket_t s)
{
#if defined(_WIN32)
    closesocket((SOCKET)s);
#else
    close((int)s);
#endif
}

static bool sx_socket_set_nonblocking(sx_socket_t s, bool nonblocking)
{
#if defined(_WIN32)
    u_long mode = nonblocking ? 1UL : 0UL;
    return ioctlsocket((SOCKET)s, FIONBIO, &mode) == 0;
#else
    int fd = (int)s;
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;

    if (nonblocking) flags |= O_NONBLOCK;
    else             flags &= ~O_NONBLOCK;

    return fcntl(fd, F_SETFL, flags) == 0;
#endif
}

SEGAN_LIB_API sx_socket_t sx_socket_open(const sx_ushort port, const bool bindtoport, const bool broadcast)
{
    return sx_socket_open_ex(port, bindtoport, broadcast, false);
}

SEGAN_LIB_API sx_socket_t sx_socket_open_ex(const sx_ushort port, const bool bindtoport, const bool broadcast, const bool nonblocking)
{
    sx_trace();

    sx_socket_t s = SX_INVALID_SOCKET;

#if defined(_WIN32)
    SOCKET ws = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ws == INVALID_SOCKET)
    {
        sx_int err = sx_net_last_error_code();
        sx_print("Error: Can't initialize socket. error: %s !", sx_net_last_error_string(err));
        sx_return(SX_INVALID_SOCKET);
    }
    s = (sx_socket_t)ws;
#else
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        sx_int err = sx_net_last_error_code();
        sx_print("Error: Can't initialize socket. error: %s !", sx_net_last_error_string(err));
        sx_return(SX_INVALID_SOCKET);
    }
    s = (sx_socket_t)fd;
#endif

    if (broadcast)
    {
#if defined(_WIN32)
        int yes = 1;
        if (setsockopt((SOCKET)s, SOL_SOCKET, SO_BROADCAST, (const char*)&yes, (int)sizeof(yes)) != 0)
#else
        int yes = 1;
        if (setsockopt((int)s, SOL_SOCKET, SO_BROADCAST, &yes, (socklen_t)sizeof(yes)) != 0)
#endif
        {
            sx_int err = sx_net_last_error_code();
            sx_print("Error: Unable to enable broadcast! error: %s !", sx_net_last_error_string(err));
        }
    }

    if (bindtoport)
    {
        struct sockaddr_in address;
#if defined(_WIN32)
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind((SOCKET)s, (const struct sockaddr*)&address, (int)sizeof(address)) != 0)
        {
            sx_int err = sx_net_last_error_code();
            sx_print("Error: Unable to bind socket! error: %s !", sx_net_last_error_string(err));
            sx_socket_platform_close(s);
            sx_return(SX_INVALID_SOCKET);
        }
#else
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind((int)s, (const struct sockaddr*)&address, (socklen_t)sizeof(address)) != 0)
        {
            sx_int err = sx_net_last_error_code();
            sx_print("Error: Unable to bind socket! error: %s !", sx_net_last_error_string(err));
            sx_socket_platform_close(s);
            sx_return(SX_INVALID_SOCKET);
        }
#endif
    }

    if (nonblocking)
    {
        if (!sx_socket_set_nonblocking(s, true))
        {
            sx_int err = sx_net_last_error_code();
            sx_print("Error: Unable to make socket non-blocking! error: %s !", sx_net_last_error_string(err));
            sx_socket_platform_close(s);
            sx_return(SX_INVALID_SOCKET);
        }
    }

    sx_print("Info: Socket has been opened on port : %d", port);
    sx_return(s);
}

SEGAN_LIB_API void sx_socket_close(sx_socket_t socket)
{
    if (socket == SX_INVALID_SOCKET) return;
    sx_socket_platform_close(socket);
}

SEGAN_LIB_API bool sx_socket_send_in(sx_socket_t socket, const struct sockaddr* address, const void* buffer, const int size)
{
    sx_assert(socket != SX_INVALID_SOCKET && buffer && size > 0);
    sx_trace();

#if defined(_WIN32)
    int sent = sendto((SOCKET)socket, (const char*)buffer, size, 0, address, (int)sizeof(struct sockaddr_in));
    sx_return(sent == size);
#else
    ssize_t sent = sendto((int)socket, buffer, (size_t)size, 0, address, (socklen_t)sizeof(struct sockaddr_in));
    sx_return(sent == (ssize_t)size);
#endif
}

SEGAN_LIB_API sx_int sx_socket_receive(sx_socket_t socket, void* buffer, const int size, struct sockaddr* from)
{
    sx_assert(socket != SX_INVALID_SOCKET && buffer && size > 0);
    sx_trace();

#if defined(_WIN32)
    int fromlen = (int)sizeof(struct sockaddr_in);
    int received = recvfrom((SOCKET)socket, (char*)buffer, size, 0, from, &fromlen);
    sx_return(received <= 0 ? 0 : (sx_int)received);
#else
    socklen_t fromlen = (socklen_t)sizeof(struct sockaddr_in);
    ssize_t received = recvfrom((int)socket, buffer, (size_t)size, 0, from, &fromlen);
    sx_return(received <= 0 ? 0 : (sx_int)received);
#endif
}
