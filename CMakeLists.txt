cmake_minimum_required(VERSION 3.16)
project(BlackScholes VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build library
add_library(bs_lib
    src/greeks.cpp
    src/implied_vol.cpp
    src/pricing.cpp
)

target_include_directories(bs_lib PUBLIC include)

# Main executable
add_executable(black_scholes main.cpp)
target_link_libraries(black_scholes PRIVATE bs_lib)

# Adding tests
add_executable(run_tests tests/test_pricing.cpp)
target_link_libraries(run_tests PRIVATE bs_lib)
