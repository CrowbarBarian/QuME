#ifndef QUME_BSPEDGE_H
#define QUME_BSPEDGE_H

#include "QuME_Common.h"


class QuME_BSP_Edge
{
public:
    void DebugDump(wxTextOutputStream& out)
    {
        out << "a: " << this->a << ", b: " << this->b;
    }

    wxUint16 a;
    wxUint16 b;
};

#endif // QuME_BSP_EDGE_H
