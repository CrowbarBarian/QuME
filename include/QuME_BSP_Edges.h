#ifndef QUME_BSPEDGES_H
#define QUME_BSPEDGES_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

#define EDGE_SIZE_ON_DISK 4

class QuME_BSP_Edge
{
public:
    wxUint32 a;
    wxUint32 b;
};

class QuME_BSP_Edges
{
public:
    QuME_BSP_Edges();
    ~QuME_BSP_Edges();
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
    void DebugDump(wxTextOutputStream& out);

    wxUint32 Count;
    QuME_BSP_Edge* EdgeArray;
    LinkedList<QuME_BSP_Edge> EdgeList;
};

#endif // QuME_BSP_EDGES_H
