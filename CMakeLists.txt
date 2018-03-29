cmake_minimum_required(VERSION 3.0.2)

set(PROJECT_NAME libexpat-example)
project(${PROJECT_NAME})

# Choose Debug/Release mode
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if (NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler flags
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

set(CXX_STANDARD 98)
set(CXX_STANDARD_REQUIRED ON)

# - Threads library
find_package(Threads REQUIRED)
include_directories(SYSTEM ${THREADS_INCLUDE_DIR})

# Build

# - expat library
set(EXPAT_LIBRARY "expat")

set(EXPAT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libexpat/expat/lib")


# - data structures library
set(DATA_STRUCTURES_WRAPPER_LIBRARY "data-structures-lib")
set(DATA_STRUCTURES_WRAPPER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data-structures-lib/include")

# - libraries
add_subdirectory("data-structures-lib/src")
add_subdirectory("3rdparty/libexpat/expat")

# - application
add_subdirectory("application")

# Report
message(STATUS "")
message(STATUS "==============================================================")
message(STATUS "Configuration for ${PROJECT_NAME}")
message(STATUS "==============================================================")
message(STATUS "")
message(STATUS "Configuration: ${CMAKE_BUILD_TYPE}")
message(STATUS "Include directories:")
message(STATUS "   - ${DATA_STRUCTURES_WRAPPER_LIBRARY}: ${DATA_STRUCTURES_WRAPPER_INCLUDE_DIR}")
message(STATUS "   - ${EXPAT_LIBRARY}: ${EXPAT_INCLUDE_DIR}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "")
message(STATUS "==============================================================")
message(STATUS "")
