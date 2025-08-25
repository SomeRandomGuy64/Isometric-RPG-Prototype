add_library(glm INTERFACE)

add_library(stb_image INTERFACE)

add_subdirectory(SDL)
add_subdirectory(fmt EXCLUDE_FROM_ALL)

target_include_directories(glm INTERFACE glm)

add_library(imgui STATIC)

target_include_directories(imgui PUBLIC imgui)

target_sources(imgui PRIVATE 
  imgui/imgui.h
  imgui/imgui.cpp

  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_tables.cpp
  
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_sdl3.cpp
  )
  
  target_link_libraries(imgui PUBLIC SDL3::SDL3_test)
  
target_include_directories(stb_image INTERFACE stb_image)