cmake_minimum_required( VERSION 3.1...3.16 )

#get boost
include( ${WORKSPACE_ROOT_PATH}/thirdparty/boost/boost_include.cmake )

#get clang
include( ${WORKSPACE_ROOT_PATH}/thirdparty/llvm/clang_include.cmake )

#get fmt
include( ${WORKSPACE_ROOT_PATH}/thirdparty/fmt/fmt_include.cmake )

#get spdlog
include( ${WORKSPACE_ROOT_PATH}/thirdparty/spdlog/spdlog_include.cmake )

#get json
include( ${WORKSPACE_ROOT_PATH}/thirdparty/nlohmann/json_include.cmake )

#get inja
include( ${WORKSPACE_ROOT_PATH}/thirdparty/inja/inja_include.cmake )

#get common
include( ${WORKSPACE_ROOT_PATH}/src/common/common_include.cmake )

#get mega
include( ${WORKSPACE_ROOT_PATH}/src/mega/mega_include.cmake )

#include( ${WORKSPACE_ROOT_PATH}/thirdparty/qt/qt_include.cmake )
#list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALLATION}/lib/cmake/" )

find_package(Qt6 COMPONENTS Core Widgets REQUIRED )


add_executable( editor
    main_window.hpp
    main.cpp
)

#link_qt( editor )
#link_qtdock( editor )

set_target_properties( editor PROPERTIES 
    AUTOMOC ON
    AUTORCC ON
    AUTOUIC ON
    #CXX_STANDARD 14
    #CXX_STANDARD_REQUIRED ON
    #CXX_EXTENSIONS OFF
    #VERSION ${VERSION_SHORT}
    EXPORT_NAME "Schematic Editor"
    #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
    #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
    #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/bin"
)

target_link_libraries( editor Qt6::Core) 
target_link_libraries( editor Qt6::Gui) 
target_link_libraries( editor Qt6::Widgets) 

link_boost( editor program_options )
link_boost( editor filesystem )
link_boost( editor atomic )
link_orc( editor )
link_fmt( editor )
link_spdlog( editor )
link_json( editor )
link_inja( editor )
link_common( editor )
link_database( editor )
link_tool( editor )

target_include_directories( editor PUBLIC ${MEGA_INCLUDE_DIR} )

install( TARGETS editor DESTINATION bin )
