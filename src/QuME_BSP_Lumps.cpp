#include "QuME_BSP_Lumps.h"

//Lump name table
wxString lumpNames[LUMP_MAX+1] = {
                            wxT("Entities"),
                            wxT("Planes"),
                            wxT("Vertices"),
                            wxT("Visibility"),
                            wxT("Nodes"),
                            wxT("Texture Information"),
                            wxT("Faces"),
                            wxT("Light Maps"),
                            wxT("Leaves"),
                            wxT("Leaf Face Table"),
                            wxT("Leaf Brush Table"),
                            wxT("Edges"),
                            wxT("Face Edge Table"),
                            wxT("Models"),
                            wxT("Brushes"),
                            wxT("Brush Sides"),
                            wxT("Pop"),
                            wxT("Areas"),
                            wxT("Area Portals"),
                            wxT("Bad Lump Index")}; //Bad way to do this I think...

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
    out << "Lumps: \n";
    for(wxUint32 i = 0; i < LUMP_MAX; i++)
    {
        out << lumpNames[i] << ": ";
        this->Lump[i].DebugDump(out);
    }
    out << "\n-----------------------------------\n";
}
