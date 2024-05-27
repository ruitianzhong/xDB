set(TEST_NAME ${PROJECT_NAME}_unit_test)
add_executable(${PROJECT_NAME}_unit_test
        test-main.cpp
        ${PROJECT_SOURCE_DIR}/sql/create_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/delete_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/drop_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/exp.cpp
        ${PROJECT_SOURCE_DIR}/sql/insert_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/parser_result.cpp
        ${PROJECT_SOURCE_DIR}/sql/select_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/show_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/table.cpp
        ${PROJECT_SOURCE_DIR}/sql/update_stmt.cpp
        ${PROJECT_SOURCE_DIR}/sql/use_stmt.cpp
        sql/create_stmt_test.cpp
)
add_test(NAME smoke-test COMMAND ${TEST_NAME} --gtest_filter=SMOKE.*)

add_test(NAME create-stmt-test COMMAND ${TEST_NAME} --gtest_filter=CREATE_STMT.*)

add_test(NAME delete-stmt-test COMMAND ${TEST_NAME} --gtest_filter=DELETE_STMT.*)

target_link_libraries(${TEST_NAME} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
