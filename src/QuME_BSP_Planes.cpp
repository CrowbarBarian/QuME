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

bool QuME_BSP_Planes::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    //this->Count = length / 20;
    this->BSPPlaneArray.Count = length / 20;

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

    //this->BSPPlane = new QuME_BSP_Plane[this->Count];
    this->BSPPlaneArray.Item = new QuME_BSP_Plane[this->BSPPlaneArray.Count];
    if(this->BSPPlaneArray.Item == nullptr)
    {
        delete binData;
        return false;
    }

    binData->UseBasicPrecisions();

    for(wxUint32 i = 0; i < this->BSPPlaneArray.Count; i++)
    {
        this->BSPPlaneArray.Item[i].Plane.Normal.x = binData->ReadFloat();
        this->BSPPlaneArray.Item[i].Plane.Normal.y = binData->ReadFloat();
        this->BSPPlaneArray.Item[i].Plane.Normal.z = binData->ReadFloat();
        this->BSPPlaneArray.Item[i].Plane.Distance = binData->ReadFloat();
        this->BSPPlaneArray.Item[i].Flags = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}
