cmake_minimum_required(VERSION 2.8)
project(Recognizer)
find_package(OpenCV REQUIRED)
add_executable(test_recognizer test_recognizer.cpp recognizer.cpp os.cpp log.cpp shapecontext.cpp thin.cpp
	edmonds.cpp HungarianMethod.cpp)
target_link_libraries(test_recognizer ${OpenCV_LIBS})
