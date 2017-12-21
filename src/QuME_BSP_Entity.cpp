#include "QuME_BSP_Entity.h"

QuME_BSP_Entity::QuME_BSP_Entity()
{
    this->index = 0;
    this->keyCount = 0;
    this->keys = nullptr;
    this->keyTail = nullptr;
}

QuME_BSP_Entity::~QuME_BSP_Entity()
{
    while(this->keys != nullptr)
    {
        ent_key* t = this->keys->next;
        delete this->keys;
        this->keys = t;
    }
    this->keyTail = nullptr;
}
QuME_BSP_Entity& QuME_BSP_Entity::operator=(const QuME_BSP_Entity& o)
{
    //make a copy of the other's keys
    for(ent_key* i = o.keys; i != nullptr; i = i->next)
    {
        this->addKey(i->keyName, i->keyValue);
    }
    return *this;
}

/*bool QuME_BSP_Entity::addKey(const wchar_t* name, const wchar_t* value)
{
    this->keyCount++;
    ent_key* k=nullptr;

    if(this->keys == nullptr)
    {
        this->keys = new ent_key;
        this->keys->next = nullptr;
        k = this->keys;
        this->keyTail = k;
    }
    else
    {
        k = this->keyTail;
        k->next = new ent_key;
        k = k->next;
        k->next = nullptr;
    }
    k->keyName = std::wstring(name);
    k->keyValue = std::wstring(value);
    return true;
}*/

bool QuME_BSP_Entity::addKey(const std::wstring &name, const std::wstring &value)
{
    this->keyCount++;
    ent_key* k=nullptr;

    if(this->keys == nullptr)
    {
        this->keys = new ent_key;
        this->keys->next = nullptr;
        k = this->keys;
    }
    else
    {
        k = this->keyTail;
        k->next = new ent_key;
        k = k->next;
        k->next = nullptr;
    }
	this->keyTail = k; //another bug squashed...forgot to add this line
    k->keyName = name;
    k->keyValue = value;
    return true;
}

#if 0
void QuME_BSP_Entity::SetIndex(wxUint32 index)
{
    this->index = index;
}


wxUint32 QuME_BSP_Entity::GetIndex()
{
    return this->index;
}

wxUint32 QuME_BSP_Entity::GetKeyCount()
{
    return this->keyCount;
}
#endif // 0

void QuME_BSP_Entity::DebugDump(wxTextOutputStream& out)
{
    out << L"Index: " << this->index << L"\n";
    out << L"Keys: " << this->keyCount << L"\n";
    for(ent_key* k = this->keys; k != nullptr; k = k->next)
    {
        out << k->keyName << L": " << k->keyValue << L"\n";
    }

}

