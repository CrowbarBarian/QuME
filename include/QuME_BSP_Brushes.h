#ifndef QUME_BSP_BRUSHES_H
#define QUME_BSP_BRUSHES_H

#include "QuME_Common.h"
#include "QuME_BSP_Planes.h"
#include "QuME_Vector.h"
#include "QuME_BSP_Edges.h"
#include "QuME_Lists.h"
#include "QuME_Arrays.h"

//class QuME_BSP_Data;

class BrushEdge
{
public:
    BrushEdge()
    {
        IndexArray[0] = 0;
        IndexArray[1] = 0;
        NextBrushEdgeIndex = 0;
    }
    BrushEdge(wxUint32 a, wxUint32 b)
    {
        IndexArray[0] = a;
        IndexArray[1] = b;
        NextBrushEdgeIndex = 0;
    }
    union
    {
        struct
        {
            wxUint32 a;
            wxUint32 b;
        };
        wxUint32 IndexArray[2]; //the two points that make up this edge
    };
    wxUint32 NextBrushEdgeIndex;
};

class QuME_BSP_BrushSide
{
public:
    QuME_BSP_BrushSide()
    {
        VertexIndexArrayCount = 0;
        VertexIndexArray = nullptr;
        //Edge = nullptr;
        BrushEdgeArray = nullptr;

    }

    ~QuME_BSP_BrushSide()
    {
        VertexIndexArrayCount = 0;
        SAFE_ARRAY_DELETE(VertexIndexArray);
    }

    wxUint16 PlaneIndex;
    wxInt16 TextureIndex;

    QuME_Vector Normal;

    //QuME_BSP_Edge* Edge;


    LinkedList<wxUint32> VertexIndexList;
    wxUint32* VertexIndexArray;
    wxUint32 VertexIndexArrayCount;

    LinkedList<BrushEdge> BrushEdgeList;
    BrushEdge* BrushEdgeArray;
    wxUint32 BrushEdgeArrayCount;
};

class QuME_BSP_BrushSides
{
public:
    QuME_BSP_BrushSides()
    {
        Count = 0;
        BrushSide = nullptr;
    }
    ~QuME_BSP_BrushSides()
    {
        Count = 0;
        SAFE_ARRAY_DELETE(BrushSide);
    }

    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);

    wxUint32 Count;
    QuME_BSP_BrushSide* BrushSide;
    QuME_Array<QuME_BSP_BrushSide> BrushSideArray;
};

class QuME_BSP_Brush
{
public:
    QuME_BSP_Brush()
    {
    }

    ~QuME_BSP_Brush()
    {
    }

    wxUint32 FirstBrushSide;
    wxUint32 SideCount;
    wxUint32 Content;

    LinkedList<wxUint32> Indices;
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

    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);

    wxUint32 Count;
    QuME_BSP_Brush* Brush;
};

#endif // QUME_BSP_BRUSHES_H
