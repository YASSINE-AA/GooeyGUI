cmake_minimum_required(VERSION 3.10)

project(GooeyGUI VERSION 1.0 LANGUAGES C)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(SOURCES
    src/backends/glfw_backend.c
    src/core/gooey.c
    src/core/gooey_common.c
    src/utils/logger/gooey_logger.c
    src/utils/theme/gooey_theme.c
    src/utils/glad/glad.c
    src/utils/glfw/glfw_utils.c
    src/widgets/gooey_button.c
    src/widgets/gooey_canvas.c
    src/widgets/gooey_checkbox.c
    src/widgets/gooey_dropdown.c
    src/widgets/gooey_label.c
    src/widgets/gooey_layout.c
    src/widgets/gooey_list.c
    src/widgets/gooey_menu.c
    src/widgets/gooey_messagebox.c
    src/widgets/gooey_radiobutton.c
    src/widgets/gooey_slider.c
    src/widgets/gooey_textbox.c
    src/signals/gooey_signals.c
)

set(HEADERS
    internal/core/gooey_common.h
    internal/core/gooey_backend_internal.h
    internal/utils/glfw/glfw_utils.h
    internal/utils/glad/glad.h
    internal/utils/linmath/linmath.h
    internal/utils/logger/gooey_logger_internal.h
    internal/utils/theme/gooey_theme_internal.h
    internal/gooey_event_internal.h
    internal/gooey_widgets_internal.h
    include/gooey.h
    include/widgets/gooey_button.h
    include/widgets/gooey_canvas.h
    include/widgets/gooey_checkbox.h
    include/widgets/gooey_dropdown.h
    include/widgets/gooey_label.h
    include/widgets/gooey_layout.h
    include/widgets/gooey_list.h
    include/widgets/gooey_menu.h
    include/widgets/gooey_messagebox.h
    include/widgets/gooey_radiobutton.h
    include/widgets/gooey_slider.h
    include/widgets/gooey_textbox.h
    include/signals/gooey_signals.h

)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g3 -fsanitize=address,undefined -fPIC")

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/cjson)
add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/freetype)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

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
    ${PROJECT_SOURCE_DIR}/third_party/freetype/include
    ${PROJECT_SOURCE_DIR}/third_party/glfw/include
    ${PROJECT_SOURCE_DIR}/third_party/cjson/include
)

set_target_properties(freetype PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

set (EXAMPLE_BASIC 
    examples/showcase.c
)
# add option to build examples 
option(BUILD_EXAMPLES "Build examples programs" ON)
if (BUILD_EXAMPLES)
  add_executable(example_basic ${EXAMPLE_BASIC})
  target_link_libraries(example_basic PRIVATE ${PROJECT_NAME})
  target_include_directories(example_basic
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    PRIVATE
    ${PROJECT_SOURCE_DIR}/internal
  )
  set_property(TARGET example_basic PROPERTY FOLDER "Examples")
endif()
install(FILES ${HEADERS} DESTINATION include)

install(FILES ${HEADERS} DESTINATION include)

enable_testing()
