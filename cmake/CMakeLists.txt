
cmake_minimum_required( VERSION 3.1...3.16 )

project( megatest )

MARK_AS_ADVANCED(MULTITHREADED_BUILD)

set( CMAKE_CXX_STANDARD 17 )

set( MEGA_PROJECT "" CACHE STRING "The mega structure project to build" )
set( MEGA_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../ )
set( MEGA_CMAKE ${CMAKE_CURRENT_LIST_DIR} )
set( MEGA_BUILD ${MEGA_ROOT}/build )
set( MEGA_INSTALL ${MEGA_ROOT}/install )
set( MEGA_INTERFACE ${MEGA_ROOT}/interface )
set( MEGA_IMPL ${MEGA_ROOT}/impl )
set( MEGA_LOGS ${MEGA_ROOT}/logs )
set( MEGA_SRC ${MEGA_ROOT}/src )

set( CMAKE_INSTALL_PREFIX ${MEGA_INSTALL}/${MEGA_PROJECT} CACHE PATH INTERNAL FORCE)

find_path( MEGA_THIRD_PARTY_DIR boost PATHS W:/root/thirdparty_x64 )

set(CMAKE_CONFIGURATION_TYPES RelWithDebInfo CACHE STRING INTERNAL FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_path( MEGASTRUCTURE_INSTALL bin/mega.exe PATHS W:/root/megastructure/install )
set( MEGASTRUCTURE ${MEGASTRUCTURE_INSTALL}/bin/mega.exe )

#set( CLANG_CXX_DEFINES "-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH -D_DEBUG -D_ITERATOR_DEBUG_LEVEL=2 -DWIN32_LEAN_AND_MEAN -D_DLL -DNOMINMAX -DBOOST_ALL_NO_LIB -D_CRT_SECURE_NO_WARNINGS -DBOOST_USE_WINDOWS_H -D_WIN32_WINNT=0x0601 -DWIN32 -D_WINDOWS " )


set( CLANG_CXX_DEFINES "-DUNICODE -D_USE_MATH_DEFINES -DSIZEOF_VOID_P=64 -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -D_ALLOW_COMPILER_AND_STL_VERSION_MISMATCH -DWIN32_LEAN_AND_MEAN -D_MT -D_DLL -DNOMINMAX -DBOOST_ALL_NO_LIB -D_CRT_SECURE_NO_WARNINGS -DBOOST_USE_WINDOWS_H -D_WIN32_WINNT=0x0601 -DWIN32 -D_WINDOWS " )
#set( CLANG_CXX_OPTIONS  "-GR -Gd -MDd -EHa -RTC1" )
#set( UNREAL_CXX_DEFINES "-DUE_BUILD_DEVELOPMENT=1 -DWITH_ENGINE=0 -DWITH_EDITOR=0 -DWITH_UNREAL_DEVELOPER_TOOLS=0 -DWITH_PLUGIN_SUPPORT=0 -DIS_MONOLITHIC=0 -DIS_PROGRAM=0 -DUBT_COMPILED_PLATFORM=Windows -DPLATFORM_WINDOWS=0x0601" )

#set( CLANG_CXX_OPTIONS  "-source-charset:utf-8 -execution-charset:utf-8 -EHsc -bigobj -MD -DUNICODE" )
#set( CLANG_CXX_OPTIONS  "-MP" )
#set( CLANG_CXX_OPTIONS  "-O3 -mllvm -polly -MD -DNDEBUG" )

#-Xclang -target=x86_64 
set( CLANG_CXX_OPTIONS  "-g -fexceptions" )
#set( LINK_CXX_OPTIONS "-GR -Gd -MT -EHa -RTC1" )

#set( CLANG_CXX_OPTIONS  "-fexceptions" )
set( CLANG_COMPILER_OPTIONS "-Xclang -std=c++17 -Xclang -fcoroutines-ts -Xclang -flto-visibility-public-std -Xclang -fexceptions" )
set( CLANG_COMPILER_WARNINGS "-Wno-inconsistent-missing-override -Wno-deprecated-declarations -Wno-deprecated -Wno-nonportable-include-path -Wno-ignored-attributes -Wno-microsoft-unqualified-friend -Wno-gnu-string-literal-operator-template" )
#-Wall -Wextra -Wconversion -Wcast-qual -Wdeprecated
#-Xclang -Wno-inconsistent-missing-override  -Xclang -Wno-deprecated-declarations -Xclang -Wno-deprecated -Xclang -Wno-nonportable-include-path

#
#Microsoft args: -GR -Gd -MDd -std:c++17 

set( CLANG_CXX_FLAGS "-DEG_CLANG_COMPILATION ${CLANG_CXX_DEFINES} ${CLANG_CXX_OPTIONS} ${CLANG_COMPILER_OPTIONS} ${CLANG_COMPILER_WARNINGS}" )

message( "Clang flags: " ${CLANG_CXX_FLAGS} )

add_custom_target( Interface 
	${MEGASTRUCTURE} --build ${CMAKE_CFG_INTDIR} --dir ${MEGA_ROOT} --project ${MEGA_PROJECT} --flags ${CLANG_CXX_FLAGS}
	BYPRODUCTS ${MEGA_INTERFACE}/${MEGA_PROJECT}/interface.db
	COMMENT "Generating interface: ${MEGA_PROJECT}" )
    
add_custom_target( Clean 
	${MEGASTRUCTURE} --clean --dir ${MEGA_ROOT} --project ${MEGA_PROJECT}
	COMMENT "Cleaning project: ${MEGA_PROJECT}" )

add_subdirectory( ../reddwarf ${CMAKE_CURRENT_BINARY_DIR}/reddwarf )
add_subdirectory( ../deepthought ${CMAKE_CURRENT_BINARY_DIR}/deepthought )