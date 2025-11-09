find_package(Pistache REQUIRED)
find_package(libpqxx REQUIRED)
find_package(hiredis REQUIRED)
find_package(redis++ REQUIRED)
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

file(GLOB PROTO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

message("PROTO_FILES: ${PROTO_FILES}")
message("PROTO_SRCS: ${PROTO_SRCS}")
message("PROTO_HDRS: ${PROTO_HDRS}")


file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" ${PROTO_SRCS})


file(GLOB HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        ${PROTO_HDRS}
)


add_executable(ListBackend ${SOURCES} ${HEADERS}
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        src/main.cpp)


target_include_directories(ListBackend PRIVATE
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ListBackend PRIVATE
        Pistache::Pistache
        libpqxx::pqxx
        hiredis::hiredis
        redis++::redis++_static
        Threads::Threads
        ${Protobuf_LIBRARIES}
)
