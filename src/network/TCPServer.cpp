#include <network/TCPServer.h>

namespace zinc {

void TCPServer::start() {
    if (!m_port) return;
    if (m_base == nullptr) return;
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_port);

    m_listener = evconnlistener_new_bind(m_base, m_onAccept, m_base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!m_listener) {
        m_logger.error("Failed to create listener", true);
    }
    m_logger.debug("Started TCPServer");
    m_started = true;
    event_base_dispatch(m_base);
}
void TCPServer::stop() {
    if (!m_started) return;
    m_logger.debug("Stopped TCPServer");
    if (m_listener) {
        evconnlistener_free(m_listener);
        m_listener = nullptr;
    }
}
void TCPServer::setPort(const unsigned short& port) {
    m_port = port;
}

}
