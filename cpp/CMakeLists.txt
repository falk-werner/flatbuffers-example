cmake_minimum_required (VERSION 3.10)
project(flatbuffers-example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(FLATBUFFERS REQUIRED flatbuffers)

find_program(FLATC flatc REQUIRED)

add_custom_command(
    OUTPUT todolist_generated.h
    COMMAND flatc
        -I ${CMAKE_CURRENT_SOURCE_DIR}
        -o ${CMAKE_CURRENT_BINARY_DIR}
        -c
        ${CMAKE_CURRENT_SOURCE_DIR}/todolist.fbs
    DEPENDS todolist.fbs
)

add_library(todolist STATIC
    src/todolist.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/todolist_generated.h
)

target_include_directories(todolist PRIVATE
    src
    ${CMAKE_CURRENT_BINARY_DIR}
    ${FLATBUFFERS_INCLUDE_DIRS}
)

target_link_libraries(todolist PUBLIC
    ${FLATBUFFERS_LIBRARIES}
)

target_compile_options(todolist PUBLIC
    ${FLATBUFFERS_CFLAGS}
    ${FLATBUFFERS_CFLAGS_OTHER}
)


add_executable(todo-add src/todo_add.cpp )
target_link_libraries(todo-add PUBLIC todolist)

add_executable(todo-list src/todo_list.cpp )
target_link_libraries(todo-list PUBLIC todolist)
