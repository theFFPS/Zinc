#pragma once

#include <map>
#include <mutex>
#include <vector>

namespace zinc {

struct ZincMessengerBridge {
private:
    std::map<std::vector<unsigned char>, std::map<std::vector<unsigned char>, std::vector<std::vector<unsigned char>>>> m_targets;
    std::vector<unsigned char> m_serverMessengerId;
    std::mutex m_mutex;
public:
    void init();

    void send(const std::vector<unsigned char>& recvId, const std::vector<unsigned char>& senderId, const std::vector<unsigned char>& data);
    bool canRead(const std::vector<unsigned char>& recvId, const std::vector<unsigned char>& senderId);
    size_t senderAmount(const std::vector<unsigned char>& recvId);
    std::vector<std::vector<unsigned char>> getSenders(const std::vector<unsigned char>& recvId);
    std::vector<std::vector<unsigned char>> read(const std::vector<unsigned char>& recvId, const std::vector<unsigned char>& senderId);

    void registerTarget(const std::vector<unsigned char>& id);
    bool isRegistered(const std::vector<unsigned char>& id);
    void unregisterTarget(const std::vector<unsigned char>& id);

    void sendToServerTarget(const std::vector<unsigned char>& senderId, const std::vector<unsigned char>& data);
    bool canReadFromServerTarget(const std::vector<unsigned char>& recvId);
    std::vector<std::vector<unsigned char>> readFromServerTarget(const std::vector<unsigned char>& recvId);
};
extern ZincMessengerBridge g_zincMessengerBridge;

}