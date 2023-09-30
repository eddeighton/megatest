
#include "mega/reference_io.hpp"
#include "mega/macros.hpp"
#include "mega/reference_io.hpp"

#include "service/protocol/common/context.hpp"
#include "service/cycle.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>

// #ifndef MEGA_CLANG_COMPILATION

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

TEST( BasicTests, ThisShouldNOTCompile )
{
    Root r = mega::Context::get()->getThisRoot();

    mega::Cycle cycle1;
    {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // r.BadSymbolThisSHuldNOTWork( 123 );

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        for( int i = 0; i != 10; ++i )
        {
            auto result = r.TestCallee( i, 234 );

            LOG( Info, "Got result of: " << result );

            LOG( Info, r.m_U16() );
            LOG( Info, r.m_I16() );
            LOG( Info, r.m_U32() );
            LOG( Info, r.m_I32() );
            LOG( Info, r.m_U64() );
            LOG( Info, r.m_I64() );
            LOG( Info, r.m_F32() );
            LOG( Info, r.m_F64() );
            LOG( Info, r.m_TimeStamp() );

            // all user defined supported types
            LOG( Info, r.m_reference() );
            //LOG( Info, r.m_vector_reference() );
            LOG( Info, r.m_string() );
            //LOG( Info, r.m_vector_I32() );

            // math types
            /*LOG( Info, r.m_F2() );
            LOG( Info, r.m_F3() );
            LOG( Info, r.m_F4() );
            LOG( Info, r.m_Quat() );
            LOG( Info, r.m_F33() );*/
        }
    }
}
/*
TEST( BasicTests, Delete )
{
    using namespace mega::log::Structure;

    auto& log = mega::Context::get()->getLog();
    auto  t   = log.getTimeStamp();

    {
        Root                   r = mega::Context::get()->getThisRoot();
        ObjA objects[ 4 ];

        {
            mega::Cycle cycle;
            objects[ 0 ] = r.Parent_ZeroToMany_OneToOne.ObjA();
            objects[ 1 ] = objects[ 0 ].Parent_ZeroToMany_OneToMany.ObjA();
            objects[ 2 ] = objects[ 1 ].Parent_ZeroToMany_OneToMany.ObjA();
            objects[ 3 ] = objects[ 2 ].Parent_ZeroToMany_OneToMany.ObjA();
        }

        r.Parent_ZeroToMany_OneToOne( WriteOperation::REMOVE, objects[ 0 ].Child_ZeroToMany_OneToOne.Get() );
    }

    for( int j = 0; j != 8; ++j )
    {
        {
            mega::Cycle cycle;
        }
    }

    int iTotalDestructs = 0;
    for( auto i = log.begin< Read >( t ); i != log.end< Read >(); ++i )
    {
        const Read& record = *i;
        if( record.getType() == mega::log::Structure::eDestruct )
        {
            ++iTotalDestructs;
        }
    }
    ASSERT_EQ( iTotalDestructs, 4 );
}

TEST( BasicTests, SaveNested )
{
    mega::Cycle cycle;
    Root        r = mega::Context::get()->getThisRoot();

    ObjB t1 = r.Parent_ZeroToOne_OneToOne.ObjB();
    t1.m_string( "test nested object" );

    r.Save( "TestProg_SaveNested.xml" );
    {
        r.Parent_ZeroToOne_OneToOne( WriteOperation::REMOVE, t1.Child_ZeroToOne_OneToOne.Get() );
        ObjB t2 = r.Parent_ZeroToOne_OneToOne();
        ASSERT_TRUE( !( ( mega::reference )t2 ).valid() );
    }

    r.Load( "TestProg_SaveNested.xml" );
    {
        ObjB t2 = r.Parent_ZeroToOne_OneToOne();
        ASSERT_TRUE( ( ( mega::reference )t2 ).valid() );
        ASSERT_EQ( t2.m_string(), "test nested object" );
    }
}

TEST( BasicTests, ZeroToMany_OneToOne )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();
    {
        mega::reference ref = r;
        ASSERT_EQ( ref.getType(), mega::ROOT_TYPE_ID ) << "Root is wrong: " << ref;
        ASSERT_TRUE( ref.valid() );
    }
    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );

    ObjA t1 = r.Parent_ZeroToMany_OneToOne.ObjA();
    ASSERT_TRUE( ( ( mega::reference )t1 ).valid() );

    auto theList = r.Parent_ZeroToMany_OneToOne();
    ASSERT_EQ( theList.size(), 1 );
    ASSERT_EQ( theList.front(), t1.Child_ZeroToMany_OneToOne.Get() );

    r.Parent_ZeroToMany_OneToOne( WriteOperation::REMOVE, t1.Child_ZeroToMany_OneToOne.Get() );

    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
}

TEST( BasicTests, ZeroToMany_OneToOne_Many )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();
    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );

    std::vector< ObjA::Child_ZeroToMany_OneToOne > added;
    for( int i = 0; i != 10; ++i )
    {
        ObjA result = r.Parent_ZeroToMany_OneToOne.ObjA();
        added.push_back( result.Child_ZeroToMany_OneToOne.Get() );
    }
    ASSERT_EQ( 10, added.size() );
    ASSERT_EQ( r.Parent_ZeroToMany_OneToOne(), added );

    {
        std::random_device rd;
        std::mt19937       g( rd() );
        std::shuffle( added.begin(), added.end(), g );
    }

    for( auto& t : added )
    {
        r.Parent_ZeroToMany_OneToOne( WriteOperation::REMOVE, t );
    }

    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
}

TEST( BasicTests, ZeroToOne_OneToOne )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();
    ASSERT_TRUE( ( ( mega::reference )r ).valid() );
    ObjB t1 = r.Parent_ZeroToOne_OneToOne.ObjB();
    ASSERT_TRUE( ( ( mega::reference )t1 ).valid() );

    r.Parent_ZeroToOne_OneToOne( WriteOperation::REMOVE, t1.Child_ZeroToOne_OneToOne.Get() );
    ObjB t2 = r.Parent_ZeroToOne_OneToOne();
    ASSERT_TRUE( !( ( mega::reference )t2 ).valid() );
}

TEST( BasicTests, Reset )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();

    std::vector< ObjA::Child_ZeroToMany_OneToOne > added;
    for( int i = 0; i != 10; ++i )
    {
        ObjA result = r.Parent_ZeroToMany_OneToOne.ObjA();
        added.push_back( result.Child_ZeroToMany_OneToOne.Get() );
    }
    ASSERT_EQ( 10, added.size() );
    ASSERT_EQ( r.Parent_ZeroToMany_OneToOne(), added );

    r.Parent_ZeroToMany_OneToOne( WriteOperation::RESET );
    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
}
*/