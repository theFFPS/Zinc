#pragma once

#include <type/ByteBuffer.h>

namespace zinc {

struct ZincPacket {
private:
    int m_id;
    ByteBuffer m_data;
public: 
    ZincPacket() : m_id(-1), m_data(ByteBuffer()) {}
    ZincPacket(const int& id) : m_id(id), m_data(ByteBuffer()) {}
    ZincPacket(const int& id, const ByteBuffer& data) : m_id(id), m_data(ByteBuffer(data.getBytes(), data.getReadPointer(), data.getIsBigEndian())) {}

    int& getId();
    int getId() const;
    ByteBuffer& getData();
    ByteBuffer getData() const;

    void setId(const int& id);
    void setData(const ByteBuffer& data);

    bool operator==(const ZincPacket& packet) const;
    bool operator!=(const ZincPacket& packet) const;
};


}