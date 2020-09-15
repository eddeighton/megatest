
#include <string>

#include "ed/ed.hpp"
#include "eg/macros.hpp"

void dumpEdFile( const std::string& strFilePath )
{
    try
    {
        Ed::BasicFileSystem basicFileSystem;
        Ed::File edFile( basicFileSystem, strFilePath );
        
        std::ostringstream os;
        {
            edFile.removeTypes();
            edFile.expandShorthand();
            Ed::Node nResult;
            edFile.toNode( nResult );
            os << nResult << "\n";
        }
        LOG( os.str() );
    }
    catch( std::exception& ex )
    {
        LOG( "Exception while loading ed file: " << ex.what() );
    }
}
