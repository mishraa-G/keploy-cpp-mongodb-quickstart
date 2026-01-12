cmake_minimum_required(VERSION 3.10)
project(cpp_mongo_quickstart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

add_executable(app src/main.cpp)
target_include_directories(app PRIVATE src/include)
target_link_libraries(app PRIVATE mongocxx bsoncxx)
