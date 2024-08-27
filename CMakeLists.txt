cmake_minimum_required(VERSION 3.22)
project(haley)

include(cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(haley/log)
include_directories(haley/util)

link_directories(${PROJECT_SOURCE_DIR}/lib)

option(BUILD_TEST "ON for complile test" ON)

set(LIB_SRC
    haley/log/event.cc
    haley/util/utils.cc
)
add_library(haley SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(haley)

set(LIBS
)
target_link_libraries(haley "${LIBS}")

if(BUILD_TEST)
haley_add_executable(test_logger "tests/test_logger.cc" haley haley)
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
