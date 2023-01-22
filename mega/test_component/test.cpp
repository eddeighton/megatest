
#include "test.hpp"

#include "mega/reference_io.hpp"
#include "mega/macros.hpp"

#include "service/protocol/common/context.hpp"
#include "service/cycle.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

template < typename T >
void print( const T& dur, std::ostream& os )
{
    using DurationType = std::chrono::duration< mega::I64, std::ratio< 1, 1'000'000'000 > >;
    auto c             = std::chrono::duration_cast< DurationType >( dur ).count();
    auto sec           = ( c % 1'000'000'000'000 ) / 1'000'000'000;
    auto ms            = ( c % 1'000'000'000 ) / 1'000'000;
    auto us            = ( c % 1'000'000 ) / 1'000;
    os << sec << "." << std::setw( 3 ) << std::setfill( '0' ) << ms << "ms." << std::setw( 3 ) << std::setfill( '0' )
       << us << "us";
}

#pragma mega

/*
#ifndef MEGA_COMPILATION

struct Root
{
    Root( mega::reference );

    int m_testDimension();
    Root m_testDimension( int );
};

#endif
*/

/*
std::string test1()
{
    mega::Cycle cycle;
    {
        Root root = mega::Context::get()->getThisRoot();

        root.m_testDimension( 4 );
        const int iValue1 = root.m_testDimension();

        root.m_testDimension1( 5 );
        const int iValue2 = root.m_testDimension1();

        std::ostringstream os;
        os << iValue1 << ", " << iValue2 << " : " << root.SomeFunction( iValue1, iValue2 );
        return os.str();
    }
}
*/
std::string test2()
{
    std::ostringstream os;
    /*for( auto machine : mega::Context::get()->getMachines() )
    {
        os << "\nFound machine: " << static_cast< mega::U32 >( machine );
        for( mega::MP machineProcess : mega::Context::get()->getProcesses( machine ) )
        {
            os << "\nFound machineProcess: " << machineProcess;
            for( mega::MPO mpo : mega::Context::get()->getMPO( machineProcess ) )
            {
                if( mpo == mega::Context::get()->getThisMPO() )
                {
                    os << "\nFound self mpo: " << mpo << std::endl;
                    Root root = mega::Context::get()->getThisRoot();
                    test::Square s = root.Square();
                    s.m_x( 123 );
                    os << "Created square with: " << s.m_x() << std::endl;
                }
                else
                {
                    os << "\nFound other mpo: " << mpo << std::endl;
                    auto start = std::chrono::steady_clock::now();

                    mega::Cycle cycle;
                    {
                        Root root = mega::Context::get()->getRoot( mpo );
                        // root.TestAction();

                        os << "Got root: " << root << " mpo:" << mpo << std::endl;
                        os << "before write: root.m_testDimension(): " << root.m_testDimension() << std::endl;
                        root.m_testDimension( root.m_testDimension() + 1 );
                        os << "after write:  root.m_testDimension(): " << root.m_testDimension() << std::endl;

                        os << "m_str before: " << root.m_str() << std::endl;
                        {
                            std::ostringstream os;
                            os << root.m_str() << " " << mpo;
                            root.m_str( os.str() );
                        }
                        os << "m_str after: " << root.m_str() << std::endl;
                    }

                    os << "Time: ";
                    print( std::chrono::duration_cast< std::chrono::steady_clock::duration >(
                               std::chrono::steady_clock::now() - start ),
                           os );
                }
            }
        }
    }*/
    return os.str();
}



std::string test3()
{
    std::ostringstream os;
    mega::Cycle cycle;
    {
        Root root = mega::Context::get()->getThisRoot();

        // allocate WallSocket
        WallSocket w = root.WallSocket();
        
        // link the wall socket to the root
        {
            root.SocketSurface( w.SocketInstall.Get() );
            for( auto v : root.SocketSurface() )
            {
                if( v == w.SocketInstall.Get() )
                {
                    os << "It works" << std::endl;
                }
            }
            if( w.SocketInstall() == root.SocketSurface.Get() )
            {
                os << "Also other way round!" << std::endl;
            }
        }

        // unlink
        {
            root.SocketSurface( WriteOperation::REMOVE, w.SocketInstall.Get() );
            if( root.SocketSurface().empty() )
            {
                os << "Non singular link side reset" << std::endl;
            }
            if( !w.SocketInstall() )
            {
                os << "Singular link reset" << std::endl;
            }
        }

        // link other way round
        {
            w.SocketInstall( root.SocketSurface.Get() );
            for( auto v : root.SocketSurface() )
            {
                if( v == w.SocketInstall.Get() )
                {
                    os << "It works again" << std::endl;
                }
            }
            if( w.SocketInstall() == root.SocketSurface.Get() )
            {
                os << "Also other way round again!" << std::endl;
            }
        }

        // unlink other way
        {
            w.SocketInstall( WriteOperation::REMOVE, root.SocketSurface.Get() );
            if( root.SocketSurface().empty() )
            {
                os << "Non singular link side reset" << std::endl;
            }
            if( !w.SocketInstall() )
            {
                os << "Singular link reset" << std::endl;
            }
        }

        //Root::SocketSurface rootSurface = w.SocketInstall();
        // int i = v.m_intValue();
       // os << "Read root.Sockets() : var.m_intValue()" << v <<  std::endl;
    }
    return os.str();
}

/*
std::string test4()
{
    mega::Cycle cycle;
    {
        Root root = mega::Context::get()->getThisRoot();

        std::ostringstream os;
        os << "root: " << root << std::endl;

        auto start = std::chrono::steady_clock::now();
        for ( int i = 0; i < 3; ++i )
        {
            root.m_testDimension( i );
            root.m_str( "Hello World" );

            LOG( Info, "Ed was here using macro 1" );
            LOG( Info, "Wrote value of: " << root.m_str() );
            LOG( Info, "root.m_testDimension: " << root.m_testDimension() );
        }
        os << "Time: ";
        using DurType = std::chrono::steady_clock::duration;
        print( std::chrono::duration_cast< DurType >( std::chrono::steady_clock::now() - start ), os );

        root.Save( "test.xml" );

        os << " Saved root to test.xml";
        return os.str();
    }
}
*/