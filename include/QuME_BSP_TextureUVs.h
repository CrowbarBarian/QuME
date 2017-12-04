#ifndef QUME_BSPTEXTUREUVS_H
#define QUME_BSPTEXTUREUVS_H

#include "QuME_Common.h"

struct UVList
{
    UVList* next;
    wxFloat64 U;
    wxFloat64 V;
};


class QuME_BSP_TextureUVs
{
public:
    QuME_BSP_TextureUVs();
    ~QuME_BSP_TextureUVs();

    UVList* AddUVs(wxFloat64 U, wxFloat64 V);
    void ListToArray();

    wxUint32 UVCount;
    UVList* Head;
    UVList* Tail; //used to cache last added UV for speed
    wxFloat64* UArray;
    wxFloat64* VArray;
};

#endif // QuME_BSP_TEXTUREUVS_H
