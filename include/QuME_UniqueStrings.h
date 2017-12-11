#ifndef QUME_UNIQUESTRINGS_H
#define QUME_UNIQUESTRINGS_H

#include "QuME_Common.h"

struct UniqueStringList
{
    UniqueStringList* next;
    std::wstring stringData;
};


class QuME_UniqueStrings
{
public:
    QuME_UniqueStrings();
    ~QuME_UniqueStrings();
    bool IsUnique(std::wstring& s);

    UniqueStringList* UniqueStrings;
};

#endif // QuME_BSP_TEXNAMESSEEN_H
