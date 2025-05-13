#pragma once

#include "../ZincPacket.h"
#include "../ZincConnection.h"

namespace zinc {

ZincPacket BrandChannel(ByteBuffer& buffer, const ZincConnection::State& state);

}