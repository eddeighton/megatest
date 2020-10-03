cmake_minimum_required(VERSION 2.8)

find_path( HOUDINI_INSTALL_PATH toolkit/cmake/HoudiniConfig.cmake PATHS "X:/Houdini 18.0.532" )

set( CMAKE_PREFIX_PATH "${HOUDINI_INSTALL_PATH}/toolkit/cmake;${CMAKE_PREFIX_PATH}" )

function( link_houdini targetname )
    find_package( Houdini REQUIRED )
	target_link_libraries( ${targetname} Houdini )
    #houdini_configure_target( ${targetname} )
endfunction( link_houdini )
