
#include <iostream>

struct Brick
{
    void connect( @Brick brick ) {}

    @Brick.Socket testRefParam( @Brick brick ) { return brick.Socket.GET(); }

    int test( @Brick brick )
    {
        // LOG( Info, "From test: " << i );
        return 123;
    }

    void print( int indent )
    {
        std::ostringstream os;
        for( int i = 0; i != indent; ++i )
            os << ' ';
        if( Socket.AttachedPlug() )
        {
            os << getName() << " -> " << Socket.AttachedPlug.getName();
        }
        else
        {
            os << getName();
        }
        LOG( Info, os.str() );
        for( auto b : OwnedBricks() )
        {
            b.print( indent + 2 );
        }
    }

    std::string getName() { return "Brick"; }
};
