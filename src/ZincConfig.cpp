#include <ZincConfig.h>
#include <external/JSON.h>
#include <util/crypto/Base64.h>
#include <util/crypto/Random.h>
#include <fstream>

namespace zinc {

ZincConfig g_zincConfig;

nlohmann::json readJSONFile(const std::string& filepath) {
    std::string buff, line;
    std::fstream f (filepath);
    while (f.good() && getline(f, line)) buff += line + "\n";
    f.close();
    if (buff.empty()) buff = "{}";
    return nlohmann::json::parse(buff);
}
void writeJSONFile(const std::string& filepath, const nlohmann::json& JSON) {
    std::ofstream f (filepath);
    f << JSON.dump(4);
    f.close();
}

void ZincConfig::save(const std::string& workdir) {
    m_logger.info("Saving configuration at " + workdir + "/config");
    system(("mkdir -p " + workdir + "/{,worlds,config,trust}").c_str());
    if (m_core.m_security.m_rbacPath.size()) {
        if (m_core.m_security.m_rbacPath[0] == '/') system(("mkdir -p " + m_core.m_security.m_rbacPath).c_str());
        else system(("mkdir -p " + workdir + "/" + m_core.m_security.m_rbacPath).c_str());
    } else {
        m_core.m_security.m_rbacPath = "config/rbac";
        system(("mkdir -p " + workdir + "/" + m_core.m_security.m_rbacPath).c_str());
    }
    nlohmann::json coreSettings = {
        { "network", {
            { "threshold", m_core.m_network.m_threshold },
            { "server_port", m_core.m_network.m_serverPort },
            { "srvctl_port", m_core.m_network.m_srvctlPort },
            { "motd", m_core.m_network.m_motd },
            { "max_player_count", m_core.m_network.m_maxPlayerCount },
            { "icon_path", m_core.m_network.m_iconPath }
        }},
        { "security", {
            { "online_mode", m_core.m_security.m_onlineMode },
            { "whitelist", m_core.m_security.m_whitelist },
            { "allow_survival_flight", m_core.m_security.m_allowSurvivalFlight },
            { "allow_pvp", m_core.m_security.m_allowPVP },
            { "backup_interval_minutes", m_core.m_security.m_backupInterval },
            { "rate_limit_seconds", m_core.m_security.m_rateLimit },
            { "max_concurrent_account", m_core.m_security.m_maxConcurrentAccount },
            { "rbac_path", m_core.m_security.m_rbacPath }
        }},
        { "optimizations", {
            { "view_distance", m_core.m_optimizations.m_viewDistance },
            { "simulation_distance", m_core.m_optimizations.m_simulationDistance }
        }}
    };
    writeJSONFile(workdir + "/config/zinc.json", coreSettings);
}
void ZincConfig::load(const std::string& workdir) {
    m_logger.info("Loading configuration from " + workdir + "/config");
    system(("mkdir -p " + workdir + "/{,worlds,config,trust}").c_str());
    nlohmann::json coreSettings = readJSONFile(workdir + "/config/zinc.json");
        if (coreSettings.contains("network")) {
            if (coreSettings["network"].contains("threshold")) m_core.m_network.m_threshold = coreSettings["network"]["threshold"];
            if (coreSettings["network"].contains("server_port")) m_core.m_network.m_serverPort = coreSettings["network"]["server_port"];
            if (coreSettings["network"].contains("srvctl_port")) m_core.m_network.m_srvctlPort = coreSettings["network"]["srvctl_port"];
            if (coreSettings["network"].contains("motd")) m_core.m_network.m_motd = coreSettings["network"]["motd"];
            if (coreSettings["network"].contains("max_player_count")) m_core.m_network.m_maxPlayerCount = coreSettings["network"]["max_player_count"];
            if (coreSettings["network"].contains("icon_path")) m_core.m_network.m_iconPath = coreSettings["network"]["icon_path"];
        }
        if (coreSettings.contains("security")) {
            if (coreSettings["security"].contains("online_mode")) m_core.m_security.m_onlineMode = coreSettings["security"]["online_mode"];
            if (coreSettings["security"].contains("whitelist")) m_core.m_security.m_whitelist = coreSettings["security"]["whitelist"];
            if (coreSettings["security"].contains("allow_survival_flight")) 
                m_core.m_security.m_allowSurvivalFlight = coreSettings["security"]["allow_survival_flight"];
            if (coreSettings["security"].contains("allow_pvp")) m_core.m_security.m_allowPVP = coreSettings["security"]["allow_pvp"];
            if (coreSettings["security"].contains("backup_interval_minutes")) 
                m_core.m_security.m_backupInterval = coreSettings["security"]["backup_interval_minutes"];
            if (coreSettings["security"].contains("rate_limit_seconds")) m_core.m_security.m_rateLimit = coreSettings["security"]["rate_limit_seconds"];
            if (coreSettings["security"].contains("max_concurrent_account")) 
                m_core.m_security.m_maxConcurrentAccount = coreSettings["security"]["max_concurrent_account"];
            if (coreSettings["security"].contains("rbac_path")) m_core.m_security.m_rbacPath = coreSettings["security"]["rbac_path"];
        }
        if (coreSettings.contains("optimizations")) {
            if (coreSettings["optimizations"].contains("view_distance")) 
                m_core.m_optimizations.m_viewDistance = coreSettings["optimizations"]["view_distance"];
            if (coreSettings["optimizations"].contains("simulation_distance")) 
                m_core.m_optimizations.m_simulationDistance = coreSettings["optimizations"]["simulation_distance"];
        }
        if (coreSettings.contains("worlds")) {
            if (coreSettings["worlds"].contains("default_world")) 
                m_core.m_worlds.m_defaultWorld = coreSettings["worlds"]["default_world"];
            if (coreSettings["worlds"].contains("registered_worlds")) {
                for (const auto& registeredWorld : coreSettings["worlds"]["registered_worlds"]) {
                    CoreConfig::WorldConfig worldConfig;
                    std::string worldId = "world_" + Base64::encode(RandomUtil::randomBytes(4));
                    if (registeredWorld.contains("world_config")) {
                        if (registeredWorld["world_config"].contains("world_type")) {
                            std::string worldType = registeredWorld["world_config"]["world_type"];
                            if (worldType == "limbo") worldConfig.m_worldType = CoreConfig::WorldConfig::WorldType::Limbo;
                            else if (worldType == "template") worldConfig.m_worldType = CoreConfig::WorldConfig::WorldType::Template;
                            else if (worldType == "normal") worldConfig.m_worldType = CoreConfig::WorldConfig::WorldType::Normal;
                        }
                        if (registeredWorld["world_config"].contains("storage_format")) {
                            std::string storageFormat = registeredWorld["world_config"]["storage_format"];
                            if (storageFormat == "anvil") worldConfig.m_storageFormat = CoreConfig::WorldConfig::StorageFormat::MCAnvil;
                            else if (storageFormat == "alpha") worldConfig.m_storageFormat = CoreConfig::WorldConfig::StorageFormat::Alpha;
                            else if (storageFormat == "slime") worldConfig.m_storageFormat = CoreConfig::WorldConfig::StorageFormat::HypixelSlime;
                        }
                        if (registeredWorld["world_config"].contains("redstone_mode")) {
                            std::string redstoneMode = registeredWorld["world_config"]["redstone_mode"];
                            if (redstoneMode == "disabled") worldConfig.m_redstoneMode = CoreConfig::WorldConfig::RedstoneMode::Disabled;
                            else if (redstoneMode == "vanilla") worldConfig.m_redstoneMode = CoreConfig::WorldConfig::RedstoneMode::Vanilla;
                            else if (redstoneMode == "optimized") worldConfig.m_redstoneMode = CoreConfig::WorldConfig::RedstoneMode::Optimized;
                        }
                        if (registeredWorld["world_config"].contains("required_permission")) 
                            worldConfig.m_requiredPermission = registeredWorld["world_config"]["required_permission"];
                        if (registeredWorld["world_config"].contains("options")) 
                            worldConfig.m_options = registeredWorld["world_config"]["options"];
                    }
                    if (registeredWorld.contains("world_id")) worldId = registeredWorld["world_id"];
                    m_core.m_worlds.m_registeredWorlds.insert({ worldId, worldConfig });
                }
            }
        }
        if (coreSettings.contains("features")) {
            for (const auto& feature : coreSettings["features"]) {
                m_core.m_features.push_back(feature);
            }
        }
    if (m_core.m_security.m_rbacPath.size()) {
        if (m_core.m_security.m_rbacPath[0] == '/') system(("mkdir -p " + m_core.m_security.m_rbacPath).c_str());
        else system(("mkdir -p " + workdir + "/" + m_core.m_security.m_rbacPath).c_str());
    } else {
        m_core.m_security.m_rbacPath = "config/rbac";
        system(("mkdir -p " + workdir + "/" + m_core.m_security.m_rbacPath).c_str());
    }
    std::fstream f (workdir + "/config/" + m_core.m_network.m_iconPath, std::ios::binary);
        if (f.good()) {
            f.seekg(0, std::ios::end);
            size_t size = f.tellg();
            f.seekg(0, std::ios::beg);
            std::vector<char> iconBytes (size);
            f.read(iconBytes.data(), size);
            m_core.m_network.m_iconBase64 = Base64::encode(iconBytes);
        }
        f.close();
    save(workdir);
}

}