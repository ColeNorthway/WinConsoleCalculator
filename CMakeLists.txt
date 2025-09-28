cmake_minimum_required(VERSION 4.1)

project(calculator)

# Set the C++ standard to C++20 and turn off GNU extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXECUTABLE_SUFFIX ".exe")

add_library(calLib
	controller.cpp
	arithmetic.cpp
	execution.cpp
	errorHandling.cpp
)

add_executable(calculator main.cpp)

target_link_libraries(calculator
    PRIVATE
    gdi32
    user32
    kernel32
	calLib
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
