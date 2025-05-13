#include <network/TCPConnection.h>
#include <util/TCPUtil.h>

namespace zinc {

std::string TCPConnection::getIP() const {
    return m_ip;
}
struct sockaddr* TCPConnection::getAddr() const {
    return m_addr;
}
bufferevent* TCPConnection::getBuffer() {
    return m_bev;
}
const bufferevent* TCPConnection::getBuffer() const {
    return m_bev;
}
evutil_socket_t TCPConnection::getFd() const {
    return m_fd;
}
void TCPConnection::setBuffer(bufferevent *bev) {
    m_bev = bev;
}
void TCPConnection::setAddr(struct sockaddr* addr) {
    m_addr = addr;
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
void TCPConnection::setFd(evutil_socket_t fd) {
    m_fd = fd;
}
void TCPConnection::send(const ByteBuffer& data) {
    TCPUtil::send(m_bev, data);
}
ByteBuffer TCPConnection::read() {
    return TCPUtil::read(m_bev);
}
void TCPConnection::close() {
    if (m_bev) bufferevent_free(m_bev);
}
bool TCPConnection::operator==(const TCPConnection& connection) const {
    return m_fd == connection.getFd();
}
bool TCPConnection::operator!=(const TCPConnection& connection) const {
    return m_fd != connection.getFd();
}

}