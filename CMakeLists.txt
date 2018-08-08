cmake_minimum_required(VERSION 3.1)

#
# Avoid source tree pollution
#
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CheckCCompilerFlag)
include(CheckCSourceCompiles)

find_package (OpenSSL)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${OpenCV_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${PYTHON_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    )

add_executable(calc_jpeg_md5sum ${SRC_DIR}/calc_jpeg_md5sum.cpp)
TARGET_LINK_LIBRARIES(calc_jpeg_md5sum ${OPENSSL_LIBRARIES})
