/*
 HexUtils.cpp

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

using namespace std;

#include "HexUtils.h"

unsigned int HexToInt(string theLine)
{
    int rv=0;

    for(size_t i=0; i<theLine.length(); i++)
    {
        int ch=tolower(theLine.at(i));

        unsigned int v=0;
        if(isdigit(ch))
            v=ch-'0';
        else
        {
            if(isxdigit(ch))
            {
                v=ch-'a'+10;
            }
        }
        rv=rv*16+v;
    }
    return rv;
}

string toHex(byte val)
{
    string hex="0123456789ABCDEF";
    byte hi=val/16;
    byte lo=val%16;
    return hex.substr(hi,1)+hex.substr(lo,1);
}
string toHex(address val)
{
    string hex="0123456789ABCDEF";
    byte hi=val/256;
    byte lo=val%256;
    string rv= toHex(hi)+toHex(lo);
    return rv;
}
