#ifndef QUME_BSPDMODELS_H
#define QUME_BSPDMODELS_H

#include "QuME_Common.h"

#define BRUSHMODEL_SIZE_ON_DISK 48

class QuME_BSP_BrushModel
{
public:
    void SetMins(wxFloat64 x, wxFloat64 y, wxFloat64 z)
    {
        this->Mins[0] = x;
        this->Mins[1] = y;
        this->Mins[2] = z;
    }
    void SetMaxs(wxFloat64 x, wxFloat64 y, wxFloat64 z)
    {
        this->Maxs[0] = x;
        this->Maxs[1] = y;
        this->Maxs[2] = z;
    }
    void SetOrigin(wxFloat64 x, wxFloat64 y, wxFloat64 z)
    {
        this->Origin[0] = x;
        this->Origin[1] = y;
        this->Origin[2] = z;
    }
    void DebugDump(wxTextOutputStream& out);

    wxFloat64 Mins[3], Maxs[3];
    wxFloat64 Origin[3];
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

    wxUint32 Count;
    QuME_BSP_BrushModel* BrushModel;
};

#endif // QuME_BSP_DMODELS_H
