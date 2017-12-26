/***************************************************************
 * Name:		QuME_BSP_Textures.cpp
 * Purpose:		Code for the BSP texture class
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_Textures.h"

QuME_BSP_Textures::QuME_BSP_Textures()
{
    Count = 0;
    Texture = nullptr;
}

QuME_BSP_Textures::~QuME_BSP_Textures()
{
    Count = 0;
    if(Texture !=nullptr) delete[] Texture;
}

bool QuME_BSP_Textures::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length, const std::wstring& baseDir)
{
    this->Count = length / TEXTURE_SIZE_ON_DISK;

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

    this->Texture = new QuME_BSP_Texture[this->Count];
    if(this->Texture == nullptr)
    {
        delete binData;
        return false;
    }

    binData->UseBasicPrecisions(); //Need this to get correct input data...we're reading 32-bit floats, not 80-bit

    for(wxUint32 i = 0; i < this->Count; i++)
    {
        QuME_BSP_Texture* t = &this->Texture[i];
        t->uAxis.x = binData->ReadFloat();
        t->uAxis.y = binData->ReadFloat();
        t->uAxis.z = binData->ReadFloat();
        t->uOffset = binData->ReadFloat();
        t->vAxis.x = binData->ReadFloat();
        t->vAxis.y = binData->ReadFloat();
        t->vAxis.z = binData->ReadFloat();
        t->vOffset = binData->ReadFloat();
        t->Flags = binData->Read32();
        t->Value = binData->Read32();

        //read in texture name (ASCII)
        wxUint8 c[TEXTURE_NAME_SIZE];
        binData->Read8(c, TEXTURE_NAME_SIZE);
        c[TEXTURE_NAME_SIZE - 1] = 0; //just in case...

        copys2ws(c, t->MaterialName); //convert ASCII name to wstring object

        //fix the problem of random upper/lower case in the file names
        //this is a result of the DOS/Windows file-system case-insensitivity
        std::transform(t->MaterialName.begin(), t->MaterialName.end(), t->MaterialName.begin(), ::tolower);
        t->NextTexInfo = binData->Read32();

        //open .wal file to get necessary info on width and height
        wxInputStream* walFile = new wxFileInputStream(baseDir +
                                                       L"textures/" +
                                                       t->MaterialName +
                                                       L".wal");
        if(!walFile->IsOk())
        {
            this->FaultTexture = baseDir + L"textures/" + t->MaterialName + L".wal";
            return false;
        }
        wxDataInputStream* walData = new wxDataInputStream(*walFile);

        walData->Read8(c, TEXTURE_NAME_SIZE);
        c[TEXTURE_NAME_SIZE - 1] = 0; //just in case...
        std::wstring walName;
        copys2ws(c, walName);

        //fix random uppercase in names
        std::transform(walName.begin(), walName.end(), walName.begin(), ::tolower);

        t->xRes = walData->Read32();
        t->yRes = walData->Read32();

        delete walData;
        walData = nullptr;

        delete walFile;
        walFile = nullptr;
    }

    if(!binData->IsOk())
    {
        delete this->Texture;
        delete binData;
        return false;
    }

    delete binData;
    return true;
}


void QuME_BSP_Textures::DebugDump(wxTextOutputStream& out)
{
    out << L"Textures: " << this->Count << L"\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << L"Texture " << i << "\n";
        this->Texture[i].DebugDump(out);
    }
    out << L"\n------------------------------\n";
}
