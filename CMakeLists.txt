cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are strongly discouraged.")
endif()
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-overloaded-virtual")
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS  "-DDEBUG ${CMAKE_CXX_FLAGS}")
else()
	set(CMAKE_CXX_FLAGS  "-DNDEBUG ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(src)
