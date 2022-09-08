
#include "test.hpp"

#include "mega/execution_context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>

#pragma mega

int testFunction()
{
    Root root = mega::ExecutionContext::get()->getRoot();

    root.m_testDimension( 1 );
    const int iValue1 = root.m_testDimension();   

    root.m_testDimension( 2 );
    const int iValue2 = root.m_testDimension();
    
    return iValue1 + iValue2;
}
