cmake_minimum_required(VERSION 3.14)

# ------ GLFW -------
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")
set(GLFW_INSTALL OFF CACHE INTERNAL "")
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# ------ glbinding -------
set(OPTION_BUILD_TOOLS OFF CACHE INTERNAL "")
set(OPTION_BUILD_EXAMPLES OFF CACHE INTERNAL "")
add_subdirectory(glbinding EXCLUDE_FROM_ALL)

# ------ fnt -------
add_subdirectory(fmt EXCLUDE_FROM_ALL)

# ------ asio -------
add_library(asio INTERFACE)
add_library(asio::asio ALIAS asio)
target_include_directories(asio INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE _WIN32_WINNT=0x0601)

# ------ imgui -------
add_library(imgui)
target_sources(imgui
  PUBLIC
    imgui/imgui.h
  PRIVATE
    imgui/imconfig.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
)
add_library(imgui::imgui ALIAS imgui)
target_include_directories(imgui PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
  $<INSTALL_INTERFACE:include>
)

# ------ GLFW -------
add_library(imgui_glfw)
target_sources(imgui_glfw
  PUBLIC
    imgui/backends/imgui_impl_glfw.h
  PRIVATE
    imgui/backends/imgui_impl_glfw.cpp
)
add_library(imgui::glfw ALIAS imgui_glfw)
target_include_directories(imgui_glfw PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(imgui_glfw PUBLIC imgui glfw)
target_compile_definitions(imgui_glfw
                             PUBLIC GLFW_INCLUDE_NONE)

# ------ OpenGL3 -------
add_library(imgui_opengl3)
target_sources(imgui_opengl3
  PUBLIC
    imgui/backends/imgui_impl_opengl3.h
  PRIVATE
    imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui::opengl3 ALIAS imgui_opengl3)
target_include_directories(imgui_opengl3 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(imgui_opengl3
                             PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
target_link_libraries(imgui_opengl3 PUBLIC imgui glbinding)

# ------ VULKAN -------
add_library(imgui_vulkan)
target_sources(imgui_vulkan
  PUBLIC
    imgui/backends/imgui_impl_vulkan.h
  PRIVATE
    imgui/backends/imgui_impl_vulkan.cpp
)
add_library(imgui::vulkan ALIAS imgui_vulkan)
target_include_directories(imgui_vulkan PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
  $<INSTALL_INTERFACE:include>
)
target_compile_definitions(imgui_vulkan PUBLIC -DImTextureID=ImU64)
target_link_libraries(imgui_vulkan PUBLIC imgui Vulkan::Vulkan)