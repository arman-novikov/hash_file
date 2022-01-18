cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME hash_file)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost 1.71 COMPONENTS program_options iostreams REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

add_library(hasher-lib STATIC
    hasher/ihasher.cpp
    hasher/ihasher.hpp
    hasher/md5hasher.cpp
    hasher/md5hasher.hpp
)

target_link_libraries(hasher-lib PRIVATE
    ${Boost_LIBRARIES}
)

add_library(worker-lib STATIC
    worker/worker.hpp
    worker/worker.cpp
)

target_link_libraries(worker-lib PRIVATE
    hasher-lib
    Threads::Threads
)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    worker-lib
)
