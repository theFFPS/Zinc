#include <type/ByteBuffer.h>
#include <gtest/gtest.h>

using namespace zinc;

TEST(ByteBufferTest, WriteReadByte) {
    ByteBuffer buffer;
    char value = 'A';
    buffer.writeByte(value);
    char result = buffer.readByte();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadUnsignedByte) {
    ByteBuffer buffer;
    unsigned char value = 255;
    buffer.writeUnsignedByte(value);
    unsigned char result = buffer.readUnsignedByte();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadShort) {
    ByteBuffer buffer;
    short value = 32767;
    buffer.writeShort(value);
    short result = buffer.readShort();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadUnsignedShort) {
    ByteBuffer buffer;
    unsigned short value = 65535;
    buffer.writeUnsignedShort(value);
    unsigned short result = buffer.readUnsignedShort();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadInt) {
    ByteBuffer buffer;
    int value = 2147483647;
    buffer.writeInt(value);
    int result = buffer.readInt();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadUnsignedInt) {
    ByteBuffer buffer;
    unsigned int value = 4294967295;
    buffer.writeUnsignedInt(value);
    unsigned int result = buffer.readUnsignedInt();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadLong) {
    ByteBuffer buffer;
    long value = 9223372036854775807L;
    buffer.writeLong(value);
    long result = buffer.readLong();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadUnsignedLong) {
    ByteBuffer buffer;
    unsigned long value = 18446744073709551615UL;
    buffer.writeUnsignedLong(value);
    unsigned long result = buffer.readUnsignedLong();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadFloat) {
    ByteBuffer buffer;
    float value = 3.14159f;
    buffer.writeFloat(value);
    float result = buffer.readFloat();
    EXPECT_FLOAT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadDouble) {
    ByteBuffer buffer;
    double value = 3.141592653589793;
    buffer.writeDouble(value);
    double result = buffer.readDouble();
    EXPECT_DOUBLE_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadVarInt) {
    ByteBuffer buffer;
    int value = 2147483647;
    buffer.writeVarInt(value);
    int result = buffer.readVarInt();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadVarLong) {
    ByteBuffer buffer;
    long value = 9223372036854775807L;
    buffer.writeVarLong(value);
    long result = buffer.readVarLong();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadString) {
    ByteBuffer buffer;
    std::string value = "Hello, World!";
    buffer.writeString(value);
    std::string result = buffer.readString();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadJSON) {
    ByteBuffer buffer;
    nlohmann::json value = {{"key", "value"}};
    buffer.writeJSON(value);
    nlohmann::json result = buffer.readJSON();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadPosition) {
    ByteBuffer buffer;
    std::vector<int> value = {1, 2, 3};
    buffer.writePosition(value);
    std::vector<int> result = buffer.readPosition();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadArray) {
    ByteBuffer buffer;
    std::vector<int> value = {1, 2, 3, 4, 5};
    buffer.writeArray(value, &ByteBuffer::writeInt);
    std::vector<int> result = buffer.readArray(value.size(), &ByteBuffer::readInt);
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadPrefixedArray) {
    ByteBuffer buffer;
    std::vector<int> value = {1, 2, 3, 4, 5};
    buffer.writePrefixedArray(value, &ByteBuffer::writeInt);
    std::vector<int> result = buffer.readPrefixedArray(&ByteBuffer::readInt);
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadUUID) {
    ByteBuffer buffer;
    uuids::uuid value = uuids::uuid::from_string("123e4567-e89b-12d3-a456-426655440000").value();
    buffer.writeUUID(value);
    uuids::uuid result = buffer.readUUID();
    EXPECT_EQ(value, result);
}

TEST(ByteBufferTest, WriteReadBitSet) {
    ByteBuffer buffer;
    BitSet value(std::vector<long>{1, 2, 3});
    buffer.writeBitSet(value);
    BitSet result = buffer.readBitSet();
    EXPECT_EQ(result.toLongArray(), value.toLongArray());
}
TEST(ByteBufferTest, WriteReadEnum) {
    ByteBuffer buffer;
    enum TestEnum { VALUE } value = TestEnum::VALUE;
    buffer.writeEnum(value);
    TestEnum result = buffer.readEnum<TestEnum>();
    EXPECT_EQ(result, value);
}
TEST(ByteBufferTest, WriteReadFixedBitSet) {
    ByteBuffer buffer;
    BitSet value(std::vector<long>{1, 2, 3});
    buffer.writeFixedBitSet(value);
    BitSet result = buffer.readFixedBitSet(24);
    EXPECT_EQ(result.toLongArray(), value.toLongArray());
}

TEST(ByteBufferTest, WriteReadOptional) {
    ByteBuffer buffer;
    std::optional<int> value = 123;
    buffer.writeOptional(value, &ByteBuffer::writeInt);
    bool shouldRead = true;
    std::optional<int> result = buffer.readOptional(shouldRead, &ByteBuffer::readInt);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), value.value());
}

TEST(ByteBufferTest, WriteReadPrefixedOptional) {
    ByteBuffer buffer;
    std::optional<int> value = 123;
    buffer.writePrefixedOptional(value, &ByteBuffer::writeInt);
    std::optional<int> result = buffer.readPrefixedOptional(&ByteBuffer::readInt);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), value.value());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
