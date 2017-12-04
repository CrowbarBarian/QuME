#ifndef QUME_ARRAY_H_INCLUDED
#define QUME_ARRAY_H_INCLUDED

#include "wx/defs.h" //for wxUint32

template <class T>
class QuME_Array
{
public:
    QuME_Array()
    {
        Count = 0;
        Item = nullptr;
    }
    ~QuME_Array()
    {
        Count = 0;
        if(Item != nullptr) delete[] Item;
    }

    wxUint32 Count;
    T* Item;
};

#endif // QUME_ARRAY_H_INCLUDED
