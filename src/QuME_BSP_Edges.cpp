#include "QuME_BSP_Edges.h"

QuME_BSP_Edges::QuME_BSP_Edges()
{
    this->Count = 0;
    this->EdgeArray = nullptr;
}

QuME_BSP_Edges::~QuME_BSP_Edges()
{
    this->Count = 0;
    if(this->EdgeArray != nullptr) SAFE_ARRAY_DELETE(this->EdgeArray);
}

bool QuME_BSP_Edges::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    this->Count = length / Q2_BSP_EDGE_DATA_SIZE;

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

    this->EdgeArray = new QuME_BSP_Edge[this->Count];
    if(this->EdgeArray == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < this->Count; i++)
    {
        this->EdgeArray[i].a = binData->Read16();
        this->EdgeArray[i].b = binData->Read16();
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
    out << L"Edges:\nCount: " << this->Count << L"\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << L"a: " << this->EdgeArray[i].a << L", b: " << this->EdgeArray[i].b << L"\n";
    }
    out << L"\n----------------------------------\n";
}
