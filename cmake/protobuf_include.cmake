

cmake_minimum_required( VERSION 3.1...3.16 )

include(FindProtobuf)

include( ${MEGA_THIRD_PARTY_DIR}/protobuf/install/cmake/protobuf-targets.cmake )
include( ${MEGA_THIRD_PARTY_DIR}/protobuf/install/cmake/protobuf-module.cmake )
include( ${MEGA_THIRD_PARTY_DIR}/protobuf/install/cmake/protobuf-config.cmake )
include( ${MEGA_THIRD_PARTY_DIR}/protobuf/install/cmake/protobuf-options.cmake )

find_package( Protobuf PATHS ${MEGA_THIRD_PARTY_DIR}/protobuf/install/cmake )

#message("Protobuf_LIBRARIES: 	${Protobuf_LIBRARIES}")
#message("Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
#message("Protobuf_VERSION: 		${Protobuf_VERSION}")
#message("Protobuf_FOUND: 		${Protobuf_FOUND}")

function( link_protobuf targetname )
    target_link_libraries( ${targetname} ${Protobuf_LIBRARIES} )
endfunction( link_protobuf )


