cmake_minimum_required(VERSION 3.14)

project(stl
    VERSION 1.0
    LANGUAGES CXX
)

add_library(headerfiles INTERFACE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(headerfiles
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(headerfiles
  INTERFACE
    cxx_std_11
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/main.cpp
    tests/vector_test.cpp

)

target_link_libraries(tests
    PRIVATE
        headerfiles
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
