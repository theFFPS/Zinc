#include <ZincConfig.h>
#include <external/JSON.h>
#include <util/crypto/Base64.h>
#include <fstream>

namespace zinc {

ZincConfig g_zincConfig;

void ZincConfig::save(const std::string& workdir) {
    m_logger.info("Saved configuration at " + workdir + "/zinc.json");

    nlohmann::json JSON = {
        { "security", {
            { "online_mode", m_onlineMode },
            { "prevent_proxy_connections", m_preventProxyConnections },
            { "whitelist", m_whitelist },
            { "max_player_count", m_maxPlayerCount },
            { "backup_interval_minutes", m_backupInterval }
        } },
        { "game", {
            { "allow_pvp", m_allowPVP },
            { "allow_survival_flight", m_allowSurvivalFlight },
            { "view_distance", m_viewDistance }
        } },
        { "network", {
            { "threshold", m_threshold },
            { "server_port", m_serverPort },
            { "zinc_srvctl_port", m_srvctlPort }
        } },
        { "server_list", {
            { "motd", m_motd },
            { "icon_path", m_iconPath }
        } }
    };
    system(("mkdir -p " + workdir).c_str());
    std::ofstream f (workdir + "/zinc.json");
    f << JSON.dump(true);
    f.close();
}
void ZincConfig::load(const std::string& workdir) {
    m_logger.info("Loading configuration from " + workdir + "/zinc.json");
    system(("mkdir -p " + workdir).c_str());
    std::fstream f (workdir + "/zinc.json");
    if (!f.good()) {
        f.close();
        save(workdir);
        return;
    }
    std::string buff, line;
    while (getline(f, line)) buff += line + "\n";
    f.close();
    nlohmann::json JSON = nlohmann::json::parse(buff);

    m_onlineMode = JSON["security"]["online_mode"];
    m_preventProxyConnections = JSON["security"]["prevent_proxy_connections"];
    m_whitelist = JSON["security"]["whitelist"];
    m_maxPlayerCount = JSON["security"]["max_player_count"];
    m_backupInterval = JSON["security"]["backup_interval_minutes"];

    m_allowPVP = JSON["game"]["allow_pvp"];
    m_allowSurvivalFlight = JSON["game"]["allow_survival_flight"];
    m_viewDistance = JSON["game"]["view_distance"];

    m_threshold = JSON["network"]["threshold"];
    m_serverPort = JSON["network"]["server_port"];
    m_srvctlPort = JSON["network"]["zinc_srvctl_port"];

    m_motd = JSON["server_list"]["motd"];
    m_iconPath = JSON["server_list"]["icon_path"];

    f.open(workdir + "/" + m_iconPath, std::ios::binary);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        size_t fileSize = f.tellg();
        f.seekg(0, std::ios::beg);
        if (fileSize) {
            std::vector<unsigned char> buffer (fileSize);
            f.read((char*)buffer.data(), fileSize);
            m_iconBase64 = Base64::encode(buffer);
        }
    }
    f.close();
}

}