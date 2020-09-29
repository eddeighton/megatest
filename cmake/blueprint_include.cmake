cmake_minimum_required(VERSION 2.8)

find_path( BLUEPRINT_INSTALL_PATH share/blueprint-config.cmake PATHS ${MEGA_THIRD_PARTY_DIR}/../blueprint/install_releaseSymbols )

#include( ${BLUEPRINT_INSTALL_PATH}/share/blueprint-config.cmake )

set( CMAKE_PREFIX_PATH "${BLUEPRINT_INSTALL_PATH}/share;${CMAKE_PREFIX_PATH}" )
find_package( blueprint )

function( link_blueprint targetname )
	target_include_directories( ${targetname} PRIVATE ${BLUEPRINT_INSTALL_PATH}/include )
	target_link_libraries( ${targetname} Blueprint::blueprintlib )
endfunction( link_blueprint )
