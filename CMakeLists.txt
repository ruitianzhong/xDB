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
        sql_lexer.cpp
)

add_custom_command(
        OUTPUT sql_lexer.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.l
        COMMAND flex -o sql_lexer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.l
        COMMENT "Generating lexer"
        VERBATIM
)


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(check
        pwd
)









