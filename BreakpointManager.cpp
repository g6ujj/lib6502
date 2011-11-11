#include "BreakpointManager.h"



BreakpointManager::~BreakpointManager()
{
    //dtor
}

BreakpointManager& BreakpointManager::operator=(const BreakpointManager& rhs)
{
    if(this!= &rhs)
    {
        // Decallocate, allocate new space, copy values
        this->vbp=rhs.vbp;
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

