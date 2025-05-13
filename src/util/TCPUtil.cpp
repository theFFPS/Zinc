#include <util/TCPUtil.h>
#include <event2/buffer.h>

namespace zinc {

size_t TCPUtil::read(bufferevent* bev, ByteBuffer& buffer) {
    evbuffer* input = bufferevent_get_input(bev);
    size_t length = evbuffer_get_length(input);
    std::vector<char> data (length);
    evbuffer_copyout(input, data.data(), length);
    buffer.writeBytes(data);
    return length;
}
ByteBuffer TCPUtil::read(bufferevent* bev) {
    ByteBuffer buffer;
    read(bev, buffer);
    return buffer;
}
void TCPUtil::send(bufferevent* bev, const ByteBuffer& buffer) {
    bufferevent_write(bev, buffer.data(), buffer.size());
}
void TCPUtil::drain(bufferevent* bev, const size_t& length) {
    evbuffer* input = bufferevent_get_input(bev);
    evbuffer_drain(input, length);
}

}