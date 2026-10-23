cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(libobigplanet)

file(GLOB SRCS src/*.cpp)
file(GLOB INCLUDES include/*.hpp)

add_library(libobigplanet STATIC ${SRCS})
set_target_properties(libobigplanet PROPERTIES PREFIX "")
target_include_directories(libobigplanet PUBLIC include)
target_include_directories(libobigplanet PRIVATE third_party)

find_package(ZLIB REQUIRED)
target_link_libraries(libobigplanet PRIVATE ZLIB::ZLIB)

include(GNUInstallDirs)

install(TARGETS libobigplanet DESTINATION lib/libobigplanet)
install(FILES ${INCLUDES} DESTINATION include/libobigplanet)

enable_testing()
add_subdirectory(test)