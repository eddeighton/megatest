
cmake_minimum_required(VERSION 3.2)

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/thirdparty/gtest/gtest_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

MegaComponent( LIBRARY MoveTests CPP test.cpp DEPENDENCIES 

    ../root.mega 
    ../toaster.mega 
    ../lego.mega   
    ../data_types.mega  

)

link_boost( MoveTests filesystem )
link_boost( MoveTests serialization )
link_common( MoveTests )
link_gtest( MoveTests )