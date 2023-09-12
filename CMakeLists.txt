cmake_minimum_required(VERSION 3.26)
project(my_little_scraper)

set(CMAKE_CXX_STANDARD 20)

add_executable(scraper main.cpp)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(scraper PRIVATE cpr::cpr)

find_package(armadillo CONFIG REQUIRED)
target_link_libraries(scraper PRIVATE armadillo)

