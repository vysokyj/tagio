# See: https://www.npmjs.com/package/cmake-js
cmake_minimum_required(VERSION 2.8)
project(tagio)

# Import functions
include(ExternalProject)

# Configure variables
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Download and make TagLib
ExternalProject_Add(
taglib
PREFIX "${CMAKE_CURRENT_BINARY_DIR}/taglib"
#URL http://taglib.github.io/releases/taglib-1.11.tar.gz
URL http://taglib.github.io/releases/taglib-1.9.1.tar.gz
INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/taglib"
#CMAKE_ARGS "-DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/taglib"
CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/taglib"
)

# Make project
link_directories(${CMAKE_BINARY_DIR}/taglib/lib)
include_directories(BEFORE ${CMAKE_JS_INC} ${CMAKE_BINARY_DIR}/taglib/include)
file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} taglib)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} tag)
