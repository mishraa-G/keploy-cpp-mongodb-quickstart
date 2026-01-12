cmake_minimum_required(VERSION 3.10)
project(cpp_mongo_quickstart)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOC REQUIRED libmongoc-1.0)

add_executable(app src/main.cpp)
target_include_directories(app PRIVATE src/include ${MONGOC_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${MONGOC_LIBRARIES})
