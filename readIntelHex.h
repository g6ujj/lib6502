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
    string errMessage;


    errHRec loadLine(string theLine,Memory *ram);
    bool validChecksum(string theLine);
};

#endif // READINTELHEX_H
