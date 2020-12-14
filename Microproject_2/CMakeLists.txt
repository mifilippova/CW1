cmake_minimum_required(VERSION 3.19)
project(Microproject_2)

set(CMAKE_CXX_STANDARD 11)

add_executable(Microproject_2 main.cpp)
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(Microproject_2 PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(Microproject_2 "${CMAKE_THREAD_LIBS_INIT}")
endif()
