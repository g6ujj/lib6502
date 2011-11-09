#include "Breakpoint.h"


Breakpoint::~Breakpoint()
{
    //dtor
}

//Breakpoint::Breakpoint(const Breakpoint& other)
//{
//    //copy ctor
//}

Breakpoint& Breakpoint::operator=(const Breakpoint& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

Breakpoint::Breakpoint(address addr)
{
    this->wVal=addr;
    this->flags = bp_PC | bp_equal;
    this->enabled = true;
}
