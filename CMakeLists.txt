
cmake_minimum_required(VERSION 2.8)

project(pclights)

find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME}
	src/main.cpp
)

# build firmware project
if(NOT BUILD_FIRMWARE)
	set(BUILD_FIRMWARE "true")
endif(NOT BUILD_FIRMWARE)

if(BUILD_FIRMWARE STREQUAL "true")

	include(ExternalProject)

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
			"-DAVR_UPLOAD_PORT=/dev/ttyACM0"
			"-DAVR_UPLOAD_BAUD=115200"
	)

endif(BUILD_FIRMWARE STREQUAL "true")