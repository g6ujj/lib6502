#ifndef BREAKPOINTMANAGER_H
#define BREAKPOINTMANAGER_H

#include <vector>
#include "Breakpoint.h"
//#include "BreakpointListener.h"


class BreakpointManager:Breakpoint
{
    public:
        /** Default constructor */
        BreakpointManager():vbp() {};
        BreakpointManager(const BreakpointManager& rhs);
        BreakpointManager& operator=(const BreakpointManager& rhs);
        /** Default destructor */
        virtual ~BreakpointManager();

        void addBreakpoint(Breakpoint bp);

        void deleteAll();
        void deleteBreakpoint(Breakpoint bp);
        void deleteBreakpoint(int idx);

        void enableAll();
        void enableBreakpoint(int idx);

        void disableAll();
        void disableBreakpoint(int idx);

        Breakpoint getBreakpoint(int idx);

    protected:
    private:

    vector<Breakpoint>vbp;
};

#endif // BREAKPOINTMANAGER_H
