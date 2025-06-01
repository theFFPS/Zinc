#include "type/ByteBuffer.h"
#include <loader/ZincMessengerBridge.h>
#include <loader/ZincServerMessenger.h>
#include <chrono>
#include <thread>

namespace zinc {

ZincServerMessenger g_zincServerMessenger;

void ZincServerMessenger::init(const std::vector<unsigned char>& id) {
    if (m_serverMessengerId.empty()) m_serverMessengerId = id;
}
void ZincServerMessenger::start() {
    m_started = true;
    while (m_started) {
        std::vector<std::vector<unsigned char>> senders = g_zincMessengerBridge.getSenders(m_serverMessengerId);
        for (const std::vector<unsigned char>& sender : senders) parse(sender);
        while (m_tasks) {}
        m_workers.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void _parseMessage(const std::vector<unsigned char>& sender, const std::vector<unsigned char>& message) {
    g_zincServerMessenger.parseMessage(sender, message);
}
void ZincServerMessenger::parse(const std::vector<unsigned char>& sender) {
    std::vector<std::vector<unsigned char>> messages = g_zincMessengerBridge.read(m_serverMessengerId, sender);
    m_mutex.lock();
    m_tasks += messages.size();
    m_mutex.unlock();
    m_workers = std::vector<std::thread>(m_tasks);
    for (const std::vector<unsigned char>& message : messages) m_workers.push_back(std::thread(_parseMessage, sender, message));
}
void ZincServerMessenger::parseMessage(const std::vector<unsigned char>& sender, const std::vector<unsigned char>& message) {
    ByteBuffer buffer;
    ByteBuffer replyBuffer;
    buffer.writeArray<unsigned char>(message, &ByteBuffer::writeUnsignedByte);
    bool hasCallbackId = buffer.readByte();
    std::vector<unsigned char> callbackId;
    if (hasCallbackId) callbackId = buffer.readPrefixedArray(&ByteBuffer::readUnsignedByte);
    std::string type = buffer.readString();
    replyBuffer.writeByte(zinc_safe_cast<size_t, char>(callbackId.size()));
    if (callbackId.size()) replyBuffer.writePrefixedArray<unsigned char>(callbackId, &ByteBuffer::writeUnsignedByte);
    if (type == "value") {
        std::string valueType = buffer.readString();
    } else if (type == "call") {
        std::string operation = buffer.readString();
        if (operation == "base.test") {
            replyBuffer.writeString("value");
            replyBuffer.writeString("bool");
            replyBuffer.writeByte(true);
        }
    } else {
        replyBuffer.writeString("value");
        replyBuffer.writeString("error");
        replyBuffer.writeString("wrong_packet");
        replyBuffer.writeString("Unknown SDK packet type");
    }
    if (replyBuffer.size()) g_zincMessengerBridge.send(sender, m_serverMessengerId, 
                                                        replyBuffer.readArray<unsigned char>(&ByteBuffer::readUnsignedByte, 
                                                        replyBuffer.size()));
    m_mutex.lock();
    m_tasks--;
    m_mutex.unlock();
}

}