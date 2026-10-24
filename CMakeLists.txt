cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(libobigplanet VERSION 0.3.0)

file(GLOB SRCS src/*.cpp)
file(GLOB INCLUDES include/*.hpp)

add_library(libobigplanet STATIC ${SRCS})
set_target_properties(libobigplanet PROPERTIES PREFIX "")
target_include_directories(libobigplanet 
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(nlohmann_json REQUIRED)
find_package(utf8cpp REQUIRED)
find_package(ZLIB REQUIRED)
target_link_libraries(libobigplanet PRIVATE nlohmann_json::nlohmann_json utf8cpp::utf8cpp ZLIB::ZLIB)

if(MSVC)
	add_compile_definitions(UTF_CPP_CPLUSPLUS=202302L)
	add_compile_options(/Zc:__cplusplus)
	target_compile_options(libobigplanet PRIVATE /wd4066)
else()
	target_compile_options(libobigplanet PRIVATE -Wno-multichar)
endif()

enable_testing()
add_subdirectory(test)