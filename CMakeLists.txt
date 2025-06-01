cmake_minimum_required(VERSION 3.10)
project(Zinc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*_test\\.cc$")
file(GLOB_RECURSE SDK_SOURCES "zincsdk/src/*.cpp")
list(FILTER SDK_SOURCES EXCLUDE REGEX ".*_test\\.cc$")
include_directories(inc)
include_directories(zincsdk/include)

if(TARGET CURL::libcurl)
    message(STATUS "Using Conan-provided CURL target")
else()
    find_package(CURL REQUIRED)
endif()

if(TARGET zlib-ng::zlib-ng)
    message(STATUS "Using Conan-provided zlib-ng target")
else()
    find_package(zlib-ng REQUIRED)
endif()

if(TARGET OpenSSL::SSL)
    message(STATUS "Using Conan-provided OpenSSL target")
else()
    find_package(OpenSSL REQUIRED)
endif()

if(TARGET GTest::gtest)
    message(STATUS "Using Conan-provided GTest target")
else()
    find_package(GTest REQUIRED)
endif()

if(TARGET libevent::libevent)
    message(STATUS "Using Conan-provided Libevent target")
else()
    find_package(Libevent REQUIRED)
endif()


if(TARGET curlpp::curlpp)
    message(STATUS "Using Conan-provided curlpp target")
else()
    find_package(curlpp REQUIRED)
endif()

if(TARGET zstd::libzstd_static)
    message(STATUS "Using Conan-provided curlpp target")
else()
    find_package(zstd REQUIRED)
endif()

add_library(zinc_static STATIC ${SOURCES})
add_library(zincsdk STATIC ${SDK_SOURCES})
add_library(zincsdk_shared SHARED ${SDK_SOURCES})

add_executable(zinc src/main.cc)
target_link_libraries(zinc PRIVATE zinc_static)
target_link_libraries(zincsdk_shared PRIVATE zinc_static)

add_executable(test_NBT test/test_NBT.cpp)
target_link_libraries(test_NBT PRIVATE zinc_static GTest::gtest)

add_executable(test_ByteBuffer test/test_ByteBuffer.cpp)
target_link_libraries(test_ByteBuffer PRIVATE zinc_static GTest::gtest)

add_executable(test_Base64 test/test_Base64.cpp)
target_link_libraries(test_Base64 PRIVATE zinc_static GTest::gtest)

add_executable(test_RSA test/test_RSA.cpp)
target_link_libraries(test_RSA PRIVATE zinc_static GTest::gtest)

add_executable(test_ZLibUtil test/test_ZLibUtil.cpp)
target_link_libraries(test_ZLibUtil PRIVATE zinc_static GTest::gtest)

add_executable(test_AES test/test_AES.cpp)
target_link_libraries(test_AES PRIVATE zinc_static GTest::gtest)

add_executable(test_ZSTDUtil test/test_ZSTDUtil.cpp)
target_link_libraries(test_ZSTDUtil PRIVATE zinc_static GTest::gtest)

target_link_libraries(zinc_static PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto libevent::libevent zlib-ng::zlib-ng curlpp::curlpp zstd::libzstd_static)
target_link_libraries(zincsdk PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto libevent::libevent zlib-ng::zlib-ng curlpp::curlpp zstd::libzstd_static)
target_link_libraries(zincsdk_shared PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto libevent::libevent zlib-ng::zlib-ng curlpp::curlpp zstd::libzstd_static)
target_link_libraries(zinc PRIVATE CURL::libcurl OpenSSL::SSL OpenSSL::Crypto libevent::libevent zlib-ng::zlib-ng curlpp::curlpp zstd::libzstd_static)

target_compile_options(zinc_static PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -O5 -march=native -fPIC -g -fstack-protector-strong)
target_compile_options(zincsdk PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -O5 -march=native -fPIC -g -fstack-protector-strong)
target_compile_options(zincsdk_shared PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wpedantic -O5 -march=native -fPIC -g -fstack-protector-strong)
target_compile_options(zinc PRIVATE -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -O5 -march=native -fPIC -g -fstack-protector-strong)

enable_testing()
add_test(NAME ZLibUtilTest COMMAND test_ZLibUtil)
add_test(NAME ZSTDUtilTest COMMAND test_ZSTDUtil)
add_test(NAME RSATest COMMAND test_RSA)
add_test(NAME ByteBufferTest COMMAND test_ByteBuffer)
add_test(NAME Base64Test COMMAND test_Base64)
add_test(NAME NBTTest COMMAND test_NBT)
add_test(NAME AESTest COMMAND test_AES)