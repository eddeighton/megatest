
#include <string>

#include "ed/ed.hpp"
#include "eg/macros.hpp"

#include "blueprint/factory.h"

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

void dumpBlueprint( const std::string& strFilePath )
{
    try
    {
        Blueprint::Factory factory;
        Blueprint::Site::Ptr pNewBlueprint = 
            factory.load( strFilePath );
        
        const Blueprint::Site::PtrVector& sites = 
            pNewBlueprint->getSpaces();
        for( Blueprint::Site::Ptr pSite : sites )
        {
            LOG( "Site: " << pSite->Blueprint::Node::getName() );
        }
        
        
    }
    catch( std::exception& ex )
    {
        LOG( "Exception while loading ed file: " << ex.what() );
    }
}
/*
static float densityFunction( const UT_Vector3 &P )
{
    return 1 - P.length(); // Return signed distance to unit sphere
}

void testHoudini( const std::string& strFilePath )
{
    GU_Detail            gdp;
    UT_BoundingBox       bounds;
    
    bounds.setBounds( -1, -1, -1, 1, 1, 1 );
    gdp.polyIsoSurface( HDK_Sample::densityFunction, bounds, 20, 20, 20 );
    gdp.save( strFilePath.c_str(), true, NULL );
    
    LOG( "Saved geo file to: " << strFilePath );
}*/
void testHoudini( const std::string& strFilePath )
{
    LOG( "testHoudini: " << strFilePath );
}