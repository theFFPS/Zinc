#pragma once

#include <map>
#include <string>
#include <vector>
#include <type/ByteBuffer.h>

namespace zinc {

extern std::map<std::string, int> g_fireworkExplosionShapes;
std::string getFireworkExplosionShapeId(const int& shape);

extern std::map<std::string, int> g_dyeColors;
std::string getDyeColorId(const int& shape);

extern std::map<std::string, int> g_consumeEffectTypes;
extern std::map<std::string, std::vector<char>(*)(const ConsumeEffectData&)> g_consumeEffectWriters;
extern std::map<std::string, ConsumeEffectData(*)(ByteBuffer&)> g_consumeEffectReaders;
std::string getConsumeEffectTypeId(const int& type);

extern std::map<std::string, int> g_recipeDisplayTypes;
extern std::map<std::string, std::vector<char>(*)(const RecipeDisplayData&)> g_recipeDisplayWriters;
extern std::map<std::string, RecipeDisplayData(*)(ByteBuffer&)> g_recipeDisplayReaders;
std::string getRecipeDisplayTypeId(const int& type);

extern std::map<std::string, int> g_slotDisplayTypes;
extern std::map<std::string, std::vector<char>(*)(const SlotDisplayData&)> g_slotDisplayWriters;
extern std::map<std::string, SlotDisplayData(*)(ByteBuffer&)> g_slotDisplayReaders;
std::string getSlotDisplayTypeId(const int& type);

}