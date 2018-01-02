#ifndef QUME_BSPTEXTURES_H
#define QUME_BSPTEXTURES_H

#include "QuME_Common.h"
#include "QuME_Lists.h"
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
    wxUint32 Value; //used for light emitting surfaces, for radiosity calcs
    wxUint32 xRes;
    wxUint32 yRes;
    std::wstring MaterialName;
    wxInt32 NextTexInfo;
};

class QuME_BSP_Textures
{
public:
    QuME_BSP_Textures();
    ~QuME_BSP_Textures();

    void DebugDump(wxTextOutputStream& out);
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length, const std::wstring& baseDir);

    QuME_Array<QuME_BSP_Texture> TextureInfo;
    std::wstring FaultTexture;
};

#endif // QuME_BSP_TEXTURES_H
