
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${MEGA_CMAKE}/program_include.cmake )

set( EG_SOURCE_CODE 
    deepthought/testhost/testproject/deep 
    reddwarf/python/testproject/python 
    reddwarf/testhost/testproject/red
    reddwarf/unreal/testproject/unreal
    )
    
Make_Mega_Program( "${EG_SOURCE_CODE}" "" true )

