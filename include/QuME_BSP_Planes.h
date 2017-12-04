#ifndef QUME_BSP_PLANES_H
#define QUME_BSP_PLANES_H

#include "QuME_Common.h"
#include "QuME_Vector.h"
#include "QuME_Plane.h"
#include "QuME_Arrays.h"

struct QuME_BSP_Plane
{
    QuME_Plane Plane;
    wxUint32 Flags;
};

class QuME_BSP_Planes
{
public:
    QuME_BSP_Planes();
    ~QuME_BSP_Planes();
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);

    //wxUint32 Count;
    //QuME_BSP_Plane* BSPPlane;
    QuME_Array<QuME_BSP_Plane> BSPPlaneArray;
};

#endif // QUME_BSP_PLANES_H
