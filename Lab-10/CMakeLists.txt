cmake_minimum_required(VERSION 3.23)
project(C___Lab10_Weather)

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Users/maxfi/Desktop/c++")
set(CMAKE_CXX_STANDARD 23)



include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)


FetchContent_MakeAvailable(cpr)

set(CMAKE_EXE_LINKER_FLAGS "--static")


add_executable(C___Lab10_Weather main.cpp requests.cpp requests.h)
target_link_libraries(C___Lab10_Weather PRIVATE cpr::cpr)


if (WIN32)
    option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif ()


