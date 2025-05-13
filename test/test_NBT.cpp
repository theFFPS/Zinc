#include <gtest/gtest.h>
#include <type/nbt/NBTElement.h>
#include <type/nbt/NBTElementType.h>
#include <type/nbt/NBTSettings.h>
#include <type/ByteBuffer.h>

TEST(NBTTest, EncodeDecodeNBTElement) {
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
    zinc::ByteBuffer buffer;
    element.encode(buffer);
    EXPECT_TRUE(element == zinc::NBTElement(buffer));
}

TEST(NBTTest, LEEncodeDecodeNBTElement) {
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
    zinc::ByteBuffer buffer (false);
    element.encode(buffer);
    EXPECT_TRUE(element == zinc::NBTElement(buffer));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
