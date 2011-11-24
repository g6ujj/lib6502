/*
 Breakpoint.cpp

 Author:
      Neil Stoker <gg6ujj@gmail.com>

 Copyright (c) 2011 Neil Stoker

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
