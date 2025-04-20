cmake_minimum_required(VERSION 3.10)
project(Zinc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*_test\\.cc$")
include_directories(inc)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(GTest REQUIRED)

add_executable(zinc src/main.cc ${SOURCES})
target_link_libraries(zinc PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto event_core)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(zinc PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(ByteBuffer_test src/type/ByteBuffer_test.cc ${SOURCES})
target_link_libraries(ByteBuffer_test GTest::gtest GTest::gtest_main event_core)

add_executable(BitSet_test src/type/BitSet_test.cc ${SOURCES})
target_link_libraries(BitSet_test GTest::gtest GTest::gtest_main event_core)

enable_testing()
add_test(NAME ByteBuffer_test COMMAND ByteBuffer_test)
add_test(NAME BitSet_test COMMAND BitSet_test)
