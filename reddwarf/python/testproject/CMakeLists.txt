
cmake_minimum_required( VERSION 3.1...3.16 )

include( ${MEGA_CMAKE}/program_include.cmake )

set( EG_SOURCE_CODE 
    reddwarf/testhost/testproject/test
    reddwarf/testhost/testproject/another 
    reddwarf/python/testproject/test
    deepthought/testhost/testproject/test  
    )
    
Make_Mega_Program( "${EG_SOURCE_CODE}" "" true )

