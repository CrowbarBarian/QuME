/***************************************************************
 * Name:		QuME_Lists.h
 * Purpose:		Defines Linked List template
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#ifndef QUME_LISTS_H_INCLUDED
#define QUME_LISTS_H_INCLUDED

#include "QuME_Common.h" //for wxUint64 def

template <class T>
class ListItem
{
public:
    ListItem()
    {
        next = nullptr;
        prev = nullptr;
        Index = 0;
    }
    explicit ListItem(const T& inData):Data(inData)
    {
        next = nullptr;
        prev = nullptr;
    }
    ~ListItem()
    {
		next = nullptr;
		prev = nullptr;
    }

    ListItem* next;
    ListItem* prev;
    wxUint64 Index;
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
            delete Head;
            Head = t;
            Count--;
        }
        Tail = nullptr;
    }

    wxUint64 Add(const T& Item);
    wxUint64 AddIfUnique(const T& Item);
    wxUint64 Append(const T& Item);
    wxUint64 Append(const QuME_LinkedList<T>& List);

    wxUint64 AppendIfUnique(const T& Item);
    wxUint64 ToArray(T*& OutArray);
    ListItem<T>* Remove(T& Item);
    ListItem<T>* RemoveByIndex(wxUint64 idx);

    ListItem<T>* Head;
    ListItem<T>* Tail;
    wxUint64 NextIndex;
    wxUint64 Count;
};

template <class T>
wxUint64 QuME_LinkedList<T>::Add(const T& Item)
{
	if(Head == nullptr)
	{
		Head = new ListItem<T>(Item);
		Tail = Head;
	}
	else
	{
		Head->prev = new ListItem<T>(Item);
		Head->prev->next = Head;
		Head = Head->prev;
		Head->prev = nullptr;
	}
	Count++;
	Head->Index = NextIndex++;
	return Head->Index;
}

template <class T>
wxUint64 QuME_LinkedList<T>::AddIfUnique(const T& Item)
{
	wxUint64 idx = 0;
    if(Head == nullptr)
    {
    	idx = this->Add(Item);
    	return idx;
    }

    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        if(l->Data == Item) return l->Index;
    }

    idx = this->Add(Item);
    return idx;
}

template <class T>
wxUint64 QuME_LinkedList<T>::Append(const T& Item)
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
        Tail->next = nullptr;
    }
    Count++;
    Tail->Index = NextIndex++;
    return Tail->Index;
}

template <class T>
wxUint64 QuME_LinkedList<T>::Append(const QuME_LinkedList<T>& List)
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
wxUint64 QuME_LinkedList<T>::AppendIfUnique(const T& Item)
{
	wxUint64 idx = 0;
    if(Head == nullptr)
    {
    	idx = this->Append(Item);
    	return idx;
    }

    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        if(l->Data == Item) return l->Index;
    }

    idx = this->Append(Item);
    return idx;
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

template <class T>
ListItem<T>* QuME_LinkedList<T>::RemoveByIndex(wxUint64 idx)
{
    ListItem<T>* t = Head;

    while(t != nullptr)
    {
        if(t->Index == idx)
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
wxUint64 QuME_LinkedList<T>::ToArray(T*& OutArray)
{
    wxUint64 t = 0;
    SAFE_ARRAY_DELETE(OutArray);
    OutArray = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        OutArray[t++] = l->Data;
    }
    return t;
}

#endif // QUME_LISTS_H_INCLUDED
