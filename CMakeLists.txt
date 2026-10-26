cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(libobigplanet VERSION 0.3.0)

file(GLOB SRCS src/*.cpp)
file(GLOB INCLUDES include/*.hpp)

add_library(libobigplanet STATIC ${SRCS} ${INCLUDES})
set_target_properties(libobigplanet PROPERTIES PREFIX "")
target_include_directories(libobigplanet 
	PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> 
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "ppc64" AND NOT CMAKE_SYSTEM_PROCESSOR MATCHES "le")
	include(FetchContent)
	FetchContent_Declare(zlib GIT_REPOSITORY https://github.com/madler/zlib.git GIT_TAG v1.3.1)
	set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(zlib)
	add_library(ZLIB::ZLIB ALIAS zlib)
else()
	find_package(ZLIB REQUIRED)
endif()

find_package(nlohmann_json REQUIRED)
find_package(utf8cpp REQUIRED)
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