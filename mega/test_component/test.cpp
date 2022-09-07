
#include "test.hpp"

#include "mega/execution_context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>

#pragma mega

int testFunction()
{
    Root root = mega::ExecutionContext::get()->getRoot();

    int iValue = root.m_testDimension();

    /*mega::reference ref;
    {
        ref.physical.execution = pExecutionContext->getThisExecutionIndex();
        ref.physical.object    = 0;
        ref.typeID             = FloorSocket::ID;
        ref.instance           = 0;
        ref.physical.type      = mega::PHYSICAL_ADDRESS;
    }

    FloorSocket root{ ref };


    FloorSocket c = root.FloorSocket();
    //c.m_intValue();
    //c.m_intValue(123);

    return 123;//c.m_intValue();*/
    return 0;
}
