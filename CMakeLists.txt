
cmake_minimum_required(VERSION 2.8.3)

project(pclights)

find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME}
	src/main.cpp
)
