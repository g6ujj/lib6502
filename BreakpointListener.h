
#ifndef BREAKPOINTLISTENER_H
#define BREAKPOINTLISTENER_H
#include "Breakpoint.h"

class BreakpointListener
{
    public:
    virtual void tellAddedBreakpoint(Breakpoint *bp) =0;
    virtual void tellDeletedBreakpoint(Breakpoint *bp) =0;
    virtual void tellEnabledBreakpoint(Breakpoint *bp) =0;
    virtual void tellDisabledBreakpoint(Breakpoint *bp) =0;
};


#endif //BREAKPOINTLISTENER_H
