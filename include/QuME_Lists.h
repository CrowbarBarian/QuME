#ifndef QUME_LISTS_H_INCLUDED
#define QUME_LISTS_H_INCLUDED

#include "QuME_Common.h" //for wxUint32 def

template <class T>
class ListItem
{
public:
    ListItem()
    {
        next = nullptr;
        prev = nullptr;
    }
    explicit ListItem(const T& inData):Data(inData)
    {
        next = nullptr;
        prev = nullptr;
    }

    ListItem* next;
    ListItem* prev;
    T Data;
};

template <class T>
class QuME_LinkedList
{
public:
    typedef ListItem<T> list_type;
    typedef list_type* list_pointer;

    QuME_LinkedList()
    {
        Head = nullptr;
        Tail = nullptr;
        Count = 0;
        NextIndex = 0;
    }

    ~QuME_LinkedList()
    {
        while(Head != nullptr)
        {
            ListItem<T>* t = Head->next;
            SAFE_DELETE(Head);
            Head = t;
            Count--;
        }
        Tail = nullptr;
    }

    wxUint32 Append(const T& Item);
    //wxUint32 Append(T Item);
    wxUint32 Append(const QuME_LinkedList<T>& List);

    bool AddIfUnique(const T& Item);
    //T* ToArray(wxUint32& OutCount);
    wxUint32 ToArray(T*& OutArray);
    wxUint32 ToArray(T OutArray);
    ListItem<T>* Remove(T& Item);

    ListItem<T>* Head;
    ListItem<T>* Tail;
    wxUint32 NextIndex;
    wxUint32 Count;
};

template <class T>
wxUint32 QuME_LinkedList<T>::Append(const T& Item)
{
    if(Head == nullptr)
    {
        Head = new ListItem<T>(Item);
        Tail = Head;
    }
    else
    {
        Tail->next = new ListItem<T>(Item);
        Tail->next->prev = Tail;
        Tail = Tail->next;
    }
    Count++;
    return NextIndex++;
}

template <class T>
wxUint32 QuME_LinkedList<T>::Append(const QuME_LinkedList<T>& List)
{
    wxUint32 nindex;
    for(ListItem<T>* i = List.Head; i != nullptr; i = i->next)
    {
        //make copy of payload
        nindex = this->Append(i->Data);
    }
    return nindex; //<-- does this work??
}

template <class T>
bool QuME_LinkedList<T>::AddIfUnique(const T& Item)
{
    if(Head == nullptr)
    {
        Head = new ListItem<T>(Item);
        Tail = Head;
        Count++;
        return true;
    }

    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        if(l->Data == Item) return false;
    }

    Tail->next = new ListItem<T>(Item);
    Tail->next->prev = Tail;
    Tail = Tail->next;
    Count++;
    return true;
}

template <class T>
ListItem<T>* QuME_LinkedList<T>::Remove(T& Item)
{
    ListItem<T>* t = Head;

    while(t != nullptr)
    {
        if(t->Data == Item)
        {
            if(t->prev == nullptr) //head of list(or only list item)
            {
                Head = t->next;
                if(Head !=nullptr) Head->prev = nullptr;
                t->next = nullptr;
                t->prev = nullptr;
                Count--;
                return t;
            }
            else if(t->next == nullptr) //end of list
            {
                Tail = t->prev;
                Tail->next = nullptr;
                t->next = nullptr;
                t->prev = nullptr;
                Count--;
                return t;
            }
            else //middle of list
            {
                t->prev->next = t->next;
                t->next->prev = t->prev;
                t->next = nullptr;
                t->prev = nullptr;
                Count--;
                return t;
            }
        }
        t = t->next;
    }
    return nullptr; //not found
}

#if 0
template <class T>
T* QuME_LinkedList<T>::ToArray(wxUint32& OutCount)
{
    OutCount = 0;
    T* t = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        t[OutCount++] = l->Data;
    }
    return t;
}
#endif // 0

template <class T>
wxUint32 QuME_LinkedList<T>::ToArray(T*& OutArray)
{
    wxUint32 t = 0;
    SAFE_ARRAY_DELETE(OutArray);
    OutArray = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        OutArray[t++] = l->Data;
    }
    return t;
}

template <class T>
wxUint32 QuME_LinkedList<T>::ToArray(T OutArray)
{
    wxUint32 t = 0;
    SAFE_ARRAY_DELETE(OutArray);
    OutArray = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        OutArray[t++] = l->Data;
    }
    return t;
}

#endif // QUME_LISTS_H_INCLUDED
