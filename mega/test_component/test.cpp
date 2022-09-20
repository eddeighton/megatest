
#include "test.hpp"

#include "runtime/mpo_context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>

#pragma mega

std::string testFunction()
{
    /*
    {
        mega::network::ConversationID SimID = mega::MPOContext::get()->createSim();

        mega::Cycle cycle( mega::MPOContext::get()->getRoot() );

        Root simRoot = mega::MPOContext::get()->getRoot( SimID );

        simRoot.m_testDimension();
    }
*/

    {
        const mega::MPOContext::SimIDVector currentSimulations = mega::MPOContext::get()->getSimulationIDs();
        for ( const auto& simID : currentSimulations )
        {
            std::cout << "SimID: " << simID << std::endl;
        }
        if ( !currentSimulations.empty() )
        {
            const auto simID = currentSimulations.front();
            std::cout << "Attempting to acquire root of simID: " << simID << std::endl;
            Root root = mega::MPOContext::get()->getRoot( simID );
            root.m_testDimension( root.m_testDimension() + 1 );
            std::cout << "Acquired root of sim: " << currentSimulations.front()
                      << " and set m_testDimension to: " << root.m_testDimension() << std::endl;
        }
    }

    {
        Root root = mega::MPOContext::get()->getRoot();

        root.m_testDimension( 4 );
        const int iValue1 = root.m_testDimension();

        root.m_testDimension1( 5 );
        const int iValue2 = root.m_testDimension1();

        std::ostringstream os;
        os << iValue1 << ", " << iValue2 << " : " << root.SomeFunction( iValue1, iValue2 );
        return os.str();
    }
}
