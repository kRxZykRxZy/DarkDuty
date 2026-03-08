#pragma once
/*
 * Cross-platform UDP socket abstraction.
 * Windows  → Winsock2
 * Linux/FreeBSD/macOS → POSIX sockets
 */

#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <winsock2.h>
#  include <ws2tcpip.h>
   using SockFd = SOCKET;
   static constexpr SockFd INVALID_SOCKFD = INVALID_SOCKET;
#  define SOCK_ERR    SOCKET_ERROR
#  define WOULD_BLOCK WSAEWOULDBLOCK
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <fcntl.h>
#  include <unistd.h>
#  include <errno.h>
   using SockFd = int;
   static constexpr SockFd INVALID_SOCKFD = -1;
#  define SOCK_ERR    -1
#  define WOULD_BLOCK EAGAIN
#  define closesocket close
#endif

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

// ─── Network packet types ─────────────────────────────────────────────────────
enum class NetMsgType : uint8_t {
    HELLO      = 1,  // client → server: connect
    WELCOME    = 2,  // server → client: assign id
    INPUT      = 3,  // client → server: player input
    STATE      = 4,  // server → client: world snapshot
    CHAT       = 5,  // both directions
    DISCONNECT = 6,  // graceful disconnect
    PING       = 7,
    PONG       = 8
};

// ─── Fixed-size packet header ─────────────────────────────────────────────────
#pragma pack(push, 1)
struct NetHeader {
    uint8_t  type;    // NetMsgType
    uint8_t  playerId;
    uint16_t seq;
    uint16_t payloadLen;
};

struct NetInput {
    NetHeader hdr;
    uint8_t   buttons;    // bit 0=up 1=down 2=left 3=right 4=fire 5=reload
    uint8_t   weaponSlot;
    float     aimX;
    float     aimY;
};

struct NetPlayerState {
    uint8_t  id;
    float    x, y;
    float    facing;
    uint8_t  hp;
    uint8_t  weaponSlot;
    uint8_t  ammo;
};

struct NetStatePacket {
    NetHeader      hdr;
    uint8_t        numPlayers;
    NetPlayerState players[4];
    uint16_t       numBullets; // simplified: server broadcasts bullet events
};
#pragma pack(pop)

// ─── Platform init/shutdown ───────────────────────────────────────────────────
inline bool netInit() {
#ifdef _WIN32
    WSADATA wd;
    return WSAStartup(MAKEWORD(2,2), &wd) == 0;
#else
    return true;
#endif
}
inline void netShutdown() {
#ifdef _WIN32
    WSACleanup();
#endif
}

inline int netLastError() {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

// ─── UDP Socket wrapper ───────────────────────────────────────────────────────
class UdpSocket {
public:
    UdpSocket() = default;
    ~UdpSocket() { close(); }

    // Non-copyable
    UdpSocket(const UdpSocket&) = delete;
    UdpSocket& operator=(const UdpSocket&) = delete;

    bool open(uint16_t port = 0) {
        fd_ = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (fd_ == INVALID_SOCKFD) return false;

        // Non-blocking
#ifdef _WIN32
        u_long nb = 1;
        ioctlsocket(fd_, FIONBIO, &nb);
#else
        int flags = fcntl(fd_, F_GETFL, 0);
        fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
#endif
        // Allow broadcast
        int bcast = 1;
        setsockopt(fd_, SOL_SOCKET, SO_BROADCAST,
                   reinterpret_cast<const char*>(&bcast), sizeof(bcast));

        if (port > 0) {
            sockaddr_in addr{};
            addr.sin_family      = AF_INET;
            addr.sin_port        = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;
            if (::bind(fd_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCK_ERR) {
                close();
                return false;
            }
        }
        return true;
    }

    bool sendTo(const void* data, int len, const std::string& ip, uint16_t port) {
        sockaddr_in addr{};
        addr.sin_family      = AF_INET;
        addr.sin_port        = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        int sent = ::sendto(fd_, reinterpret_cast<const char*>(data), len, 0,
                            reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        return sent == len;
    }

    // Returns bytes received, -1 if nothing available
    int recvFrom(void* buf, int maxLen, std::string* fromIp, uint16_t* fromPort) {
        sockaddr_in from{};
        socklen_t   fromLen = sizeof(from);
        int n = ::recvfrom(fd_, reinterpret_cast<char*>(buf), maxLen, 0,
                           reinterpret_cast<sockaddr*>(&from), &fromLen);
        if (n < 0) return -1;
        if (fromIp)   *fromIp   = inet_ntoa(from.sin_addr);
        if (fromPort) *fromPort = ntohs(from.sin_port);
        return n;
    }

    bool isOpen() const { return fd_ != INVALID_SOCKFD; }

    void close() {
        if (fd_ != INVALID_SOCKFD) {
            closesocket(fd_);
            fd_ = INVALID_SOCKFD;
        }
    }

private:
    SockFd fd_ = INVALID_SOCKFD;
};

// ─── LAN Server ───────────────────────────────────────────────────────────────
static constexpr uint16_t DEFAULT_PORT = 7777;
static constexpr int      MAX_CLIENTS  = 4;

struct ClientInfo {
    std::string ip;
    uint16_t    port    = 0;
    uint8_t     id      = 0;
    bool        active  = false;
    float       timeout = 0.f;
    uint16_t    lastSeq = 0;
};

class LanServer {
public:
    bool start(uint16_t port = DEFAULT_PORT) {
        if (!sock_.open(port)) return false;
        running_ = true;
        port_    = port;
        return true;
    }

    void stop() {
        sock_.close();
        running_ = false;
    }

    // Call every frame; fills outgoing state, processes incoming inputs
    // Returns true if any client connected/disconnected
    bool poll(float dt,
              std::function<void(uint8_t clientId, const NetInput&)> onInput,
              std::function<void(uint8_t clientId, bool joined)>     onConnect)
    {
        if (!running_) return false;

        // Timeout clients
        for (auto& c : clients_) {
            if (!c.active) continue;
            c.timeout -= dt;
            if (c.timeout <= 0.f) {
                c.active = false;
                if (onConnect) onConnect(c.id, false);
            }
        }

        // Receive packets
        static char buf[1024];
        std::string ip; uint16_t fromPort = 0;
        int n;
        while ((n = sock_.recvFrom(buf, sizeof(buf), &ip, &fromPort)) > 0) {
            if (n < (int)sizeof(NetHeader)) continue;
            NetHeader* hdr = reinterpret_cast<NetHeader*>(buf);

            if (hdr->type == (uint8_t)NetMsgType::HELLO) {
                // Find or add client
                ClientInfo* found = nullptr;
                for (auto& c : clients_) {
                    if (c.active && c.ip == ip && c.port == fromPort) { found = &c; break; }
                }
                if (!found) {
                    for (auto& c : clients_) {
                        if (!c.active) { found = &c; break; }
                    }
                    if (found) {
                        found->ip     = ip;
                        found->port   = fromPort;
                        found->id     = ++nextId_;
                        found->active = true;
                        if (onConnect) onConnect(found->id, true);
                    }
                }
                if (found) {
                    found->timeout = 5.f;
                    // Send WELCOME
                    NetHeader welcome{};
                    welcome.type     = (uint8_t)NetMsgType::WELCOME;
                    welcome.playerId = found->id;
                    welcome.seq      = 0;
                    welcome.payloadLen = 0;
                    sock_.sendTo(&welcome, sizeof(welcome), ip, fromPort);
                }
            }
            else if (hdr->type == (uint8_t)NetMsgType::INPUT) {
                if (n >= (int)sizeof(NetInput)) {
                    NetInput* inp = reinterpret_cast<NetInput*>(buf);
                    // Find client
                    for (auto& c : clients_) {
                        if (c.active && c.ip == ip && c.port == fromPort) {
                            c.timeout = 5.f;
                            if (onInput) onInput(c.id, *inp);
                            break;
                        }
                    }
                }
            }
            else if (hdr->type == (uint8_t)NetMsgType::PING) {
                NetHeader pong{};
                pong.type = (uint8_t)NetMsgType::PONG;
                sock_.sendTo(&pong, sizeof(pong), ip, fromPort);
            }
        }
        return true;
    }

    // Broadcast game state to all clients
    void broadcastState(const NetStatePacket& state) {
        for (auto& c : clients_) {
            if (!c.active) continue;
            sock_.sendTo(&state, sizeof(state), c.ip, c.port);
        }
    }

    int clientCount() const {
        int n = 0;
        for (auto& c : clients_) if (c.active) n++;
        return n;
    }

    bool isRunning() const { return running_; }
    uint16_t port()  const { return port_;    }

private:
    UdpSocket  sock_;
    bool       running_  = false;
    uint16_t   port_     = DEFAULT_PORT;
    uint8_t    nextId_   = 0;
    ClientInfo clients_[MAX_CLIENTS] = {};
};

// ─── LAN Client ───────────────────────────────────────────────────────────────
class LanClient {
public:
    bool connect(const std::string& serverIp, uint16_t port = DEFAULT_PORT) {
        if (!sock_.open(0)) return false;
        serverIp_   = serverIp;
        serverPort_ = port;
        connected_  = false;
        myId_       = 0;
        seq_        = 0;
        timeout_    = 5.f;
        sendHello();
        return true;
    }

    void disconnect() {
        if (connected_) {
            NetHeader bye{};
            bye.type     = (uint8_t)NetMsgType::DISCONNECT;
            bye.playerId = myId_;
            sock_.sendTo(&bye, sizeof(bye), serverIp_, serverPort_);
        }
        sock_.close();
        connected_ = false;
    }

    // Send input to server
    void sendInput(const InputState& inp) {
        NetInput pkt{};
        pkt.hdr.type       = (uint8_t)NetMsgType::INPUT;
        pkt.hdr.playerId   = myId_;
        pkt.hdr.seq        = seq_++;
        pkt.hdr.payloadLen = sizeof(NetInput) - sizeof(NetHeader);
        pkt.buttons  = (inp.up     ? 0x01 : 0)
                     | (inp.down   ? 0x02 : 0)
                     | (inp.left   ? 0x04 : 0)
                     | (inp.right  ? 0x08 : 0)
                     | (inp.fire   ? 0x10 : 0)
                     | (inp.reload ? 0x20 : 0);
        pkt.weaponSlot = (uint8_t)inp.weaponSlot;
        pkt.aimX       = inp.aimX;
        pkt.aimY       = inp.aimY;
        sock_.sendTo(&pkt, sizeof(pkt), serverIp_, serverPort_);
    }

    // Poll for incoming state; callback receives latest world snapshot
    void poll(float dt,
              std::function<void(const NetStatePacket&)> onState)
    {
        timeout_ -= dt;
        if (timeout_ <= 0.f) connected_ = false;

        static char buf[1024];
        int n;
        while ((n = sock_.recvFrom(buf, 1024, nullptr, nullptr)) > 0) {
            if (n < (int)sizeof(NetHeader)) continue;
            NetHeader* hdr = reinterpret_cast<NetHeader*>(buf);
            if (hdr->type == (uint8_t)NetMsgType::WELCOME) {
                myId_      = hdr->playerId;
                connected_ = true;
                timeout_   = 5.f;
            }
            else if (hdr->type == (uint8_t)NetMsgType::STATE && connected_) {
                timeout_ = 5.f;
                if (n >= (int)sizeof(NetStatePacket) && onState) {
                    onState(*reinterpret_cast<NetStatePacket*>(buf));
                }
            }
            else if (hdr->type == (uint8_t)NetMsgType::PONG) {
                timeout_ = 5.f;
            }
        }

        // Keep-alive HELLO if not connected
        if (!connected_) {
            helloCooldown_ -= dt;
            if (helloCooldown_ <= 0.f) { sendHello(); helloCooldown_ = 1.f; }
        }
    }

    bool     isConnected() const { return connected_; }
    uint8_t  myId()        const { return myId_;       }

private:
    void sendHello() {
        NetHeader hello{};
        hello.type = (uint8_t)NetMsgType::HELLO;
        sock_.sendTo(&hello, sizeof(hello), serverIp_, serverPort_);
    }

    UdpSocket   sock_;
    std::string serverIp_;
    uint16_t    serverPort_  = DEFAULT_PORT;
    bool        connected_   = false;
    uint8_t     myId_        = 0;
    uint16_t    seq_         = 0;
    float       timeout_     = 5.f;
    float       helloCooldown_ = 0.f;
};
