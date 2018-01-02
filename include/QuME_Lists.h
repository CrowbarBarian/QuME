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
class QuME_Array
{
public:
	QuME_Array()
	{
		this->Data = nullptr;
		this->Count = 0;
	}
	explicit QuME_Array(QuME_Array<T>& inArray)
	{
		if(inArray.Count > 0)
		{
			this->Data = new T[inArray.Count];
			this->Count = inArray.Count;
			for(wxUint64 i = 0; i < inArray.Count; i++)
			{
				this->Data[i] = inArray.Data[i];
			}
		}
	}
	QuME_Array<T>& operator=(QuME_Array<T>& inArray)
	{
		if(inArray.Count > 0)
		{
			this->Data = new T[inArray.Count];
			this->Count = inArray.Count;
			for(wxUint64 i = 0; i < inArray.Count; i++)
			{
				this->Data[i] = inArray.Data[i];
			}
		}
		return *this;
	}
	void Allocate(std::size_t c)
	{
		SAFE_ARRAY_DELETE(this->Data);
		this->Count = c;
		this->Data = new T[c];
	}
	T& operator[](std::size_t Index)
	{
		return this->Data[Index];
	}
	const T& operator[](std::size_t Index) const
	{
		return this->Data[Index];
	}
	T* Data;
	std::size_t Count;
};

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
    //remember, all items added to a list are cloned
    //so don't forget to delete originals if not needed
    explicit ListItem(T& inData):Data(inData)
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
    std::size_t Index;
    T Data;
};

template <class T>
class QuME_LinkedList
{
public:
    //typedef ListItem<T> list_type;
    //typedef list_type* list_pointer;

    QuME_LinkedList()
    {
        Head = nullptr;
        Tail = nullptr;
        Count = 0;
        NextIndex = 0;
    }

    explicit QuME_LinkedList(QuME_LinkedList<T>& inList)
    {
    	this->Append(inList);
    }

    ~QuME_LinkedList()
    {
    	this->ClearList();
    }

    void ClearList();
    bool isUnique(T& Item);
    std::size_t Add(T& Item);
    std::size_t AddIfUnique(T& Item);
    std::size_t Append(T& Item);
    std::size_t Append(QuME_LinkedList<T>& List);

    std::size_t AppendIfUnique(T& Item);
    std::size_t ToArray(T*& OutArray);
    void ToArray(QuME_Array<T>& OutArray);
    ListItem<T>* Remove(T& Item);
    ListItem<T>* RemoveByIndex(std::size_t idx);

    ListItem<T>* Head;
    ListItem<T>* Tail;
    std::size_t NextIndex;
    std::size_t Count;
};

template <class T>
void QuME_LinkedList<T>::ClearList()
{
	while(this->Head != nullptr)
	{
		ListItem<T>* t = this->Head->next;
		delete this->Head;
		this->Head = t;
		this->Count--;
	}
	this->NextIndex = 0;
	this->Tail = nullptr;
}

template <class T>
bool QuME_LinkedList<T>::isUnique(T& Item)
{
	if(Head == nullptr) return true; //always unique in an empty list
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        if(l->Data == Item) return false; //found it, not unique
    }
	return true; //not found in list, unique
}

template <class T>
std::size_t QuME_LinkedList<T>::Add(T& Item)
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
std::size_t QuME_LinkedList<T>::AddIfUnique(T& Item)
{
	std::size_t idx = 0;
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
std::size_t QuME_LinkedList<T>::Append(T& Item)
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
std::size_t QuME_LinkedList<T>::Append(QuME_LinkedList<T>& List)
{
    std::size_t nindex;
    for(ListItem<T>* i = List.Head; i != nullptr; i = i->next)
    {
        //make copy of payload
        nindex = this->Append(i->Data);
    }
    return nindex; //<-- does this work??
}

template <class T>
std::size_t QuME_LinkedList<T>::AppendIfUnique(T& Item)
{
	std::size_t idx = 0;
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
ListItem<T>* QuME_LinkedList<T>::RemoveByIndex(std::size_t idx)
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
std::size_t QuME_LinkedList<T>::ToArray(T*& OutArray)
{
    std::size_t t = 0;
    SAFE_ARRAY_DELETE(OutArray);
    OutArray = new T[Count];
    for(ListItem<T>* l = Head; l != nullptr; l = l->next)
    {
        OutArray[t++] = l->Data;
    }
    return t;
}

//convenience method
template <class T>
void QuME_LinkedList<T>::ToArray(QuME_Array<T>& OutArray)
{
	OutArray.Count = this->ToArray(OutArray.Data);
}

#endif // QUME_LISTS_H_INCLUDED
