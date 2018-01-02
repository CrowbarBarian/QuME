/***************************************************************
 * Name:		QuME_BSP_Edges.cpp
 * Purpose:		Class to handle BSP edges
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_Edges.h"

QuME_BSP_Edges::QuME_BSP_Edges()
{
}

QuME_BSP_Edges::~QuME_BSP_Edges()
{
}

bool QuME_BSP_Edges::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
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

    this->Edges.Allocate(length / Q2_BSP_EDGE_DATA_SIZE);

    for(wxUint32 i = 0; i < this->Edges.Count; i++)
    {
    	wxInt16 ta = binData->Read16();
    	wxInt16 tb = binData->Read16();
        this->Edges[i].a = ta;
        this->Edges[i].b = tb;
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

void QuME_BSP_Edges::DebugDump(wxTextOutputStream& out)
{
    out << L"Edges:\nCount: " << std::to_wstring(this->Edges.Count) << L"\n";
    for(wxUint32 i = 0; i < this->Edges.Count; i++)
    {
        out << L"a: " << this->Edges[i].a << L", b: " << this->Edges[i].b << L"\n";
    }
    out << L"\n----------------------------------\n";
}
