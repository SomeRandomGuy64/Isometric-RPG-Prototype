# Add source to this project's executable.
add_executable(${PROJECT_NAME} 
  main.cpp
  glad.c
  "${CMAKE_CURRENT_SOURCE_DIR}/../Header Files/shader.h"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)

target_include_directories(${PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Header Files"
)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3 OpenGL::GL fmt::fmt glm stb_image imgui)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)

# Shader files
file(GLOB_RECURSE SHADER_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders/*.geom"
    "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.comp"
)

target_sources(${PROJECT_NAME} PRIVATE ${SHADER_FILES})

source_group("Shaders" FILES ${SHADER_FILES})

# Copy shader files to bin directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders"
)