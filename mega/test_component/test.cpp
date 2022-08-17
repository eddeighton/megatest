
#include "test.hpp"

#include "boost/config.hpp"

#include "boost/lockfree/stack.hpp"

#include <vector>
#include <iostream>

#pragma mega
/*
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
*/
// extern "C" BOOST_SYMBOL_EXPORT TestMegaImpl mega_test;
// extern TestMegaImpl                                mega_test;

struct Foobar
{
    int data;
};

boost::lockfree::stack< int > freeList;

std::vector< Foobar > allocation;

Foobar* allocate( int& index )
{
    if ( freeList.empty() )
    {
        index = allocation.size();
        allocation.push_back( Foobar{} );
    }
    else
    {
        freeList.pop( index );
    }
    return &allocation[ index ];
}

void free( int index, Foobar* pFoobar )
{
    pFoobar->Foobar::~Foobar();
    freeList.push( index );
}

int testFunction()
{
    int     index1 = 0;
    int     index2 = 0;
    int     index3 = 0;
    Foobar* p1     = allocate( index1 );
    Foobar* p2     = allocate( index2 );
    Foobar* p3     = allocate( index3 );

    free( index2, p2 );
    free( index1, p1 );
    p1 = allocate( index1 );
    p2 = allocate( index2 );

    test::Square s;
    int          iResult = s.m_data() + s.m_state();
    return iResult;
}
