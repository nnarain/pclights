
#
# PC lights Firmware build
#
# @author Natesh Narain <nnaraindev@gmail.com>
#

cmake_minimum_required(VERSION 2.8)

project(pclights)

# set flags for g++/gcc compiler
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
endif()

find_package(Boost COMPONENTS system program_options REQUIRED)

include_directories(
    include/
	${Boost_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
)

if(WIN32)
	add_definitions(
		-DBOOST_ALL_NO_LIB # disable auto linking
		-D_WIN32_WINNT=0x0501
	)
	target_link_libraries(${PROJECT_NAME}
		ws2_32
	)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	include(ExternalProject)
	ExternalProject_Add(
	    firmware

	    URL "firmware"

	    SOURCE_DIR "${CMAKE_SOURCE_DIR}/firmware"
	    BINARY_DIR "${CMAKE_BINARY_DIR}/firmware"

	    DOWNLOAD_COMMAND ""
	    UPDATE_COMMAND   ""
	    PATCH_COMMAND    ""
	    INSTALL_COMMAND  ""

	    CMAKE_ARGS
	        "-DAVR_UPLOAD_PORT=${AVR_UPLOAD_TOOL}"
	        "-DAVR_UPLOAD_BAUD=115200"
	)
endif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
