#ifndef QUME_BSP_PLANES_H
#define QUME_BSP_PLANES_H

#include "QuME_Common.h"
#include "QuME_Vector.h"
#include "QuME_Plane.h"
#include "QuME_Lists.h"
//#include "QuME_Arrays.h"

#define Q2_BSP_PLANE_DATA_SIZE 20

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
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength);

    QuME_Array<QuME_BSP_Plane> Planes;
};

#endif // QUME_BSP_PLANES_H
