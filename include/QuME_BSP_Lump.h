#ifndef QUME_BSPLUMP_H
#define QUME_BSPLUMP_H

#include "QuME_Common.h"

class QuME_BSP_Lump
{
public:
    wxUint32 GetLength()
    {
        return Length;
    }
    void SetLength(wxUint32 val)
    {
        Length = val;
    }
    wxUint32 GetOffset()
    {
        return Offset;
    }
    void SetOffset(wxUint32 val)
    {
        Offset = val;
    }

    void SetLump(wxUint32 off, wxUint32 len)
    {
        Offset = off;
        Length = len;
    }

    void DebugDump(wxTextOutputStream& out)
    {
        out << "Offset: " << this->Offset << ", Length: " << this->Length << "\n";
    }

    wxUint32 Length;
    wxUint32 Offset;
};

#endif // QuME_BSP_LUMP_H
