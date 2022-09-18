
#include "test.hpp"

#include "mega/mpo_context.hpp"

#include "common/assert_verify.hpp"

#include <vector>
#include <iostream>

#pragma mega

std::string testFunction()
{
    Root root = mega::MPOContext::get()->getRoot();

    root.m_testDimension( 0 );
    const int iValue1 = root.m_testDimension();   

    root.m_testDimension1( 5 );
    const int iValue2 = root.m_testDimension1();


    const std::string strResult = root.SomeFunction( iValue1, iValue2 );
    
    return strResult;
}
