#pragma once

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <util/Logger.h>
#include <util/TCPUtil.h>

namespace zinc {

class TCPServer {
private:
    bool m_started = false;
    Logger m_logger = Logger("TCPServer");
    unsigned short m_port;
    event_base* m_base;
    evconnlistener* m_listener;
    void(*m_onAccept)(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr);
public:
    TCPServer() : m_port(0), m_base(nullptr), m_listener(nullptr) {}
    TCPServer(const unsigned short& port, void(*onAccept)(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr)) 
        : m_port(port), m_base(nullptr), m_listener(nullptr), m_onAccept(onAccept) {
        m_base = event_base_new();
        if (!m_base) {
            m_logger.error("Failed to create event base", true);
        }
    }
    ~TCPServer() {
        if (!m_port) return;
        stop();
        if (m_base) {
            event_base_free(m_base);
        }
    }

    void start();
    void stop();

    void setPort(const unsigned short& port);
};

}
