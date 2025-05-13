#pragma once

#include <type/ByteBuffer.h>
#include <event2/bufferevent.h>
#include <arpa/inet.h>
#include <string>

namespace zinc {

struct TCPConnection {
private:
    bufferevent* m_bev;
    evutil_socket_t m_fd;
    std::string m_ip;
    struct sockaddr* m_addr;
public:
    TCPConnection() : m_bev(nullptr), m_fd(-1), m_ip("0.0.0.0") {}
    TCPConnection(bufferevent* bev, const evutil_socket_t& fd, struct sockaddr* addr) : m_bev(bev), m_fd(fd), m_ip("0.0.0.0"), m_addr(addr) {
        m_ip.clear();
        m_ip.resize(INET6_ADDRSTRLEN);
        if (addr->sa_family == AF_INET) {
            struct sockaddr_in* sin = (struct sockaddr_in*)addr;
            inet_ntop(AF_INET, &sin->sin_addr, m_ip.data(), INET6_ADDRSTRLEN);
        } else {
            struct sockaddr_in6* sin6 = (struct sockaddr_in6*)addr;
            inet_ntop(AF_INET6, &sin6->sin6_addr, m_ip.data(), INET6_ADDRSTRLEN);
        }
    }
    ~TCPConnection() {
        close();
    }

    std::string getIP() const;
    struct sockaddr* getAddr() const;
    bufferevent* getBuffer();
    const bufferevent* getBuffer() const;
    evutil_socket_t getFd() const;

    void setBuffer(bufferevent *bev);
    void setAddr(struct sockaddr* addr);
    void setFd(evutil_socket_t fd);

    void send(const ByteBuffer& data);
    ByteBuffer read();

    void close();

    bool operator==(const TCPConnection& connection) const;
    bool operator!=(const TCPConnection& connection) const;
};

}