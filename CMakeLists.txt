cmake_minimum_required(VERSION 3.0)
project(emcrk
	DESCRIPTION "CROOK-5 data structures and support tools"
	LANGUAGES C
)

include(GNUInstallDirs)
include(cmake/appversion.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_compile_options(-Wall)

# ---- Target: emcrk -----------------------------------------------------

set(EMCRK_PUBLIC_HEADERS
	include/r40.h
	include/process.h
	include/kfind.h
	include/cfg.h
	include/obj.h
	include/exl.h
)

add_library(emcrk SHARED
	src/r40.c
	src/cfg.c
	src/kfind.c
	src/process.c
	src/exl.c
	src/obj.c
	${EMCRK_PUBLIC_HEADERS}
)

if(WIN32)
	target_link_libraries(emcrk ws2_32)
endif(WIN32)

set_target_properties(emcrk PROPERTIES
	C_STANDARD 99
	OUTPUT_NAME "emcrk"
	SOVERSION ${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}
	PUBLIC_HEADER "${EMCRK_PUBLIC_HEADERS}"
)

target_compile_definitions(emcrk PRIVATE EMCRK_VERSION="${APP_VERSION}")

install(TARGETS emcrk
	EXPORT emcrkTarget
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/emcrk
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/emcrk-config-version.cmake"
	VERSION ${APP_VERSION}
	COMPATIBILITY AnyNewerVersion
)

export(
	EXPORT emcrkTarget
	FILE "${CMAKE_CURRENT_BINARY_DIR}/emcrk-config-targets.cmake"
)

set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake/emcrk)
configure_package_config_file(cmake/emcrk-config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/emcrk-config.cmake"
	INSTALL_DESTINATION ${ConfigPackageLocation}
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(EXPORT emcrkTarget
	FILE emcrk-config-targets.cmake
	DESTINATION ${ConfigPackageLocation}
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/emcrk-config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/emcrk-config-version.cmake"
	DESTINATION ${ConfigPackageLocation}
	COMPONENT Devel
)

# ---- Target: crkfind ---------------------------------------------------

add_executable(crkfind src/crkfind.c)
set_property(TARGET crkfind PROPERTY C_STANDARD 99)
target_link_libraries(crkfind emcrk)
install(TARGETS crkfind RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# ---- Target: crkcfg ----------------------------------------------------

add_executable(crkcfg src/crkcfg.c)
set_property(TARGET crkcfg PROPERTY C_STANDARD 99)
target_link_libraries(crkcfg emcrk)
install(TARGETS crkcfg RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# ---- Target: r40 -------------------------------------------------------

add_executable(r40 src/r40tool.c)
set_property(TARGET r40 PROPERTY C_STANDARD 99)
target_link_libraries(r40 emcrk)
install(TARGETS r40 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# vim: tabstop=4
