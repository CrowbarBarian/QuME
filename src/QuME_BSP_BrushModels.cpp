#include "QuME_BSP_BrushModels.h"

QuME_BSP_BrushModels::QuME_BSP_BrushModels()
{
    this->Count = 0;
    this->BrushModel = nullptr;
}

QuME_BSP_BrushModels::~QuME_BSP_BrushModels()
{
    this->Count = 0;
    if(this->BrushModel != nullptr) SAFE_ARRAY_DELETE(this->BrushModel);
}

bool QuME_BSP_BrushModels::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    this->Count = length / BRUSHMODEL_SIZE_ON_DISK;

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

    this->BrushModel = new QuME_BSP_BrushModel[this->Count];
    if(this->BrushModel == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < this->Count; i++)
    {
        QuME_BSP_BrushModel* bmt = &this->BrushModel[i];
        wxFloat64 a, b, c;
        a = binData->ReadFloat();
        b = binData->ReadFloat();
        c = binData->ReadFloat();
        bmt->SetMins(a,b,c);
        a = binData->ReadFloat();
        b = binData->ReadFloat();
        c = binData->ReadFloat();
        bmt->SetMaxs(a,b,c);
        a = binData->ReadFloat();
        b = binData->ReadFloat();
        c = binData->ReadFloat();
        bmt->SetOrigin(a,b,c);
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
    out << L"BrushModels: " << this->Count << L"\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << L"BrushModel #" << i << L"\n";
        this->BrushModel[i].DebugDump(out);
    }
    out << L"\n-----------------------------------\n";
}

void QuME_BSP_BrushModel::DebugDump(wxTextOutputStream& out)
{
    out << L"Mins: X:" << this->Mins[0] << L" Y: " << this->Mins[1] << L" Z: " << this->Mins[2] << L"\n";
    out << L"Maxs: X:" << this->Maxs[0] << L" Y: " << this->Maxs[1] << L" Z: " << this->Maxs[2] << L"\n";
    out << L"Origin: X:" << this->Origin[0] << L" Y: " << this->Origin[1] << L" Z: " << this->Origin[2] << L"\n";
    out << L"HeadNode: " << this->HeadNode << L"\n";
    out << L"First Face: " << this->FirstFace << L"\n";
    out << L"Number of faces: " << this->NumFaces << L"\n";
}

