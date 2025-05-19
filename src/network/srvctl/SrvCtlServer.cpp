#include <network/srvctl/SrvCtlServer.h>
#include <util/Logger.h>

namespace zinc {

Logger m_srvCtlLogger = Logger("SrvCtlServer");

SrvCtlServer g_srvCtlServer;

void SrvCtlServer::start() {
    m_srvCtlLogger.info("SrvCtl listening on port " + std::to_string(m_port));
    m_started = true;
    m_server.start();
}
void SrvCtlServer::stop() {
    if (!m_started) return;
    m_srvCtlLogger.info("SrvCtl stopped");
    m_server.stop();
}
void SrvCtlServer::setPort(const int& port) {
    m_port = port;
    m_server.setPort(port);
}
void SrvCtlServer::addClient(SrvCtlConnection* client) {
    if (!isConnected(client->getTCPConnection().getFd())) m_clients.insert({ client->getTCPConnection().getFd(), client });
}
bool SrvCtlServer::isConnected(evutil_socket_t fd) const {
    return m_clients.contains(fd);
}
void SrvCtlServer::removeClient(evutil_socket_t fd) {
    if (isConnected(fd)) {
        m_clients[fd]->getTCPConnection().close();
        delete m_clients[fd];
        m_clients.erase(fd);
    }
}
SrvCtlConnection* SrvCtlServer::getClient(evutil_socket_t fd) {
    if (!isConnected(fd)) m_srvCtlLogger.error("Attempted to get unknown client", true); 
    return m_clients[fd];
}
void SrvCtlServer::onAccept(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr) {
    bufferevent* bev = bufferevent_socket_new((event_base*) ptr, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        m_srvCtlLogger.error("Failed to create bufferevent");
        return;
    }
    SrvCtlConnection* connection = new SrvCtlConnection();
    connection->getTCPConnection().setAddr(addr);
    connection->getTCPConnection().setFd(fd);
    connection->getTCPConnection().setBuffer(bev);
    g_srvCtlServer.addClient(connection);
    m_srvCtlLogger.debug("Client [" + connection->getTCPConnection().getIP() + "] connected!"); 
    bufferevent_setcb(bev, onRead, nullptr, onEvent, ptr);
    bufferevent_enable(bev, EV_READ);
}
void SrvCtlServer::onRead(bufferevent* bev, void* ptr) {
    if (!g_srvCtlServer.isConnected(bufferevent_getfd(bev))) return;
    SrvCtlConnection* connection = g_srvCtlServer.getClient(bufferevent_getfd(bev));
    ByteBuffer buffer = connection->getTCPConnection().read();
    ByteBuffer resultBuffer;
    if (buffer.size() < 9) {
        resultBuffer.writeNumeric<unsigned long>(1 + resultBuffer.getVarNumericLength<int>(-1));
        resultBuffer.writeVarNumeric<int>(-1);   // status s2c
        resultBuffer.writeByte(-1);     // state: error -1 (invalid packet)
        TCPUtil::drain(bev, buffer.size());
        connection->getTCPConnection().send(resultBuffer);
        return;
    }
    size_t length = buffer.readNumeric<size_t>();
    if (buffer.size() < (length + 8)) {
        resultBuffer.writeNumeric<unsigned long>(1 + resultBuffer.getVarNumericLength<int>(-1));
        resultBuffer.writeVarNumeric<int>(-1);   // status s2c
        resultBuffer.writeByte(-1);     // state: error -1 (invalid packet)
        TCPUtil::drain(bev, buffer.size());
        connection->getTCPConnection().send(resultBuffer);
        return;
    }
    int packetId = buffer.readVarNumeric<int>();
    TCPUtil::drain(bev, 8 + length);
    switch (connection->getState()) {
    case SrvCtlConnection::State::Handshake: {
        // don't check packet id. it's useless now
        connection->setState(SrvCtlConnection::State::Login);
        resultBuffer.writeNumeric<unsigned long>(1 + resultBuffer.getVarNumericLength<int>(-1));
        resultBuffer.writeVarNumeric<int>(-1);   // status s2c
        resultBuffer.writeByte(1);      // state: success
        connection->getTCPConnection().send(resultBuffer);
        break;
    }
    case SrvCtlConnection::State::Login: {
        switch (packetId) {
        case 0: { // login start
            break;
        }
        default: {
            resultBuffer.writeNumeric<unsigned long>(1 + resultBuffer.getVarNumericLength<int>(-1));
            resultBuffer.writeVarNumeric<int>(-1);  // status s2c
            resultBuffer.writeByte(-1);    // state: error -1 (invalid packet)
            connection->getTCPConnection().send(resultBuffer);
            break;
        }
        }
        break;
    }
    default: {
        resultBuffer.writeNumeric<unsigned long>(1 + resultBuffer.getVarNumericLength<int>(-1));
        resultBuffer.writeVarNumeric<int>(-1);  // status s2c
        resultBuffer.writeByte(-2);    // state: error -2 (unsupported protocol state)
        connection->getTCPConnection().send(resultBuffer);
        break;
    }
    }
}
void SrvCtlServer::onEvent(bufferevent *bev, short events, void *ctx) {
    int fd = bufferevent_getfd(bev);
    m_srvCtlLogger.debug("Event triggered: " + std::to_string(events) + " on fd " + std::to_string(fd));
    if (!g_srvCtlServer.isConnected(bufferevent_getfd(bev))) return;
    SrvCtlConnection* connection = g_srvCtlServer.getClient(bufferevent_getfd(bev));
    if (events & BEV_EVENT_EOF) {
        m_srvCtlLogger.info("Client [" + connection->getTCPConnection().getIP() + "] disconnected!"); 
        g_srvCtlServer.removeClient(fd);
    } else if (events & BEV_EVENT_ERROR) {
        m_srvCtlLogger.error("Client [" + connection->getTCPConnection().getIP() + "] encountered libevent error!"); 
        g_srvCtlServer.removeClient(fd);
    } else if (events & BEV_EVENT_TIMEOUT) {
        m_srvCtlLogger.info("Client [" + connection->getTCPConnection().getIP() + "] timed out!"); 
        g_srvCtlServer.removeClient(fd);
    }
}

}
