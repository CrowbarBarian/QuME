/***************************************************************
 * Name:		QuME_BSP_SurfaceEdges.cpp
 * Purpose:		Class to handle BSP surface edges
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_SurfaceEdges.h"

QuME_BSP_SurfaceEdges::QuME_BSP_SurfaceEdges()
{
    //ctor
    this->Count = 0;
    this->SurfaceEdge = nullptr;
}

QuME_BSP_SurfaceEdges::~QuME_BSP_SurfaceEdges()
{
    //dtor
    this->Count = 0;
    if(this->SurfaceEdge != nullptr) delete[] this->SurfaceEdge;
}

bool QuME_BSP_SurfaceEdges::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    this->Count = length / 4;

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

    this->SurfaceEdge = new wxInt32[this->Count];
    if(this->SurfaceEdge == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < this->Count; i++)
    {
        this->SurfaceEdge[i] = binData->Read32();
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

void QuME_BSP_SurfaceEdges::DebugDump(wxTextOutputStream& out)
{
    out << L"Surface edge indices:\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << L"Surface Edge #" << i << ": " << this->SurfaceEdge[i] << L"\n";
    }
    out << L"\n-------------------------------------------\n";
}
