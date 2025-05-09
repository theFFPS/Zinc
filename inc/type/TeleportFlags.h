#pragma once

#include "BitSet.h"

namespace zinc {

struct TeleportFlags {
private:
    bool m_relX, m_relY, m_relZ, m_relYaw, m_relPitch;
    bool m_relVelocityX, m_relVelocityY, m_relVelocityZ, m_rotVelocity;
public:
    TeleportFlags() : m_relX(false), m_relY(false), m_relZ(false), m_relYaw(false), m_relPitch(false), 
                      m_relVelocityX(false), m_relVelocityY(false), m_relVelocityZ(false), m_rotVelocity(false) {}
    TeleportFlags(const int& bitsetInt) : m_relX(bitsetInt & 1), m_relY(bitsetInt & 2), m_relZ(bitsetInt & 4), m_relYaw(bitsetInt & 8), m_relPitch(bitsetInt & 16),
                                    m_relVelocityX(bitsetInt & 32), m_relVelocityY(bitsetInt & 64), m_relVelocityZ(bitsetInt & 128), m_rotVelocity(bitsetInt & 256){}
    TeleportFlags(const BitSet& bitset) : m_relX(bitset.get(0)), m_relY(bitset.get(1)), m_relZ(bitset.get(2)), m_relYaw(bitset.get(3)), m_relPitch(bitset.get(4)),
                                    m_relVelocityX(bitset.get(5)), m_relVelocityY(bitset.get(6)), m_relVelocityZ(bitset.get(7)), m_rotVelocity(bitset.get(8)){}
    
    void setRelX(const bool& relX);
    void setRelY(const bool& relY);
    void setRelZ(const bool& relZ);
    void setRelYaw(const bool& relYaw);
    void setRelPitch(const bool& relPitch);
    void setRelVelocityX(const bool& relVelocityX);
    void setRelVelocityY(const bool& relVelocityY);
    void setRelVelocityZ(const bool& relVelocityZ);
    void setRotVelocity(const bool& rotVelocity);

    void flipRelX();
    void flipRelY();
    void flipRelZ();
    void flipRelYaw();
    void flipRelPitch();
    void flipRelVelocityX();
    void flipRelVelocityY();
    void flipRelVelocityZ();
    void flipRotVelocity();

    bool& getRelX();
    bool getRelX() const;
    bool& getRelY();
    bool getRelY() const;
    bool& getRelZ();
    bool getRelZ() const;
    bool& getRelYaw();
    bool getRelYaw() const;
    bool& getRelPatch();
    bool getRelPatch() const;
    bool& getRelVelocityX();
    bool getRelVelocityX() const;
    bool& getRelVelocityY();
    bool getRelVelocityY() const;
    bool& getRelVelocityZ();
    bool getRelVelocityZ() const;
    bool& getRotVelocity();
    bool getRotVelocity() const;

    int encode() const;
};

}