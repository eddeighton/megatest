
cmake_minimum_required( VERSION 3.1...3.16 )

set( MEGA_HOST testhost )

if( MEGA_PROJECT STREQUAL "testproject" )
	add_subdirectory( testproject )
endif()

if( MEGA_PROJECT STREQUAL "testproject2" )
	add_subdirectory( testproject2 )
endif()
