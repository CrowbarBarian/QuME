#ifndef QUME_UNIQUESTRINGS_H
#define QUME_UNIQUESTRINGS_H

#include "QuME_Common.h"

struct UniqueStringList
{
    UniqueStringList* next;
    wxString String;
};


class QuME_UniqueStrings
{
public:
    QuME_UniqueStrings();
    ~QuME_UniqueStrings();
    bool IsUnique(wxString& s);

    UniqueStringList* UniqueStrings;
};

#endif // QuME_BSP_TEXNAMESSEEN_H
