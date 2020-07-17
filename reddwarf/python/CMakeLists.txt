
cmake_minimum_required( VERSION 3.1...3.16 )

set( MEGA_HOST python )

if( MEGA_PROJECT STREQUAL "testproject" )
	add_subdirectory( testproject )
endif()