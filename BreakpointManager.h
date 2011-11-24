/*
 BreakpointManager.h

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
