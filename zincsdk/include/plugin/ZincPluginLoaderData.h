#pragma once

#include <vector>

namespace zincsdk {

struct ZincPluginLoaderData {
    void *m_zincPluginMessengerBridge;
    std::vector<char> m_zincPluginId;
};
extern ZincPluginLoaderData g_zincPluginLoaderData;

}