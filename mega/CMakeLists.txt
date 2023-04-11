
cmake_minimum_required(VERSION 3.2)

include( ${WORKSPACE_ROOT_PATH}/src/mega/mega_include.cmake )

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

MegaProject( Basic )

MegaComponent( INTERFACE TestInterface MEGA meta.mega links.mega root.mega )

link_boost( TestInterface filesystem )
link_common( TestInterface )

#add_subdirectory(stuff)
add_subdirectory(test_component)
