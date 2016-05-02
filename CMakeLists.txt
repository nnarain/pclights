
cmake_minimum_required(VERSION 2.8)

project(pclights)

find_package(Boost REQUIRED system program_options thread)

set(EXTERNAL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -g")

include_directories(
	include/
	${EXTERNAL_SOURCE_DIR}/simplelogger/include/
	${EXTERNAL_SOURCE_DIR}/serial-messages/include/

	${Boost_INCLUDE_DIR}
)

link_directories(
	${CMAKE_BINARY_DIR}/simplelogger	
)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/serial.cpp
	src/async_buffer.cpp
)

# Build external libraries and Projects

include(ExternalProject)

# build logger
ExternalProject_Add(
	simplelogger-external

	URL "external/simplelogger"

	SOURCE_DIR "${CMAKE_SOURCE_DIR}/external/simplelogger"
	BINARY_DIR "${CMAKE_BINARY_DIR}/simplelogger"

	DOWNLOAD_COMMAND ""
	INSTALL_COMMAND  ""
)

add_dependencies(${PROJECT_NAME}
	simplelogger-external
)

target_link_libraries(${PROJECT_NAME}
	simplelogger
	${Boost_LIBRARIES}
)

# build firmware project
if(NOT BUILD_FIRMWARE)
	set(BUILD_FIRMWARE "true")
endif(NOT BUILD_FIRMWARE)

if(BUILD_FIRMWARE STREQUAL "true")

	ExternalProject_Add(
		firmware

		URL "firmware"

		SOURCE_DIR "${CMAKE_SOURCE_DIR}/firmware"
		BINARY_DIR  "${CMAKE_BINARY_DIR}/firmware"

		DOWNLOAD_COMMAND ""
		UPDATE_COMMAND  ""
		PATCH_COMMAND   ""
		INSTALL_COMMAND ""

		CMAKE_ARGS
			"-DEXTERNAL_COMMON_DIR=${EXTERNAL_SOURCE_DIR}"

			"-DAVR_UPLOAD_PORT=/dev/ttyACM1"
			"-DAVR_UPLOAD_BAUD=115200"
	)

endif(BUILD_FIRMWARE STREQUAL "true")
