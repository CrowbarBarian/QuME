/***************************************************************
 * Name:		QuME_PAK_File.h
 * Purpose:		Defines for PAK file handler
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#ifndef QUME_PAK_FILE_H
#define QUME_PAK_FILE_H

#include "QuME_Common.h"
#include "QuME_Lists.h"

#define QUME_PAK_DIR_ENTRY_LENGTH 64
#define QUME_PAK_FILE_NAME_LENGTH 56
#define MAX_PAKS 100

class QuME_PAK_DirEntry
{
public:
	QuME_PAK_DirEntry();
	QuME_PAK_DirEntry(std::wstring& fname, wxUint32 foff, wxUint32 fsize);
	QuME_PAK_DirEntry(QuME_PAK_DirEntry& o);
	~QuME_PAK_DirEntry();

	QuME_PAK_DirEntry& operator=(QuME_PAK_DirEntry& o);

	bool operator==(QuME_PAK_DirEntry& o);

	std::wstring fileName;
	wxUint32 fileOffset;
	wxUint32 fileSize;
};

class QuME_PAK_File
{
public:
    QuME_PAK_File();
    explicit QuME_PAK_File(std::wstring fname);
    QuME_PAK_File(QuME_PAK_File& o);
    ~QuME_PAK_File();

    void LoadPAKFileInfo();

    QuME_PAK_DirEntry* GetFileInfo(std::wstring fName);
    size_t LoadFile(std::wstring fName, wxUint8** buffer);
    bool operator==(QuME_PAK_File& o);

    std::wstring pakFileName;
    wxUint32 pakID;
    wxUint32 dirOffset;
    wxUint32 dirLength;

    QuME_Array<QuME_PAK_DirEntry> dirEntries;
    QuME_LinkedList<QuME_PAK_DirEntry> dirEntryList;
};

#endif // QUME_PAK_FILE_H
