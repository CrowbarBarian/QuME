#ifndef QUME_BSPFACES_H
#define QUME_BSPFACES_H

#include "QuME_Common.h"

#include "QuME_BSP_Edges.h"


struct QuME_BSP_Triangle
{
    wxUint32 v[3]; //index into vertex array of bsp file
    wxUint32 uv[3]; //index into UV array of bsp file
};

struct VertexList
{
    VertexList* prev;
    VertexList* next;
    wxUint32 VertexIndex;
    wxUint32 UVIndex;
};

class QuME_BSP_Face
{
public:
    QuME_BSP_Face();
    ~QuME_BSP_Face();

    wxInt32 Triangulate();
    void DebugDump(wxTextOutputStream& out);

    VertexList* AddVertexToList(wxUint32 v, wxUint32 UV);
    VertexList* RemoveVertexFromList(wxUint32 v);
    VertexList* GetVertex(wxUint32 v);

    wxUint32 TriangleCount; //should always be vertex count - 2
    QuME_BSP_Triangle* Triangle;
    VertexList* Head; //a linked list for handling the vertex index array rearranging
    VertexList* Tail;
    wxUint32 VertexListCount;
    wxUint32* VertexArray; //ordered array of this face's vertex indexes
    wxUint32* TextureUV; //ordered array of this face's UV coordinate vertex indexes
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

    wxUint32 Count;
    QuME_BSP_Face* Face;
};

#endif // QuME_BSP_FACES_H
