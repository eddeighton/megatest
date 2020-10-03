

cmake_minimum_required( VERSION 3.1...3.16 )

macro( Make_Mega_Program arg_EGSourceFiles arg_CPPSourceFiles arg_HostType )

include( ${MEGA_CMAKE}/boost_include.cmake )
include( ${MEGA_CMAKE}/pybind11_include.cmake )
include( ${MEGA_CMAKE}/protobuf_include.cmake )
include( ${MEGA_CMAKE}/common_include.cmake )
include( ${MEGA_CMAKE}/ed_include.cmake )
include( ${MEGA_CMAKE}/eg_include.cmake )
include( ${MEGA_CMAKE}/mega_include.cmake )
include( ${MEGA_CMAKE}/unreal_include.cmake )
include( ${MEGA_CMAKE}/blueprint_include.cmake )
#include( ${MEGA_CMAKE}/houdini_include.cmake )


get_filename_component( CurrentFolderProjectName ${CMAKE_CURRENT_LIST_DIR} NAME )
if(NOT ${MEGA_PROJECT} STREQUAL ${CurrentFolderProjectName} )
    message( FATAL_ERROR "Mega Project not current directory: ${MEGA_PROJECT} : ${CurrentFolderProjectName}" )
endif()
    
set( MEGA_PROGRAM ${MEGA_COORDINATOR}_${MEGA_HOST}_${CurrentFolderProjectName} )

set( MegaHostType "${arg_HostType}" )

message( "--------------------------------------------------------------------------" )
if( ${MegaHostType} STREQUAL "basic" )
message( ${MEGA_PROGRAM} " Basic Host Specified" )
set( IsBasicHost true )
set( IsUnrealHost false )
set( IsPythonHost false )
set( IsGeometryHost false )
elseif( ${MegaHostType} STREQUAL "unreal" )
message( ${MEGA_PROGRAM} " Unreal Host Specified" )
set( IsBasicHost false )
set( IsUnrealHost true )
set( IsPythonHost false )
set( IsGeometryHost false )
elseif( ${MegaHostType} STREQUAL "python" )
message( ${MEGA_PROGRAM} " Python Host Specified" )
set( IsBasicHost false )
set( IsUnrealHost false )
set( IsPythonHost true )
set( IsGeometryHost false )
elseif( ${MegaHostType} STREQUAL "geometry" )
message( ${MEGA_PROGRAM} " Geometry Host Specified" )
set( IsBasicHost false )
set( IsUnrealHost false )
set( IsPythonHost false )
set( IsGeometryHost true )
else()
#error unknown host type
message( FATAL_ERROR ${MEGA_PROGRAM} " Unknown host type specified: " ${MegaHostType} )
endif()

#cannot use macro argument directly - capture into variable
set( EGSourceFiles "${arg_EGSourceFiles}" )
set( CPPSourceFiles "${arg_CPPSourceFiles}" )

foreach( inputname IN LISTS EGSourceFiles )

    if(NOT EXISTS ${MEGA_SRC}/${inputname}.eg )
        message( FATAL_ERROR "Unable to locate file: ${inputname}.eg" )
    endif()
    
    string( REPLACE "/" "_" flatname ${inputname} )
    
    list(APPEND EG_FLAT_NAMES           ${flatname} )
    list(APPEND EG_SOURCES              ${MEGA_SRC}/${inputname}.eg )
    list(APPEND EG_OPERATIONS_HEADERS   ${MEGA_INTERFACE}/${MEGA_PROJECT}/${flatname}_operations.hpp )
    list(APPEND EG_OPERATIONS_IMPL      ${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/${flatname}_operations.cpp )
    list(APPEND EG_OPERATIONS_OBJECTS   ${flatname}_object.obj )
endforeach()

if( ${IsPythonHost} )
    list(APPEND EG_OPERATIONS_IMPL      ${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/python.cpp )
    list(APPEND EG_OPERATIONS_OBJECTS   python_object.obj )
endif()

if( ${IsUnrealHost} )
    list(APPEND EG_OPERATIONS_IMPL      ${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/unreal.cpp )
    list(APPEND EG_OPERATIONS_OBJECTS   unreal_object.obj )
endif()

message( "Project: ${MEGA_PROJECT} Current: ${CMAKE_CURRENT_LIST_DIR}" )
message( ${MEGA_PROGRAM} " EG Source:  " "${EGSourceFiles}" )
message( ${MEGA_PROGRAM} " CPP Source: " "${CPPSourceFiles}" )

#message( ${MEGA_PROGRAM} " EG_SOURCES:              " ${EG_SOURCES} )
#message( ${MEGA_PROGRAM} " EG_OPERATIONS_HEADERS:   " ${EG_OPERATIONS_HEADERS} )
#message( ${MEGA_PROGRAM} " EG_OPERATIONS_IMPL:      " ${EG_OPERATIONS_IMPL} )
#message( ${MEGA_PROGRAM} " EG_OPERATIONS_OBJECTS:   " ${EG_OPERATIONS_OBJECTS} )

set( MEGA_PROGRAM ${MEGA_COORDINATOR}_${MEGA_HOST}_testproject )

set( EGCOMPONENT_HEADER ${MEGASTRUCTURE_INSTALL}/include/egcomponent/egcomponent.hpp )

#set( PYTHON_SRC ${MEGASTRUCTURE_INSTALL}/include/python_lib/python_iterator.hpp
#                ${MEGASTRUCTURE_INSTALL}/include/python_lib/python_reference.hpp
#                ${MEGASTRUCTURE_INSTALL}/include/python_lib/python_reference_factory.hpp )

set( EG_LIBRARY_SRC ${EG_INSTALL_PATH}/include/eg/frame.cpp
                    ${EG_INSTALL_PATH}/include/eg/basic_scheduler.cpp )

set_source_files_properties( ${EG_LIBRARY_SRC} PROPERTIES HEADER_FILE_ONLY TRUE )
set_source_files_properties(
  ${EG_LIBRARY_SRC}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)
set_source_files_properties( ${EG_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE )

set( EG_INTERFACE_SRC
	${MEGA_INTERFACE}/${MEGA_PROJECT}/include.hpp
	${MEGA_INTERFACE}/${MEGA_PROJECT}/interface.hpp
	${MEGA_INTERFACE}/${MEGA_PROJECT}/structures.hpp
	${EG_OPERATIONS_HEADERS} )

set_source_files_properties( ${EG_INTERFACE_SRC} PROPERTIES HEADER_FILE_ONLY TRUE )
set_source_files_properties(
  ${EG_INTERFACE_SRC}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)
	
set( EG_PROGRAM_SRC
	${EG_OPERATIONS_IMPL}
	${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/netstate.hpp
	${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/component.cpp
	${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/runtime.cpp )
	
set_source_files_properties( ${EG_PROGRAM_SRC} PROPERTIES HEADER_FILE_ONLY TRUE )
set_source_files_properties(
  ${EG_PROGRAM_SRC}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

add_custom_target( ${MEGA_PROGRAM}_eg
	${MEGASTRUCTURE} 
        --build ${CMAKE_CFG_INTDIR} 
        --dir ${MEGA_ROOT} 
        --project ${MEGA_PROJECT} 
        --coordinator ${MEGA_COORDINATOR} 
        --host ${MEGA_HOST} 
        --bin ${CMAKE_CURRENT_BINARY_DIR} 
        --flags ${CLANG_CXX_FLAGS}
        --names "${EG_FLAT_NAMES}"
        #--wait
	
	#MAIN_DEPENDENCY ${MEGA_SCHEMA_FILE}
	#DEPENDS ${MEGA_SCHEMA_FILE}
	BYPRODUCTS ${MEGA_PROGRAM}_runtime_object.obj
    
    #COMMENT "${MEGA_PROGRAM}_eg ${MEGASTRUCTURE} --build ${CMAKE_CFG_INTDIR} --dir ${MEGA_ROOT} --project ${MEGA_PROJECT} --coordinator ${MEGA_COORDINATOR} --host ${MEGA_HOST} --bin ${CMAKE_CURRENT_BINARY_DIR} --flags ${CLANG_CXX_FLAGS} --names ${EG_FLAT_NAMES} --wait"

	 )
	#COMMENT "Generating eg source code"
    
add_dependencies( ${MEGA_PROGRAM}_eg Interface  )

set( EG_OBJECT_FILES 	${EG_OPERATIONS_OBJECTS}
						runtime_object.obj
						component_object.obj
						frame_object.obj
						basic_scheduler_object.obj )
set_source_files_properties(
  ${EG_OBJECT_FILES}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

add_library( ${MEGA_PROGRAM} SHARED 
	${CPPSourceFiles}  
	${EG_SOURCES}
	${EG_LIBRARY_SRC} 
	${EGCOMPONENT_HEADER} 
	${EG_OBJECT_FILES} 
	${EG_INTERFACE_SRC} 
	${EG_PROGRAM_SRC} 
	)
	
source_group("C++" FILES ${CPPSourceFiles})
source_group("eg" FILES ${EG_SOURCES})
source_group("eglib" FILES ${EG_LIBRARY_SRC})
source_group("mega" FILES ${EGCOMPONENT_HEADER})
source_group("interface" FILES ${EG_INTERFACE_SRC})
source_group("impl" FILES ${EG_PROGRAM_SRC})
source_group("objects" FILES ${EG_OBJECT_FILES})
	
add_dependencies( ${MEGA_PROGRAM} ${MEGA_PROGRAM}_eg )


#target_compile_options( ${MEGA_PROGRAM} PUBLIC /await )

target_compile_definitions( ${MEGA_PROGRAM} PUBLIC -DMEGASTRUCTURE_EG_COMPONENT )

target_include_directories( ${MEGA_PROGRAM} PRIVATE ${MEGA_INTERFACE}/${MEGA_PROJECT} )
target_include_directories( ${MEGA_PROGRAM} PRIVATE ${MEGASTRUCTURE_INSTALL}/include )

link_boost( ${MEGA_PROGRAM} system )
link_protobuf( ${MEGA_PROGRAM} )
link_common( ${MEGA_PROGRAM} )
link_eg( ${MEGA_PROGRAM} )

if( ${IsPythonHost} )
link_pybind11( ${MEGA_PROGRAM} )
link_mega_python_lib( ${MEGA_PROGRAM} )
elseif( ${IsGeometryHost} )
link_ed( ${MEGA_PROGRAM} )
link_blueprint( ${MEGA_PROGRAM} )
#link_houdini( ${MEGA_PROGRAM} )
endif()

link_unreal_core( ${MEGA_PROGRAM} )

#if( ${IsUnrealHost} )
#link_mega_link_unreal_lib( ${MEGA_PROGRAM} )
#endif()

#source_group( TREE ${MEGA_ROOT} FILES ${CPPSourceFiles} )

set_target_properties( ${MEGA_PROGRAM} PROPERTIES DEBUG_POSTFIX d )

#set( WIN_CXX_DEFINES "-DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_MT -D_DLL -DBOOST_ALL_NO_LIB -D_CRT_SECURE_NO_WARNINGS -DBOOST_USE_WINDOWS_H" )

target_compile_definitions( ${MEGA_PROGRAM} PRIVATE ${CLANG_CXX_DEFINES} ${CLANG_CXX_OPTIONS} )

target_link_options( ${MEGA_PROGRAM} PUBLIC /SAFESEH:NO ${LINK_CXX_OPTIONS} )

install( TARGETS ${MEGA_PROGRAM} DESTINATION bin )


endmacro()