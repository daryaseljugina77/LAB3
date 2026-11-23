cmake_minimum_required(VERSION 3.16)

project(LAB3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    Array.cpp
    FList.cpp
    DList.cpp
    Stack.cpp
    Queue.cpp
    HashLab.cpp
    CompleteTree.cpp
    Serialization.cpp
)

add_library(structures STATIC ${SOURCES})

target_include_directories(structures PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(structures PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE structures)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

enable_testing()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/google_test.cpp")
    add_executable(google_test google_test.cpp)

    target_link_libraries(google_test PRIVATE
        structures
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(google_test)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp")
    add_executable(benchmark_test benchmark.cpp)

    target_link_libraries(benchmark_test PRIVATE
        structures
        benchmark::benchmark
        benchmark::benchmark_main
    )
endif()
