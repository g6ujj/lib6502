/*
 Memory.h

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
#ifndef MEMORY_H
#define MEMORY_H

#include <string>
using namespace std;

typedef unsigned short address;
typedef unsigned char byte;
typedef short word;
typedef unsigned char zpaddr;
typedef unsigned char stack;


#define MAXRAM 65535
class Memory
{
public:
    Memory();
    virtual ~Memory() {};

    byte read(address addr);
    byte read(zpaddr addr);

    void write(address addr,byte b);
    void write(zpaddr addr,byte b);
    string dump(void);
protected:
private:
    byte ram[MAXRAM];
};

#endif // MEMORY_H
