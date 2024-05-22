add_subdirectory(Sources)

add_executable(Test
    ./Main.c
)
target_include_directories(Test PUBLIC ./Headers ../Headers)

target_link_libraries(Test
    Test_Lib
    LibStack
    c
    rt
    cmocka
)