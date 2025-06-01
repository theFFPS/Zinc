#pragma once

#include "../TCPServer.h"
#include "ZincConnection.h"
#include <util/crypto/RSA.h>
#include <mutex>

namespace zinc {

struct ZincServer {
private:
    bool m_init, m_started = false;
    TCPServer m_server;
    int m_port;
    std::unordered_map<evutil_socket_t, ZincConnection*> m_clients;
    std::unordered_map<std::string, long> m_rateLimiterData;
    std::unordered_map<std::string, int> m_concurrentAccounts;
    RSAWrapper m_rsa = RSAWrapper(RSA_PKCS1_PADDING);
public:
    int m_onlinePlayers = 0;
    std::mutex m_mutex;
    RSAWrapper m_cookieRSA;

    ZincServer() : m_init(true), m_server(25565, onAccept), m_port(25565) {}
    ZincServer(const unsigned short& port) : m_init(true), m_server(port, onAccept), m_port(port) {}
    ~ZincServer() {
        if (m_init) stop();
    }

    void start();
    void stop();

    void setPort(const int& port);

    RSAWrapper& getRSA();
    RSAWrapper getRSA() const;

    void addClient(ZincConnection* client);
    bool isConnected(evutil_socket_t fd) const;
    void removeClient(evutil_socket_t fd);
    ZincConnection* getClient(evutil_socket_t fd);

    static void onAccept(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr);
    static void onRead(bufferevent* bev, void* ptr);
    static void onEvent(bufferevent *bev, short events, void *ctx);
};

extern ZincServer g_zincServer;

}