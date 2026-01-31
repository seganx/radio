#pragma once

#include "core/def.h"

#define TYPE_PING           1
#define TYPE_LOGIN          10
#define TYPE_LOGOUT         11
#define TYPE_CREATE         20
#define TYPE_JOIN           30
#define TYPE_LEAVE          31
#define TYPE_PACKET_UNRELY  40
#define TYPE_PACKET_RELY    41
#define TYPE_PACKET_RELIED  42

#define FLAG_MASTER         1

#define ERR_INVALID         -1
#define ERR_EXPIRED         -2
#define ERR_IS_FULL         -3
#define ERR_MATCHMAKE       -4

#define DEVICE_LEN          32
#define THREAD_COUNTS       32
#define ADDRESS_LEN         32
#define ROOM_PROP_LEN       32
#define ROOM_COUNT          1024
#define ROOM_CAPACITY       4
#define ROOM_PARAMS         4
#define LOBBY_CAPACITY      (ROOM_COUNT * ROOM_CAPACITY)

#define LOG                 1

typedef struct Player
{
    char    device[DEVICE_LEN];
    sx_byte    from[ADDRESS_LEN];
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_byte    flag;
    sx_ulong   active_time;
}
Player;

typedef struct Lobby
{
    sx_uint    count;
    Player  players[LOBBY_CAPACITY];
}
Lobby;

typedef struct Room
{
    sx_byte    capacity;
    sx_sbyte   count;
    sx_ulong   open_time;
    sx_ulong   join_timeout;
    sx_byte    properties[ROOM_PROP_LEN];
    sx_int    matchmaking[ROOM_PARAMS];
    Player* players[ROOM_CAPACITY];
}
Room;

typedef struct Config
{
    sx_ushort  port;
    sx_uint    player_timeout;
    sx_uint    player_master_timeout;
} 
Config;

typedef struct Server
{
    Config  config;
    sx_uint    socket;
    sx_uint    token;
    Lobby   lobby;
    Room    rooms[ROOM_COUNT];

    struct sx_mutex* mutex;
}
Server;

#pragma pack(push,1)
typedef struct Ping
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_ulong   time;
}
Ping;

typedef struct PingResponse
{
    sx_byte    type;
    sx_sbyte   error;
    sx_ulong   time;
    sx_ulong   now;
    sx_byte    flag;
}
PingResponse;

typedef struct Login
{
    sx_byte    type;
    char    device[DEVICE_LEN];
    sx_uint    checksum;
}
Login;

typedef struct LoginResponse
{
    sx_byte    type;
    sx_sbyte   error;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_uint    checksum;
}
LoginResponse;

typedef struct Logout
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_uint    checksum;
}
Logout;

typedef struct Create
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    sx_byte    capacity;
    sx_byte    join_timeout;
    sx_byte    properties[ROOM_PROP_LEN];
    sx_int    matchmaking[ROOM_PARAMS];
}
Create;

typedef struct CreateResponse
{
    sx_byte    type;
    sx_sbyte   error;
    short   room;
    sx_sbyte   index;
    sx_byte    flag;
}
CreateResponse;

typedef struct Join
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    sx_int    matchmaking[ROOM_PARAMS * 2];
}
Join;

typedef struct JoinResponse
{
    sx_byte    type;
    sx_sbyte   error;
    short   room;
    sx_sbyte   index;
    sx_byte    flag;
    sx_byte    properties[ROOM_PROP_LEN];
}
JoinResponse;

typedef struct Leave
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
}
Leave;

typedef struct LeaveResponse
{
    sx_byte    type;
    sx_sbyte   error;
}
LeaveResponse;

typedef struct PacketUnreliable
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_sbyte   target;
    sx_byte    datasize;
}
PacketUnreliable;

typedef struct PacketReliable
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_sbyte   target;
    sx_byte    ack;
    sx_byte    datasize;
}
PacketReliable;

typedef struct PacketRelied
{
    sx_byte    type;
    sx_uint    token;
    short   id;
    short   room;
    sx_sbyte   index;
    sx_sbyte   target;
    sx_byte    ack;
}
PacketRelied;

typedef struct ErrorResponse
{
    sx_byte    type;
    sx_sbyte   error;
}
ErrorResponse;

#pragma pack(pop)
