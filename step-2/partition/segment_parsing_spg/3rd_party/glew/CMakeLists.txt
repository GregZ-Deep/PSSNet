cmake_minimum_required(VERSION 3.1)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})


set(CMAKE_C_FLAGS "-g -Wall -fPIC")	
add_library(${PROJECT_NAME} STATIC src/glew.c)
set_target_properties(${PROJECT_NAME} PROPERTIES
                      FOLDER "3rd_party")


target_include_directories(${PROJECT_NAME} PRIVATE ${SegmentParsingSPG_glew_INCLUDE_DIR})


target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

