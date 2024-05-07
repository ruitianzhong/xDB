set(CMAKE_EXPORT_COMPILE_COMMANDS)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)


cmake_minimum_required(VERSION 3.15)

project(xDB
        LANGUAGES C CXX
        DESCRIPTION "A DBMS")

add_executable(${PROJECT_NAME} main.cpp
        binder/binder.cpp
        test.cpp
)

add_custom_command(
        OUTPUT test.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/binder/test.l
        COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/binder/test.l > ${CMAKE_CURRENT_BINARY_DIR}/test.cpp
        COMMENT "Prebuild command"
        VERBATIM
)


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(check
        pwd
)









