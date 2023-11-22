cmake_minimum_required(VERSION 3.26)
project(my_little_scraper)

set(CMAKE_CXX_STANDARD 20)
set(LEXBOR_CXX_FLAGS)
# Find lexbor
link_directories("/Users/chrisd/.vcpkg-clion/vcpkg_scraper/packages/lexbor_x64-osx/lib/")

add_executable(InfoHunter main.cpp
        scraping-handler.h
        scraper_definitions.cpp
        csv_handler.h
        csv_handler_definitions.cpp)

find_package(cpr CONFIG REQUIRED)
target_link_libraries(InfoHunter PRIVATE cpr::cpr)

find_package(armadillo CONFIG REQUIRED)
target_link_libraries(InfoHunter PRIVATE armadillo)

#find_package(lexbor CONFIG REQUIRED)
target_link_libraries(InfoHunter PRIVATE liblexbor_static.a)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(InfoHunter PRIVATE wx::core wx::base)

find_package(bsoncxx CONFIG REQUIRED)
target_link_libraries(InfoHunter PRIVATE $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)