#include "QuME_BSP_Brushes.h"

bool QuME_BSP_BrushSides::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    //Count = length / 4;
    BrushSideArray.Count = length / 4;

    wxDataInputStream* binData = new wxDataInputStream( *infile );

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    if (infile->SeekI(offset, wxFromStart) == wxInvalidOffset)
    {
        delete binData;
        return false;
    }

    //BrushSide = new QuME_BSP_BrushSide[Count];
    BrushSideArray.Item = new QuME_BSP_BrushSide[BrushSideArray.Count];
    if(BrushSideArray.Item == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < BrushSideArray.Count; i++)
    {
        BrushSideArray.Item[i].PlaneIndex = binData->Read16();
        BrushSideArray.Item[i].TextureIndex = binData->Read16();
    }

    /*
    for(wxUint32 i = 0; i < Count; i++)
    {
        BrushSide[i].PlaneIndex = binData->Read16();
        BrushSide[i].TextureIndex = binData->Read16();
    }
    */

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

bool QuME_BSP_Brushes::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    Count = length / 12;

    wxDataInputStream* binData = new wxDataInputStream( *infile );

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    if (infile->SeekI(offset, wxFromStart) == wxInvalidOffset)
    {
        delete binData;
        return false;
    }

    Brush = new QuME_BSP_Brush[Count];
    if(Brush == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < Count; i++)
    {
        Brush[i].FirstBrushSide = binData->Read32();
        Brush[i].SideCount = binData->Read32();
        Brush[i].Content = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

