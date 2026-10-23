cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(libobigplanet)

file(GLOB SRCS src/*.cpp)
file(GLOB INCLUDES include/*.hpp)

add_library(libobigplanet STATIC ${SRCS})
set_target_properties(libobigplanet PROPERTIES PREFIX "")
target_include_directories(libobigplanet PUBLIC include)

find_package(nlohmann_json REQUIRED)
target_link_libraries(libobigplanet PRIVATE nlohmann_json::nlohmann_json)

find_package(utf8cpp REQUIRED)
target_link_libraries(libobigplanet PRIVATE utf8cpp::utf8cpp)

find_package(ZLIB REQUIRED)
target_link_libraries(libobigplanet PRIVATE ZLIB::ZLIB)

include(GNUInstallDirs)

install(TARGETS libobigplanet DESTINATION lib/libobigplanet)
install(FILES ${INCLUDES} DESTINATION include/libobigplanet)

enable_testing()
add_subdirectory(test)