#pragma once

#include "../TCPServer.h"
#include "SrvCtlConnection.h"
#include <map>

namespace zinc {

struct SrvCtlServer {
private:
    int m_init, m_started = false;
    TCPServer m_server;
    int m_port;
    std::unordered_map<evutil_socket_t, SrvCtlConnection*> m_clients;
public:
    SrvCtlServer() : m_init(true), m_server(25575, onAccept), m_port(25575) {}
    SrvCtlServer(const int& port) : m_init(true), m_server(port, onAccept), m_port(port) {}
    ~SrvCtlServer() {
        if (m_init) stop();
    }

    void start();
    void stop();

    void setPort(const int& port);

    void addClient(SrvCtlConnection* client);
    bool isConnected(evutil_socket_t fd) const;
    void removeClient(evutil_socket_t fd);
    SrvCtlConnection* getClient(evutil_socket_t fd);

    static void onAccept(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr);
    static void onRead(bufferevent* bev, void* ptr);
    static void onEvent(bufferevent *bev, short events, void *ctx);
};

extern SrvCtlServer g_srvCtlServer;

}