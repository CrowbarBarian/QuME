#ifndef QUME_BSPFACE_H
#define QUME_BSPFACE_H

#include "QuME_Common.h"


struct QuME_BSP_Triangle
{
        wxUint32 v[3]; //index into vertex array of bsp file
        wxUint32 uv[3]; //index into UV array of bsp file
};

struct VertexList
{
public:

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

    wxUint16 GetPlane()
    {
        return Plane;
    }
    void SetPlane(wxUint16 val)
    {
        Plane = val;
    }
    wxInt16 GetPlaneSide()
    {
        return PlaneSide;
    }
    void SetPlaneSide(wxInt16 val)
    {
        PlaneSide = val;
    }
    wxInt32 GetFirstEdge()
    {
        return FirstEdge;
    }
    void SetFirstEdge(wxInt32 val)
    {
        FirstEdge = val;
    }
    wxInt16 GetEdgeCount()
    {
        return EdgeCount;
    }
    void SetEdgeCount(wxInt16 val)
    {
        EdgeCount = val;
    }
    wxInt16 GetTextureInfo()
    {
        return TextureInfo;
    }
    void SetTextureInfo(wxInt16 val)
    {
        TextureInfo = val;
    }
    wxUint8 GetLightmapStyles(wxUint32 index)
    {
        return LightmapStyles[index];
    }
    void SetLightmapStyles(wxUint32 index, wxUint8 val)
    {
        LightmapStyles[index] = val;
    }
    wxInt32 GetLightmapOffset()
    {
        return LightmapOffset;
    }
    void SetLightmapOffset(wxInt32 val)
    {
        LightmapOffset = val;
    }
    void SetVertexArray(wxUint32* vlist)
    {
        VertexArray = vlist;
    }
    wxUint32* GetVertexArray()
    {
        return VertexArray;
    }
    void SetTextureUVs(wxUint32* tuvs)
    {
        TextureUVs = tuvs;
    }
    wxUint32* GetTextureUVs()
    {
        return TextureUVs;
    }
    QuME_BSP_Triangle* GetTriangles()
    {
        return Triangles;
    }
    wxUint32 GetTriangleCount()
    {
        return TriangleCount;
    }
    void DebugDump(wxTextOutputStream& out);
    wxInt32 Triangulate();

    VertexList* AddVertexToList(wxUint32 v, wxUint32 UV);
    VertexList* RemoveVertexFromList(wxUint32 v);
    VertexList* GetVertex(wxUint32 v);

    wxUint32 TriangleCount; //should always be vertex count - 2
    QuME_BSP_Triangle* Triangles;
    VertexList* Head; //a linked list for handling the vertex index array rearranging
    VertexList* Tail;
    wxUint32 VertexListCount;
    wxUint32* VertexArray; //ordered array of this face's vertex indexes
    wxUint32* TextureUVs; //ordered array of this face's UV coordinate vertex indexes
    wxUint16 Plane; // index of the plane the face is parallel to
    wxInt16 PlaneSide; // set if the normal is parallel to the plane normal
    wxInt32 FirstEdge; // index of the first edge (in the face edge array)
    wxInt16 EdgeCount; // number of consecutive edges (in the face edge array)
    wxInt16 TextureInfo; // index of the texture info structure
    wxUint8 LightmapStyles[4]; // styles (bit flags) for the lightmaps
    wxInt32 LightmapOffset; // offset of the lightmap (in bytes) in the lightmap lump
};

#endif // QuME_BSP_FACE_H
