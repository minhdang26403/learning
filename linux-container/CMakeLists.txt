cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clangd.

project(LearnCppConcurrency VERSION 2024.1
  DESCRIPTION "A simple Linux container"
  LANGUAGES C)

# #####################################################################################################################
# COMPILER SETUP
# #####################################################################################################################

# Set the binary directory within the build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -lcap -lseccomp")

# Executables
add_executable(contained contained.c)
