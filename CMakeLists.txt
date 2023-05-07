cmake_minimum_required(VERSION 3.20)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

project(ZPlugin_alphaWrap)

# version
set(Version "1.0")
# basically, we use Release
if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
endif()

##################
##### target #####
##################
# Target for this build
add_executable(${PROJECT_NAME})

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
########################
##### target (end) #####
########################


#####################
##### Libraries #####
#####################
###
# json
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(plugin)

# Add external libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    nlohmann_json
    nlohmann_json::nlohmann_json
    alphaWrap
)
###########################
##### Libraries (end) #####
###########################

###################
##### Include #####
###################
# Add a include files
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/include
    ${CMAKE_CURRENT_SOURCE_DIR}/submodule/libigl/include
)
#########################
##### Include (end) #####
#########################

###########################
##### compile options #####
###########################
# because we want to use std::filesystem, we require C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_definitions(${PROJECT_NAME} PRIVATE _USE_MATH_DEFINES)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4819")
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif ()
#################################
##### compile options (end) #####
#################################

#####################
##### Resources #####
#####################
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/plugin/Release/alphaWrap.dll" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/plugin/Release/alphaWrap.dll" "${CMAKE_CURRENT_SOURCE_DIR}/ZPlugs64/alphaWrap_2022/alphaWrap.dll")
elseif (APPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/plugin/libalphaWrap.dylib" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/alphaWrap.lib")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/build/Release/plugin/libalphaWrap.dylib" "${CMAKE_CURRENT_SOURCE_DIR}/ZPlugs64/alphaWrap_2022/alphaWrap.lib")
endif ()
###########################
##### Resources (end) #####
###########################
