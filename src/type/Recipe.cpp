#include <type/Recipe.h>
#include <type/ByteBuffer.h>
#include <registry/DefaultRegistries.h>

namespace zinc {

std::vector<char> SlotDisplay::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarNumeric<int>(g_slotDisplayRegistry.m_registryData[m_type.toString()]);
    SlotDisplayData data;
    data.m_itemType = m_itemType;
    data.m_itemStack = m_itemStack;
    data.m_tag = m_tag;
    data.m_children = m_children;
    data.m_customData = m_customData;
    buffer.writeByteArray(g_slotDisplayRegistry.m_writers[m_type.toString()](data));
    return buffer.getBytes();
}
bool SlotDisplay::operator==(const SlotDisplay& slotDisplay) const {
    return slotDisplay.toBytes() == toBytes();
}
bool SlotDisplay::operator!=(const SlotDisplay& slotDisplay) const {
    return !operator==(slotDisplay);
}

std::vector<char> RecipeDisplay::toBytes() const {
    ByteBuffer buffer;
    RecipeDisplayData data;
    data.m_width = m_width;
    data.m_height = m_height;
    data.m_ingredients = m_ingredients;
    data.m_fuel = m_fuel;
    data.m_result = m_result;
    data.m_craftingStation = m_craftingStation;
    data.m_cookingTime = m_cookingTime;
    data.m_experience = m_experience;
    data.m_customData = m_customData;
    buffer.writeVarNumeric<int>(g_recipeDisplayRegistry.m_registryData[m_type.toString()]);
    buffer.writeByteArray(g_recipeDisplayRegistry.m_writers[m_type.toString()](data));
    return buffer.getBytes();
}
bool RecipeDisplay::operator==(const RecipeDisplay& recipeDisplay) const {
    return toBytes() == recipeDisplay.toBytes();
}
bool RecipeDisplay::operator!=(const RecipeDisplay& recipeDisplay) const {
    return !operator==(recipeDisplay);
}

}