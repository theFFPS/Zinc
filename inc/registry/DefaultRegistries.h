#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>
#include <network/minecraft/ZincConnection.h>
#include <type/ByteBuffer.h>

namespace zinc {

template<typename T> struct Registry {
    std::unordered_map<std::string, int> m_registryData;
    std::unordered_map<std::string, std::function<std::vector<char>(const T&)>> m_writers;
    std::unordered_map<std::string, std::function<T(ByteBuffer&)>> m_readers;
    Identifier m_defaultIdentifier = Identifier("minecraft", "default");

    Registry() {}
    Registry(const std::unordered_map<std::string, int>& registryData, 
             const std::unordered_map<std::string, std::function<std::vector<char>(const T&)>>& writers,
             const std::unordered_map<std::string, std::function<T(ByteBuffer&)>>& readers, const Identifier& defaultIdentifier) 
        : m_registryData(registryData), m_writers(writers), m_readers(readers), m_defaultIdentifier(defaultIdentifier) {}

    Identifier getIdentifierFromValue(const int& value) {
        for (const auto& registryValue : m_registryData) if (registryValue.second == value) return Identifier(registryValue.first);
        return m_defaultIdentifier;
    }
    void registerValue(const Identifier& identifier, const std::function<std::vector<char>(const T&)>& writer, 
                       const std::function<T(ByteBuffer&)>& reader, const int& value) {
        m_registryData.emplace(identifier.toString(), value);
        m_writers.emplace(identifier.toString(), writer);
        m_readers.emplace(identifier.toString(), reader);
    }
    void registerValue(const Identifier& identifier, const int& value) {
        m_registryData.emplace(identifier.toString(), value);
    }
};

extern Registry<char> g_fireworkExplosionShapesRegistry;
extern Registry<char> g_dyeColorsRegistry;
extern Registry<ConsumeEffectData> g_consumeEffectRegistry;
extern Registry<RecipeDisplayData> g_recipeDisplayRegistry;
extern Registry<SlotDisplayData> g_slotDisplayRegistry;
extern Registry<char> g_mushroomVariantsRegistry;

extern std::unordered_map<std::string, std::function<void(ByteBuffer&, ZincConnection*)>> g_zincServerPluginChannels;
extern std::unordered_map<std::string, std::function<void(std::optional<std::vector<char>>&, ZincConnection*)>> g_zincCookieResponseParsers;
extern std::unordered_map<ZincConnection::State, std::vector<std::string>> g_zincCookieRequests;
extern std::unordered_map<ZincConnection::State, std::unordered_map<std::string, std::function<ByteBuffer(ZincConnection*)>>> g_zincServerInitPluginChannels;

}