#ifndef QUME_BSPFACES_H
#define QUME_BSPFACES_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

#include "QuME_BSP_Edges.h"

#define Q2_BSP_FACE_DATA_SIZE 20


struct QuME_BSP_Triangle
{
    wxUint32 v[3]; //index into vertex array of bsp file
    wxUint32 uv[3]; //index into UV array of bsp file
};

class tempVertex
{
public:
	tempVertex()
	{
		this->VertexIndex = 0;
		this->UVIndex = 0;
	}
	tempVertex(wxUint32 Vi, wxUint32 UVi)
	{
		this->VertexIndex = Vi;
		this->UVIndex = UVi;
	}
	tempVertex(tempVertex& o)
	{
		this->VertexIndex = o.VertexIndex;
		this->UVIndex = o.UVIndex;
	}
	tempVertex& operator=(tempVertex& o)
	{
		this->VertexIndex = o.VertexIndex;
		this->UVIndex = o.UVIndex;
		return *this;
	}
	wxUint32 VertexIndex;
	wxUint32 UVIndex;
};

class QuME_Triangle
{
public:
	tempVertex Vertex[3];
};

class QuME_BSP_Face
{
public:
    QuME_BSP_Face();
    ~QuME_BSP_Face();

    wxInt32 Triangulate();
    void DebugDump(wxTextOutputStream& out);

    QuME_Array<QuME_Triangle> Triangles;

    QuME_LinkedList<tempVertex> Vertices;
    QuME_Array<tempVertex> VArray;
    wxUint16 Plane; // index of the plane the face is parallel to
    wxInt16 PlaneSide; // set if the normal is parallel to the plane normal
    wxInt32 FirstEdge; // index of the first edge (in the face edge array)
    wxInt16 EdgeCount; // number of consecutive edges (in the face edge array)
    wxInt16 TextureInfo; // index of the texture info structure
    wxUint8 LightmapStyles[4]; // styles (bit flags) for the lightmaps
    wxInt32 LightmapOffset; // offset of the lightmap (in bytes) in the lightmap lump
};

class QuME_BSP_Faces
{
public:

    QuME_BSP_Faces();
    ~QuME_BSP_Faces();
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
    void DebugDump(wxTextOutputStream& out);

    QuME_Array<QuME_BSP_Face> Faces;
};

#endif // QuME_BSP_FACES_H
