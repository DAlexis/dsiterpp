project("dsiterpp")

set(LIB_SOURCE
    ${PROJECT_SOURCE_DIR}/src/euler-explicit.cpp
    ${PROJECT_SOURCE_DIR}/src/time-iter.cpp
    ${PROJECT_SOURCE_DIR}/src/runge-kutta.cpp
    ${PROJECT_SOURCE_DIR}/src/error-estimator.cpp
    ${PROJECT_SOURCE_DIR}/src/runge-error-estimator.cpp
    ${PROJECT_SOURCE_DIR}/src/integration.cpp
    ${PROJECT_SOURCE_DIR}/src/bifurcation.cpp
)

set(LIB_HPP
    ${PROJECT_SOURCE_DIR}/dsiterpp/time-iter.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/euler-explicit.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/runge-kutta.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/error-estimator.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/runge-error-estimator.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/integration.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/bifurcation.hpp
    ${PROJECT_SOURCE_DIR}/dsiterpp/utils.hpp
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE} ${LIB_HPP})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC profiler)

target_compile_options(
    ${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:DEBUG>:-O0>"
        "$<$<CONFIG:DEBUG>:-pg>"
        "$<$<CONFIG:DEBUG>:-g>"

        "$<$<CONFIG:RELEASE>:-O3>"
        "$<$<CONFIG:RELEASE>:-funroll-loops>"
        "$<$<CONFIG:RELEASE>:-ffast-math>"
)

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
