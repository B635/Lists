find_program(PROTOBUF_COMPILER protoc)

set(PROTO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/book.proto
    ${CMAKE_CURRENT_SOURCE_DIR}/book_api.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_SOURCES})


add_library(PosterProto ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(PosterProto protobuf::libprotobuf)


target_include_directories(PosterProto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})