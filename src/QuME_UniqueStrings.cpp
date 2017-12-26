/***************************************************************
 * Name:		QuME_UniqueStrings
 * Purpose:		Code to determine if a string has been seen before
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_UniqueStrings.h"

QuME_UniqueStrings::QuME_UniqueStrings()
{
    this->UniqueStrings = nullptr;
}

QuME_UniqueStrings::~QuME_UniqueStrings()
{
    UniqueStringList* t;
    while(this->UniqueStrings != nullptr)
    {
        t = this->UniqueStrings->next;
        delete this->UniqueStrings;
        this->UniqueStrings = t;
    }
    this->UniqueStrings = nullptr;
}

bool QuME_UniqueStrings::IsUnique(std::wstring& t)
{
    if(this->UniqueStrings == nullptr)
    {
        this->UniqueStrings = new UniqueStringList();
        this->UniqueStrings->stringData = t;
        this->UniqueStrings->next = nullptr;
        return true;
    }
    UniqueStringList* i;
    UniqueStringList* k;
    for(i = this->UniqueStrings; i != nullptr; i = i->next)
    {
        k = i;
        if(i->stringData == t)
        {
            return false;
        }
    }
    k->next = new UniqueStringList;
    k = k->next;
    k->next = nullptr;
    k->stringData = t;
    return true;
}
