#include "QuME_PAK_File.h"

QuME_PAK_DirEntry::QuME_PAK_DirEntry()
{
	//this->fileName = nullptr;
	//this->fileName = new std::wstring;
	this->fileOffset = 0;
	this->fileSize = 0;
}

QuME_PAK_DirEntry::QuME_PAK_DirEntry(const std::wstring fname, const wxUint32 foff, const wxUint32 fsize)
{
	this->fileName = fname;
	this->fileOffset = foff;
	this->fileSize = fsize;
}

QuME_PAK_DirEntry::QuME_PAK_DirEntry(const QuME_PAK_DirEntry& o)
{
	this->fileName = o.fileName;
	this->fileOffset = o.fileOffset;
	this->fileSize = o.fileSize;
}

QuME_PAK_DirEntry::~QuME_PAK_DirEntry()
{
	//SAFE_DELETE(this->fileName);
}

QuME_PAK_DirEntry& QuME_PAK_DirEntry::operator=(const QuME_PAK_DirEntry& o)
{
	this->fileName = o.fileName;
	this->fileOffset = o.fileOffset;
	this->fileSize = o.fileSize;
	return *this;
}

bool QuME_PAK_DirEntry::operator==(const QuME_PAK_DirEntry& o)
{
	return (this->fileName == o.fileName);
}

QuME_PAK_File::QuME_PAK_File()
{
	this->dirLength = 0;
	this->dirOffset = 0;
	this->dirEntryArray = nullptr;
	this->dirEntryArrayCount = 0;
	this->pakID = ('P' | ('A' << 8) | ('C' << 16) | ('K' << 24)); //magic number
}

QuME_PAK_File::QuME_PAK_File(const QuME_PAK_File& o)
{
	this->dirLength = o.dirLength;
	this->dirOffset = o.dirOffset;
	this->dirEntryArray = nullptr;
	this->dirEntryArrayCount = 0;
	this->dirEntries.Append(o.dirEntries); //make deep copy
	this->pakFileName = o.pakFileName;
	this->pakID = o.pakID;
}

QuME_PAK_File::QuME_PAK_File(std::wstring fname)
{
	this->pakFileName = fname;
	this->pakID = ('P' | ('A' << 8) | ('C' << 16) | ('K' << 24)); //magic number
	this->dirEntryArray = nullptr;
	this->dirEntryArrayCount = 0;
	this->LoadPAKFileInfo();
}

QuME_PAK_File::~QuME_PAK_File()
{
	this->dirLength = 0;
	this->dirOffset = 0;
	//std::cout << "***********************************************\nDeleting " << this->pakFileName << "\n";
	//std::cout.flush();
	//SAFE_ARRAY_DELETE(this->dirEntryArray);
	this->dirEntryArrayCount = 0;
	//pakFileName = L"";
}

void QuME_PAK_File::LoadPAKFileInfo()
{
	wxFileInputStream input_stream(this->pakFileName);

    if(!input_stream.IsOk())
	{
		throw std::runtime_error("Cannot open .pak file!");
		return;
	}

	wxDataInputStream binData(input_stream);

	if(input_stream.SeekI(0, wxFromStart) == wxInvalidOffset)
	{
		throw std::runtime_error("Cannot seek to start of file!");
		return;
	}

	wxUint32 magic = binData.Read32();

	if(magic != this->pakID)
	{
		throw std::runtime_error("Not a valid .pak file!");
		return;
	}

	this->dirOffset = binData.Read32();
	if(!binData.IsOk())
    {
    	throw std::runtime_error("Cannot read .pak file header!");
        return;
    }

    this->dirLength = binData.Read32();
   	if(!binData.IsOk())
    {
    	throw std::runtime_error("Cannot read .pak file header!");
        return;
    }

	if(input_stream.SeekI(this->dirOffset, wxFromStart) == wxInvalidOffset)
	{
		throw std::runtime_error("Cannot seek to start of .pak file directory!");
		return;
	}

    for(wxUint32 entryCount = 0; entryCount < this->dirLength / QUME_PAK_DIR_ENTRY_LENGTH; entryCount++)
	{
		wchar_t nameBuf[56];
		for(wxUint32 nIndex = 0; nIndex < QUME_PAK_FILE_NAME_LENGTH; nIndex++)
		{
			nameBuf[nIndex] = binData.Read8();
			if(!binData.IsOk())
			{
		    	throw std::runtime_error("Cannot read filename in entry #" + std::to_string(entryCount));
		    	return;
			}
		}
		std::wstring fname = nameBuf;
		wxUint32 foff = binData.Read32();
		if(!binData.IsOk())
		{
			throw std::runtime_error("Cannot read file offset in entry #" + std::to_string(entryCount));
			return;
		}
		wxUint32 flen = binData.Read32();
		if(!binData.IsOk())
		{
			throw std::runtime_error("Cannot read file length in entry #" + std::to_string(entryCount));
			return;
		}
		QuME_PAK_DirEntry dEntry;
		dEntry.fileName = fname;
		//std::cout << "FileName: " << fname << "\n";
		dEntry.fileOffset = foff;
		dEntry.fileSize = flen;
		this->dirEntries.Append(dEntry);
	}

	this->dirEntryArrayCount = this->dirEntries.ToArray(this->dirEntryArray);
	//std::cout << this->pakFileName << "\n";

	//for(wxUint64 i = 0; i < this->dirEntryArrayCount; i++)
	//{
	//	std::cout << this->dirEntryArray[i].fileName << "\n";
	//	std::cout.flush();
	//}
}

QuME_PAK_DirEntry* QuME_PAK_File::GetFileInfo(std::wstring fName)
{
	try
	{
		if(this->dirEntryArrayCount == 0)
		{
			this->LoadPAKFileInfo();
		}
		if(this->dirEntryArrayCount == 0) //bad pak file, abort!
		{
			return nullptr;
		}
	}
	catch (const std::runtime_error& e)
	{
		std::string error = "File error! Error was \"";
		error += e.what();
		error += "\"";
		throw std::runtime_error(error);
	}

	wxUint32 fileIndex = 0;
	while(fileIndex < this->dirEntryArrayCount)
	{
		if(fName == this->dirEntryArray[fileIndex].fileName)
			return &this->dirEntryArray[fileIndex];
	}
	return nullptr;
}

size_t QuME_PAK_File::LoadFile(std::wstring fName, wxUint8** buffer)
{
    QuME_PAK_DirEntry* dirEntry = this->GetFileInfo(fName);

    if(dirEntry != nullptr)
	{
		wxFileInputStream input_stream(this->pakFileName);

		if(!input_stream.IsOk())
		{
			throw std::runtime_error("Cannot open .pak file!");
			return 0;
		}

		wxDataInputStream binData(input_stream);

		if(input_stream.SeekI(dirEntry->fileOffset, wxFromStart) == wxInvalidOffset)
		{
			throw std::runtime_error("Cannot seek to start of file in .pak file! File offset " + std::to_string(dirEntry->fileOffset));
			return 0;
		}

		*buffer = new wxUint8[dirEntry->fileSize];
		binData.Read8(*buffer, dirEntry->fileSize);
		return dirEntry->fileSize;
	}
	else
	{
		*buffer = nullptr;
		return 0;
	}
}

bool QuME_PAK_File::operator==(const QuME_PAK_File& o)
{
	return (this->pakFileName == o.pakFileName);
}
