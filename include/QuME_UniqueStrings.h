#ifndef QUME_UNIQUESTRINGS_H
#define QUME_UNIQUESTRINGS_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

class QuME_UniqueStrings
{
public:
    QuME_UniqueStrings();
    ~QuME_UniqueStrings();
    bool IsUnique(std::wstring& s);

    QuME_LinkedList<std::wstring> UniqueStringsList;
};

#endif // QuME_BSP_TEXNAMESSEEN_H
