#pragma once
#include <cstdint>

enum class NetMsgType : uint8_t {
    HELLO      = 1,
    WELCOME    = 2,
    INPUT      = 3,
    STATE      = 4,
    CHAT       = 5,
    DISCONNECT = 6,
    PING       = 7,
    PONG       = 8
};

#pragma pack(push, 1)

struct NetHeader {
    uint8_t  type;
    uint8_t  playerId;
    uint16_t seq;
    uint16_t payloadLen;
};

struct NetInput {
    NetHeader hdr;
    uint8_t   buttons;
    uint8_t   weaponSlot;
    float     aimX;
    float     aimY;
};

struct NetPlayerState {
    uint8_t id;
    float   x, y, z;
    float   yaw;
    uint8_t hp;
    uint8_t weaponSlot;
};

struct NetStatePacket {
    NetHeader      hdr;
    uint8_t        numPlayers;
    NetPlayerState players[4];
};

#pragma pack(pop)
