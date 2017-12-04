#ifndef QUME_BSPVERTICES_H
#define QUME_BSPVERTICES_H

#include "QuME_Common.h"

#include "QuME_Vector.h"
#include "QuME_Lists.h"

class QuME_BSP_Vertices
{
    public:
        QuME_BSP_Vertices();
        ~QuME_BSP_Vertices();
        bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
        void DebugDump(wxTextOutputStream& out);

        wxUint32 ArrayCount;
        QuME_Vector* VertexArray;

        LinkedList<QuME_Vector> VertexList;
};

#endif // QuME_BSP_VERTICES_H
