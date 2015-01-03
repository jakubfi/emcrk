add_library(emcrk-lib SHARED
	r40.c
	conf.c
	kfind.c
	names.c
	process.c
	exl.c
)

set_target_properties(emcrk-lib PROPERTIES
	OUTPUT_NAME "emcrk"
	SOVERSION ${EMCRK_VERSION_MAJOR}.${EMCRK_VERSION_MINOR}
	PUBLIC_HEADER "${CMAKE_SOURCE_DIR}/include/emcrk.h"
)

install(TARGETS emcrk-lib
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)

install(FILES
	${CMAKE_SOURCE_DIR}/include/emcrk/r40.h
	${CMAKE_SOURCE_DIR}/include/emcrk/process.h
	${CMAKE_SOURCE_DIR}/include/emcrk/kfind.h
	${CMAKE_SOURCE_DIR}/include/emcrk/conf.h
	DESTINATION include/emcrk
)

add_executable(crkfind
	crkfind.c
)

add_executable(r40
	r40tool.c
)

target_link_libraries(crkfind emcrk-lib)
target_link_libraries(r40 emcrk-lib)

install(TARGETS crkfind
	RUNTIME DESTINATION bin
)

install(TARGETS r40
	RUNTIME DESTINATION bin
)

# vim: tabstop=4
