
cmake_minimum_required(VERSION 2.8)

set(AVR_MCU "atmega328p")
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc.toolchain.cmake")

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")

project(pclights-firmware)

include_directories(
	include/
	${EXTERNAL_DIR}/stdperiph/include/
)

add_definitions(-DF_CPU=16000000)
add_avr_executable(${PROJECT_NAME}
	src/main.cpp
)

