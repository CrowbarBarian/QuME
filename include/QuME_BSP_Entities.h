#ifndef QUME_BSPENTITIES_H_INCLUDED
#define QUME_BSPENTITIES_H_INCLUDED

#include "QuME_Common.h"

#include "QuME_BSP_Entity.h"

enum ENT_PARSE_STATE
{
    ENT_START = 0,
    LOOKING_FOR_ENT,
    NEW_ENT,
    KEY_NAME,
    KEY_VAL,
    END_ENT,
    END_ENTS,
    BAD_ENTS
};

struct QuME_BSP_EntList
{
    QuME_BSP_EntList* next;
    QuME_BSP_Entity entity;
};

class QuME_BSP_Entities
{
public:
    QuME_BSP_Entities();
    ~QuME_BSP_Entities();
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
    QuME_BSP_Entity* GetEntity(wxUint32 index);
    QuME_BSP_EntList* createNewEnt(wxUint32 index);
    void DebugDump(wxTextOutputStream& out);

    wxUint32 Count;
    QuME_BSP_EntList* EntList;
    QuME_BSP_Entity* Entity;
};


#endif // QuME_BSP_ENTITIES_H_INCLUDED
