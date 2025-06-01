#include <loader/ZincMessengerBridge.h>
#include <loader/ZincServerMessenger.h>
#include <pthread.h>
#include <util/crypto/Random.h>

namespace zinc {

ZincMessengerBridge g_zincMessengerBridge;

void ZincMessengerBridge::init() {
    if (m_serverMessengerId.empty()) m_serverMessengerId = RandomUtil::randomBytes(32);
    g_zincServerMessenger.init(m_serverMessengerId);
}

void ZincMessengerBridge::send(const std::vector<unsigned char>& recvId, const std::vector<unsigned char>& senderId, 
                                const std::vector<unsigned char>& data) {
    std::lock_guard lock(m_mutex);
    if (isRegistered(recvId)) {
        if (m_targets[recvId].contains(senderId)) m_targets[recvId][senderId].push_back(data);
        else m_targets[recvId].insert({ senderId, { data } });
    }
}
bool ZincMessengerBridge::canRead(const std::vector<unsigned char>& recvId, const std::vector<unsigned char>& senderId) {
    if (senderAmount(recvId)) return m_targets[recvId].contains(senderId);
    return false;
}
size_t ZincMessengerBridge::senderAmount(const std::vector<unsigned char>& recvId) {
    if (isRegistered(recvId)) return m_targets[recvId].size();
    return 0;
}
std::vector<std::vector<unsigned char>> ZincMessengerBridge::getSenders(const std::vector<unsigned char>& recvId) {
    if (senderAmount(recvId)) {
        std::vector<std::vector<unsigned char>> senders;
        for (const auto& _pair : m_targets[recvId]) senders.push_back(_pair.first);
        return senders;
    }
    return {};
}
std::vector<std::vector<unsigned char>> ZincMessengerBridge::read(const std::vector<unsigned char>& recvId, const std::vector<unsigned char>& senderId) {
    std::lock_guard lock(m_mutex);
    if (canRead(recvId, senderId)) {
        std::vector<std::vector<unsigned char>> messages = m_targets[recvId][senderId];
        m_targets[recvId][senderId].clear();
        return messages;
    }
    return {};
}

void ZincMessengerBridge::registerTarget(const std::vector<unsigned char>& id) {
    std::lock_guard lock(m_mutex);
    if (!isRegistered(id)) m_targets.insert({ id, {} });
}
bool ZincMessengerBridge::isRegistered(const std::vector<unsigned char>& id) {
    return m_targets.contains(id);
}
void ZincMessengerBridge::unregisterTarget(const std::vector<unsigned char>& id) {
    std::lock_guard lock(m_mutex);
    if (isRegistered(id)) m_targets.erase(id);
}

void ZincMessengerBridge::sendToServerTarget(const std::vector<unsigned char>& senderId, const std::vector<unsigned char>& data) {
    send(m_serverMessengerId, senderId, data);
}
bool ZincMessengerBridge::canReadFromServerTarget(const std::vector<unsigned char>& recvId) {
    return canRead(recvId, m_serverMessengerId);
}
std::vector<std::vector<unsigned char>> ZincMessengerBridge::readFromServerTarget(const std::vector<unsigned char>& recvId) {
    return read(recvId, m_serverMessengerId);
}

}