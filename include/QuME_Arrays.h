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
        Data = nullptr;
    }
    ~QuME_Array()
    {
        Count = 0;
        SAFE_ARRAY_DELETE(Data);
    }

    wxUint32 Count;
    T* Data;
};

#endif // QUME_ARRAY_H_INCLUDED
