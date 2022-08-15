
cmake_minimum_required( VERSION 3.1...3.16 )

project( megatest )

set( CMAKE_CXX_STANDARD 17 )

set( MEGA_SRC_ROOT ${CMAKE_SOURCE_DIR} )
set( MEGA_BUILD_ROOT ${CMAKE_BINARY_DIR} )

add_subdirectory(mega)

add_subdirectory(retail_test)
add_subdirectory(test_prog)
add_subdirectory(editor)
