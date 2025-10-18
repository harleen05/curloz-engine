cmake_minimum_required(VERSION 3.10)
project(Curloz_Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CMAKE_BUILD_TYPE "Build type" Debug)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
		message(STATUS "Debug mode(default)")
		add_compile_options(-g -O0 -DDEBUG)
		add_compile_definitions(DEBUG)
		add_link_options(-g)
endif()

if(UNIX AND NOT APPLE)
		set(OpenGL_GL_PREFERENCE "GLVND")
endif()

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*/*.cpp src/*/*.c)
add_executable(Curloz_Engine ${SOURCES})
target_include_directories(Curloz_Engine PRIVATE include)

target_link_libraries(Curloz_Engine OpenGL::GL)
if(WIN32)
		link_directories(lib/windows)
		target_link_libraries(Curloz_Engine
				${CMAKE_SOURCE_DIR}/lib/windows/libglfw3.a 
				${CMAKE_SOURCE_DIR}/lib/windows/libassimp.a
				${CMAKE_SOURCE_DIR}/lib/windows/libz.a
			)
endif()
if(UNIX AND NOT APPLE)
		link_directories(lib/linux)
		target_link_libraries(Curloz_Engine
				${CMAKE_SOURCE_DIR}/lib/linux/libglfw3.a 
				${CMAKE_SOURCE_DIR}/lib/linux/libassimp.a
				${CMAKE_SOURCE_DIR}/lib/linux/libz.a
			)
endif()	
