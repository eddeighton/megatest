
cmake_minimum_required( VERSION 3.1...3.16 )

project( basic LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )

set( MEGA_PROJECT_SRC_ROOT ${CMAKE_SOURCE_DIR} )
set( MEGA_PROJECT_BUILD_ROOT ${CMAKE_BINARY_DIR} )

set( UNITY_PROJECT_DIR ${WORKSPACE_ROOT_PATH}/src/basic_unity/src )

IF( ${WIN32} )

add_definitions(-D_WIN32_WINNT=0x0601)
add_definitions(-DNOMINMAX -D_NOMINMAX)
add_definitions(-D_MT)
add_definitions(-D_DLL)

 # set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

#enable folders within generated IDE build systems
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set( UNITY_EDITOR ${BUILD_PATH}/unity_win/editor/2021.3.23f1/Editor/Unity.exe )

ELSE( ${WIN32} )

set( UNITY_EDITOR ${BUILD_PATH}/unity_linux/2021.3.19f1/Editor/Unity )

ENDIF( ${WIN32} )

add_subdirectory(mega)

add_subdirectory(test_prog)
#add_subdirectory(editor)
