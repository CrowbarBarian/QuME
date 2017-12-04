#include "QuME_BSP_Lump.h"

QuME_BSP_Lump::QuME_BSP_Lump()
{
    //ctor
    Offset = 0;
    Length = 0;
}

void QuME_BSP_Lump::DebugDump(wxTextOutputStream& out)
{
    out << "Offset: " << this->Offset << ", Length: " << this->Length << "\n";
}
