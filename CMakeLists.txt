cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME search_of_balls)
project(${PROJECT_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)

set(PROJECT_LIBRARY "${PROJECT_NAME}")
set(PROJECT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(PROJECT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories("${PROJECT_INCLUDE}")

# OUTPUT DIR
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set(OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
else()
  set(OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
endif()

# COPY RESOURCE
set(COPY_RESOURCE copy_resource)
add_custom_target(${COPY_RESOURCE} ALL)
add_custom_command(
	TARGET ${COPY_RESOURCE} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/resource
			${OUT_DIR}/resource
)

# OPENCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
message(${OpenCV_INCLUDE_DIRS})

# BUILD
add_subdirectory(src)
add_subdirectory(main)

# REPORT
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message( STATUS "")