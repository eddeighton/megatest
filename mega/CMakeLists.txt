
cmake_minimum_required(VERSION 3.2)

include( ${WORKSPACE_ROOT_PATH}/src/mega/mega_include.cmake )

include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

MegaProject( Basic )

MegaComponent( INTERFACE RuntimeSupport 

    MEGA
        root.mega
        lego.mega
        toaster.mega
        data_types.mega

    CPP
        test.cpp
)

target_include_directories( RuntimeSupport PUBLIC ${MEGA_PROJECT_SRC_ROOT} )

link_boost( RuntimeSupport filesystem )
link_boost( RuntimeSupport serialization )
link_common( RuntimeSupport )

add_subdirectory(basic_tests)
# add_subdirectory(move_tests)
