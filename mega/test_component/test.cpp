
#include "test.hpp"

#include <iostream>

#include "boost/config.hpp"

#pragma mega

class TestMegaImpl : public TestMega
{
public:
    virtual ~TestMegaImpl() {}

    virtual int testFunction()
    {
        test::Square s;
        int iResult = s.m_data();
        return iResult;
    }
};

extern "C" BOOST_SYMBOL_EXPORT TestMegaImpl mega_test;
TestMegaImpl                                mega_test;
