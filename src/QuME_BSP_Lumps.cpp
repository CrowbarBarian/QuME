#include "QuME_BSP_Lumps.h"

//Lump name table
std::wstring lumpNames[LUMP_MAX+1] = {
                            L"Entities",
                            L"Planes",
                            L"Vertices",
                            L"Visibility",
                            L"Nodes",
                            L"Texture Information",
                            L"Faces",
                            L"Light Maps",
                            L"Leaves",
                            L"Leaf Face Table",
                            L"Leaf Brush Table",
                            L"Edges",
                            L"Face Edge Table",
                            L"Models",
                            L"Brushes",
                            L"Brush Sides",
                            L"Pop",
                            L"Areas",
                            L"Area Portals",
                            L"Bad Lump Index"}; //Bad way to do this I think...

QuME_BSP_Lumps::QuME_BSP_Lumps()
{

}

QuME_BSP_Lumps::~QuME_BSP_Lumps()
{

}

bool QuME_BSP_Lumps::LoadLumps(wxFileInputStream* infile, wxUint32 LumpTableOffset)
{
    if(LumpTableOffset == UNKNOWN_LUMP_TABLE) return false; //unsupported version

    wxDataInputStream binData( *infile );

    if(!binData.IsOk())
    {
        return false;
    }

    if (infile->SeekI(LumpTableOffset, wxFromStart) == wxInvalidOffset)
    {
        return false;
    }

    for(wxUint32 i = 0; i < LUMP_MAX; i++)
    {
        this->Lump[i].Offset = binData.Read32();
        this->Lump[i].Length = binData.Read32();
    }
    if(!binData.IsOk())
    {
        return false;
    }
    return true;

}

void QuME_BSP_Lumps::DebugDump(wxTextOutputStream& out)
{
    out << L"Lumps: \n";
    for(wxUint32 i = 0; i < LUMP_MAX; i++)
    {
        out << lumpNames[i] << L": ";
        this->Lump[i].DebugDump(out);
    }
    out << L"\n-----------------------------------\n";
}
