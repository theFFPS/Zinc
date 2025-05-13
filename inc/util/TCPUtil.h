#pragma once

#include <type/ByteBuffer.h>
#include <event2/bufferevent.h>

namespace zinc {

struct TCPUtil {
    static size_t read(bufferevent* bev, ByteBuffer& buffer);
    static ByteBuffer read(bufferevent* bev);
    static void send(bufferevent* bev, const ByteBuffer& buffer);
    static void drain(bufferevent* bev, const size_t& length);
};

}