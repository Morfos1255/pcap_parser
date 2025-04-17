project(simba_decoder LANGUAGES CXX)

file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(simba_decoder STATIC ${LIB_SOURCES})

target_include_directories(simba_decoder
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(simba_decoder
        PRIVATE common
)