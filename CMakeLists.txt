cmake_minimum_required(VERSION 4.2.1)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(
    cpp3Dgraphics
    DESCRIPTION ""
    HOMEPAGE_URL ""
    VERSION 0.0.1
    LANGUAGES CXX C
)

include(cmake/PreventInSourceBuilds.cmake)
include(ProjectOptions.cmake)
project_local_options()

target_compile_features(project_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_subdirectory(src bin)
