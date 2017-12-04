#ifndef QUME_BSPVERTEX_H_INCLUDED
#define QUME_BSPVERTEX_H_INCLUDED

#include "QuME_Common.h"

class QuME_BSP_Vertex
{
public:
    QuME_BSP_Vertex();
    QuME_BSP_Vertex(wxFloat32 x, wxFloat32 y, wxFloat32 z);
    ~QuME_BSP_Vertex();
    void SetVertex(wxFloat32 x, wxFloat32 y, wxFloat32 z);
    void SetX(wxFloat32 x) { this->x = x; }
    void SetY(wxFloat32 y) { this->y = y; }
    void SetZ(wxFloat32 z) { this->z = z; }
    wxFloat32 GetX() { return this->x; }
    wxFloat32 GetY() { return this->y; }
    wxFloat32 GetZ() { return this->z; }
    void DebugDump(wxTextOutputStream& out);
    wxFloat32 dot(const QuME_BSP_Vertex& o);

    wxFloat32 x;
    wxFloat32 y;
    wxFloat32 z;
};

#endif // QuME_BSP_VERTEX_H_INCLUDED
