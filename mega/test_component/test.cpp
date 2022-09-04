
#include "test.hpp"

#include "mega/execution_context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>

#pragma mega

int testFunction()
{
    mega::ExecutionContext* pExecutionContext = mega::ExecutionContext::execution_get();
    VERIFY_RTE( pExecutionContext );

    mega::reference ref;
    {
        ref.physical.execution = pExecutionContext->getThisExecutionIndex();
        ref.physical.object    = 0;
        ref.typeID             = FloorSocket::ID;
        ref.instance           = 0;
        ref.physical.type      = mega::PHYSICAL_ADDRESS;
    }

    FloorSocket root{ ref };

    FloorSocket c = root.FloorSocket();

    return 0;
}
