cmake_minimum_required(VERSION 2.8)
project(Recognizer)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(OpenCV REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include)
message(STATUS ${PROJECT_SOURCE_DIR})
message(STATUS ${PROJECT_BINARY_DIR})
message(STATUS ${OpenCV_LIBS})
add_subdirectory(src bin)

