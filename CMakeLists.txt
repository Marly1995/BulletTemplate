project(BulletMagnet)
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(BulletMagnet main.cpp
				src/BulletShape.cpp src/BulletShape.h
				src/BulletWorld.cpp src/BulletWorld.h)


target_link_libraries(BulletMagnet ${CONAN_LIBS})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "BulletMagnet")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "BulletMagnet")
