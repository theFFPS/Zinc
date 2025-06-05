#include "external/UUID.h"
#include "type/ByteBuffer.h"
#include <ZincConfig.h>
#include <external/JSON.h>
#include <ios>
#include <mutex>
#include <util/crypto/Base64.h>
#include <util/crypto/Random.h>
#include <type/ByteBuffer.h>
#include <fstream>
#include <thread>

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
    system(("mkdir -p " + workdir + "/{,worlds,config,trust,backup}").c_str());
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
            { "rbac_path", m_core.m_security.m_rbacPath },
            { "unban_support", m_core.m_security.m_unbanSupport }
        }},
        { "optimizations", {
            { "view_distance", m_core.m_optimizations.m_viewDistance },
            { "simulation_distance", m_core.m_optimizations.m_simulationDistance }
        }},
        { "worlds", {
            { "default_world", m_core.m_worlds.m_defaultWorld },
            { "registered_worlds", m_core.m_worlds.serialize() }
        }},
        { "features", m_core.m_features }
    };
    writeJSONFile(workdir + "/config/zinc.json", coreSettings);
    saveBanList(workdir);
}
void ZincConfig::load(const std::string& workdir) {
    m_logger.info("Loading configuration from " + workdir + "/config");
    system(("mkdir -p " + workdir + "/{,worlds,config,trust,backup}").c_str());
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
            if (coreSettings["security"].contains("unban_support")) m_core.m_security.m_unbanSupport = coreSettings["security"]["unban_support"];
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
            m_core.m_features.clear();
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
            long size = f.tellg();
            f.seekg(0, std::ios::beg);
            std::vector<char> iconBytes (zinc_safe_cast<long, size_t>(size));
            f.read(iconBytes.data(), size);
            m_core.m_network.m_iconBase64 = Base64::encode(iconBytes);
        }
        f.close();
    loadBanList(workdir);
    save(workdir);
}
void ZincConfig::loadBanList(const std::string& workdir) {
    nlohmann::json banlist = readJSONFile(workdir + "/banlist.json");
    m_banMutex.lock();
    if (banlist.contains("ipbans")) for (const auto& JSON : banlist["ipbans"]) {
        BanData data;
        data.m_playerIp = JSON["ip"];
        data.m_isIpBan = true;
        data.m_playerName = JSON["name"];
        const auto& uuid = uuids::uuid::from_string(std::string(JSON["uuid"]));
        if (uuid.has_value()) data.m_playerUUID = uuid.value();
        ByteBuffer buffer;
        buffer.writeArray<unsigned char>(Base64::decode(JSON["reason"]), &ByteBuffer::writeUnsignedByte);
        data.m_reason = buffer.readNBTElement();
        data.m_banTime = JSON["from"];
        data.m_banId = JSON["id"];
        data.m_bannedBy = JSON["by"];
        if (JSON.contains("time")) {
            data.m_isTemporaryban = true;
            data.m_time = JSON["time"];
        }
        m_bans.push_back(data);
    }
    if (banlist.contains("bans")) for (const auto& JSON : banlist["bans"]) {
        BanData data;
        data.m_isIpBan = false;
        data.m_playerName = JSON["name"];
        const auto& uuid = uuids::uuid::from_string(std::string(JSON["uuid"]));
        if (uuid.has_value()) data.m_playerUUID = uuid.value();
        ByteBuffer buffer;
        buffer.writeArray<unsigned char>(Base64::decode(JSON["reason"]), &ByteBuffer::writeUnsignedByte);
        data.m_reason = buffer.readNBTElement();
        data.m_banTime = JSON["from"];
        data.m_banId = JSON["id"];
        data.m_bannedBy = JSON["by"];
        if (JSON.contains("time")) {
            data.m_isTemporaryban = true;
            data.m_time = JSON["time"];
        }
        m_bans.push_back(data);
    }
    m_banMutex.unlock();
}
void ZincConfig::ban(const std::string& playerName, const TextComponent& reason, const std::string& moderatorId) {
    BanData data;
    data.m_isIpBan = false;
    data.m_isTemporaryban = false;
    data.m_playerName = playerName;
    data.m_reason = reason.encode();
    data.m_bannedBy = Base64::encode(moderatorId);
    data.m_banTime = time(nullptr);
    data.m_banId = Base64::encode(RandomUtil::randomBytes(8));
    g_zincConfig.m_banMutex.lock();
    g_zincConfig.m_bans.push_back(data);
    g_zincConfig.m_banMutex.unlock();
}
void ZincConfig::ban(const uuids::uuid& playerUUID, const TextComponent& reason, const std::string& moderatorId) {
    BanData data;
    data.m_isIpBan = false;
    data.m_isTemporaryban = false;
    data.m_playerUUID = playerUUID;
    data.m_reason = reason.encode();
    data.m_bannedBy = Base64::encode(moderatorId);
    data.m_banTime = time(nullptr);
    data.m_banId = Base64::encode(RandomUtil::randomBytes(8));
    g_zincConfig.m_banMutex.lock();
    g_zincConfig.m_bans.push_back(data);
    g_zincConfig.m_banMutex.unlock();
}
void ZincConfig::tempBan(const std::string& playerName, const TextComponent& reason, const long& _time, const std::string& moderatorId) {
    BanData data;
    data.m_isIpBan = false;
    data.m_isTemporaryban = true;
    data.m_playerName = playerName;
    data.m_reason = reason.encode();
    data.m_bannedBy = Base64::encode(moderatorId);
    data.m_banTime = time(nullptr);
    data.m_time = _time;
    data.m_banId = Base64::encode(RandomUtil::randomBytes(8));
    g_zincConfig.m_banMutex.lock();
    g_zincConfig.m_bans.push_back(data);
    g_zincConfig.m_banMutex.unlock();
}
void ZincConfig::tempBan(const uuids::uuid& playerUUID, const TextComponent& reason, const long& _time, const std::string& moderatorId) {
    BanData data;
    data.m_isIpBan = false;
    data.m_isTemporaryban = true;
    data.m_playerUUID = playerUUID;
    data.m_reason = reason.encode();
    data.m_bannedBy = Base64::encode(moderatorId);
    data.m_banTime = time(nullptr);
    data.m_time = _time;
    data.m_banId = Base64::encode(RandomUtil::randomBytes(8));
    g_zincConfig.m_banMutex.lock();
    g_zincConfig.m_bans.push_back(data);
    g_zincConfig.m_banMutex.unlock();
}
void ZincConfig::banIp(const std::string& playerIp, const TextComponent& reason, const std::string& moderatorId) {
    BanData data;
    data.m_isIpBan = true;
    data.m_isTemporaryban = false;
    data.m_playerIp = playerIp;
    data.m_reason = reason.encode();
    data.m_bannedBy = Base64::encode(moderatorId);
    data.m_banTime = time(nullptr);
    data.m_banId = Base64::encode(RandomUtil::randomBytes(8));
    g_zincConfig.m_banMutex.lock();
    g_zincConfig.m_bans.push_back(data);
    g_zincConfig.m_banMutex.unlock();
}
void ZincConfig::tempBanIp(const std::string& playerIp, const TextComponent& reason, const long& _time, const std::string& moderatorId) {
    BanData data;
    data.m_isIpBan = true;
    data.m_isTemporaryban = true;
    data.m_playerIp = playerIp;
    data.m_reason = reason.encode();
    data.m_bannedBy = Base64::encode(moderatorId);
    data.m_banTime = time(nullptr);
    data.m_time = _time;
    data.m_banId = Base64::encode(RandomUtil::randomBytes(8));
    g_zincConfig.m_banMutex.lock();
    g_zincConfig.m_bans.push_back(data);
    g_zincConfig.m_banMutex.unlock();
}
void _unbanName(const std::string& playerName) {
    g_zincConfig.m_banMutex.lock();
    auto _ = std::remove_if(g_zincConfig.m_bans.begin(), g_zincConfig.m_bans.end(),
        [&](const BanData& ban) {
            return ban.m_playerName == playerName;
        }
    );
    g_zincConfig.m_banMutex.unlock();
}
void _unbanUUID(const uuids::uuid& playerUUID) {
    g_zincConfig.m_banMutex.lock();
    auto _ = std::remove_if(g_zincConfig.m_bans.begin(), g_zincConfig.m_bans.end(),
        [&](const BanData& ban) {
            return ban.m_playerUUID == playerUUID;
        }
    );
    g_zincConfig.m_banMutex.unlock();
}
void _unbanIp(const std::string& playerIp) {
    g_zincConfig.m_banMutex.lock();
    auto _ = std::remove_if(g_zincConfig.m_bans.begin(), g_zincConfig.m_bans.end(),
        [&](const BanData& ban) {
            return ban.m_playerIp == playerIp && ban.m_isIpBan;
        }
    );
    g_zincConfig.m_banMutex.unlock();
}
void ZincConfig::unban(const std::string& playerName) {
    std::thread t(_unbanName, playerName);
    t.detach();
}
void ZincConfig::unban(const uuids::uuid& playerUUID) {
    std::thread t(_unbanUUID, playerUUID);
    t.detach();
}
void ZincConfig::unbanIp(const std::string& playerIp) {
    std::thread t(_unbanIp, playerIp);
    t.detach();
}
void ZincConfig::saveBanList(const std::string& workdir) {
    nlohmann::json banlist = {
        { "ipbans", std::vector<char>({}) },
        { "bans", std::vector<char>({}) }
    };
    for (const BanData& banData : m_bans) {
        ByteBuffer buffer;
        buffer.writeNBTElement(banData.m_reason);
        nlohmann::json banValue = {
            { "name", banData.m_playerName },
            { "uuid", uuids::to_string(banData.m_playerUUID) },
            { "reason", Base64::encode(buffer.getBytes()) },
            { "from", banData.m_banTime },
            { "id", banData.m_banId },
            { "by", banData.m_bannedBy }
        };
        if (banData.m_isTemporaryban) banValue["time"] = banData.m_time;
        if (banData.m_isIpBan) {
            banValue["ip"] = banData.m_playerIp;
            banlist["ipbans"].push_back(banValue);
        } else banlist["bans"].push_back(banValue);
    }
    writeJSONFile(workdir + "/banlist.json", banlist);
}

}