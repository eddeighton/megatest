
#include "mega/test_component/test.hpp"

#include "service/network/log.hpp"
#include "service/network/network.hpp"

#include "service/tool.hpp"

#include "/workspace/root/src/mega/src/tests/edsUnitTestWrapper.hpp"

#include "boost/asio/spawn.hpp"
#include "boost/program_options.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"

#include <spdlog/stopwatch.h>

#include <gtest/gtest.h>

#include <iostream>
#include <string>

int main( int argc, const char* argv[] )
{
    std::string             strConsoleLogLevel = "info", strLogFileLevel = "debug", strTest;
    boost::filesystem::path logFolder = boost::filesystem::current_path() / "log";
    {
        namespace po                      = boost::program_options;
        bool                    bShowHelp = false;
        po::options_description options( "General" );

        // clang-format off
        std::string strHostName;
        options.add_options()
        ( "help,?",         po::bool_switch( &bShowHelp ),                         "Show Command Line Help" )
        ( "log",            po::value< boost::filesystem::path >( &logFolder ),    "Logging folder" )
        ( "console",        po::value< std::string >( &strConsoleLogLevel ),       "Console logging level" )
        ( "level",          po::value< std::string >( &strLogFileLevel ),          "Log file logging level" )
        ( "gtest_filter",   po::value< std::string >( &strTest ),                  "GTest Test Filter" )
        ;
        // clang-format on

        po::parsed_options parsedOptions = po::command_line_parser( argc, argv ).options( options ).run();

        po::variables_map vm;
        po::store( parsedOptions, vm );
        po::notify( vm );

        if( bShowHelp )
        {
            std::cout << options << std::endl;
            return 0;
        }
    }

    mega::U64 szResult = 0U;

    mega::network::configureLog( logFolder, "test_prog", mega::network::fromStr( strConsoleLogLevel ),
                                 mega::network::fromStr( strLogFileLevel ) );

    {
        mega::service::Tool tool( mega::network::MegaDaemonPort() );
        try
        {
            mega::service::Tool::Functor functor = [ &szResult, strTest ]( boost::asio::yield_context& yield_ctx )
            {
                std::unique_ptr< EDUTS::UnitTestResultWrapper > results;
                try
                {
                    EDUTS::UnitTestWrapper test( EDUTS::UnitTestOptions( false, false, 1, strTest.c_str(), "" ) );
                    szResult = test.run();
                    results = test.getResult();
                }
                catch( std::runtime_error& e )
                {
                    std::cout << "Encountered exception: " << e.what() << std::endl;
                }
                catch( ... )
                {
                    std::cout << "Encountered unknown exception" << std::endl;
                }

            };
            tool.run( functor );
        }
        catch( std::exception& ex )
        {
            std::cout << "Exception: " << ex.what() << std::endl;
            return 1;
        }
    }
    return 0;
}
