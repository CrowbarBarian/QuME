#include "QuME_BSP_Brushes.h"

bool QuME_BSP_BrushSides::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength)
{
    //Count = length / 4;
    this->BrushSideArray.Count = lumpLength / Q2_BSP_BRUSH_SIDE_DATA_SIZE;

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
    this->BrushSideArray.Data = new QuME_BSP_BrushSide[BrushSideArray.Count];
    if(this->BrushSideArray.Data == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < BrushSideArray.Count; i++)
    {
        this->BrushSideArray.Data[i].PlaneIndex = binData->Read16();
        this->BrushSideArray.Data[i].TextureIndex = static_cast<wxInt16>(binData->Read16());
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

bool QuME_BSP_Brushes::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength)
{
    this->Count = lumpLength / Q2_BSP_BRUSH_DATA_SIZE;

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

    this->Brush = new QuME_BSP_Brush[Count];
    if(this->Brush == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < Count; i++)
    {
        this->Brush[i].FirstBrushSide = binData->Read32();
        this->Brush[i].SideCount = binData->Read32();
        this->Brush[i].Content = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

