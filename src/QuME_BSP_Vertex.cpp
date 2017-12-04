#include "QuME_BSP_Vertex.h"

QuME_BSP_Vertex::QuME_BSP_Vertex()
{
    this->x=0.0;
    this->y=0.0;
    this->z=0.0;
}

QuME_BSP_Vertex::QuME_BSP_Vertex(wxFloat32 x, wxFloat32 y, wxFloat32 z)
{
    this->x=x;
    this->y=y;
    this->z=z;
}

QuME_BSP_Vertex::~QuME_BSP_Vertex()
{
}

void QuME_BSP_Vertex::SetVertex(wxFloat32 x, wxFloat32 y, wxFloat32 z)
{
    this->x=x;
    this->y=y;
    this->z=z;
}

//this isn't the best way to do this, but it is expedient...
wxFloat32 QuME_BSP_Vertex::dot(const QuME_BSP_Vertex& o)
{
    wxFloat32 s = this->x * o.x;
    s += this->y * o.y;
    s += this->z * o.z;
    return s;
}

void QuME_BSP_Vertex::DebugDump(wxTextOutputStream& out)
{
    out << "X: " << this->x << ", Y: " << this->y << ", Z: " << this->z << "\n";
}
