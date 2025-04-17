project(pcap_parser LANGUAGES CXX)

file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(pcap_parser STATIC ${LIB_SOURCES})

target_include_directories(pcap_parser
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(pcap_parser
        PRIVATE common
)
