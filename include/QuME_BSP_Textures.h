#ifndef QUME_BSPTEXTURES_H
#define QUME_BSPTEXTURES_H

#include "QuME_Common.h"

#include "QuME_BSP_Texture.h"

class QuME_BSP_Textures
{
public:
    QuME_BSP_Textures();
    ~QuME_BSP_Textures();

    void DebugDump(wxTextOutputStream& out);
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length, const std::wstring& baseDir);

    wxUint32 Count;
    QuME_BSP_Texture* Texture;
    std::wstring FaultTexture;
};

#endif // QuME_BSP_TEXTURES_H
