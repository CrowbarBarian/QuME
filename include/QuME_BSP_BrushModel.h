#ifndef QUME_BSPBSPMODEL_H
#define QUME_BSPBSPMODEL_H

#include "QuME_Common.h"

class QuME_BSP_BrushModel
{
    public:
        QuME_BSP_BrushModel();
        ~QuME_BSP_BrushModel();
        wxFloat32* GetMins() { return Mins; }
        void SetMins(wxFloat32 x, wxFloat32 y, wxFloat32 z);
        wxFloat32* GetMaxs() { return Maxs; }
        void SetMaxs(wxFloat32 x, wxFloat32 y, wxFloat32 z);
        wxFloat32* GetOrigin() { return Origin; }
        void SetOrigin(wxFloat32 x, wxFloat32 y, wxFloat32 z);
        wxInt32 GetHeadNode() { return HeadNode; }
        void SetHeadNode(wxInt32 hn) { HeadNode = hn; }
        wxInt32 GetFirstFace() { return FirstFace; }
        void SetFirstFace(wxInt32 ff) { FirstFace = ff; }
        wxInt32 GetNumFaces() { return NumFaces; }
        void SetNumFaces(wxInt32 nf) { NumFaces = nf; }
        void DebugDump(wxTextOutputStream& out);

        wxFloat32 Mins[3], Maxs[3];
        wxFloat32 Origin[3];
        wxInt32 HeadNode;
        wxInt32 FirstFace;
        wxInt32 NumFaces;
};

#endif // QuME_BSP_DMODEL_H
