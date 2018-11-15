# See: https://www.npmjs.com/package/cmake-js
cmake_minimum_required(VERSION 2.8)
project(tagio)

# Import functions
include(ExternalProject)

# Configure variables
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic -Wall")
endif()

if (APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif()

# Sanitise IDE support - CMAKE_JS_INC is defined by cmake-js itself
# Version is dependant to cmake-js version!!
if(NOT DEFINED CMAKE_JS_INC)
    set(CMAKE_JS_INC "$ENV{HOME}/.cmake-js/node-x64/v9.11.2/include/node" "./node_modules/nan")
    #message(WARNING "CMAKE_JS_INC not defined!")
endif()


# Download and make TagLib
ExternalProject_Add(
taglib
PREFIX "${CMAKE_SOURCE_DIR}/taglib"
#URL http://taglib.org/releases/taglib-1.11.1.tar.gz
URL http://taglib.org/releases/taglib-1.9.1.tar.gz
INSTALL_DIR "${CMAKE_SOURCE_DIR}/taglib"
CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/taglib -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON
)

# Make project
link_directories(${CMAKE_SOURCE_DIR}/taglib/lib)
include_directories(BEFORE ${CMAKE_JS_INC} ${CMAKE_SOURCE_DIR}/taglib/include)
file(GLOB SOURCE_FILES "src/*.cc" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} taglib)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} tag)
