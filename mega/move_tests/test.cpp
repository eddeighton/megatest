

#include "mega/values/runtime/reference_io.hpp"
#include "mega/macros.hpp"

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

static_assert( false, "This code should NOT be compiled" );

#endif

#pragma mega
/*
TEST( MoveTests, LocalMove )
{
    auto ctx = mega::Context::get();

    Root r = ctx->getThisRoot();

    {
        mega::Cycle cycle;
        // create tree of root->ObjA->ObjA->ObjA
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

TEST( MoveTests, SimMove )
{
    auto ctx = mega::Context::get();

    auto thisMPO     = ctx->getThisMPO();
    auto newExecutor = ctx->constructExecutor( thisMPO.getMachineID() );
    auto remoteMPO1  = ctx->constructMPO( newExecutor );
    auto remoteMPO2  = ctx->constructMPO( newExecutor );
    Root root1       = ctx->getRoot( remoteMPO1 );
    Root root2       = ctx->getRoot( remoteMPO2 );

    ObjA root1ObjA, root1ObjAChild, root2ObjA;

    {
        mega::Cycle cycle;

        // create obja on root1 with child obja
        root1ObjA      = root1.Parent_ZeroToMany_OneToOne.ObjA();
        root1ObjAChild = root1ObjA.Parent_ZeroToMany_OneToMany.ObjA();
        root2ObjA      = root2.Parent_ZeroToMany_OneToOne.ObjA();
    }

    {
        // test
        mega::Cycle cycle;

        ASSERT_FALSE( root1.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_FALSE( root2.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_FALSE( root1ObjA.Parent_ZeroToMany_OneToMany().empty() );
        ASSERT_TRUE( root2ObjA.Parent_ZeroToMany_OneToMany().empty() );
    }

    {
        mega::Cycle cycle;
        // issue move via action

        root1ObjA.MoveTest.m_from( root1ObjAChild );
        root1ObjA.MoveTest.m_to( root2ObjA );
        root1ObjA.MoveTest();
    }

    {
        mega::Cycle cycle;
    }
    {
        // test
        mega::Cycle cycle;

        ASSERT_FALSE( root1.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_FALSE( root2.Parent_ZeroToMany_OneToOne().empty() );
        ASSERT_TRUE( root1ObjA.Parent_ZeroToMany_OneToMany().empty() );
        ASSERT_FALSE( root2ObjA.Parent_ZeroToMany_OneToMany().empty() );
    }

    ctx->destroyExecutor( newExecutor );
}*/

/*
TEST( MoveTests, RemoteMove )
{
    auto ctx = mega::Context::get();

    auto thisMPO     = ctx->getThisMPO();
    auto newExecutor = ctx->constructExecutor( thisMPO.getMachineID() );
    auto remoteMPO   = ctx->constructMPO( newExecutor );
    Root remoteRoot          = ctx->getRoot( remoteMPO );

    {
        mega::Cycle cycle;

        // create remoteRoot->ObjA->ObjA->Obja
        ASSERT_TRUE( remoteRoot.Parent_ZeroToMany_OneToOne().empty() );
        ObjA objA1 = remoteRoot.Parent_ZeroToMany_OneToOne.ObjA();
        ASSERT_FALSE( remoteRoot.Parent_ZeroToMany_OneToOne().empty() );
        auto objA2 = objA1.Parent_ZeroToMany_OneToMany.ObjA();
        ASSERT_FALSE( objA1.Parent_ZeroToMany_OneToMany().empty() );
        auto objA3 = objA2.Parent_ZeroToMany_OneToMany.ObjA();
        ASSERT_FALSE( objA2.Parent_ZeroToMany_OneToMany().empty() );

        // reparent objA3 onto objA1 using Move
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
        ASSERT_EQ( remoteRoot.Parent_ZeroToMany_OneToOne().size(), 1 );
        ObjA objA1 = remoteRoot.Parent_ZeroToMany_OneToOne()[ 0 ].ObjA.Get();
        ASSERT_EQ( objA1.Parent_ZeroToMany_OneToMany().size(), 1 );
        ObjA objA2 = objA1.Parent_ZeroToMany_OneToMany()[ 0 ].ObjA.Get();
        ASSERT_EQ( objA2.Parent_ZeroToMany_OneToMany().size(), 1 );
        ObjA objA3 = objA2.Parent_ZeroToMany_OneToMany()[ 0 ].ObjA.Get();
        ASSERT_EQ( objA3.Parent_ZeroToMany_OneToMany().size(), 0 );
    }

    ctx->destroyExecutor( newExecutor );
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
*/
