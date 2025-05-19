#include <network/minecraft/channels/DefaultChannels.h>

namespace zinc {

ZincPacket BrandChannel(ByteBuffer&, ZincConnection* connection) {
    ZincPacket packet;
    if (connection->getState() == ZincConnection::State::Login) {
        packet.setId(0);
        packet.getData().writeString(
            "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
            "{\"text\":\"You can't request server brand during Login\",\"color\":\"red\"}]");
    } else {
        packet.setId(1);
        packet.getData().writeIdentifier(Identifier("minecraft", "brand"));
        packet.getData().writeString("zinc");
    }
    return packet;
}

}