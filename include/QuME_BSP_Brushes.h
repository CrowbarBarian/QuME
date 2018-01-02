#ifndef QUME_BSP_BRUSHES_H
#define QUME_BSP_BRUSHES_H

#include "QuME_Common.h"
#include "QuME_BSP_Planes.h"
#include "QuME_Vector.h"
#include "QuME_Bounds.h"
#include "QuME_BSP_Vertices.h"
#include "QuME_BSP_Edges.h"
#include "QuME_Lists.h"
//#include "QuME_Arrays.h"

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
	}

    ~QuME_BSP_BrushSide()
    {
    }

    wxUint32 PlaneIndex;
    wxInt32 TextureIndex;

    QuME_Vector Normal;

	//brush relative indices
	QuME_Array<wxUint32> BrushSideVertices;
    QuME_LinkedList<wxUint32> BrushVertexList;

    //global relative indices
    QuME_Array<wxUint32> BrushSideVertexIndices;
    QuME_LinkedList<wxUint32> BrushSideVertexIndexList;

    QuME_Array<wxUint32> BrushSideUVs;
    QuME_LinkedList<wxUint32> BrushSideUVIndices;
};

class QuME_BSP_BrushSides
{
public:
    QuME_BSP_BrushSides()
    {
    }
    ~QuME_BSP_BrushSides()
    {
    }

    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength);

    QuME_Array<QuME_BSP_BrushSide> BrushSideArray;
};

class QuME_BSP_Brush
{
public:
    QuME_BSP_Brush()
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

	QuME_Array<QuME_Vector> BrushVertices;
    QuME_LinkedList<QuME_Vector> BrushVertexList;
};

class QuME_BSP_Brushes
{
public:
    QuME_BSP_Brushes()
    {
	}

    ~QuME_BSP_Brushes()
    {
    }

    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength);

    QuME_Array<QuME_BSP_Brush> Brushes;
};

#endif // QUME_BSP_BRUSHES_H
