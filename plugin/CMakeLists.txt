cmake_minimum_required(VERSION 3.15)

if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

project(alphaWrap)

##################
##### target #####
##################
# Target for this build
add_library(${PROJECT_NAME} SHARED)

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    src/wrap.cpp
    src/readGoZAndTriangulate.cpp
)
########################
##### target (end) #####
########################


#####################
##### Libraries #####
#####################
# CGAL
find_package(CGAL CONFIG REQUIRED)
# Eigen
find_package(Eigen3 CONFIG REQUIRED)
# json
find_package(nlohmann_json CONFIG REQUIRED)

# Add external libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    CGAL::CGAL
    Eigen3::Eigen
    nlohmann_json
    nlohmann_json::nlohmann_json
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
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/submodule/FromZ/src
)
#########################
##### Include (end) #####
#########################


###########################
##### compile options #####
###########################
# because we want to use std::filesystem, we require C++17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4819")
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif ()
#################################
##### compile options (end) #####
#################################