
cmake_minimum_required( VERSION 3.1...3.16 )

project( basic )

set( CMAKE_CXX_STANDARD 17 )

set( MEGA_PROJECT_SRC_ROOT ${CMAKE_SOURCE_DIR} )
set( MEGA_PROJECT_BUILD_ROOT ${CMAKE_BINARY_DIR} )

set( UNITY_PROJECT_DIR ${WORKSPACE_ROOT_PATH}/src/basic_unity/src )

IF( ${WIN32} )
set( UNITY_EDITOR ${BUILD_PATH}/unity_win/editor/2021.3.14f1/Editor/Unity.exe )
ELSE( ${WIN32} )
set( UNITY_EDITOR ${BUILD_PATH}/unity_linux/2021.3.19f1/Editor/Unity )
ENDIF( ${WIN32} )

add_subdirectory(mega)

add_subdirectory(test_prog)
add_subdirectory(editor)
