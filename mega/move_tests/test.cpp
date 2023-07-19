
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

    //ctx->destroyExecutor( newExecutor );
}
/*
TEST( MoveTests, SimpleMove )
{
    auto ctx = mega::Context::get();

    auto thisMPO     = ctx->getThisMPO();
    auto newExecutor = ctx->constructExecutor( thisMPO.getMachineID() );
    auto remoteMPO   = ctx->constructMPO( newExecutor );
    Root remoteRoot  = ctx->getRoot( remoteMPO );
    Root r           = ctx->getThisRoot();

    ObjA objectToMove;

    {
        mega::Cycle cycle;
        ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
        objectToMove = r.Parent_ZeroToMany_OneToOne.ObjA();
        ASSERT_FALSE( r.Parent_ZeroToMany_OneToOne().empty() );
        objectToMove.Move( remoteRoot );
        // object still in same place for this cycle
        ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
    }

    {
        mega::Cycle cycle;
        ASSERT_TRUE( r.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_FALSE( remoteRoot.Parent_ZeroToMany_OneToOne().empty() );
    }

    ctx->destroyExecutor( newExecutor );
}
*/