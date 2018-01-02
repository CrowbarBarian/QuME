/***************************************************************
 * Name:		QuME_BSP_Vertices.cpp
 * Purpose:		Code to handle BSP vertices
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_Vertices.h"

QuME_BSP_Vertices::QuME_BSP_Vertices()
{
}

QuME_BSP_Vertices::~QuME_BSP_Vertices()
{
}

bool QuME_BSP_Vertices::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength)
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

    this->Vertices.Allocate(lumpLength / Q2_BSP_VERTEX_DATA_SIZE);

    binData->UseBasicPrecisions(); //Need this to get correct input data

    for(wxUint32 i = 0; i < this->Vertices.Count; i++)
    {
		this->Vertices[i].x = binData->ReadFloat();
		this->Vertices[i].y = binData->ReadFloat();
		this->Vertices[i].z = binData->ReadFloat();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

void QuME_BSP_Vertices::DebugDump(wxTextOutputStream& out)
{
    out << L"Vertices: " << std::to_wstring(this->Vertices.Count) << L"\n";
    for(wxUint32 i = 0; i < this->Vertices.Count; i++)
    {
        out << L"Vertex " << i << ": ";
        this->Vertices[i].DebugDump(out);
    }
    out << L"\n------------------------------------\n";
}
