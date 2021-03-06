#ifndef QUME_BSPEDGES_H
#define QUME_BSPEDGES_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

#define Q2_BSP_EDGE_DATA_SIZE 4

class QuME_BSP_Edge
{
public:
    wxInt32 a;
    wxInt32 b;
};

class QuME_BSP_Edges
{
public:
    QuME_BSP_Edges();
    ~QuME_BSP_Edges();
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
    void DebugDump(wxTextOutputStream& out);

    QuME_Array<QuME_BSP_Edge> Edges;
    QuME_LinkedList<QuME_BSP_Edge> EdgeList;
};

#endif // QuME_BSP_EDGES_H
