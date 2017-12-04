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
    explicit ListItem(const T& Item):Item(Item)
    {
        next = nullptr;
        prev = nullptr;
    }

    ListItem* next;
    ListItem* prev;
    T Item;
};

template <class T>
class LinkedList
{
public:
    typedef ListItem<T> list_type;
    typedef list_type* list_pointer;

    LinkedList()
    {
        Head = nullptr;
        Tail = nullptr;
        Count = 0;
        NextIndex = 0;
    }

    ~LinkedList()
    {
        while(Head != nullptr)
        {
            ListItem<T>* t = Head->next;
            delete Head;
            Head = t;
            Count--;
        }
    }

    wxUint32 Append(T& Item);
    wxUint32 Append(LinkedList<T>& List);

    bool AddIfUnique(const T& Item);
    T* ToArray(wxUint32& OutCount);
    wxUint32 ToArray(T*& OutArray);
    ListItem<T>* Remove(T& Item);

    ListItem<T>* Head;
    ListItem<T>* Tail;
    wxUint32 NextIndex;
    wxUint32 Count;
};

template <class T>
wxUint32 LinkedList<T>::Append(T& Item)
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
wxUint32 LinkedList<T>::Append(LinkedList<T>& List)
{
    wxUint32 nindex;
    for(ListItem<T>* i = List.Head; i != nullptr; i = i->next)
    {
        //make copy of payload
        nindex = this->Append(i->Item);
    }
    return nindex; //<-- does this work??
}

template <class T>
bool LinkedList<T>::AddIfUnique(const T& Item)
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
        if(l->Item == Item) return false;
    }

    Tail->next = new ListItem<T>(Item);
    Tail->next->prev = Tail;
    Tail = Tail->next;
    Count++;
    return true;
}

template <class T>
ListItem<T>* LinkedList<T>::Remove(T& Item)
{
    ListItem<T>* t = Head;

    while(t != nullptr)
    {
        if(t->Item == Item)
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

template <class T>
T* LinkedList<T>::ToArray(wxUint32& OutCount)
{
    OutCount = 0;
    T* t = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        t[OutCount++] = l->Item;
    }
    return t;
}

template <class T>
wxUint32 LinkedList<T>::ToArray(T*& OutArray)
{
    wxUint32 t = 0;
    OutArray = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        OutArray[t++] = l->Item;
    }
    return t;
}

#endif // QUME_LISTS_H_INCLUDED
