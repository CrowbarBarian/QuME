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
}

QuME_BSP_SurfaceEdges::~QuME_BSP_SurfaceEdges()
{
}

bool QuME_BSP_SurfaceEdges::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
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

    this->SurfaceEdgeIndices.Allocate(length / QUME_BSP_SURF_EDGE_DATA_SIZE);

    for(wxUint32 i = 0; i < this->SurfaceEdgeIndices.Count; i++)
    {
        this->SurfaceEdgeIndices[i] = binData->Read32();
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
    for(wxUint32 i = 0; i < this->SurfaceEdgeIndices.Count; i++)
    {
        out << L"Surface Edge #" << i << ": " << this->SurfaceEdgeIndices[i] << L"\n";
    }
    out << L"\n-------------------------------------------\n";
}
