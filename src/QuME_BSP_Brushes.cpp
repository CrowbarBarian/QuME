/***************************************************************
 * Name:		QuME_BSP_Brushes.cpp
 * Purpose:		Classes to handle BSP brushes and brush sides
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_Brushes.h"

bool QuME_BSP_BrushSides::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength)
{
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

    this->BrushSideArray.Allocate(lumpLength / Q2_BSP_BRUSH_SIDE_DATA_SIZE);

    for(wxUint32 i = 0; i < BrushSideArray.Count; i++)
    {
        this->BrushSideArray[i].PlaneIndex = binData->Read16();
        this->BrushSideArray[i].TextureIndex = static_cast<wxInt16>(binData->Read16());
    }

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

    this->Brushes.Allocate(lumpLength / Q2_BSP_BRUSH_DATA_SIZE);

    for(wxUint32 i = 0; i < this->Brushes.Count; i++)
    {
        this->Brushes[i].FirstBrushSide = binData->Read32();
        this->Brushes[i].SideCount = binData->Read32();
        this->Brushes[i].Content = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

