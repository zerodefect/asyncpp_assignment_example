
# CMake project file.
# Set everything up before commencing with any builds.

cmake_minimum_required(VERSION 3.9)

project(asyncpp_assignment_example)

message(STATUS "Project:    ${PROJECT_NAME}")
message(STATUS "Platform:   ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM})")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

################################################################
# Find Thread - used implicitly by GTest
find_package(Threads REQUIRED)

include(ExternalProject)

# Some (but not all!) libraries use this to figure out where
# to put their library outputs. This handles, for example,
# Fedora's tendency to put libs in lib64 rather than lib
# This is where CMAKE_INSTALL_LIBDIR comes from
include(GNUInstallDirs)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# Create the expected include location, so we can reference it later
# without CMake complaining it doesn't exist
file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/include)

################################################################
# ASYNC++.
ExternalProject_Add(
    async_cpp
    GIT_REPOSITORY https://github.com/Amanieu/asyncplusplus.git
    #GIT_SHALLOW 1 # (ambiguous when using GIT_TAG)
    #GIT_PROGRESS 1
    GIT_TAG bf6c7abd7f315d7f5838ef45f62f5febdce339f2
    UPDATE_DISCONNECTED 1
    TIMEOUT 60
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=RELWITHDEBINFO -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

ExternalProject_Add_Step(
    async_cpp CopyToBin
    COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_INSTALL_LOCATION}/${CMAKE_INSTALL_LIBDIR}/libasync++.so ${CMAKE_BINARY_DIR}/bin
    DEPENDEES install
)

include_directories(SYSTEM ${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib ${EXTERNAL_INSTALL_LOCATION}/${CMAKE_INSTALL_LIBDIR})

# Set the location where our binaries will be stored.
# WARN/TODO: Not quite right, because .lib or .a files should
#            not go in the bin folder!
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

link_directories(${CMAKE_BINARY_DIR}/bin)

# Local header files here ONLY
set(TARGET_HPP
    )

# Local source files here
set(TARGET_CPP
    main.cpp
    )

# Define an executable
add_executable(asyncpp_assignment_example ${TARGET_HPP} ${TARGET_CPP})

add_dependencies(asyncpp_assignment_example
    async_cpp)

target_link_libraries(asyncpp_assignment_example
    async++
    )
