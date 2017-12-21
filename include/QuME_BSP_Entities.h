#ifndef QUME_BSPENTITIES_H_INCLUDED
#define QUME_BSPENTITIES_H_INCLUDED

#include "QuME_Common.h"
#include "QuME_Lists.h"

#include "QuME_BSP_Entity.h"

class QuME_KeyPair
{
public:
	QuME_KeyPair();
	QuME_KeyPair(const std::wstring& inName, const std::wstring& inValue);

	QuME_KeyPair& operator=(const QuME_KeyPair& o);

	~QuME_KeyPair();

	bool ParseKeyPair(wxUint8* buffer, wxUint32& currentIndex, wxUint32 length);

	std::wstring KeyName;
	std::wstring KeyValue;
};

class QuME_Entity
{
public:
	QuME_Entity();
	QuME_Entity(const QuME_Entity& o);

	QuME_Entity& operator=(const QuME_Entity& o);

	~QuME_Entity();

	void DebugDump(wxTextOutputStream& out);

    bool ParseEntity(wxUint8* buffer, wxUint32& currentIndex, wxUint32 length);

	QuME_LinkedList<QuME_KeyPair> Keys;
	QuME_KeyPair* KeyArray;
	wxUint32 KeyCount;
};

class QuME_BSP_Entities
{
public:
    QuME_BSP_Entities();
    ~QuME_BSP_Entities();
    bool ParseEntities(wxUint8* buffer, wxUint32 length);
    bool LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length);
    //wxInt32 ParseKeyPair(wxUint8* buffer, wxUint32& currentIndex, wxUint32 length;
    //bool ParseEntities(wxUint8* buffer[], wxUint32 length);
    void DebugDump(wxTextOutputStream& out);

    wxUint32 EntityCount;
    //QuME_BSP_EntList* EntList;
    QuME_Entity* EntityArray;
    QuME_LinkedList<QuME_Entity> EntityList;
};


#endif // QuME_BSP_ENTITIES_H_INCLUDED
