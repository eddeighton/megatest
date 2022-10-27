
#include "mega/test_component/test.hpp"

#include "service/network/log.hpp"
#include "service/network/network.hpp"

#include "service/tool.hpp"
#include "service/api.hpp"

#include "boost/asio/spawn.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include <spdlog/stopwatch.h>

#include <iostream>
#include <string>

int main( int argc, const char* argv[] )
{
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
    }

    auto logThreads = mega::network::configureLog( logFolder, "terminal", mega::network::fromStr( strConsoleLogLevel ),
                                                   mega::network::fromStr( strLogFileLevel ) );

    {
        mega::service::Tool tool( mega::network::MegaDaemonPort() );
        try
        {
            mega::service::Tool::Functor functor = []( boost::asio::yield_context& yield_ctx )
            {
                const std::string strResult = test3();
                SPDLOG_INFO( "Test function returned: {}", strResult );
            };
            tool.run( functor );
        }
        catch ( std::exception& ex )
        {
            std::cout << "Exception: " << ex.what() << std::endl;
            return 1;
        }
    }
    return 0;
}
