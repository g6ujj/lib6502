/*
 readIntelHex.h

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

#ifndef READINTELHEX_H
#define READINTELHEX_H

#include <map>
#include <string>
#include "Memory.h"

enum errHRec {
	HRecEOF=-100,
	HRecInvalidStart,
	HRecInvalidCount,
	HRecInvalidAddress,
	HRecInvalidType,
	HRecTooShort,
	HRecChecksumErr,
	HRecFail,
	HRecFile,
	HRecOk=0
};
enum tHRec {
	hRecDataRecord=0,
	hRecEOFRecord,
	hRecExtSegment,
	hRecStartSegment,
	hRecExtendedLinear,
	hRecStartLinear
};
class readIntelHex
{
    public:
        /** Default constructor */
        //readIntelHex();
        /** Default destructor */
        //virtual ~readIntelHex();
        bool openFile(string fName,Memory *ram);
    protected:
    private:
    //errMessage;


    errHRec loadLine(string theLine,Memory *ram);
    bool validChecksum(string theLine);
};

#endif // READINTELHEX_H
