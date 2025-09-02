cmake_minimum_required(VERSION 3.16)
project(MyCProject LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # para clangd e LSP

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Projeto principal
add_library(gbemu ${SOURCES})
target_include_directories(gbemu PUBLIC include external)

# # Executável (opcional)
add_executable(gbemulator src/main.c)
target_link_libraries(gbemulator PRIVATE gbemu ${SDL2_LIBRARIES})

# Testes
enable_testing()

file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.c)
add_executable(tests ${TESTS})
target_include_directories(tests PRIVATE include external)
target_link_libraries(tests PRIVATE gbemu)
add_test(NAME unit_tests COMMAND tests)
set_tests_properties(unit_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
