cmake_minimum_required(VERSION 3.26)
project(my_little_scraper)

set(CMAKE_CXX_STANDARD 20)
set(LEXBOR_CXX_FLAGS)
# Find lexbor
link_directories("/Users/chrisd/.vcpkg-clion/vcpkg_scraper/packages/lexbor_x64-osx/lib/")

add_executable(scraper main.cpp
        scraping-handler.h
        scraper_definitions.cpp)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(scraper PRIVATE cpr::cpr)

find_package(armadillo CONFIG REQUIRED)
target_link_libraries(scraper PRIVATE armadillo)

#find_package(lexbor CONFIG REQUIRED)
target_link_libraries(scraper PRIVATE liblexbor_static.a)