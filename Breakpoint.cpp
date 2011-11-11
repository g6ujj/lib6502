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
    this->enabled=rhs.enabled;
    this->bVal=rhs.wVal;
    this->wVal=rhs.wVal;
    this->flags=rhs.flags;
    //assignment operator
    return *this;
}


bool Breakpoint::Equals(bpFlags_t flags, address addr, byte aByte)
{
    if((this->flags & bp_PC) && (flags & bp_PC) && this->wVal==addr)
        return true;
    if((this->flags & bp_mem) && (flags & bp_mem) &&(this->wVal == addr) && (this->bVal == aByte))
        return true;
    if((this->flags == flags) && (this->bVal == aByte))
        return true;
    return false;
}

bool Breakpoint::operator==(const Breakpoint &other) const
{
    if((this->flags == other.flags) && (this->bVal==other.bVal) &&(this->wVal==other.wVal))
        return true;
    return false;
}
bool Breakpoint::operator!=(const Breakpoint &other) const
{
    return !(*this == other);
}
