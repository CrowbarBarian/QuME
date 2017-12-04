#ifndef QUME_BSPLUMPS_H_INCLUDED
#define QUME_BSPLUMPS_H_INCLUDED

#include "QuME_Common.h"

#define LUMP_MAX 19
#define UNKNOWN_LUMP_TABLE 0xffffffff

enum LUMP_INDEX
{
    LUMP_ENTITIES = 0,
    LUMP_PLANES,
    LUMP_VERTICES,
    LUMP_VISIBILITY,
    LUMP_NODES,
    LUMP_TEXTURE_INFORMATION,
    LUMP_FACES,
    LUMP_LIGHT_MAPS,
    LUMP_LEAVES,
    LUMP_LEAF_FACE_TABLE,
    LUMP_LEAF_BRUSH_TABLE,
    LUMP_EDGES,
    LUMP_SURFACE_EDGES,
    LUMP_MODELS,
    LUMP_BRUSHES,
    LUMP_BRUSH_SIDES,
    LUMP_POP,
    LUMP_AREAS,
    LUMP_AREA_PORTALS,
    LUMP_BAD_LUMP_INDEX
};

class QuME_BSP_Lump
{
public:
    void DebugDump(wxTextOutputStream& out)
    {
        out << "Offset: " << this->Offset << ", Length: " << this->Length << "\n";
    }

    wxUint32 Length;
    wxUint32 Offset;
};

class QuME_BSP_Lumps
{
public:

    QuME_BSP_Lumps();
    ~QuME_BSP_Lumps();
    bool LoadLumps(wxFileInputStream* infile, wxUint32 offset);
    void DebugDump(wxTextOutputStream& out);

    QuME_BSP_Lump Lump[LUMP_MAX];
};


#endif // QuME_BSP_LUMPS_H_INCLUDED
