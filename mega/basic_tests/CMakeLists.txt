
cmake_minimum_required(VERSION 3.2)

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/thirdparty/gtest/gtest_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

MegaComponent( LIBRARY BasicTests 

    CPP 

        basic.cpp
    
)

target_include_directories( BasicTests PUBLIC ${MEGA_PROJECT_SRC_ROOT} )

link_boost( BasicTests filesystem )
link_boost( BasicTests serialization )
link_common( BasicTests )
link_gtest( BasicTests )
