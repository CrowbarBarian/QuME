#ifndef QUME_BSP_BRUSHES_H
#define QUME_BSP_BRUSHES_H

#include "QuME_Common.h"
#include "QuME_BSP_Planes.h"
#include "QuME_Vector.h"
#include "QuME_Bounds.h"
#include "QuME_BSP_Vertices.h"
#include "QuME_BSP_Edges.h"
#include "QuME_Lists.h"
#include "QuME_Arrays.h"

//class QuME_BSP_Data;

#define Q2_BSP_BRUSH_SIDE_DATA_SIZE 4
#define Q2_BSP_BRUSH_DATA_SIZE 12

class BrushEdge
{
public:
    BrushEdge()
    {
        this->a = 0;
        this->b = 0;
        NextBrushEdgeIndex = 0;
    }
    BrushEdge(wxUint32 ina, wxUint32 inb)
    {
        this->a = ina;
        this->b = inb;
        NextBrushEdgeIndex = 0;
    }

	wxUint32 a;
	wxUint32 b;
    wxUint32 NextBrushEdgeIndex;
};

class QuME_BSP_BrushSide
{
public:
    QuME_BSP_BrushSide()
    {
        VertexIndexArrayCount = 0;
        VertexIndexArray = nullptr;
        BrushEdgeArray = nullptr;
        UVIndexArrayCount = 0;
        UVIndexArray = nullptr;
    }

    ~QuME_BSP_BrushSide()
    {
        VertexIndexArrayCount = 0;
        SAFE_ARRAY_DELETE(VertexIndexArray);
        UVIndexArrayCount = 0;
        SAFE_ARRAY_DELETE(UVIndexArray);
    }

    wxUint32 PlaneIndex;
    wxInt32 TextureIndex;

    QuME_Vector Normal;

    QuME_LinkedList<wxUint32> VertexIndexList;
    wxUint32* VertexIndexArray;
    wxUint32 VertexIndexArrayCount;

    QuME_LinkedList<wxUint32> UVIndices;
    wxUint32* UVIndexArray;
    wxUint32 UVIndexArrayCount;

    QuME_LinkedList<BrushEdge> BrushEdgeList;
    BrushEdge* BrushEdgeArray;
    wxUint32 BrushEdgeArrayCount;
};

class QuME_BSP_BrushSides
{
public:
    QuME_BSP_BrushSides():BrushSideArray()
    {
        Count = 0;
        BrushSide = nullptr;
    }
    ~QuME_BSP_BrushSides()
    {
        Count = 0;
        SAFE_ARRAY_DELETE(BrushSide);
    }

    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength);

    wxUint32 Count;
    QuME_BSP_BrushSide* BrushSide;
    QuME_Array<QuME_BSP_BrushSide> BrushSideArray;
};

class QuME_BSP_Brush
{
public:
    QuME_BSP_Brush():VertexIndices()
    {
    	this->FirstBrushSide = 0;
    	this->SideCount = 0;
    	this->Content = 0;
    }

    ~QuME_BSP_Brush()
    {
    }

    QuME_Bounds Bounds;

    wxUint32 FirstBrushSide;
    wxUint32 SideCount;
    wxUint32 Content;

    QuME_LinkedList<wxUint32> VertexIndices;
};

class QuME_BSP_Brushes
{
public:
    QuME_BSP_Brushes()
    {
        Count = 0;
        Brush = nullptr;
    }

    ~QuME_BSP_Brushes()
    {
        Count = 0;
        SAFE_ARRAY_DELETE(Brush);
    }

    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength);

    wxUint32 Count;
    QuME_BSP_Brush* Brush;
};

#endif // QUME_BSP_BRUSHES_H
