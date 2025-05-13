#pragma once

#include "../TCPConnection.h"
#include "ZincPacket.h"
#include <util/crypto/AES.h>
#include <external/UUID.h>
#include <mutex>

namespace zinc {

struct ZincConnectionProperty {
    std::string m_name;
    std::string m_value;
    std::optional<std::string> m_signature;
};
void writeZincConnectionProperty(const ZincConnectionProperty& property, ByteBuffer& buffer);
struct ZincConnectionInfo {
    struct NetworkInfo {
        std::string m_serverAddr;
        int m_serverPort;
        int m_protocolVersion;
        std::vector<unsigned char> m_verifyToken;
    } m_networkInfo;
    struct PlayerInfo {
        std::string m_playerName;
        uuids::uuid m_playerUUID;
        std::vector<ZincConnectionProperty> m_properties;
    } m_playerInfo;
    struct SettingsInfo {
        std::string m_locale;
        unsigned char m_renderDistance;
        enum class ChatMode : int { Enabled, CommandsOnly, Hidden } m_chatMode;
        bool m_enableChatColors; // maybe some plugins use it
        enum class SkinPartsBits : unsigned char { Cape = 1, Jacket, LeftSleeve = 4, RightSleeve = 8, LeftPants = 16, RightPants = 32, Hat = 64 };
        unsigned char m_skinParts;
        enum class MainHand : int { Left, Right } m_mainHand;
        bool m_enableTextFiltering;
        bool m_idkWhyIEvenListItHereButOk__enableServerListing; // useless, server listing is disabled by default for everyone
        enum class ParticleStatus : int { All, Decreased, Minimal } m_particleStatus;
    } m_settingsInfo;
};
struct ZincConnection {
public:
    enum class State : unsigned char {
        Handshake, Status, Login, Transfer, Config, Play
    };
private:
    TCPConnection m_tcpConnection;
    State m_state;
    bool m_isCompressed = false;
    bool m_isEncrypted = false;

    AESWrapper m_encrypt, m_decrypt;

    std::mutex m_mutex;
public:
    ZincConnectionInfo m_info;
    long m_lastKeepAlive, m_lastPing;

    ZincConnection() : m_tcpConnection(TCPConnection()), m_state(State::Handshake) {}
    ZincConnection(const TCPConnection& connection) : m_tcpConnection(connection), m_state(State::Handshake) {}

    TCPConnection& getTCPConnection();
    TCPConnection getTCPConnection() const;
    State& getState();
    State getState() const;
    bool& getIsCompressed();
    bool getIsCompressed() const;
    bool& getIsEncrypted();
    bool getIsEncrypted() const;

    void setState(const State& state);
    void setIsCompressed(const bool& isCompressed);
    void setIsEncrypted(const bool& isEncrypted);

    void setupEncryption(const std::vector<unsigned char>& secret);

    ZincPacket read();
    void send(const ZincPacket& packet);

    bool operator==(const ZincConnection& connection) const;
    bool operator!=(const ZincConnection& connection) const;
};

}