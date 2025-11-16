cmake_minimum_required(VERSION 3.10)
project(Lists LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PISTACHE REQUIRED libpistache)

find_package(PostgreSQL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${POSTGRESQL_INCLUDE_DIRS})
include_directories(${PISTACHE_INCLUDE_DIRS})

add_subdirectory(proto)

set(SOURCES
    src/main.cpp
    src/api/api_server.cpp
)

add_executable(posterdb ${SOURCES})


target_link_libraries(posterdb
    ${PISTACHE_LIBRARIES}
    ${POSTGRESQL_LIBRARIES}
    hiredis
    nlohmann_json::nlohmann_json
    protobuf::libprotobuf
    PosterProto
)

target_compile_options(posterdb PRIVATE ${PISTACHE_CFLAGS_OTHER})