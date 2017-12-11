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
        out << L"U-Axis: ";
        uAxis.DebugDump(out);
        out << L"U Offset: " << uOffset << L"\n";
        out << L"V-Axis: ";
        vAxis.DebugDump(out);
        out << L"V Offset: " << vOffset << L"\n";
        out << L"Flags: " << Flags << L"\n";
        out << L"Value: " << Value << L"\n";
        out << L"Texture Name: " << MaterialName << L"\n";
        out << L"Next Texture Info: " << NextTexInfo << L"\n";
    }

    QuME_Vector uAxis;
    wxFloat64 uOffset;
    QuME_Vector vAxis;
    wxFloat64 vOffset;
    wxUint32 Flags;
    wxUint32 Value;
    wxUint32 xRes;
    wxUint32 yRes;
    std::wstring MaterialName;
    wxInt32 NextTexInfo;
};

#endif // QuME_BSP_TEXTURE_H
