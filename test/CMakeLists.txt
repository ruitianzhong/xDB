set(TEST_NAME ${PROJECT_NAME}_unit_test)
add_executable(${PROJECT_NAME}_unit_test
        smoke_test.cpp
)
add_test(NAME unit COMMAND ${TEST_NAME})

target_link_libraries(${TEST_NAME} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)
