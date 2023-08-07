
#include "mega/reference_io.hpp"
#include "mega/macros.hpp"
#include "mega/reference_io.hpp"

#include "jit/jit_exception.hpp"

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

#ifndef MEGA_CLANG_COMPILATION

struct Root;
struct ObjA
{
    ObjA() = default;
    ObjA( mega::reference );

    struct _Parent_ZeroToMany_OneToMany
    {
        std::vector< ObjA > operator()();

        struct _ObjZeroToMany_OneToOne
        {
            ObjA operator()();

        } ObjA;

        struct _Get
        {
            _Parent_ZeroToMany_OneToMany operator()();
        } Get;
    } Parent_ZeroToMany_OneToMany;

    struct _Child_ZeroToMany_OneToMany
    {
        struct _Move
        {
            void operator()( _Parent_ZeroToMany_OneToMany );
        } Move;
    } Child_ZeroToMany_OneToMany;
};

struct Root
{
    Root() = default;
    Root( mega::reference );

    struct _Parent_ZeroToMany_OneToOne
    {
        std::vector< ObjA > operator()();

        struct _ObjZeroToMany_OneToOne
        {
            ObjA operator()();

        } ObjA;

    } Parent_ZeroToMany_OneToOne;
};

static_assert( false, "This code should NOT be compiled" );

#endif

#pragma mega

TEST( MoveTests, InMPOMove )
{
    auto ctx = mega::Context::get();

    Root r = ctx->getThisRoot();

    {
        mega::Cycle cycle;
        ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
        ObjA objA1 = r.Parent_ZeroToMany_OneToOne.ObjA();
        ASSERT_FALSE( r.Parent_ZeroToMany_OneToOne().empty() );
        auto objA2 = objA1.Parent_ZeroToMany_OneToMany.ObjA();
        ASSERT_FALSE( objA1.Parent_ZeroToMany_OneToMany().empty() );
        auto objA3 = objA2.Parent_ZeroToMany_OneToMany.ObjA();
        ASSERT_FALSE( objA2.Parent_ZeroToMany_OneToMany().empty() );

        // reparent objA3 onto objA1
        objA3.Child_ZeroToMany_OneToMany.Move( objA1.Parent_ZeroToMany_OneToMany.Get() );
        ASSERT_TRUE( objA2.Parent_ZeroToMany_OneToMany().empty() );
        ASSERT_EQ( objA1.Parent_ZeroToMany_OneToMany().size(), 2 );

        // move it back onto objA2
        objA3.Child_ZeroToMany_OneToMany.Move( objA2.Parent_ZeroToMany_OneToMany.Get() );
        ASSERT_EQ( objA2.Parent_ZeroToMany_OneToMany().size(), 1 );
        ASSERT_EQ( objA1.Parent_ZeroToMany_OneToMany().size(), 1 );
    }
}

TEST( MoveTests, InMPOMoveRemote )
{
    auto ctx = mega::Context::get();

    auto thisMPO     = ctx->getThisMPO();
    auto newExecutor = ctx->constructExecutor( thisMPO.getMachineID() );
    auto remoteMPO   = ctx->constructMPO( newExecutor );
    Root r           = ctx->getRoot( remoteMPO );

    {
        mega::Cycle cycle;
        ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
        ObjA objA1 = r.Parent_ZeroToMany_OneToOne.ObjA();
        ASSERT_FALSE( r.Parent_ZeroToMany_OneToOne().empty() );
        auto objA2 = objA1.Parent_ZeroToMany_OneToMany.ObjA();
        ASSERT_FALSE( objA1.Parent_ZeroToMany_OneToMany().empty() );
        auto objA3 = objA2.Parent_ZeroToMany_OneToMany.ObjA();
        ASSERT_FALSE( objA2.Parent_ZeroToMany_OneToMany().empty() );

        // reparent objA3 onto objA1
        objA3.Child_ZeroToMany_OneToMany.Move( objA1.Parent_ZeroToMany_OneToMany.Get() );
        ASSERT_TRUE( objA2.Parent_ZeroToMany_OneToMany().empty() );
        ASSERT_EQ( objA1.Parent_ZeroToMany_OneToMany().size(), 2 );

        // move it back onto objA2
        objA3.Child_ZeroToMany_OneToMany.Move( objA2.Parent_ZeroToMany_OneToMany.Get() );
        ASSERT_EQ( objA2.Parent_ZeroToMany_OneToMany().size(), 1 );
        ASSERT_EQ( objA1.Parent_ZeroToMany_OneToMany().size(), 1 );
    }
    {
        mega::Cycle cycle;
        ASSERT_EQ( r.Parent_ZeroToMany_OneToOne().size(), 1 );
        ObjA objA1 = r.Parent_ZeroToMany_OneToOne()[ 0 ].ObjA.Get();
        ASSERT_EQ( objA1.Parent_ZeroToMany_OneToMany().size(), 1 );
        ObjA objA2 = objA1.Parent_ZeroToMany_OneToMany()[ 0 ].ObjA.Get();
        ASSERT_EQ( objA2.Parent_ZeroToMany_OneToMany().size(), 1 );
        ObjA objA3 = objA2.Parent_ZeroToMany_OneToMany()[ 0 ].ObjA.Get();
        ASSERT_EQ( objA3.Parent_ZeroToMany_OneToMany().size(), 0 );
    }

    // ctx->destroyExecutor( newExecutor );
}

template < typename T >
mega::reference toRef( const T& r )
{
    return static_cast< mega::reference >( r );
}

TEST( MoveTests, MoveInterMPO )
{
    auto ctx = mega::Context::get();

    auto thisMPO     = ctx->getThisMPO();
    auto newExecutor = ctx->constructExecutor( thisMPO.getMachineID() );
    auto m1          = ctx->constructMPO( newExecutor );
    Root r1          = ctx->getRoot( m1 );

    auto m2 = ctx->constructMPO( newExecutor );
    Root r2 = ctx->getRoot( m2 );

    ASSERT_EQ( toRef( r1 ).getMP(), toRef( r2 ).getMP() );
    ASSERT_NE( toRef( r1 ).getMPO().getOwnerID(), toRef( r2 ).getMPO().getOwnerID() );

    ObjA objectToMove;

    {
        mega::Cycle cycle;
        ASSERT_TRUE( r1.Parent_ZeroToMany_OneToOne().empty() );
        objectToMove = r1.Parent_ZeroToMany_OneToOne.ObjA();
        ASSERT_FALSE( r1.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_EQ( toRef( objectToMove ).getMPO(), toRef( r1 ).getMPO() );

        // ASSERT_THROW( objectToMove.Child_ZeroToMany_OneToMany.Move( remoteRoot.Parent_ZeroToMany_OneToOne.Get() ),
        //               mega::runtime::JITException );

        objectToMove.Child_ZeroToMany_OneToOne.Move( r2.Parent_ZeroToMany_OneToOne.Get() );
        // object immediately unparented from old parent
        ASSERT_TRUE( r1.Parent_ZeroToMany_OneToOne().empty() );
        // object not moved to remote root yet
        ASSERT_TRUE( r2.Parent_ZeroToMany_OneToOne().empty() );
    }

   {
        mega::Cycle cycle;

        // ASSERT_EQ( toRef( objectToMove ).getMPO(), toRef( r2 ).getMPO() );
        // ASSERT_NE( toRef( r1 ).getMPO(), toRef( r2 ).getMPO() );

        ASSERT_TRUE( r1.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_FALSE( r2.Parent_ZeroToMany_OneToOne().empty() );
    }

    // ctx->destroyExecutor( newExecutor );
}
