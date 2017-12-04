#include "QuME_BSP_TextureUVs.h"

QuME_BSP_TextureUVs::QuME_BSP_TextureUVs()
{
    UVCount = 0;
    Head = nullptr;
    Tail = nullptr;
    UArray = nullptr;
    VArray = nullptr;
}

QuME_BSP_TextureUVs::~QuME_BSP_TextureUVs()
{
    while(Head != nullptr)
    {
        UVList* t = Head;
        Head = Head->next;
        delete t;
        UVCount--;
    }
    Tail = nullptr;
    if(UArray != nullptr) delete[] UArray;
    if(VArray != nullptr) delete[] VArray;
}

//simple linked-list system
UVList* QuME_BSP_TextureUVs::AddUVs(wxFloat64 U, wxFloat64 V)
{
    if(Head == nullptr)
    {
        Head = new UVList();
        Head->next = nullptr;
        Head->U = U;
        Head->V = V;
        UVCount++;
        Tail = Head;
        return Tail;
    }
    else
    {
        UVList* t = Tail;
        t->next = new UVList();
        t = t->next;
        Tail = t;
        t->next = nullptr;
        t->U = U;
        t->V = V;
        UVCount++;
        return Tail;
    }
}

void QuME_BSP_TextureUVs::ListToArray()
{
    UArray = new wxFloat64[UVCount];
    VArray = new wxFloat64[UVCount];
    wxUint32 Index = 0;
    UVList* t = Head;
    while(t != nullptr)
    {
        UArray[Index] = t->U;
        VArray[Index] = t->V;
        t = t->next;
        Index++;
    }
}
