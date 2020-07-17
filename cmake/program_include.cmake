

cmake_minimum_required( VERSION 3.1...3.16 )

macro( Make_Mega_Program arg_EGSourceFiles arg_CPPSourceFiles HasPython )

include( ${MEGA_CMAKE}/boost_include.cmake )
include( ${MEGA_CMAKE}/pybind11_include.cmake )
include( ${MEGA_CMAKE}/clang_include.cmake )
include( ${MEGA_CMAKE}/protobuf_include.cmake )
include( ${MEGA_CMAKE}/common_include.cmake )
include( ${MEGA_CMAKE}/eg_include.cmake )
include( ${MEGA_CMAKE}/mega_include.cmake )


get_filename_component( CurrentFolderProjectName ${CMAKE_CURRENT_LIST_DIR} NAME )
set( MEGA_PROGRAM ${MEGA_COORDINATOR}_${MEGA_HOST}_${CurrentFolderProjectName} )

#cannot use macro argument directly - capture into variable
set( EGSourceFiles "${arg_EGSourceFiles}" )
set( CPPSourceFiles "${arg_CPPSourceFiles}" )
set( HasPython "${arg_HasPython}" )

foreach( inputname IN LISTS EGSourceFiles )

    string( REPLACE "/" "_" flatname ${inputname} )
    
    message( ${inputname} " : " ${flatname} )
    
    list(APPEND EG_FLAT_NAMES           ${flatname} )

    list(APPEND EG_SOURCES              ${MEGA_SRC}/${inputname}.eg )
    list(APPEND EG_OPERATIONS_HEADERS   ${MEGA_INTERFACE}/${MEGA_PROJECT}/${flatname}_operations.hpp )
    list(APPEND EG_OPERATIONS_IMPL      ${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/${flatname}_operations.cpp )
    list(APPEND EG_OPERATIONS_OBJECTS   ${flatname}_object.obj )

    #list(APPEND EG_SOURCES              ${inputname}.eg )
    #list(APPEND EG_OPERATIONS_HEADERS   ${MEGA_INTERFACE}/${MEGA_PROJECT}/${MEGA_PROGRAM}_${inputname}_eg_operations.hpp )
    #list(APPEND EG_OPERATIONS_IMPL      ${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/${MEGA_PROGRAM}_${inputname}_eg_operations.cpp )
    #list(APPEND EG_OPERATIONS_OBJECTS   ${MEGA_PROGRAM}_${inputname}_eg_object.obj )
endforeach()

if( ${HasPython} STREQUAL "true" )
    list(APPEND EG_OPERATIONS_IMPL      ${MEGA_IMPL}/${MEGA_COORDINATOR}/${MEGA_HOST}/${MEGA_PROJECT}/python.cpp )
    list(APPEND EG_OPERATIONS_OBJECTS   ${MEGA_PROGRAM}_python_bindings_object.obj )
    
endif()

message( "--------------------------------------------------------------------------" )
message( "Project: ${MEGA_PROJECT} Current: ${CMAKE_CURRENT_LIST_DIR}" )
message( ${MEGA_PROGRAM} " EG Source:  " "${EGSourceFiles}" )
message( ${MEGA_PROGRAM} " CPP Source: " "${CPPSourceFiles}" )
message( ${MEGA_PROGRAM} " Has Python: " "${HasPython}" )
#message( ${MEGA_PROGRAM} " EG_SOURCES:              " ${EG_SOURCES} )
#message( ${MEGA_PROGRAM} " EG_OPERATIONS_HEADERS:   " ${EG_OPERATIONS_HEADERS} )
#message( ${MEGA_PROGRAM} " EG_OPERATIONS_IMPL:      " ${EG_OPERATIONS_IMPL} )
#message( ${MEGA_PROGRAM} " EG_OPERATIONS_OBJECTS:   " ${EG_OPERATIONS_OBJECTS} )

set( MEGA_PROGRAM ${MEGA_COORDINATOR}_${MEGA_HOST}_testproject )

set( EGCOMPONENT_HEADER ${MEGASTRUCTURE_INSTALL}/include/egcomponent/egcomponent.hpp )

#set( PYTHON_SRC ${MEGASTRUCTURE_INSTALL}/include/python_lib/python_iterator.hpp
#                ${MEGASTRUCTURE_INSTALL}/include/python_lib/python_reference.hpp
#                ${MEGASTRUCTURE_INSTALL}/include/python_lib/python_reference_factory.hpp )

set( EG_LIBRARY_SRC ${EG_INSTALL_PATH}/include/eg/clock.cpp
                    ${EG_INSTALL_PATH}/include/eg/frame.cpp
                    ${EG_INSTALL_PATH}/include/eg/basic_scheduler.cpp )

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
	
	#MAIN_DEPENDENCY ${MEGA_SCHEMA_FILE}
	#DEPENDS ${MEGA_SCHEMA_FILE}
	BYPRODUCTS ${MEGA_PROGRAM}_runtime_object.obj
	COMMENT "Generating eg source code" )
	

set( EG_OBJECT_FILES 	${EG_OPERATIONS_OBJECTS}
						${MEGA_PROGRAM}_runtime_object.obj
						${MEGA_PROGRAM}_component_object.obj )
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

target_compile_definitions( ${MEGA_PROGRAM} PUBLIC -DMEGASTRUCTURE_EG_COMPONENT )

target_include_directories( ${MEGA_PROGRAM} PRIVATE ${MEGA_INTERFACE}/${MEGA_PROJECT} )
target_include_directories( ${MEGA_PROGRAM} PRIVATE ${MEGASTRUCTURE_INSTALL}/include )

link_boost( ${MEGA_PROGRAM} system )
link_pybind11( ${MEGA_PROGRAM} )
link_clang( ${MEGA_PROGRAM} )
link_protobuf( ${MEGA_PROGRAM} )
link_common( ${MEGA_PROGRAM} )
link_eg( ${MEGA_PROGRAM} )

if( ${HasPython} STREQUAL "true" )
link_mega_python_lib( ${MEGA_PROGRAM} )
endif()

#source_group( TREE ${MEGA_ROOT} FILES ${CPPSourceFiles} )

set_target_properties( ${MEGA_PROGRAM} PROPERTIES DEBUG_POSTFIX d )
target_compile_definitions( ${MEGA_PROGRAM} PRIVATE -DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_MT -D_DLL -DBOOST_ALL_NO_LIB -D_CRT_SECURE_NO_WARNINGS -DBOOST_USE_WINDOWS_H )

target_link_options( ${MEGA_PROGRAM} PUBLIC /SAFESEH:NO )

install( TARGETS ${MEGA_PROGRAM} DESTINATION bin )


endmacro()