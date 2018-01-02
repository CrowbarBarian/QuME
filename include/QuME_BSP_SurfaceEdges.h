#ifndef QUME_BSPSurfaceEdges_H
#define QUME_BSPSurfaceEdges_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

#define QUME_BSP_SURF_EDGE_DATA_SIZE 4

class QuME_BSP_SurfaceEdges
{
    public:
        QuME_BSP_SurfaceEdges();
        ~QuME_BSP_SurfaceEdges();
        bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
        void DebugDump(wxTextOutputStream& out);

        QuME_Array<wxInt32> SurfaceEdgeIndices;
};

#endif // QuME_BSP_SurfaceEdges_H
