#ifndef QUME_BSPTEXTURE_H
#define QUME_BSPTEXTURE_H

#include "QuME_Common.h"

#include "QuME_Vector.h"

#define TEXTURE_SIZE_ON_DISK 76
#define TEXTURE_NAME_SIZE 32

class QuME_BSP_Texture
{
public:
    void DebugDump(wxTextOutputStream& out)
    {
        out << "U-Axis: ";
        uAxis.DebugDump(out);
        out << "U Offset: " << uOffset << "\n";
        out << "V-Axis: ";
        vAxis.DebugDump(out);
        out << "V Offset: " << vOffset << "\n";
        out << "Flags: " << Flags << "\n";
        out << "Value: " << Value << "\n";
        out << "Texture Name: " << MaterialName << "\n";
        out << "Next Texture Info: " << NextTexInfo << "\n";
    }

    QuME_Vector uAxis;
    wxFloat64 uOffset;
    QuME_Vector vAxis;
    wxFloat64 vOffset;
    wxUint32 Flags;
    wxUint32 Value;
    wxUint32 xRes;
    wxUint32 yRes;
    wxString MaterialName;
    wxInt32 NextTexInfo;
};

#endif // QuME_BSP_TEXTURE_H
