#include "QuME_BSP_Texture.h"

QuME_BSP_Texture::QuME_BSP_Texture()
{
        uAxis.SetVertex(0.0, 0.0, 0.0);
        uOffset = 0.0;
        vAxis.SetVertex(0.0, 0.0, 0.0);
        vOffset = 0.0;
        Flags = 0;
        Value = 0;
        NextTexInfo = 0;
}

QuME_BSP_Texture::~QuME_BSP_Texture()
{
}

void QuME_BSP_Texture::DebugDump(wxTextOutputStream& out)
{
    out << "U-Axis: ";
    uAxis.DebugDump(out);
    out << "U Offset: " << uOffset << "\n";
    out << "V-Axis: ";
    vAxis.DebugDump(out);
    out << "V Offset: " << vOffset << "\n";
    out << "Flags: " << Flags << "\n";
    out << "Value: " << Value << "\n";
    out << "Texture Name: " << MaterialName << "\n";
    out << "Next Texture Info: " << NextTexInfo << "\n";
}
