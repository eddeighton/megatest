
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${MEGA_THIRD_PARTY_DIR}/pybind11/install/share/cmake/pybind11/pybind11Config.cmake )

find_path( pybind11_DIR NAMES pybind11Config.cmake PATHS ${MEGA_THIRD_PARTY_DIR}/pybind11/install/share/cmake/pybind11 )

find_package( pybind11 REQUIRED)

#include_directories( ${EG_THIRD_PARTY_DIR}/install/pybind11/include )

#IF( ${WIN32} )
#set( PYBIND11_CPP_STANDARD /std:c++17)
#ENDIF( ${WIN32} )

function( link_pybind11 targetname )

    target_include_directories( ${targetname} PRIVATE ${INDICATORS_INCLUDEDIR} )
	
    #target_link_libraries( ${targetname} pybind11::embed) 
	
endfunction( link_pybind11 )