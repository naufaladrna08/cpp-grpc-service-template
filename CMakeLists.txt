cmake_minimum_required(VERSION 3.5)

project(auth C CXX)

set(CMAKE_CXX_STANDARD 17)

# Find ProtoBuf
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Find MongoDB C++ driver with pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMONGOCXX REQUIRED libmongocxx)

# Compile thirdparty libraries
add_subdirectory("./thirdparty/Bcrypt.cpp")
add_subdirectory("./thirdparty/jwt-cpp")

# Include thirdparty libraries
include_directories(
  "./thirdparty/Bcrypt.cpp/include"
  "./thirdparty/jwt-cpp/include"
)

# Proto file
get_filename_component(hw_proto "./deps/auth.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/auth.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/auth.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/auth.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/auth.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories(
  "${CMAKE_CURRENT_BINARY_DIR}" 
  "/include"
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${LIBMONGOCXX_INCLUDE_DIRS}
)

# proto file
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs}
)
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${LIBMONGOCXX_LIBRARIES}
)

# Main executable
add_executable(auth main.cpp src/auth.cpp src/authInterceptor.cpp src/jwtManager.cpp)
target_link_libraries(auth
  hw_grpc_proto
  absl::flags
  absl::flags_parse
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  ${LIBMONGOCXX_LIBRARIES}
  bcrypt
  jwt-cpp::jwt-cpp
)