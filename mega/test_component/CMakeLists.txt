
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )

MegaComponent( LIBRARY TestComponent CPP test.cpp DEPENDENCIES ../square.mega )

link_boost( TestComponent filesystem )
