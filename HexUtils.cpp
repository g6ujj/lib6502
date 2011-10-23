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
