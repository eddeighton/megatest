
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

MegaComponent( LIBRARY TestComponent CPP test.cpp DEPENDENCIES ../cube.mega )

link_boost( TestComponent filesystem )
link_common( TestComponent )
