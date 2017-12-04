#ifndef QUME_BSPOBJEXPORTER_H_INCLUDED
#define QUME_BSPOBJEXPORTER_H_INCLUDED

#include "QuME_Main.h"

#include "QuME_Common.h"
#include "QuME_UniqueStrings.h"

#include <wx/thread.h>

#include "QuME_BSP_Data.h" //includes all the header files we'll need to export the data

class QuME_Frame; //forward declare this so compiler stops complaining

class QuME_BSP_ObjExporter : public wxThread
{
public:
    QuME_BSP_ObjExporter(QuME_Frame* frame,
                         QuME_BSP_Data* BSPData,
                         const wxString& BaseFileName,
                         const wxString& FullPath);

    // thread execution starts here
    virtual void* Entry() wxOVERRIDE;

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit() wxOVERRIDE;

    QuME_Frame* Frame;
    wxString StrippedFileName;
    wxString FullPathNoExt;
    bool FileError;
    QuME_BSP_Data* Data;
};

#endif // QuME_BSP_WORKERTHREAD_H_INCLUDED
