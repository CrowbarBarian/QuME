#ifndef QUME_BSPSurfaceEdges_H
#define QUME_BSPSurfaceEdges_H

#include "QuME_Common.h"

class QuME_BSP_SurfaceEdges
{
    public:
        QuME_BSP_SurfaceEdges();
        ~QuME_BSP_SurfaceEdges();
        bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
        void DebugDump(wxTextOutputStream& out);

        wxUint32 Count;
        wxInt32* SurfaceEdge;
};

#endif // QuME_BSP_SurfaceEdges_H
