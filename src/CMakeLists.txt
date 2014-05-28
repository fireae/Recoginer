add_executable(recognizer log.cpp
				munkres.cpp
				os.cpp
				recognizer.cpp
				shapecontext.cpp
				test_recognizer.cpp
				thin.cpp)
target_link_libraries(recognizer ${OpenCV_LIBS})