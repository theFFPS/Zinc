#pragma once

#include <vector>
#include <thread>
#include <mutex>

namespace zinc {

struct ZincServerMessenger {
private:
    std::vector<unsigned char> m_serverMessengerId;
    size_t m_tasks = 0;
    std::mutex m_mutex;
    std::vector<std::thread> m_workers;
public:
    bool m_started = false;

    void init(const std::vector<unsigned char>& id);
    void start();
    void parse(const std::vector<unsigned char>& sender);
    void parseMessage(const std::vector<unsigned char>& sender, const std::vector<unsigned char>& message);
};
extern ZincServerMessenger g_zincServerMessenger;
    
}