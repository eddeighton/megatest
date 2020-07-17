cmake_minimum_required(VERSION 2.8)

find_path( COMMON_INSTALL_PATH share/common-config.cmake PATHS ${MEGA_THIRD_PARTY_DIR}/../common/install )

include( ${COMMON_INSTALL_PATH}/share/common-config.cmake )

function( link_common targetname )
	target_link_libraries( ${targetname} Common::commonlib )
endfunction( link_common )
