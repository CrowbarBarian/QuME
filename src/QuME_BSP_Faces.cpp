/***************************************************************
 * Name:
 * Purpose:
 * Author:    J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright: J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:   GPL v3
 **************************************************************/

#include "QuME_BSP_Faces.h"

QuME_BSP_Face::QuME_BSP_Face()
{
}

QuME_BSP_Face::~QuME_BSP_Face()
{
}

//naive triangulation routine
//this only works on convex polygons (no checks for concavity!)
wxInt32 QuME_BSP_Face::Triangulate()
{
    Triangles.Allocate(this->VArray.Count - 2);

	//makes a triangle fan
    for(wxUint32 i = 0; i < this->VArray.Count - 2; i++)
    {
    	Triangles[i].Vertex[0] = this->VArray[0];
    	Triangles[i].Vertex[1] = this->VArray[i+1];
    	Triangles[i].Vertex[2] = this->VArray[i+2];
    }
    return this->VArray.Count - 2;
}

void QuME_BSP_Face::DebugDump(wxTextOutputStream& out)
{
    out << L"Plane #" << this->Plane << L"\n";
    out << L"Plane Side: " << this->PlaneSide << L"\n";
    out << L"First Edge: " << this->FirstEdge << L"\n";
    out << L"Number of Edges: " << this->EdgeCount << L"\n";
    out << L"Texture Info: " << this->TextureInfo << L"\n";
    out << L"Light-map Style #0: " << this->LightmapStyles[0];
    out << L", Light-map Style #1: " << this->LightmapStyles[1];
    out << L", Light-map Style #2: " << this->LightmapStyles[2];
    out << L", Light-map Style #3: " << this->LightmapStyles[3]<< L"\n";
    out << L"Light-map Offset: " << this->LightmapOffset << L"\n";
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

QuME_BSP_Faces::QuME_BSP_Faces()
{
}

QuME_BSP_Faces::~QuME_BSP_Faces()
{
}

bool QuME_BSP_Faces::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
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

    this->Faces.Allocate(length / Q2_BSP_FACE_DATA_SIZE);

    for(wxUint32 i = 0; i < this->Faces.Count; i++)
    {
        this->Faces[i].Plane = binData->Read16();
        this->Faces[i].PlaneSide = static_cast<wxInt16>(binData->Read16()); //arg...stoopid wxWidgets don't have signed bin read
        this->Faces[i].FirstEdge = static_cast<wxInt32>(binData->Read32());
        this->Faces[i].EdgeCount = static_cast<wxInt16>(binData->Read16());
        this->Faces[i].TextureInfo = static_cast<wxInt16>(binData->Read16());
        for(wxUint32 j = 0; j < 4; j++) this->Faces[i].LightmapStyles[j] = binData->Read8();
        this->Faces[i].LightmapOffset= static_cast<wxInt32>(binData->Read32());
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}
void QuME_BSP_Faces::DebugDump(wxTextOutputStream& out)
{
    out << L"Faces:\n";
    for(wxUint32 i = 0; i < this->Faces.Count; i++)
    {
        out << L"Face #" << i << L"\n";
        this->Faces[i].DebugDump(out);
    }
    out << L"\n---------------------------------------\n";
}
