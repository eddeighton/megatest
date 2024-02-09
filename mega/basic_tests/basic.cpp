#include <iostream>
#include <vector>
#include <string>
#include <sstream>

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
#ifndef MEGA_CLANG_COMPILATION
#include "mega/include.hpp"
#define function auto
#define action mega::ActionCoroutine
#endif
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

#pragma mega

#include "mega/test.hpp"

using namespace std::string_literals;

namespace
{
std::string makeName( const std::string& str )
{
    std::ostringstream os;
    os << str << "_stuff23456";
    return os.str();
}
} // namespace

function SmallBrick::getName()->std::string
{
    // BrickUtils u( *this );
    // return makeName( u.test() );
    return "testing"s;
}

function LargeBrick::getName()->std::string
{
    return makeName( "LargeBrick"s );
}

namespace Brick::Plug::SubSystem1::Sub1
{
action A1()
{
    co_return mega::complete();
}
action A2()
{
    co_return mega::complete();
}
} // namespace Brick::Plug::SubSystem1::Sub1

namespace Brick::Socket
{

action Idle()
{
    co_return mega::complete();
}

function AFunctionWithArgsAndReturnType( ^Brick brick )->std::string
{

    //Brick.m_name();

    // Read m_name through the brick mega pointer
    // using the Brick.name type path and the no params () operation
    brick.Brick.m_name();

    return "";
}

action DoStuff()
{
    co_return mega::complete();
}

namespace A::B::C
{
void foo()
{
    struct C
    {
    };
}

} // namespace A::B::C

namespace D::E::F
{

void foo()
{
    struct C
    {
    };
}

} // namespace D::E::F
} // namespace Brick::Socket
