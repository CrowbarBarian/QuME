#ifndef QUME_BSPENTITY_H_INCLUDED
#define QUME_BSPENTITY_H_INCLUDED

#include "QuME_Common.h"

struct ent_key
{
    ent_key* next;
    wxString keyName;
    wxString keyValue;
};

class QuME_BSP_Entity
{
public:

    QuME_BSP_Entity();
    ~QuME_BSP_Entity();
    bool addKey(const char* name, const char* value);
    bool addKey(const wxString& name, const wxString& value);
    QuME_BSP_Entity& operator=(const QuME_BSP_Entity& o);
    void SetIndex(wxUint32 index);
    wxUint32 GetIndex();
    wxUint32 GetKeyCount();
    void DebugDump(wxTextOutputStream& out);

    wxUint32 index;
    wxUint32 keyCount;
    ent_key* keys;
    ent_key* keyTail;
};

#endif // QuME_BSP_ENTITY_H_INCLUDED
