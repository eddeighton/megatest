
#include "test.hpp"

#include "mega/reference_io.hpp"

#include "runtime/context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>

#pragma mega

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

std::string testFunction()
{
    std::ostringstream os;
    for ( auto machine : mega::Context::get()->getMachines() )
    {
        for ( auto machineProcess : mega::Context::get()->getProcesses( machine ) )
        {
            for ( auto mpo : mega::Context::get()->getMPO( machineProcess ) )
            {
                if ( mpo != mega::Context::get()->getThisMPO() )
                {
                    auto start = std::chrono::steady_clock::now();
                    // sw.reset();
                    os << "\nFound other MPO: " << mpo << "\n";

                    mega::Cycle cycle;
                    {
                        Root root = mega::Context::get()->getRoot( mpo );
                        root.TestAction();

                        std::cout << "Started TestAction" << std::endl;
                        root.m_testDimension( root.m_testDimension() + 1 );
                    }

                    /*for ( int i = 0; i < 1000; ++i )
                    {
                        root.m_testDimension( root.m_testDimension() + 1 );
                    }*/

                    os << "Time: ";
                    print( std::chrono::duration_cast< std::chrono::steady_clock::duration >(
                               std::chrono::steady_clock::now() - start ),
                           os );

                    return os.str();
                }
            }
        }
    }
    return os.str();
}
