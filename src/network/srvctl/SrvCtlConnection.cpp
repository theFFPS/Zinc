#include <network/srvctl/SrvCtlConnection.h>

namespace zinc {

TCPConnection& SrvCtlConnection::getTCPConnection() {
    return m_tcpConnection;
}
TCPConnection SrvCtlConnection::getTCPConnection() const {
    return m_tcpConnection;
}
SrvCtlConnection::State& SrvCtlConnection::getState() {
    return m_state;
}
SrvCtlConnection::State SrvCtlConnection::getState() const {
    return m_state;
}
void SrvCtlConnection::setState(const State& state) {
    m_state = state;
}
bool SrvCtlConnection::operator==(const SrvCtlConnection& connection) const {
    return m_tcpConnection == connection.getTCPConnection();
}
bool SrvCtlConnection::operator!=(const SrvCtlConnection& connection) const {
    return m_tcpConnection != connection.getTCPConnection();
}

}