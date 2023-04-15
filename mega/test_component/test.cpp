
#include "test.hpp"

#include "mega/reference_io.hpp"
#include "mega/macros.hpp"

#include "service/protocol/common/context.hpp"
#include "service/cycle.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

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

TEST( TestProg, ThisShouldNOTCompile )
{
    Root r = mega::Context::get()->getThisRoot();
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    r.BadSymbolThisSHuldNOTWork();

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

TEST( TestProg, ZeroToMany_OneToOne )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();
    {
        mega::reference ref = r;
        ASSERT_EQ( ref.getType(), mega::ROOT_TYPE_ID ) << "Root is wrong: " << ref;
        ASSERT_TRUE( ref.is_valid() );
    }
    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );

    ObjZeroToMany_OneToOne t1 = r.Parent_ZeroToMany_OneToOne.ObjZeroToMany_OneToOne();
    ASSERT_TRUE( ( ( mega::reference )t1 ).is_valid() );

    auto theList = r.Parent_ZeroToMany_OneToOne();
    ASSERT_EQ( theList.size(), 1 );
    ASSERT_EQ( theList.front(), t1.Child_ZeroToMany_OneToOne.Get() );

    r.Parent_ZeroToMany_OneToOne( WriteOperation::REMOVE, t1.Child_ZeroToMany_OneToOne.Get() );

    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
}

TEST( TestProg, ZeroToMany_OneToOne_Many )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();
    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );

    std::vector< ObjZeroToMany_OneToOne > added;
    for( int i = 0; i != 10; ++i )
    {
        added.push_back( r.Parent_ZeroToMany_OneToOne.ObjZeroToMany_OneToOne() );
    }

    std::random_shuffle( added.begin(), added.end() );

    ASSERT_EQ( 10, added.size() );
    ASSERT_EQ( r.Parent_ZeroToMany_OneToOne().size(), added.size() );

    for( auto& t : added )
    {
        r.Parent_ZeroToMany_OneToOne( WriteOperation::REMOVE, t.Child_ZeroToMany_OneToOne.Get() );
    }

    ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
}

TEST( TestProg, ZeroToOne_OneToOne )
{
    mega::Cycle cycle;

    Root r = mega::Context::get()->getThisRoot();
    ASSERT_TRUE( ( ( mega::reference )r ).is_valid() );
    ObjChild_ZeroToOne_OneToOne t1 = r.Parent_ZeroToOne_OneToOne.ObjChild_ZeroToOne_OneToOne();
    ASSERT_TRUE( ( ( mega::reference )t1 ).is_valid() );

    r.Parent_ZeroToOne_OneToOne( WriteOperation::REMOVE, t1.Child_ZeroToOne_OneToOne.Get() );
    ObjChild_ZeroToOne_OneToOne t2 = r.Parent_ZeroToOne_OneToOne();
    ASSERT_TRUE( !( ( mega::reference )t2 ).is_valid() );
}
