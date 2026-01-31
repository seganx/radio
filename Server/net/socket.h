/********************************************************************
    created:	2012/04/11
    filename: 	Socket.h
    Author:		Sajad Beigjani
    eMail:		sajad.b@gmail.com
    Site:		www.SeganX.com
    Desc:		This file contain a basic part of network system

                NOTE: this module implemented to use in single thread.
                using in multi threaded system may cause to unpredicted
                behavior

*********************************************************************/
/********************************************************************
    created:    2012/04/11
    filename:   Socket.h
    Author:     Sajad Beigjani
    Desc:       Basic part of network system (UDP sockets)
*********************************************************************/
#pragma once

#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t   sx_socket_t;
struct sockaddr;

//! portable invalid socket value (matches INVALID_SOCKET on Windows and -1 on POSIX)
 #define SX_INVALID_SOCKET ((sx_socket_t)(-1))

//! open a UDP socket and optionally bind it to the specified port
//! (legacy behavior: blocking by default)
SEGAN_LIB_API sx_socket_t sx_socket_open(const sx_ushort port, const bool bind, const bool broadcast);

//! open with extra options (new)
SEGAN_LIB_API sx_socket_t sx_socket_open_ex(const sx_ushort port, const bool bind, const bool broadcast, const bool nonblocking);

//! close opened socket
SEGAN_LIB_API void sx_socket_close(sx_socket_t socket);

//! send data to the destination address
SEGAN_LIB_API bool sx_socket_send_in(sx_socket_t socket, const struct sockaddr* address, const void* buffer, const int size);

//! receive data and fill out address of sender
SEGAN_LIB_API sx_int sx_socket_receive(sx_socket_t socket, void* buffer, const int size, struct sockaddr* from);

#ifdef __cplusplus
}
#endif
