#include "QuME_BSP_BrushModel.h"

QuME_BSP_BrushModel::QuME_BSP_BrushModel()
{
        for(wxUint32 i = 0; i < 3; i++)
        {
            this->Mins[i] = 0.0;
            this->Maxs[i] = 0.0;
            this->Origin[i] = 0.0;
        }
        this->HeadNode = 0;
        this->FirstFace = 0;
        this->NumFaces = 0;
}

QuME_BSP_BrushModel::~QuME_BSP_BrushModel()
{
    //nothing to delete
}

void QuME_BSP_BrushModel::SetMins(wxFloat32 x, wxFloat32 y, wxFloat32 z)
{
    this->Mins[0] = x;
    this->Mins[1] = y;
    this->Mins[2] = z;
}

void QuME_BSP_BrushModel::SetMaxs(wxFloat32 x, wxFloat32 y, wxFloat32 z)
{
    this->Maxs[0] = x;
    this->Maxs[1] = y;
    this->Maxs[2] = z;
}

void QuME_BSP_BrushModel::SetOrigin(wxFloat32 x, wxFloat32 y, wxFloat32 z)
{
    this->Origin[0] = x;
    this->Origin[1] = y;
    this->Origin[2] = z;
}

void QuME_BSP_BrushModel::DebugDump(wxTextOutputStream& out)
{
    out << "Mins: X:" << this->Mins[0] << " Y: " << this->Mins[1] << " Z: " << this->Mins[2] << "\n";
    out << "Maxs: X:" << this->Maxs[0] << " Y: " << this->Maxs[1] << " Z: " << this->Maxs[2] << "\n";
    out << "Origin: X:" << this->Origin[0] << " Y: " << this->Origin[1] << " Z: " << this->Origin[2] << "\n";
    out << "HeadNode: " << this->HeadNode << "\n";
    out << "First Face: " << this->FirstFace << "\n";
    out << "Number of faces: " << this->NumFaces << "\n";
}
