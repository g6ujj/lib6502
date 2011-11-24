/*
 BreakpointManger.cpp

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

#include "BreakpointManager.h"



BreakpointManager::~BreakpointManager()
{
    //dtor
}

BreakpointManager& BreakpointManager::operator=(const BreakpointManager& rhs)
{
    if(this!= &rhs)
    {
        // Decallocate, allocate new space, copy valuesthis->vbp=rhs.vbp;
    }
    return *this;
}

void BreakpointManager::deleteAll()
{
    vbp.clear();
}

void BreakpointManager::enableAll()
{
    for(vector<Breakpoint>::iterator itr=vbp.begin(); itr!=vbp.end(); itr++)
        itr->enable();
}
void BreakpointManager::disableAll()
{
    for(vector<Breakpoint>::iterator itr=vbp.begin(); itr!=vbp.end(); itr++)
        itr->disable();
}
void BreakpointManager::addBreakpoint(Breakpoint bp)
{
    vbp.push_back(bp);
}
void BreakpointManager::deleteBreakpoint(Breakpoint bp)
{
    for(vector<Breakpoint>::iterator itr=vbp.begin(); itr!=vbp.end(); itr++)
        if(*itr==bp)
        {
            vbp.erase(itr,itr);
            break;
        }
}

