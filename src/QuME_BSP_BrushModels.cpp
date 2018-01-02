/***************************************************************
 * Name:		QuME_BSP_BrushModels.cpp
 * Purpose:		Class to handle BSP brush models
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_BrushModels.h"

QuME_BSP_BrushModels::QuME_BSP_BrushModels()
{
}

QuME_BSP_BrushModels::~QuME_BSP_BrushModels()
{
}

bool QuME_BSP_BrushModels::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    wxDataInputStream* binData = new wxDataInputStream( *infile );

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    binData->UseBasicPrecisions(); //cause we're reading 32-bit floats, not 80 :P

    if (infile->SeekI(offset, wxFromStart) == wxInvalidOffset)
    {
        delete binData;
        return false;
    }

    this->BrushModels.Allocate(length / BRUSHMODEL_SIZE_ON_DISK);

    for(wxUint32 i = 0; i < this->BrushModels.Count; i++)
    {
        QuME_BSP_BrushModel* bmt = &this->BrushModels[i];
        bmt->BBox.Min.x = binData->ReadFloat();
        bmt->BBox.Min.y = binData->ReadFloat();
        bmt->BBox.Min.z = binData->ReadFloat();
        bmt->BBox.Max.x = binData->ReadFloat();
        bmt->BBox.Max.y = binData->ReadFloat();
        bmt->BBox.Max.z = binData->ReadFloat();
        bmt->Orig.x = binData->ReadFloat();
        bmt->Orig.y = binData->ReadFloat();
        bmt->Orig.z = binData->ReadFloat();
        bmt->HeadNode = binData->Read32();
        bmt->FirstFace = binData->Read32();
        bmt->NumFaces = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;

}

void QuME_BSP_BrushModels::DebugDump(wxTextOutputStream& out)
{
    out << L"BrushModels: " << std::to_wstring(this->BrushModels.Count) << L"\n";
    for(wxUint32 i = 0; i < this->BrushModels.Count; i++)
    {
        out << L"BrushModel #" << i << L"\n";
        this->BrushModels[i].DebugDump(out);
    }
    out << L"\n-----------------------------------\n";
}

void QuME_BSP_BrushModel::DebugDump(wxTextOutputStream& out)
{
    out << L"Bounds: " << this->BBox << L"\n";
    out << L"Origin: " << this->Orig << L"\n";
    out << L"HeadNode: " << this->HeadNode << L"\n";
    out << L"First Face: " << this->FirstFace << L"\n";
    out << L"Number of faces: " << this->NumFaces << L"\n";
}

