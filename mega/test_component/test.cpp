
#include "test.hpp"

#include "boost/config.hpp"

#include <vector>
#include <iostream>

#pragma mega

int testFunction()
{
    Cube3 root;
    
    Cube3 c = root.Cube3();

    std::cout << "Created Cube3: "  << std::endl;

    return c.m_value();
}
