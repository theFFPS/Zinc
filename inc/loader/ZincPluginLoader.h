#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace zinc {

struct ZincPluginLoaderData {
    void *m_zincPluginMessengerBridge;
    std::vector<char> m_zincPluginId;
};
struct ZincPluginLoader {
private:
    std::unordered_map<std::string, std::string> m_loadedPlugins;
public:

};
extern ZincPluginLoader g_zincPluginLoader;

}