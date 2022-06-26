
#include "mega/test_component/test.hpp"

#include "boost/dll/shared_library.hpp"
#include "boost/dll/import.hpp"
#include "boost/shared_ptr.hpp"

#include <iostream>

int main( int argc, const char* argv[] )
{
    std::cout << "Hello World" << std::endl;

    boost::shared_ptr< TestMega > pTest = boost::dll::import_symbol< TestMega >(
        "TestComponent", "mega_test", boost::dll::load_mode::append_decorations );

    int iResult = pTest->testFunction();
    std::cout << "Got: " << iResult << std::endl;

    return 0;
}
