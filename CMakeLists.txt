cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project (QMLTemperature
	VERSION 0.1
	DESCRIPTION "Display temperature from dbus project"
	LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GEN_DIR generated)
set(XML com.example.Echo.xml)

file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/${GEN_DIR}")

execute_process(
    COMMAND "/usr/bin/gdbus-codegen" 
     --output-directory=${GEN_DIR} 
	--generate-c-code=echo 
	--c-namespace=Example 
	${XML} 
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RESULT_VARIABLE result
)

if(NOT result EQUAL 0)
    message(FATAL_ERROR "Program failed: ${result}")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Graphs)

qt_standard_project_setup()

# This project will output an executable file
qt_add_executable(${PROJECT_NAME} 
	QMLTemperature.cpp
	generated/echo.c
	generated/echo.h
	c_api.c
	c_api.h
	model.cpp
	model.h
)

# Add the QML file(s) to resources
qt_add_qml_module(${PROJECT_NAME} 
    URI QmlTemperatureApp
    VERSION 1.0
    QML_FILES
        main.qml
)


#execute_process(COMMAND pkg-config --cflags --libs gio-2.0
#				OUTPUT_VARIABLE glib)
#message(STATUS "glib:${glib}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO_LIB REQUIRED IMPORTED_TARGET gio-2.0)

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE Qt6::Core Qt6::Gui PkgConfig::GIO_LIB Qt6::Widgets Qt6::Qml Qt6::Graphs
)
# Create a simple configuration header
configure_file(config.h.in config.h)

# Include the configuration header in the build
target_include_directories(
	${PROJECT_NAME} 
	PUBLIC "${PROJECT_BINARY_DIR}"
	)


