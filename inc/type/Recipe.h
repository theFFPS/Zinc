#pragma once

#include "Slot.h"
#include "Identifier.h"

namespace zinc {

struct SlotDisplay {
    Identifier m_type = Identifier("minecraft:empty");
    int m_itemType;
    Slot m_itemStack;
    Identifier m_tag;
    std::vector<SlotDisplay> m_children;
    std::vector<char> m_customData;

    SlotDisplay() {}

    std::vector<char> toBytes() const;

    bool operator==(const SlotDisplay& slotDisplay) const;
    bool operator!=(const SlotDisplay& slotDisplay) const;
};
struct RecipeDisplay {
    Identifier m_type = Identifier("minecraft:crafting_shapeless");
    int m_width;
    int m_height;
    std::vector<SlotDisplay> m_ingredients;
    SlotDisplay m_fuel;
    SlotDisplay m_result;
    SlotDisplay m_craftingStation;
    int m_cookingTime;
    float m_experience;
    std::vector<char> m_customData;

    RecipeDisplay() {}

    std::vector<char> toBytes() const;

    bool operator==(const RecipeDisplay& recipeDisplay) const;
    bool operator!=(const RecipeDisplay& recipeDisplay) const;
};

}