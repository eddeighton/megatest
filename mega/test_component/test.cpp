
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

std::string test0()
{
    std::ostringstream os;
    {
        mega::Cycle cycle;
        {
            for( auto machine : mega::Context::get()->getMachines() )
            {
                for( mega::MP machineProcess : mega::Context::get()->getProcesses( machine ) )
                {
                    for( mega::MPO mpo : mega::Context::get()->getMPO( machineProcess ) )
                    {
                        os << "\nFound MPO: " << mpo;
                        Root root = mega::Context::get()->getRoot( mpo );

                        root.m_iCounter( 3 );
                        os << "\nroot.m_iCounter: " << root.m_iCounter();
                        os << "\n" << root.testFunction();

                        /*FloorSocket f = root.SocketSurface.AdvancedFloorSocket();

                        int i = f.m_intValue();

                        Var< FloorSocket, WallSocket > v = f;
                        int i2 = v.m_intValue();

                        if( mega_cast< FloorSocket >( f ) )
                        {
                            os << "\nCast from FloorSocket to FloorSocket succeeded";
                        }
                        else
                        {
                            os << "\nCast from FloorSocket to FloorSocket failed";
                        }
                        if( mega_cast< AdvancedFloorSocket >( f ) )
                        {
                            os << "\nCast from FloorSocket to AdvancedFloorSocket succeeded";
                        }
                        else
                        {
                            os << "\nCast from FloorSocket to AdvancedFloorSocket failed";
                        }
                        if( mega_cast< WallSocket >( f ) )
                        {
                            os << "\nCast from FloorSocket to WallSocket succeeded";
                        }
                        else
                        {
                            os << "\nCast from FloorSocket to WallSocket failed";
                        }

                        root.doStuff();
                        os << "\nCalled doStuff on: " << root << "\n";*/
                    }
                }
            }
        }
    }
    return os.str();
}

std::string test1()
{
    std::ostringstream os;
    for( auto machine : mega::Context::get()->getMachines() )
    {
        os << "\nFound machine: " << static_cast< mega::U32 >( machine );
        for( mega::MP machineProcess : mega::Context::get()->getProcesses( machine ) )
        {
            os << "\nFound machineProcess: " << machineProcess;
            for( mega::MPO mpo : mega::Context::get()->getMPO( machineProcess ) )
            {
                Root root = mega::Context::get()->getRoot( mpo );

                std::ostringstream osFile;
                osFile << "mpo_" << mpo << ".xml";

                root.Save( osFile.str() );
                os << "\nSaved: " << mpo << " to: " << osFile.str();
            }
        }
    }
    return os.str();
}

std::string test2()
{
    std::ostringstream os;
    for( auto machine : mega::Context::get()->getMachines() )
    {
        os << "\nFound machine: " << static_cast< mega::U32 >( machine );
        for( mega::MP machineProcess : mega::Context::get()->getProcesses( machine ) )
        {
            os << "\nFound machineProcess: " << machineProcess;
            for( mega::MPO mpo : mega::Context::get()->getMPO( machineProcess ) )
            {
                if( mpo != mega::Context::get()->getThisMPO() )
                {
                    os << "\nFound other mpo: " << mpo << std::endl;
                    auto start = std::chrono::steady_clock::now();

                    mega::Cycle cycle;
                    {
                        Root root = mega::Context::get()->getRoot( mpo );
                        root.doStuff();
                        root.m_testDimension( root.m_testDimension() + 1 );
                        {
                            std::ostringstream os;
                            os << root.m_str() << " " << mpo;
                            root.m_str( os.str() );
                        }

                        // FloorSocket f = root.SocketSurface.FloorSocket();
                        // f.m_intValue( 123 );
                        // f.m_floatValue( 10.0f );

                        os << "Got root: " << root << " mpo:" << mpo << std::endl;
                        os << "after write:  root.m_testDimension(): " << root.m_testDimension() << std::endl;
                        os << "m_str after: " << root.m_str() << std::endl;
                    }

                    os << "Time: ";
                    print( std::chrono::duration_cast< std::chrono::steady_clock::duration >(
                               std::chrono::steady_clock::now() - start ),
                           os );
                }
            }
        }
    }
    return os.str();
}

std::string test3()
{
    std::ostringstream os;
    mega::Cycle        cycle;
    {
        Root root = mega::Context::get()->getThisRoot();

        // allocate WallSocket
        WallSocket w = root.SocketSurface.WallSocket();

        // create an aux system plugged into wallsocket
        AuxSystem aux = w.Socket.AuxSystem();

        {
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

        // link explicitly
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

        root.Save( "test_linked.xml" );

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

        root.Save( "test_unlinked.xml" );
    }
    return os.str();
}

std::string test4()
{
    std::ostringstream os;
    mega::Cycle        cycle;
    {
        Root root = mega::Context::get()->getThisRoot();
        root.m_int( 123 );
        root.m_float( 3.21f );

        WallSocket otherWS = root.SocketSurface.WallSocket();

        FloorSocket                fTemp;
        std::vector< FloorSocket > floorSockets;
        floorSockets.push_back( fTemp );
        for( int i = 0; i < 4; ++i )
        {
            // allocate WallSocket
            FloorSocket f = root.SocketSurface.FloorSocket();
            f.m_intValue( i );
            f.m_floatValue( 10.0f );

            if( !fTemp )
                fTemp = f;
            floorSockets.push_back( f );

            // create an aux system plugged into wallsocket
            AuxSystem aux = f.Socket.AuxSystem();
            aux.m_floatValue( 0.2f );
        }

        for( int i = 0; i < 4; ++i )
        {
            // allocate WallSocket
            WallSocket w = root.SocketSurface.WallSocket();
            w.m_intValue( i * 2 );
            if( i % 2 )
            {
                w.m_FloorSocketRef( fTemp );
                w.m_floorSocketVector( floorSockets );
            }

            // create an aux system plugged into wallsocket
            AuxSystem aux = w.Socket.AuxSystem();
            aux.m_floatValue( 0.3f );
            // aux.m_otherWallSocket( otherWS );
        }
        root.Save( "test4.xml" );
    }
    return os.str();
}

std::string test5()
{
    std::ostringstream os;
    mega::Cycle        cycle;
    {
        Root root = mega::Context::get()->getThisRoot();
        root.Load( "test4.xml" );
        root.Save( "test5.xml" );
    }
    return os.str();
}

std::string test6()
{
    std::ostringstream os;
    mega::Cycle        cycle;
    {
        Root root = mega::Context::get()->getThisRoot();

        Root::doStuff d = root.doStuff();

        d.Stop();
    }
    return os.str();
}