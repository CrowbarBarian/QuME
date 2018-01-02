#ifndef QUME_BSPVERTICES_H
#define QUME_BSPVERTICES_H

#include "QuME_Common.h"

#include "QuME_Vector.h"
#include "QuME_Lists.h"

#define Q2_BSP_VERTEX_DATA_SIZE 12

class QuME_BSP_Vertices
{
    public:
        QuME_BSP_Vertices();
        ~QuME_BSP_Vertices();
        bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength);
        void DebugDump(wxTextOutputStream& out);

        QuME_Array<QuME_Vector> Vertices;
        QuME_LinkedList<QuME_Vector> VertexList;
};

#endif // QuME_BSP_VERTICES_H
