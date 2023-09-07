cmake_minimum_required(VERSION 3.6.0)
project(AddonsLoadHelper VERSION 1.1.0 LANGUAGES C CXX)

add_subdirectory(jsoncpp)

include_directories(jsoncpp/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=GBK -static-libstdc++")

add_executable(AddonsLoadHelper main.cpp)
target_link_libraries(AddonsLoadHelper PUBLIC jsoncpp_static)