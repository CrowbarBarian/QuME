#include "QuME_BSP_Vertices.h"
#include <wx/log.h>

QuME_BSP_Vertices::QuME_BSP_Vertices()
{
    this->ArrayCount = 0;
    this->VertexArray = nullptr;
}

QuME_BSP_Vertices::~QuME_BSP_Vertices()
{
    this->ArrayCount = 0;
    SAFE_ARRAY_DELETE(this->VertexArray);
    this->VertexArray = nullptr;
}

bool QuME_BSP_Vertices::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    this->ArrayCount = length / 12;

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

    this->VertexArray = new QuME_Vector[this->ArrayCount];
    if(this->VertexArray == nullptr)
    {
        delete binData;
        return false;
    }

    binData->UseBasicPrecisions(); //Need this to Get correct input data

    for(wxUint32 i = 0; i < this->ArrayCount; i++)
    {
        for(wxUint32 j = 0; j < 3; j++)
        {
            this->VertexArray[i].vec[j] = binData->ReadFloat();
        }
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
    out << "Vertices: " << this->ArrayCount << "\n";
    for(wxUint32 i = 0; i < this->ArrayCount; i++)
    {
        out << "Vertex " << i << ": ";
        this->VertexArray[i].DebugDump(out);
    }
    out << "\n------------------------------------\n";
}
