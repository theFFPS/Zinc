#pragma once

#include <string>
#include <util/Logger.h>
#include <type/TextComponent.h>

namespace zinc {

struct BanData {
    std::string m_playerName;
    uuids::uuid m_playerUUID;
    bool m_isTemporaryban;
    long m_time;
    TextComponent m_reason;
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
        } m_security;
        struct Optimizations {
            int m_viewDistance = 10;
            int m_simulationDistance = 10;
        } m_optimizations;
        struct WorldConfig {
            enum class WorldType : int { Limbo, Template, Normal } m_worldType = WorldType::Normal;
            enum class StorageFormat : int { MCAnvil, Alpha, HypixelSlime } m_storageFormat = StorageFormat::MCAnvil;
            enum class RedstoneMode : int { Disabled, Vanilla, Optimized } m_redstoneMode = RedstoneMode::Vanilla;
            std::string m_requiredPermission = "group.default";
            std::string m_options;
        };
        struct WorldsConfig {
            std::unordered_map<std::string, WorldConfig> m_registeredWorlds;
            std::string m_defaultWorld = "world";
        } m_worlds;
        std::vector<std::string> m_features = {
            "voice_chat", "player_models", "redpiler" // non of them is currently implemented. i'll do it later
        };
    } m_core;
    std::unordered_map<std::string, BanData> m_bannedIps;
    

    ZincConfig() {}
    ZincConfig(const std::string& workdir) {
        load(workdir);
    }

    void save(const std::string& workdir);
    void load(const std::string& workdir);
};

extern ZincConfig g_zincConfig;

}