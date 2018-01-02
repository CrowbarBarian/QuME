/***************************************************************
 * Name:		QuME_BSP_Planes.cpp
 * Purpose:		Class to handle BSP planes
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_Planes.h"

QuME_BSP_Planes::QuME_BSP_Planes()
{
    //Count = 0;
    //BSPPlane = nullptr;
}

QuME_BSP_Planes::~QuME_BSP_Planes()
{
    //Count = 0;
    //delete[] BSPPlane;
}

bool QuME_BSP_Planes::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength)
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

    this->Planes.Allocate(lumpLength / Q2_BSP_PLANE_DATA_SIZE);

    binData->UseBasicPrecisions();

    for(wxUint32 i = 0; i < this->Planes.Count; i++)
    {
        this->Planes[i].Plane.Normal.x = binData->ReadFloat();
        this->Planes[i].Plane.Normal.y = binData->ReadFloat();
        this->Planes[i].Plane.Normal.z = binData->ReadFloat();
        this->Planes[i].Plane.Distance = binData->ReadFloat();
        this->Planes[i].Flags = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}
