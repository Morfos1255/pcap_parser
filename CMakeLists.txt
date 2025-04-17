cmake_minimum_required(VERSION 3.14)
project(pcap_main LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(pcap_parser)
add_subdirectory(simba_decoder)

add_executable(pcap_main main.cpp)

target_link_libraries(pcap_main
        PRIVATE pcap_parser simba_decoder common
)