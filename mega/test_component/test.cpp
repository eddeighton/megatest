
#include "test.hpp"

#include "mega/reference_io.hpp"

#include "runtime/mpo_context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>

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

std::string testFunction()
{
    std::ostringstream os;
    for ( auto machine : mega::Context::get()->getMachines() )
    {
        for ( auto machineProcess : mega::Context::get()->getProcesses( machine ) )
        {
            for ( auto mpo : mega::Context::get()->getMPO( machineProcess ) )
            {
                if( mpo != mega::Context::get()->getThisMPO() )
                {
                    os << "\nFound other MPO: " << mpo;
                    Root root = mega::Context::get()->getRoot( mpo );
                    const int iValue1 = root.m_testDimension();
                    root.m_testDimension( iValue1 + 1 );
                    std::cout << "Value set to: " << root.m_testDimension() << std::endl;
                }
            }
        }
    }
    return os.str();
}
