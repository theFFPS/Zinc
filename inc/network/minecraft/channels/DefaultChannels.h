#pragma once

#include "../ZincPacket.h"
#include "../ZincConnection.h"

namespace zinc {

ZincPacket BrandChannel(ByteBuffer& buffer, ZincConnection* connection);

}