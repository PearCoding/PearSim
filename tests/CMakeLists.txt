CMAKE_MINIMUM_REQUIRED(VERSION 2.8.11)

PROJECT(PearSim-Test)

message(STATUS "Enabling Testing")
enable_testing()

set(PST_Root ${PS_Root}/../tests)

set(LIBS PearSimLib Qt5::Widgets Qt5::OpenGL Qt5::Test ${OPENGL_LIBRARIES} ${OpenCL_LIBRARIES})

FUNCTION(PUSH_TEST name files)
	QT5_WRAP_CPP(test${name}_moc ${files})
	add_executable(test${name} ${files} ${test${name}_moc})
	set_source_files_properties(${test${name}_moc} PROPERTIES HEADER_FILE_ONLY TRUE)
	target_link_libraries(test${name} ${LIBS})
	add_test(NAME ${name} COMMAND test${name})
ENDFUNCTION(PUSH_TEST)

PUSH_TEST(Gradient gradient_test.cpp)
PUSH_TEST(Data data_test.cpp)