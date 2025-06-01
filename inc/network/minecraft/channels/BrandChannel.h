#pragma once

#include "Channel.h"
#include "../ZincConnection.h"
#include <type/ByteBuffer.h>

namespace zinc {

ZINC_PLUGIN_CHANNEL(BrandChannel, buffer, connection);

}