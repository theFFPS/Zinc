#pragma once

#include <string>
#include <vector>
#include <map>

namespace zinc {

struct PluginMetadata {
    std::string m_pluginId;
    std::string m_pluginName;
    std::string m_pluginVersion;
    std::string m_pluginDescription;
    std::vector<std::string> m_pluginAuthors;
    std::string m_pluginEntry;
    struct Version {
        std::string m_versionMin;
        std::string m_versionMax;
    } m_pluginSDKVersion;
    std::vector<std::string> m_pluginPermissions; // SDK permissions
    struct PluginRef {
        std::string m_dependencyId;
        Version m_dependencyVersion;
    };
    std::vector<PluginRef> m_dependencies;
    std::vector<PluginRef> m_conflicts;
    std::vector<PluginRef> m_provides;
    std::map<std::string, std::string> m_entrypoints;
};

}