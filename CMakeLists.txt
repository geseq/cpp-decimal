cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(NAME cpp-decimal)

project(${NAME} LANGUAGES CXX)
enable_testing()

if(NOT "${CMAKE_CXX_STANDARD}")
  set(CMAKE_CXX_STANDARD 17)
endif()
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

add_library(${NAME} INTERFACE)
target_sources(${NAME} INTERFACE include/decimal.hpp)
target_include_directories(${NAME} INTERFACE include/)

option(ENABLE_TESTING "Enable test target generation" OFF)
option(BENCHMARK_ENABLE_TESTING "run benchmarks" OFF)

if (ENABLE_TESTING)
    include(cmake/CPM.cmake)
    CPMUsePackageLock(package-lock.cmake)

    CPMAddPackage( NAME googletest GITHUB_REPOSITORY google/googletest VERSION 1.14.0 )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    include_directories(googletest_SOURCE_DIR)
    enable_testing()

    FILE(GLOB tests CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/test/*)
    FOREACH (test ${tests})
        get_filename_component(test_name ${test} NAME)
        message("Adding test: " ${test_name})
        add_executable(${test_name} ${PROJECT_SOURCE_DIR}/test/${test_name})
        target_link_libraries(${test_name} ${CMAKE_THREAD_LIBS_INIT})
        target_link_libraries(${test_name} PRIVATE ${NAME} gtest gtest_main)
        add_test(${test_name} ${test_name})
        set_property(TEST ${test_name} PROPERTY LABELS "test")
    ENDFOREACH ()


    if (BENCHMARK_ENABLE_TESTING)
        CPMAddPackage( NAME benchmark GITHUB_REPOSITORY google/benchmark VERSION 1.5.2 OPTIONS "BENCHMARK_ENABLE_TESTING Off")

        if(TARGET benchmark::benchmark)
            # enable c++11 to avoid compilation errors
            set_target_properties(benchmark PROPERTIES CXX_STANDARD 17)
        endif()


        FILE(GLOB tests ${PROJECT_SOURCE_DIR}/bench/*)
        FOREACH (test ${tests})
            get_filename_component(test_name ${test} NAME)
            message("Adding bench: " ${test_name})
            add_executable(${test_name} ${PROJECT_SOURCE_DIR}/bench/${test_name})
            target_link_libraries(${test_name} PRIVATE ${CMAKE_THREAD_LIBS_INIT} benchmark::benchmark ${NAME})
            add_test(${test_name} ${test_name})
            set_property(TEST ${test_name} PROPERTY LABELS "bench")
            list(APPEND ignore_tests ${test_name})
        ENDFOREACH ()

        set (CMAKE_CTEST_ARGUMENTS "-L;test")
        add_custom_target(fastchan_bench COMMAND ctest -L bench -V)
    endif()
endif()

