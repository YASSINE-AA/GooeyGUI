cmake_minimum_required(VERSION 3.10)

project(GooeyGUI VERSION 1.0 LANGUAGES C)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(SOURCES
    src/backends/glfw_backend.c
    src/core/gooey.c
    src/utils/logger/gooey_logger.c
    src/utils/theme/gooey_theme.c
    src/utils/glad/glad.c
    src/utils/glfw/glfw_utils.c
)

set(HEADERS
    include/gooey.h
    internal/utils/glfw/glfw_utils.h
    internal/utils/glad/glad.h
    internal/utils/linmath/linmath.h
    internal/utils/logger/gooey_logger_internal.h
    internal/utils/theme/gooey_theme_internal.h
    internal/gooey_backend_internal.h
    internal/gooey_event_internal.h
    internal/gooey_widgets_internal.h
)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/cjson)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/freetype)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

link_directories(${PROJECT_SOURCE_DIR}/third_party/cjson)
link_directories(${PROJECT_SOURCE_DIR}/third_party/glfw)
link_directories(${PROJECT_SOURCE_DIR}/third_party/freetype)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    cjson
    glfw
    freetype
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    PRIVATE
    ${PROJECT_SOURCE_DIR}/internal
    ${third_party/freetype}/include
    ${third_party/glfw}/include
    ${third_party/cjson}/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

install(FILES ${HEADERS} DESTINATION include)

enable_testing()
