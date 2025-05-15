#pragma once

#include "../TCPServer.h"
#include "ZincConnection.h"
#include <util/crypto/RSA.h>
#include <map>

namespace zinc {

struct ZincServer {
private:
    bool m_init, m_started = false;
    TCPServer m_server;
    int m_port;
    std::map<evutil_socket_t, ZincConnection*> m_clients;
    RSAWrapper m_rsa = RSAWrapper(RSA_PKCS1_PADDING);
    std::map<std::vector<unsigned char>, std::string> m_openedLoginPluginChannels;
public:
    int m_onlinePlayers = 0;

    ZincServer() : m_init(true), m_server(25565, onAccept), m_port(25565) {}
    ZincServer(const int& port) : m_init(true), m_server(port, onAccept), m_port(port) {}
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

    std::vector<unsigned char> openLoginPluginChannel(const std::string& channel);
    std::string getLoginPluginChannel(const std::vector<unsigned char>& id);
    bool isLoginPluginChannelOpened(const std::vector<unsigned char>& id);
    void closeLoginPluginChannel(const std::vector<unsigned char>& id);

    static void onAccept(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr);
    static void onRead(bufferevent* bev, void* ptr);
    static void onEvent(bufferevent *bev, short events, void *ctx);
};

extern ZincServer g_zincServer;
extern std::map<std::string, ZincPacket(*)(ByteBuffer&, ZincConnection*)> g_zincServerPluginChannels;
extern std::map<std::string, void(*)(std::optional<std::vector<char>>&, ZincConnection*)> g_zincCookieResponseParsers;
extern std::map<ZincConnection::State, std::vector<std::string>> g_zincCookieRequests;

}