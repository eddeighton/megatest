cmake_minimum_required(VERSION 3.2)

#get boost
include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )

#get clang
include( ${WORKSPACE_ROOT_PATH}/thirdparty/llvm/clang_include.cmake )

#get spdlog
include( ${WORKSPACE_ROOT_PATH}/thirdparty/spdlog/spdlog_include.cmake )

#get json
include( ${WORKSPACE_ROOT_PATH}/thirdparty/nlohmann/json_include.cmake )

#get inja
include( ${WORKSPACE_ROOT_PATH}/thirdparty/inja/inja_include.cmake )

#get common
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

#get mega
include( ${WORKSPACE_ROOT_PATH}/src/mega/mega_include.cmake )

set( TEST_PROG_SOURCE 
        main.cpp )

add_executable( test_prog ${TEST_PROG_SOURCE} )

# target_link_options( test_prog PUBLIC -export-dynamic )

target_link_libraries( test_prog TestComponent )

target_include_directories( test_prog PUBLIC ${CMAKE_SOURCE_DIR} )

link_boost( test_prog program_options )
link_boost( test_prog filesystem )
link_boost( test_prog atomic )
link_orc( test_prog )
link_spdlog( test_prog )
link_json( test_prog )
link_inja( test_prog )
link_common( test_prog )
link_database( test_prog )
link_tool( test_prog )

install( TARGETS test_prog DESTINATION bin)
