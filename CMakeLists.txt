
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

find_package(Boost COMPONENTS system filesystem program_options REQUIRED)

include_directories(
    include/
	${Boost_INCLUDE_DIR}
)

set(HEADERS
	include/serial.h
	include/configuration.h
)
add_executable(${PROJECT_NAME}
    src/main.cpp
	src/serial.cpp
	src/configuration.cpp
	${HEADERS}
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
		shlwapi
	)
endif()
