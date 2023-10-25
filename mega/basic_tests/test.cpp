
#include "mega/values/runtime/reference_io.hpp"
#include "mega/macros.hpp"

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

#endif

#pragma mega

TEST( BasicTests, Brick )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Brick() );
        auto brick = mega_new< Brick >();
        r.Brick( brick );
        ASSERT_TRUE( r.Brick() );
        auto socket = brick.testRefParam( brick );
        ASSERT_TRUE( socket );
        ASSERT_EQ( socket, brick.Socket.GET() );

        std::vector< Brick > bricks;
        for( int i = 0; i != 10; ++i )
        {
            bricks.push_back( mega_new< Brick >() );
            brick.OwnedBricks( bricks.back() );
        }
        auto i = bricks.begin();
        for( auto& b : brick.OwnedBricks() )
        {
            ASSERT_TRUE( b == *i );
            ++i;
        }
        r.Brick.CLEAR();
    }
}

TEST( BasicTests, ToasterReadAndWrite )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Toaster() );
        auto newToaster = mega_new< Toaster >();
        ASSERT_EQ( "", newToaster.m_version() );
        newToaster.m_version( "test" );
        ASSERT_EQ( "test", newToaster.m_version() );
        r.Toaster( newToaster );
        ASSERT_TRUE( r.Toaster() );
        ASSERT_EQ( "test", r.Toaster.m_version() );
        r.Toaster.m_version( "testing" );
        ASSERT_EQ( "testing", r.Toaster.m_version() );
        LOG( Info, "Toaster version is: " << r.Toaster.m_version() );
        r.Toaster.REMOVE( newToaster );
        ASSERT_FALSE( r.Toaster() );
    }
}

TEST( BasicTests, CreateAndRemoveToaster )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Toaster() );
        auto newToaster = mega_new< Toaster >();
        LOG( Info, "Root: " << r << " net: " << (&r)->getNetworkAddress() );
        LOG( Info, "Created toaster: " << newToaster << " net: " << (&newToaster)->getNetworkAddress() );

        r.Toaster( newToaster );
        ASSERT_TRUE( r.Toaster() );
        r.Toaster.REMOVE( newToaster );
        ASSERT_FALSE( r.Toaster() );
    }
}

TEST( BasicTests, ToasterOwner )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Toaster() );
        auto newToaster = mega_new< Toaster >();
        ASSERT_FALSE( newToaster.Owner() );
        r.Toaster( newToaster );
        ASSERT_TRUE( r.Toaster() );
        auto parent = newToaster.Owner();
        ASSERT_TRUE( parent );
        ASSERT_TRUE( parent == r );
        r.Toaster.REMOVE( newToaster );
        ASSERT_FALSE( r.Toaster() );
        ASSERT_FALSE( newToaster.Owner() );
    }
}

TEST( BasicTests, BrickOwner )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Brick() );
        auto newBrick = mega_new< Brick >();
        ASSERT_FALSE( newBrick.Owner() );
        r.Brick( newBrick );
        ASSERT_TRUE( r.Brick() );
        auto parent = newBrick.Owner();
        ASSERT_TRUE( parent );
        ASSERT_TRUE( parent == r );
        r.Brick.REMOVE( newBrick );
        ASSERT_FALSE( r.Brick() );
        ASSERT_FALSE( newBrick.Owner() );

        auto largeBrick = mega_new< LargeBrick >();
        ASSERT_FALSE( largeBrick.Owner() );
        newBrick.OwnedBricks( largeBrick );
        ASSERT_TRUE( largeBrick.Owner() );
        ASSERT_TRUE( largeBrick.Owner() == newBrick );

        largeBrick.Owner.CLEAR();
        ASSERT_FALSE( largeBrick.Owner() );
        ASSERT_TRUE( newBrick.OwnedBricks().empty() );
    }
}

TEST( BasicTests, ClearToaster )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Toaster() );
        r.Toaster( mega_new< Toaster >() );
        ASSERT_TRUE( r.Toaster() );
        r.Toaster.CLEAR();
        ASSERT_FALSE( r.Toaster() );
    }
}


TEST( BasicTests, DeleteToaster )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Toaster() );
        r.Toaster( mega_new< Toaster >() );
        ASSERT_TRUE( r.Toaster() );
        mega_delete( r.Toaster() );
        ASSERT_FALSE( r.Toaster() );
    }
}


TEST( BasicTests, RecreateToaster )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        ASSERT_FALSE( r.Toaster() );
        r.Toaster( mega_new< Toaster >() );
        r.Toaster( mega_new< Toaster >() );
        r.Toaster( mega_new< Toaster >() );
        ASSERT_TRUE( r.Toaster() );
        mega_delete( r.Toaster() );
        ASSERT_FALSE( r.Toaster() );
    }
    mega::Cycle cycle2;
}

TEST( BasicTests, LegoChain )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        auto b1 = mega_new< Brick >();
        r.Brick( b1 );

        auto b2 = mega_new< Brick >();
        b1.connect( b2 );

        auto b3 = mega_new< Brick >();
        b2.connect( b3 );

        ASSERT_TRUE( r.Brick() );
        ASSERT_FALSE( b1.OwnedBricks().empty() );
        ASSERT_FALSE( b2.OwnedBricks().empty() );
        ASSERT_TRUE( b3.OwnedBricks().empty() );

        ASSERT_TRUE( b1.Socket.AttachedPlug() == b2.Plug.GET() );
        ASSERT_TRUE( b2.Socket.AttachedPlug() == b3.Plug.GET() );

        ASSERT_TRUE( b2.Plug.AttachedSocket() == b1.Socket.GET() );
        ASSERT_TRUE( b3.Plug.AttachedSocket() == b2.Socket.GET() );

        {
            const auto& bricks = b2.OwnedBricks();
            LOG( Info, "Bricks size: " << bricks.size() );
            for( auto b : b2.OwnedBricks() )
            {
                LOG( Info, b );
            }
        }

        mega_delete( b2 );

        ASSERT_TRUE( b1.OwnedBricks().empty() );
        ASSERT_FALSE( b2.Socket.AttachedPlug() );
        ASSERT_FALSE( b3.Plug.AttachedSocket() );

        mega_delete( b1 );
    }
}

TEST( BasicTests, ManyLegos )
{
    Root        r = mega::Context::get()->getThisRoot();
    mega::Cycle cycle1;
    {
        auto b = mega_new< Brick >();
        r.Brick( b );

        for( int i = 0; i != 4; ++i )
        {
            auto b1 = mega_new< SmallBrick >();
            b.connect( b1 );

            for( int j = 0; j != 4; ++j )
            {
                auto b2 = mega_new< LargeBrick >();
                b1.connect( b2 );

                for( int k = 0; k != 4; ++k )
                {
                    auto b3 = mega_new< Brick >();
                    b2.connect( b3 );
                }
            }
        }

        r.Brick().print( 0 );

        mega_delete( b );
    }

}