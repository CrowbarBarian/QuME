#ifndef QUME_BSPLOAD_H
#define QUME_BSPLOAD_H

#include "QuME_App.h"
wxDECLARE_APP(QuME_App);

#include "QuME_Common.h"
#include "QuME_Main.h"
#include "QuME_BSP_Data.h" //includes all the header files we'll need to import the data

class QuME_Frame; //forward declaration to shut up compiler complaining

class QuME_BSP_Load : public wxThread
{
public:
    QuME_BSP_Load(QuME_Frame* frame, QuME_BSP_Data* BSPData, const std::wstring& FileName);
    ~QuME_BSP_Load();
    virtual void* Entry();
    virtual void OnExit();

    QuME_Frame* Frame;
    std::wstring BSPFileName; //The actual file we'll be opening
    bool FileError; //set if a file read error happens
    QuME_BSP_Data* Data; //the BSP class instance we'll be loading data into
};

#endif // QuME_BSP_LOAD_H
