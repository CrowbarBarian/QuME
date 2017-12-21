#ifndef QUME_BSPTEXTUREUVS_H
#define QUME_BSPTEXTUREUVS_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

struct QuME_UVList
{
    QuME_UVList* next;
    wxFloat64 U;
    wxFloat64 V;
};

class QuME_UV
{
public:
	QuME_UV(wxFloat64 uIn, wxFloat64 vIn)
	{
		U = uIn;
		V = vIn;
	}
	QuME_UV()
	{
		U = 0.0;
		V = 0.0;
	}

	wxFloat64 U;
	wxFloat64 V;
};

class QuME_BSP_TextureUVs
{
public:
    QuME_BSP_TextureUVs();
    ~QuME_BSP_TextureUVs();

    QuME_LinkedList<QuME_UV> UVs;
    QuME_UV* UVArray;
    wxUint32 UVArrayCount;
};

#endif // QuME_BSP_TEXTUREUVS_H
