#include <network/minecraft/channels/DefaultChannels.h>

namespace zinc {

void BrandChannel(ByteBuffer&, ZincConnection* connection) {
    if (connection->getState() == ZincConnection::State::Login) {
        connection->sendDisconnect(TextComponentBuilder()
            .text("Zinc Login Error").color("dark_red").bold()
            .append(TextComponentBuilder().text("\n").build())
            .append(TextComponentBuilder().text("You can't request server brand during Login").bold(false).color("red").build()).build());
    } else {
        ByteBuffer buffer;
        buffer.writeString("zinc");
        connection->sendPluginMessage(Identifier("minecraft", "brand"), buffer);
    }
}

}