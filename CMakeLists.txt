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
        DESCRIPTION "xDB is a DBMS built on RocksDB"
        VERSION 0.0.1
)

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
        include/sql/exp.h
        sql/exp.cpp
        sql/select_stmt.cpp
        sql/delete_stmt.cpp
        sql/update_stmt.cpp
        common/codec/db.pb.cc
        common/codec/db.pb.h
        include/execution/executor.h
        execution/executor.cpp
        execution/execution_result.cpp
        execution/create_executor.cpp
        include/constant.h
        execution/use_executor.cpp
        execution/show_executor.cpp
        execution/insert_executor.cpp
        execution/select_executor.cpp
        execution/delete_executor.cpp
        execution/drop_executor.cpp
        execution/update_executor.cpp
        execution/processor/processor.cpp
        execution/processor/binary_evaluator.cpp
        execution/processor/unary_evaluator.cpp
        execution/processor/scalar_evaluator.cpp
        execution/processor/visit_exp.cpp
        execution/processor/exp_checker.cpp
        include/version.h
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
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/codec)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/rocksdb/include)
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
        include/sql/exp.h
        sql/exp.cpp
        common/codec/db.pb.cc
)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/codec)

add_subdirectory(test)
add_subdirectory(third_party/googletest)

target_link_directories(xDB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/third_party/rocksdb)

target_link_libraries(xDB PRIVATE rocksdb)

find_library(LZ4_LIB lz4 REQUIRED)
message(STATUS "finding ${LZ4_LIB}")

find_library(ZSTD_LIB zstd REQUIRED)
message(STATUS "finding ${ZSTD_LIB}")

find_library(SNAPPY snappy REQUIRED)
message(STATUS "finding ${SNAPPY}")

find_library(BZ2 bz2 REQUIRED)
message(STATUS "finding ${BZ2}")

find_library(gFLAGS gflags REQUIRED)
message(STATUS "finding ${gFLAGS}")

find_library(ZLIB z REQUIRED)
message(STATUS "finding ${ZLIB}")

#target_link_libraries(xDB PUBLIC ${gFLAGS} ${BZ2} ${SNAPPY} ${ZSTD_LIB} ${LZ4_LIB} ${ZLIB})

target_link_libraries(xDB PUBLIC lz4 zstd snappy bz2 gflags z protobuf)

add_custom_target(rocksdb
        COMMAND cd
        COMMAND make -j4 --file ${CMAKE_CURRENT_SOURCE_DIR}/third_party/rocksdb/Makefile --directory ${CMAKE_CURRENT_SOURCE_DIR}/third_party/rocksdb static_lib
)

add_dependencies(xDB rocksdb)

# integrate libfort

# Disable building tests and examples in libfort project
set(FORT_ENABLE_TESTING OFF CACHE INTERNAL "")

add_subdirectory(third_party/libfort)
target_link_libraries(xDB PRIVATE fort)


find_program(MAKE make)
message(STATUS "finding program make")

find_library(PROTOBUF protobuf REQUIRED)
message(STATUS "finding ${PROTOBUF}")





