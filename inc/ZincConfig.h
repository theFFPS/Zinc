#pragma once

#include <string>
#include <util/Logger.h>
#include <type/nbt/NBTElement.h>
#include <type/TextComponent.h>
#include <external/UUID.h>

namespace zinc {

struct BanData {
    bool m_isIpBan;
    std::string m_playerIp;
    std::string m_playerName;
    uuids::uuid m_playerUUID;
    long m_banTime;
    bool m_isTemporaryban;
    long m_time;
    NBTElement m_reason;
    std::string m_banId;
    std::string m_bannedBy;
};
struct ZincConfig {
private:
    Logger m_logger = Logger("ZincConfig");
public:
    struct CoreConfig {
        struct Network {
            int m_threshold = 256;
            int m_serverPort = 25565;
            int m_srvctlPort = 25575;
            std::string m_motd = "A " + LEGACY_COLOR_AQUA + "Zinc" + LEGACY_FORMAT_RESET + " Minecraft Server";
            int m_maxPlayerCount = 20;
            std::string m_iconBase64, m_iconPath = "icon.png";
        } m_network;
        struct Security {
            bool m_onlineMode = true;
            bool m_whitelist = false;
            bool m_allowSurvivalFlight = false;
            bool m_allowPVP = true;
            int m_backupInterval = 1440; // minutes
            int m_rateLimit = 2;
            int m_maxConcurrentAccount = 10;
            std::string m_rbacPath = "config/rbac";
            std::string m_unbanSupport;
        } m_security;
        struct Optimizations {
            int m_viewDistance = 10;
            int m_simulationDistance = 10;
        } m_optimizations;
        struct WorldConfig {
            enum class WorldType : int { Limbo, Template, Normal } m_worldType = WorldType::Normal;
            enum class StorageFormat : int { MCAnvil, HypixelSlime } m_storageFormat = StorageFormat::MCAnvil;
            enum class RedstoneMode : int { Disabled, Vanilla, Optimized } m_redstoneMode = RedstoneMode::Vanilla;
            std::string m_requiredPermission = "group.default";
            std::string m_options;
        };
        struct WorldsConfig {
            std::unordered_map<std::string, WorldConfig> m_registeredWorlds;
            std::string m_defaultWorld = "world";

            std::vector<nlohmann::json> serialize() {
                std::vector<nlohmann::json> result;
                for (const auto& worldPair : m_registeredWorlds) {
                    result.push_back(nlohmann::json{
                        { "world_id", worldPair.first },
                        { "world_config", {
                            { "world_type", (
                                worldPair.second.m_worldType == WorldConfig::WorldType::Limbo ? "limbo" :
                                    (worldPair.second.m_worldType == WorldConfig::WorldType::Template ? "template" : "normal")
                            ) },
                            { "storage_format", (worldPair.second.m_storageFormat == WorldConfig::StorageFormat::MCAnvil ? "anvil" : "slime") },
                            { "redstone_mode", (
                                worldPair.second.m_redstoneMode == WorldConfig::RedstoneMode::Vanilla ? "vanilla" :
                                    (worldPair.second.m_redstoneMode == WorldConfig::RedstoneMode::Disabled ? "disabled" : "optimized")
                            ) },
                            { "required_permission", worldPair.second.m_requiredPermission },
                            { "options", worldPair.second.m_options }
                        }}
                    });
                }
                return result;
            }
        } m_worlds;
        std::vector<std::string> m_features = {
            "voice_chat", "player_models", "redpiler" // non of them is currently implemented. i'll do it later
        };
    } m_core;
    std::vector<BanData> m_bans;
    std::mutex m_banMutex;

    ZincConfig() {}
    ZincConfig(const std::string& workdir) {
        load(workdir);
    }

    void save(const std::string& workdir);
    void load(const std::string& workdir);

    void loadBanList(const std::string& workdir);
    void ban(const std::string& playerName, const TextComponent& reason, const std::string& moderatorId /* UUID + playerName JSON */);
    void ban(const uuids::uuid& playerUUID, const TextComponent& reason, const std::string& moderatorId /* UUID + playerName JSON */);
    void tempBan(const std::string& playerName, const TextComponent& reason, const long& _time, const std::string& moderatorId /* UUID + playerName JSON */);
    void tempBan(const uuids::uuid& playerUUID, const TextComponent& reason, const long& _time, const std::string& moderatorId /* UUID + playerName JSON */);
    void banIp(const std::string& playerIp, const TextComponent& reason, const std::string& moderatorId /* UUID + playerName JSON */);
    void tempBanIp(const std::string& playerIp, const TextComponent& reason, const long& _time, const std::string& moderatorId /* UUID + playerName JSON */);
    void unban(const std::string& playerName);
    void unban(const uuids::uuid& playerUUID);
    void unbanIp(const std::string& playerIp);
    void saveBanList(const std::string& workdir);
};

extern ZincConfig g_zincConfig;

}