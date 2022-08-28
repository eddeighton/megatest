
#include "test.hpp"

#include <vector>
#include <iostream>

#pragma mega

int testFunction()
{
    Cube3 root;
    //Cube3 c = root.Cube3();
    //std::cout << "Created Cube3: "  << std::endl;
    return root.m_value();

    //std::cout << "No invocation" << std::endl;
    //return 345;
}
