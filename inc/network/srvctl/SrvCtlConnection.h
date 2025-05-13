#pragma once

#include "../TCPConnection.h"

namespace zinc {

struct SrvCtlConnection {
public:
    enum class State : unsigned char {
        Handshake, Login, Config, Execution
    };
private:
    TCPConnection m_tcpConnection;
    State m_state;
public:
    SrvCtlConnection() : m_tcpConnection(TCPConnection()), m_state(State::Handshake) {}
    SrvCtlConnection(const TCPConnection& connection) : m_tcpConnection(connection), m_state(State::Handshake) {}

    TCPConnection& getTCPConnection();
    TCPConnection getTCPConnection() const;
    State& getState();
    State getState() const;

    void setState(const State& state);

    bool operator==(const SrvCtlConnection& connection) const;
    bool operator!=(const SrvCtlConnection& connection) const;
};

}