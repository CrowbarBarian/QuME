#ifndef QUME_BSPENTITY_H_INCLUDED
#define QUME_BSPENTITY_H_INCLUDED

#include "QuME_Common.h"

struct ent_key
{
    ent_key* next;
    std::wstring keyName;
    std::wstring keyValue;
};

class QuME_BSP_Entity
{
public:

    QuME_BSP_Entity();
    ~QuME_BSP_Entity();
    //bool addKey(const wchar_t* name, const wchar_t* value);
    bool addKey(const std::wstring name, const std::wstring value);
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
