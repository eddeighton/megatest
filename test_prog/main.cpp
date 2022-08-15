
#include "mega/test_component/test.hpp"

#include "service/protocol/common/project.hpp"
#include "service/network/log.hpp"

#include "runtime/runtime.hpp"

#include "boost/dll/shared_library.hpp"
#include "boost/dll/import.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem/path.hpp"

#include <iostream>
#include <string>

int main( int argc, const char* argv[] )
{
    boost::filesystem::path projectPath;
    std::string             strConsoleLogLevel = "info", strLogFileLevel = "debug";
    boost::filesystem::path logFolder = boost::filesystem::current_path() / "log";
    {
        namespace po                      = boost::program_options;
        bool                    bShowHelp = false;
        po::options_description options( "General" );

        // clang-format off
        std::string strHostName;
        options.add_options()
        ( "help,?",  po::bool_switch( &bShowHelp ),                         "Show Command Line Help" )
        ( "log",     po::value< boost::filesystem::path >( &logFolder ),    "Logging folder" )
        ( "console", po::value< std::string >( &strConsoleLogLevel ),       "Console logging level" )
        ( "level",   po::value< std::string >( &strLogFileLevel ),          "Log file logging level" )
        
        ( "project", po::value< boost::filesystem::path >( &projectPath ),  "Project Path" )
        ;
        // clang-format on

        po::parsed_options parsedOptions = po::command_line_parser( argc, argv ).options( options ).run();

        po::variables_map vm;
        po::store( parsedOptions, vm );
        po::notify( vm );

        if ( bShowHelp )
        {
            std::cout << options << std::endl;
            return 0;
        }
        if ( projectPath.empty() )
        {
            std::cout << "Missing project path" << std::endl;
            return 0;
        }
    }

    auto logThreads = mega::network::configureLog( logFolder, "terminal", mega::network::fromStr( strConsoleLogLevel ),
                                                   mega::network::fromStr( strLogFileLevel ) );

    mega::runtime::initialiseRuntime( mega::network::Project( projectPath ) );

    
    auto foo = &mega::runtime::get_read;

    SPDLOG_INFO( "Initialised mega runtime with project {}", projectPath.string() );

    boost::shared_ptr< TestMega > pTest = boost::dll::import_symbol< TestMega >(
        "TestComponent", "mega_test", boost::dll::load_mode::append_decorations );

    int iResult = pTest->testFunction();
    SPDLOG_INFO( "Test function returned: {}", iResult );

    return 0;
}
