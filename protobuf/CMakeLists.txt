# Minimum CMake required
cmake_minimum_required(VERSION 3.26)

# Project
project(learn-protobuf C CXX)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Protobuf requires C++14, but we set C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required protobuf package
find_package(protobuf CONFIG REQUIRED)

if(protobuf_VERBOSE)
  message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")
endif()

set(examples
  add_person
  list_people
)

foreach(example ${examples})
  set(${example}_SRCS ${example}.cc)
  set(${example}_PROTOS addressbook.proto)

  # Executable setup
  set(executable_name ${example})
  add_executable(${executable_name} ${${example}_SRCS} ${${example}_PROTOS})

  # Include generated *.pb.h file
  target_include_directories(${executable_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

  # Link executables with protobuf library
  target_link_libraries(${executable_name} protobuf::libprotobuf)
  
  # Generate protobuf files
  protobuf_generate(TARGET ${executable_name})
endforeach()
