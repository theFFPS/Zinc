#include <gtest/gtest.h>
#include <type/ByteBuffer.h>

TEST(ByteBufferTest, WriteReadNumeric) {
    zinc::ByteBuffer buffer;
    
    buffer.writeByte(1);
    buffer.writeUnsignedByte(1);
    buffer.writeNumeric<short>(1);
    buffer.writeNumeric<unsigned short>(1);
    buffer.writeNumeric<int>(1);
    buffer.writeNumeric<unsigned>(1);
    buffer.writeNumeric<long>(1);
    buffer.writeNumeric<unsigned long>(1);
    buffer.writeNumeric<float>(1.3f);
    buffer.writeNumeric<double>(1.6);
    buffer.writeVarInt(20);
    buffer.writeVarLong(40);
    buffer.writeZigZagVarInt(20);
    buffer.writeZigZagVarLong(40);

    EXPECT_TRUE(buffer.readByte() == 1);
    EXPECT_TRUE(buffer.readUnsignedByte() == 1);
    EXPECT_TRUE(buffer.readNumeric<short>() == 1);
    EXPECT_TRUE(buffer.readNumeric<unsigned short>() == 1);
    EXPECT_TRUE(buffer.readNumeric<int>() == 1);
    EXPECT_TRUE(buffer.readNumeric<unsigned>() == 1);
    EXPECT_TRUE(buffer.readNumeric<long>() == 1);
    EXPECT_TRUE(buffer.readNumeric<unsigned long>() == 1);
    EXPECT_TRUE(buffer.readNumeric<float>() == 1.3f);
    EXPECT_TRUE(buffer.readNumeric<double>() == 1.6);
    EXPECT_TRUE(buffer.readVarInt() == 20);
    EXPECT_TRUE(buffer.readVarLong() == 40);
    EXPECT_TRUE(buffer.readZigZagVarInt() == 20);
    EXPECT_TRUE(buffer.readZigZagVarLong() == 40);
}
TEST(ByteBufferTest, WriteReadLENumeric) {
    zinc::ByteBuffer buffer;
    buffer.setIsBigEndian(false);
    
    buffer.writeByte(1);
    buffer.writeUnsignedByte(1);
    buffer.writeNumeric<short>(1);
    buffer.writeNumeric<unsigned short>(1);
    buffer.writeNumeric<int>(1);
    buffer.writeNumeric<unsigned>(1);
    buffer.writeNumeric<long>(1);
    buffer.writeNumeric<unsigned long>(1);
    buffer.writeNumeric<float>(1.3f);
    buffer.writeNumeric<double>(1.6);
    buffer.writeVarInt(20);
    buffer.writeVarLong(40);
    buffer.writeZigZagVarInt(20);
    buffer.writeZigZagVarLong(40);

    EXPECT_TRUE(buffer.readByte() == 1);
    EXPECT_TRUE(buffer.readUnsignedByte() == 1);
    EXPECT_TRUE(buffer.readNumeric<short>() == 1);
    EXPECT_TRUE(buffer.readNumeric<unsigned short>() == 1);
    EXPECT_TRUE(buffer.readNumeric<int>() == 1);
    EXPECT_TRUE(buffer.readNumeric<unsigned>() == 1);
    EXPECT_TRUE(buffer.readNumeric<long>() == 1);
    EXPECT_TRUE(buffer.readNumeric<unsigned long>() == 1);
    EXPECT_TRUE(buffer.readNumeric<float>() == 1.3f);
    EXPECT_TRUE(buffer.readNumeric<double>() == 1.6);
    EXPECT_TRUE(buffer.readVarInt() == 20);
    EXPECT_TRUE(buffer.readVarLong() == 40);
    EXPECT_TRUE(buffer.readZigZagVarInt() == 20);
    EXPECT_TRUE(buffer.readZigZagVarLong() == 40);
}
TEST(ByteBufferTest, WriteReadStrings) {
    zinc::ByteBuffer buffer;

    buffer.writeString("Hello World!");
    zinc::Identifier identifier ("zinc", "test");
    buffer.writeIdentifier(identifier);

    EXPECT_TRUE(buffer.readString() == "Hello World!");
    EXPECT_TRUE(buffer.readIdentifier() == identifier);
}
TEST(ByteBufferTest, WriteReadGameTypes) {
    zinc::ByteBuffer buffer;

    zinc::Vector3i position(298, 16, -1922);
    buffer.writePosition(position);
    buffer.writeAngle(0.125f);
    uuids::uuid UUID = uuids::uuid_name_generator(uuids::uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value())("40fpsss");
    buffer.writeUUID(UUID);
    buffer.writePrefixedOptional<std::string>("Hi!", &zinc::ByteBuffer::writeString);
    buffer.writePrefixedOptional<std::string>(std::optional<std::string>(), &zinc::ByteBuffer::writeString);
    buffer.writePrefixedArray<int>({1,2,3}, &zinc::ByteBuffer::writeVarInt);
    buffer.writePrefixedByteArray({1,2,3,4,5});
    buffer.writeIDorX<std::string>(zinc::IDorX<std::string>("Hello"), &zinc::ByteBuffer::writeString);
    buffer.writeIDorX<std::string>(zinc::IDorX<std::string>(123), &zinc::ByteBuffer::writeString);
    buffer.writeIDSet(zinc::IDSet(zinc::Identifier("zinc", "test")));
    buffer.writeIDSet(zinc::IDSet({1,2,3,4,5,6}));
    zinc::SoundEvent event (zinc::Identifier("zinc", "test"), 1000);
    buffer.writeSoundEvent(event);
    zinc::BitSet bitSet;
    bitSet.set(100);
    buffer.writeBitSet(bitSet);
    buffer.writeFixedBitSet(bitSet);
    enum class EnumT { Enum1, Enum2, Enum3 };
    std::vector<EnumT> enumSet = { EnumT::Enum1, EnumT::Enum3 };
    buffer.writeEnumSet<EnumT>(enumSet);
    zinc::TeleportFlags flags;
    flags.setRelPitch(true);
    buffer.writeTeleportFlags(flags);
    zinc::NBTElement element = zinc::NBTElement::Compound({
        zinc::NBTElement::Compound("description", {
            zinc::NBTElement::String("text", (const std::string&)"Hello World!")
        }),
        zinc::NBTElement::List("array", {
            zinc::NBTElement::Int(1),
            zinc::NBTElement::Int(2)
        }),
        zinc::NBTElement::List("earray", {}),
        zinc::NBTElement::IntArray("ints", { 1, 2, 3 }),
        zinc::NBTElement::LongArray("longs", { 1, 2, 3 }),
        zinc::NBTElement::ByteArray("bytes", { 1, 2, 3 }),
        zinc::NBTElement::IntArray("eints", {}),
        zinc::NBTElement::LongArray("elongs", {}),
        zinc::NBTElement::ByteArray("ebytes", {}),
        zinc::NBTElement::Int("int", 1),
        zinc::NBTElement::Long("long", 2),
        zinc::NBTElement::Float("float", 1.1f),
        zinc::NBTElement::Double("double", 2.2f),
        zinc::NBTElement::Byte("byte", 73),
        zinc::NBTElement::Short("short", 12)
    });
    buffer.writeNBTElement(element);

    EXPECT_TRUE(buffer.readPosition() == position);
    EXPECT_TRUE(buffer.readAngle() == 0.125f);
    EXPECT_TRUE(buffer.readUUID() == UUID);
    EXPECT_TRUE(buffer.readPrefixedOptional<std::string>(&zinc::ByteBuffer::readString).value_or("") == "Hi!");
    EXPECT_TRUE(!buffer.readPrefixedOptional<std::string>(&zinc::ByteBuffer::readString).has_value());
    EXPECT_TRUE(buffer.readPrefixedArray<int>(&zinc::ByteBuffer::readVarInt) == std::vector<int>({1,2,3}));
    EXPECT_TRUE(buffer.readPrefixedByteArray() == std::vector<char>({1,2,3,4,5}));
    EXPECT_TRUE(buffer.readIDorX<std::string>(&zinc::ByteBuffer::readString).getValue() == "Hello");
    EXPECT_TRUE(buffer.readIDorX<std::string>(&zinc::ByteBuffer::readString).getId() == 123);
    EXPECT_TRUE(buffer.readIDSet().getIdentifier() == zinc::Identifier("zinc", "test"));
    EXPECT_TRUE(buffer.readIDSet().getIDs() == std::vector<int>({1,2,3,4,5,6}));
    EXPECT_TRUE(buffer.readSoundEvent() == event);
    EXPECT_TRUE(buffer.readBitSet() == bitSet);
    EXPECT_TRUE(buffer.readFixedBitSet(bitSet.toByteArray().size()) == bitSet);
    EXPECT_TRUE(buffer.readEnumSet<EnumT>(1) == enumSet);
    EXPECT_TRUE(buffer.readTeleportFlags() == flags);
    EXPECT_TRUE(buffer.readNBTElement() == element);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
