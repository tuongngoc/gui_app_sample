cmake_minimum_required (VERSION 3.13)
project(gui_test_app VERSION 1.0 LANGUAGES CXX)
cmake_policy(SET CMP0074 NEW)

# Set the Vulkan SDK path explicitly
set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")

# Include the Vulkan headers
include_directories(${VULKAN_SDK}/Include)

# Link the Vulkan libraries
link_directories(${VULKAN_SDK}/Lib)

# Find the Vulkan package
find_package(Vulkan REQUIRED)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(glfw_vulkan)
