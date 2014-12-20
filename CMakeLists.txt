# Set CMake options.

cmake_minimum_required(VERSION 3.0.2)
project(Paper)

set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

# Allow for custom Qt prefixes, for e.g. developer builds.

if(CUSTOM_QT_DIR)
	set(CMAKE_PREFIX_PATH ${CUSTOM_QT_DIR} ${CMAKE_PREFIX_PATH})
	set(QT_ROOT_DIR ${CUSTOM_QT_DIR})
	set(QT_QMAKE_EXECUTABLE ${CUSTOM_QT_DIR}/bin/qmake)
endif()

# Note which type of build we're performing.

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No CMAKE_BUILD_TYPE specified; defaulting to Release.")
	set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

# Set compiler flags.

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Warnings.cmake)
paperEnableWarnings()

if(CMAKE_BUILD_TYPE_LOWER MATCHES debug)
	add_definitions(-DPAPER_DEBUG)
endif()

add_definitions(-DQT_NO_KEYWORDS)

# Find packages, include directories, and setup libraries.

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

find_package(QREncode REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Svg REQUIRED)

include_directories(
	"src"
	${QRENCODE_INCLUDE_DIR}
)

set(Paper_LIBS

	PaperCommon
	${QRENCODE_LIBRARY}
	${LIBLZMA_LIBRARY}
	${Qt5Core_LIBRARIES}
	${Qt5Gui_LIBRARIES}
	${Qt5Svg_LIBRARIES}

)

# Add the various targets.

add_subdirectory(src/PaperCommon)
add_subdirectory(src/PaperCLI)
