
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${WORKSPACE_ROOT_PATH}/src/mega/mega_include.cmake )

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

MegaProject( MegaTest )

MegaComponent( INTERFACE TestInterface MEGA root.mega square.mega cube.mega cube_extra.mega )

link_boost( TestInterface filesystem )
link_common( TestInterface )

#add_subdirectory(stuff)
add_subdirectory(test_component)
