set(CMAKE_EXPORT_COMPILE_COMMANDS)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)
# testing is required
enable_testing()
cmake_minimum_required(VERSION 3.15)

project(xDB
        LANGUAGES C CXX
        DESCRIPTION "A DBMS")

add_executable(${PROJECT_NAME} main.cpp
        sql_lexer.cpp
        sql_lexer.h
        sql.tab.cpp
        sql.tab.hpp
        include/sql/exp.h
        include/sql/delete_stmt.h
        include/sql/select_stmt.h
        include/sql/insert_stmt.h
        include/sql/drop_stmt.h
        include/sql/update_stmt.h
        include/sql/use_stmt.h
        include/sql/create_stmt.h
        include/sql/show_stmt.h
        include/sql/stmt.h
        include/sql/stmts.h
        sql/drop_stmt.cpp
        sql/stmt.cpp
        include/sql/table.h
        include/sql/parser_result.h
        sql/use_stmt.cpp
        sql/table.cpp
        sql/show_stmt.cpp
        sql/insert_stmt.cpp
        sql/create_stmt.cpp
        sql/parser_result.cpp
)

add_custom_command(
        OUTPUT sql_lexer.cpp sql_lexer.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.l
        COMMAND flex -o sql_lexer.cpp --header-file=sql_lexer.h ${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.l
        COMMENT "Generating lexer"
        VERBATIM
)


add_custom_command(
        OUTPUT sql.tab.hpp sql.tab.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.y
        COMMAND bison --output sql.tab.cpp -d ${CMAKE_CURRENT_SOURCE_DIR}/parser/sql.y
        COMMENT "Generating parser"
        VERBATIM
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_custom_target(check
        ./test/xDB_unit_test
)

add_dependencies(check xDB_unit_test)

add_custom_target(shell
        ./xDB
)
add_dependencies(shell xDB)


add_library(${PROJECT_NAME}_lib STATIC
        sql_lexer.cpp
        sql_lexer.h
        sql.tab.cpp
        sql.tab.hpp
        sql/create_stmt.cpp
)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(test)
add_subdirectory(third_party/googletest)









