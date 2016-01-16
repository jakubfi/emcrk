cmake_minimum_required(VERSION 2.8.4)
project(emcrk C)
include(appversion.cmake)
set(DEBUG 1)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_definitions(-Wall -std=c99 -DEMCRK_VERSION="${APP_VERSION}")

if(DEBUG)
	add_definitions(-DDEBUG -g -O0)
else(DEBUG)
	add_definitions(-DNDEBUG -O3)
endif(DEBUG)

configure_file(
	emcrk-config.cmake.in
	${PROJECT_BINARY_DIR}/emcrk-config.cmake
	@ONLY
)

configure_file(
	emcrk-config-version.cmake.in
	${PROJECT_BINARY_DIR}/emcrk-config-version.cmake
	@ONLY
)

install(FILES
	${PROJECT_BINARY_DIR}/emcrk-config.cmake
	${PROJECT_BINARY_DIR}/emcrk-config-version.cmake
	DESTINATION lib/emcrk
	COMPONENT dev
)

add_subdirectory(src)

# vim: tabstop=4
