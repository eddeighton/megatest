
cmake_minimum_required( VERSION 3.1...3.16 )

project( basic LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )

set( MEGA_PROJECT_SRC_ROOT ${CMAKE_SOURCE_DIR} )
set( MEGA_PROJECT_BUILD_ROOT ${CMAKE_BINARY_DIR} )

set( UNITY_PROJECT_DIR ${WORKSPACE_ROOT_PATH}/src/test_unity/src )
set( UNITY_EDITOR $ENV{UNITY_EDITOR} )

IF( ${WIN32} )

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DNOMINMAX -D_NOMINMAX)
add_definitions(-D_MT)
add_definitions(-D_DLL)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

ELSE( ${WIN32} )

ENDIF( ${WIN32} )

add_subdirectory(mega)

add_subdirectory(test_prog)

#add_subdirectory(editor)
