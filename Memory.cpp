/*
 Memory.cpp

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

#include "Memory.h"

Memory::Memory()
{
    //ctor
    for(address addr=0; addr<MAXRAM; addr++)
        ram[addr]=0;
}

char Memory::read(address addr)
{
    return ram[addr];
}

char Memory::read(zpaddr addr)
{
    return ram[addr];
}
char Memory::read(byte addr)
{
    return ram[addr];
}

void Memory::write(address addr,byte b)
{
    ram[addr]=b;
}
void Memory::write(zpaddr addr,byte b)
{
    ram[addr]=b;
}
