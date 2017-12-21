#include "QuME_BSP_Vertices.h"
#include <wx/log.h>

QuME_BSP_Vertices::QuME_BSP_Vertices()
{
    this->VertexCount = 0;
    this->VertexArray = nullptr;
}

QuME_BSP_Vertices::~QuME_BSP_Vertices()
{
    this->VertexCount = 0;
    SAFE_ARRAY_DELETE(this->VertexArray);
    this->VertexArray = nullptr;
}

bool QuME_BSP_Vertices::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 lumpLength)
{
    this->VertexCount = lumpLength / Q2_BSP_VERTEX_DATA_SIZE;

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

    this->VertexArray = new QuME_Vector[this->VertexCount];
    if(this->VertexArray == nullptr)
    {
        delete binData;
        return false;
    }

    binData->UseBasicPrecisions(); //Need this to Get correct input data

    for(wxUint32 i = 0; i < this->VertexCount; i++)
    {
		this->VertexArray[i].x = binData->ReadFloat();
		this->VertexArray[i].y = binData->ReadFloat();
		this->VertexArray[i].z = binData->ReadFloat();
    }

    if(!binData->IsOk())
    {
        delete[] VertexArray;
        delete binData;
        return false;
    }

    delete binData;
    return true;
}

void QuME_BSP_Vertices::DebugDump(wxTextOutputStream& out)
{
    out << L"Vertices: " << this->VertexCount << L"\n";
    for(wxUint32 i = 0; i < this->VertexCount; i++)
    {
        out << L"Vertex " << i << ": ";
        this->VertexArray[i].DebugDump(out);
    }
    out << L"\n------------------------------------\n";
}
