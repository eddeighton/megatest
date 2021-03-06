
cmake_minimum_required(VERSION 2.8)

find_path( EG_INSTALL_PATH share/eg-config.cmake PATHS ${MEGA_THIRD_PARTY_DIR}/../eg/install )

include( ${EG_INSTALL_PATH}/share/eg-config.cmake )

function( link_eg targetname )
	target_include_directories( ${targetname} PRIVATE ${EG_INSTALL_PATH}/include )
	
	#target_link_libraries( ${targetname} EG::eglib )
	
	target_link_libraries( ${targetname} EG::eg_runtime )
endfunction( link_eg )

