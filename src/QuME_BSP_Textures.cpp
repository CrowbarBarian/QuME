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

bool QuME_BSP_Textures::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length, const wxString& baseDir)
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

        t->MaterialName = wxString::FromUTF8(reinterpret_cast<char*>(c)); //convert ASCII name to wxString object

        //fix the problem of random upper/lower case in the file names
        //this is a result of the DOS/Windows file-system case-insensitivity
        t->MaterialName.LowerCase();
        t->NextTexInfo = binData->Read32();

        //open .wal file to get necessary info on width and height
        wxInputStream* walFile = new wxFileInputStream(baseDir +
                                                       "textures/" +
                                                       t->MaterialName +
                                                       ".wal");
        if(!walFile->IsOk())
        {
            this->FaultTexture = baseDir + "textures/" + t->MaterialName + ".wal";
            return false;
        }
        wxDataInputStream* walData = new wxDataInputStream(*walFile);

        walData->Read8(c, TEXTURE_NAME_SIZE);
        c[TEXTURE_NAME_SIZE - 1] = 0; //just in case...
        wxString walName = c;
        walName.LowerCase();

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
    out << "Textures: " << this->Count << "\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << "Texture " << i << "\n";
        this->Texture[i].DebugDump(out);
    }
    out << "\n------------------------------\n";
}
