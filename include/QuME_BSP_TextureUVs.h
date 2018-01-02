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
	QuME_UV(QuME_UV& o)
	{
		U = o.U;
		V = o.V;
	}
	QuME_UV& operator=(QuME_UV& o)
	{
		U = o.U;
		V = o.V;
		return *this;
	}
	bool operator==(QuME_UV& o) //use epsilon compare...
	{
		if(fabs(this->length2() - o.length2()) > QUME_UV_EPSILON * QUME_UV_EPSILON) return false;
		return true;
	}
	wxFloat64 length2()
	{
		return this->U * this->U + this->V * this->V;
	}
	wxFloat64 length()
	{
		return std::sqrt(this->length2());
	}

	wxFloat64 U;
	wxFloat64 V;
};

class QuME_BSP_TextureUVs
{
public:
    QuME_BSP_TextureUVs();
    ~QuME_BSP_TextureUVs();

    QuME_LinkedList<QuME_UV> UVList;
    QuME_Array<QuME_UV> UVs;
};

#endif // QuME_BSP_TEXTUREUVS_H
