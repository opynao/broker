cmake_minimum_required(VERSION 3.18)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} VERSION 1.0 DESCRIPTION "Crossplatform fast copy utility" LANGUAGES CXX)

#set(CMAKE_CXX_COMPILER clang++ )

# set generator
# if(WIN32)
# 	set(CMAKE_GENERATOR "Visual Studio 17 2022")
# endif()
message(STATUS "Generator is set to ${CMAKE_GENERATOR}")

# Disable in-source builds to prevent source tree corruption.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.")
endif()

# set c++ standard & compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
MESSAGE(STATUS "The compiler is set to ${CMAKE_CXX_COMPILER}")

option(ENABLE_TESTS "Set to ON to enable building of tests" ON)
if (ENABLE_TESTS)
    message(STATUS "Building of tests is enabled")
    enable_testing()
    add_subdirectory(tests)
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/common/include
                     ${CMAKE_SOURCE_DIR}/spdlog/include )

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME}_lib ${SRC_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES})

# set compiler options
if( UNIX )
    if(Debug)
        target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g -Wall)
    else()
        target_compile_options(${PROJECT_NAME} PUBLIC -O3 -Wall -Wno-sign-compare)
        #-Werror -Wextra -Wconversion -Wsign-conversion -g3 -Og
    endif(Debug)
elseif( WIN32 )
    add_definitions( -D_SCL_SECURE_NO_WARNINGS )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif( UNIX )