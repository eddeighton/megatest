
cmake_minimum_required(VERSION 2.8)

include( ${MEGASTRUCTURE_INSTALL}/share/mega-config.cmake )

function( link_mega_python_lib targetname )
	target_include_directories( ${targetname} PRIVATE ${MEGASTRUCTURE_INSTALL}/include )
	target_link_libraries( ${targetname} Mega::python_lib )
endfunction( link_mega_python_lib )