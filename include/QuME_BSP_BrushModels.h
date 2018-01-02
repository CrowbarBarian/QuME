#ifndef QUME_BSPDMODELS_H
#define QUME_BSPDMODELS_H

#include "QuME_Common.h"
#include "QuME_Lists.h"
#include "QuME_Bounds.h"
#include "QuME_Vector.h"

#define BRUSHMODEL_SIZE_ON_DISK 48

class QuME_BSP_BrushModel
{
public:
    void DebugDump(wxTextOutputStream& out);

	QuME_Bounds BBox;
	QuME_Vector Orig;
    wxInt32 HeadNode;
    wxInt32 FirstFace;
    wxInt32 NumFaces;
};

class QuME_BSP_BrushModels
{
public:
    QuME_BSP_BrushModels();
    ~QuME_BSP_BrushModels();
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
    void DebugDump(wxTextOutputStream& out);

    QuME_Array<QuME_BSP_BrushModel> BrushModels;
};

#endif // QuME_BSP_DMODELS_H
