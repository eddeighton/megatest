
cmake_minimum_required( VERSION 3.1...3.16 )

find_path( MSGPACK_INCLUDEDIR NAMES msgpack PATHS ${MEGA_THIRD_PARTY_DIR}/msgpack/install/include/ )

function( link_messagePack targetname )
	target_include_directories( ${targetname} PUBLIC ${MSGPACK_INCLUDEDIR} )
endfunction( link_messagePack )
