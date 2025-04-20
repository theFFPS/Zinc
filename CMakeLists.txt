cmake_minimum_required(VERSION 3.10)
project(Zinc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*_test\\.cc$")
include_directories(inc)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(GTest REQUIRED)

add_executable(zinc src/main.cc ${SOURCES})
target_include_directories(zinc PRIVATE /usr/include)
target_link_libraries(zinc
    PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    event_core
    CURL::libcurl
)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(zinc PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(ByteBuffer_test src/type/ByteBuffer_test.cc ${SOURCES})
target_include_directories(ByteBuffer_test PRIVATE /usr/include)
target_link_libraries(ByteBuffer_test GTest::gtest GTest::gtest_main event_core)

add_executable(BitSet_test src/type/BitSet_test.cc ${SOURCES})
target_include_directories(BitSet_test PRIVATE /usr/include)
target_link_libraries(BitSet_test GTest::gtest GTest::gtest_main event_core)

enable_testing()
add_test(NAME ByteBuffer_test COMMAND ByteBuffer_test)
add_test(NAME BitSet_test COMMAND BitSet_test)

message(STATUS "Zinc configured with:")
message(STATUS "  - Source files: ${SOURCES}")
message(STATUS "  - OpenSSL version: ${OPENSSL_VERSION}")
message(STATUS "  - LibEvent version: ${LIBEVENT_VERSION}")
