#ifndef BREAKPOINTMANAGER_H
#define BREAKPOINTMANAGER_H

#include "Breakpoint.h"
#include "BreakpointListener.h"

class BreakpointManager
{
    public:
        /** Default constructor */
        BreakpointManager();
        /** Default destructor */
        virtual ~BreakpointManager();
/*
        void deleteAll();
        void enableAll();
        void disableAll();

        void deleteBreakpointAtIndex(int idx);
        void enableBreakpointAtIndex(int idx);
        void disableBreakpointAtIndex(int idx);
        Breakpoint *getBreakpointAtIndex(int idx);

        bool addBreakpoint(Breakpoint bp);
        bool DeleteBreakpoint(Breakpoint *bpp);
*/

    protected:
    private:
};

#endif // BREAKPOINTMANAGER_H
