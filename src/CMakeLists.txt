cmake_minimum_required(VERSION 3.14)
add_executable(viwer_app "")
target_sources(viwer_app
  PUBLIC
    main.cpp
)
target_include_directories(viwer_app
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(viwer_app
    PUBLIC _USE_MATH_DEFINES _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
target_link_libraries(viwer_app asio::asio fmt::fmt 
                              imgui::glfw imgui::opengl3)
if (UNIX)
    include(CheckPIESupported)
    check_pie_supported()
    target_link_libraries(viwer_app stdc++fs atomic)
    set_property(TARGET viwer_app PROPERTY POSITION_INDEPENDENT_CODE FALSE)
    target_compile_options(viwer_app PRIVATE -Wno-deprecated -Wno-dev)
endif(UNIX)

target_compile_features(viwer_app PUBLIC cxx_std_17)
target_include_directories(viwer_app PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(viwer_app PUBLIC "/permissive-" "/Zc:__cplusplus")
    target_link_libraries(viwer_app Delayimp.lib)
    target_link_options(viwer_app PRIVATE
        "/ignore:4099" "/ignore:4199" "/ENTRY:mainCRTStartup" "/SUBSYSTEM:WINDOWS"
    )
endif()

add_custom_command(TARGET viwer_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:viwer_app>/data
)