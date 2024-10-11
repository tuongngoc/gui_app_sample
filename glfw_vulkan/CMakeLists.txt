cmake_minimum_required(VERSION 3.14)
add_executable(vulkan_example "")
target_sources(vulkan_example
  PUBLIC
    main.cpp
)

# Libraries
find_package(Vulkan REQUIRED)
#find_library(VULKAN_LIBRARY
  #NAMES vulkan vulkan-1)
#set(LIBRARIES "glfw;${VULKAN_LIBRARY}")
#set(LIBRARIES "glfw;Vulkan::Vulkan")


target_include_directories(vulkan_example
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/vulkan>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glfw/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(vulkan_example PUBLIC -DImTextureID=ImU64)
target_compile_definitions(vulkan_example
    PUBLIC _USE_MATH_DEFINES _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

target_link_libraries(vulkan_example 
                              imgui::glfw imgui::vulkan Vulkan::Vulkan)


target_compile_features(vulkan_example PUBLIC cxx_std_17)
target_include_directories(vulkan_example PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends>
    $<INSTALL_INTERFACE:include>
)


if(MSVC)
    target_compile_options(vulkan_example PUBLIC "/permissive-" "/Zc:__cplusplus")
    target_link_libraries(vulkan_example Delayimp.lib)
    target_link_options(vulkan_example PRIVATE
        "/ignore:4099" "/ignore:4199" "/ENTRY:mainCRTStartup" "/SUBSYSTEM:WINDOWS"
    )
endif()


