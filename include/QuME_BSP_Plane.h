#ifndef QUME_BSP_PLANE_H
#define QUME_BSP_PLANE_H

#include "QuME_Common.h"

struct QuME_BSP_Plane
{
    wxFloat32 a;
    wxFloat32 b;
    wxFloat32 c;
    wxFloat32 d;
    wxUint32 Flags;
};

#endif // QUME_BSP_PLANE_H
