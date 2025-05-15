#pragma once

#include <string>
#include <util/Logger.h>
#include <type/TextComponent.h>

namespace zinc {

struct ZincConfig {
private:
    Logger m_logger = Logger("ZincConfig");
public:
    int m_threshold = 256;
    int m_maxPlayerCount = 20;
    std::string m_motd = "A " + LEGACY_COLOR_AQUA + "Zinc" + LEGACY_FORMAT_RESET + " Minecraft Server";
    std::string m_iconBase64, m_iconPath = "icon.png";
    bool m_onlineMode = true;
    bool m_preventProxyConnections = true;
    bool m_whitelist = false;
    bool m_allowSurvivalFlight = false;
    bool m_allowPVP = true;
    int m_backupInterval = 1440;
    int m_viewDistance = 10;
    int m_serverPort = 25565;
    int m_srvctlPort = 25575;

    ZincConfig() {}
    ZincConfig(const std::string& workdir) {
        load(workdir);
    }

    void save(const std::string& workdir);
    void load(const std::string& workdir);
};

extern ZincConfig g_zincConfig;

}