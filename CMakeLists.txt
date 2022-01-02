cmake_minimum_required(VERSION 3.20.5)
project(emcrk
	DESCRIPTION "CROOK-5 data structures and support tools"
	LANGUAGES C
)

include(GNUInstallDirs)
include(appversion.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)

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
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/emcrk
	COMPONENT dev
)

# ---- Target: emcrk-lib -------------------------------------------------

add_library(emcrk-lib SHARED
        src/r40.c
        src/cfg.c
        src/kfind.c
        src/process.c
        src/exl.c
        src/obj.c
)

set_target_properties(emcrk-lib PROPERTIES
        OUTPUT_NAME "emcrk"
        SOVERSION ${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}
)

set_property(TARGET emcrk-lib PROPERTY C_STANDARD 99)
target_compile_options(emcrk-lib PUBLIC -Wall)
target_compile_definitions(emcrk-lib PRIVATE EMCRK_VERSION="${APP_VERSION}")

install(TARGETS emcrk-lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES
        ${CMAKE_SOURCE_DIR}/include/r40.h
        ${CMAKE_SOURCE_DIR}/include/process.h
        ${CMAKE_SOURCE_DIR}/include/kfind.h
        ${CMAKE_SOURCE_DIR}/include/cfg.h
        ${CMAKE_SOURCE_DIR}/include/obj.h
        ${CMAKE_SOURCE_DIR}/include/exl.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/emcrk
)

# ---- Target: crkfind ---------------------------------------------------

add_executable(crkfind
        src/crkfind.c
)

set_property(TARGET crkfind PROPERTY C_STANDARD 99)
target_compile_options(crkfind PUBLIC -Wall)

target_link_libraries(crkfind emcrk-lib)

install(TARGETS crkfind
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- Target: crkcfg ----------------------------------------------------

add_executable(crkcfg
        src/crkcfg.c
)

set_property(TARGET crkcfg PROPERTY C_STANDARD 99)
target_compile_options(crkcfg PUBLIC -Wall)

target_link_libraries(crkcfg emcrk-lib)

install(TARGETS crkcfg
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- Target: r40 -------------------------------------------------------

add_executable(r40
        src/r40tool.c
)

set_property(TARGET r40 PROPERTY C_STANDARD 99)
target_compile_options(r40 PUBLIC -Wall)

target_link_libraries(r40 emcrk-lib)

install(TARGETS r40
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# vim: tabstop=4
