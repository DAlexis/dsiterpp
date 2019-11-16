project(dsiterpp-unit-tests)

set(EXE_SOURCES
    euler-explicit-ut.cpp
    exponent-time-iterable.cpp
    exponent-time-iterable.hpp
    runge-kutta-ut.cpp
    auto-step-adj.cpp
)

include_directories(
	${dsiterpp_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES} ${EXE_HPP})

target_link_libraries (${PROJECT_NAME}
    gtest
    gtest_main
    dsiterpp
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

