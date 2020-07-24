
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${MEGA_CMAKE}/program_include.cmake )

set( EG_SOURCE_CODE 
    ${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/unreal )
    
Make_Mega_Program( "${EG_SOURCE_CODE}" "unreal.cpp" false )

