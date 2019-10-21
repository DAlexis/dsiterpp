cmake_minimum_required(VERSION 2.8)

project(ode-cpp-lite-unit-tests)

set(EXE_SOURCES
    euler-explicit-ut.cpp
    exponent-time-iterable.cpp
    exponent-time-iterable.hpp
    runge-kutta-ut.cpp
)

include_directories(
    ${ode-cpp-lite_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES} ${EXE_HPP})

target_link_libraries (${PROJECT_NAME}
    gtest
    gtest_main
    ${ode-cpp-lite_LIBRARY}
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

