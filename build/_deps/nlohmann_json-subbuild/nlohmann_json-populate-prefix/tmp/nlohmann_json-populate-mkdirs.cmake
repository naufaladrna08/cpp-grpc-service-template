# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-src"
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-build"
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix"
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/tmp"
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp"
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src"
  "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/naufal/Documents/Projects/C_CXX_Projects/Internal/grpc-auth-service/build/_deps/nlohmann_json-subbuild/nlohmann_json-populate-prefix/src/nlohmann_json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
