#ifndef QUME_BSP_BRUSH_EXPORTER_H
#define QUME_BSP_BRUSH_EXPORTER_H

#include "QuME_Main.h"

#include "QuME_Common.h"
#include "QuME_UniqueStrings.h"

#include <wx/thread.h>

#include "QuME_BSP_Data.h" //includes all the header files we'll need to export the data

class QuME_Frame; //forward declare this so compiler stops complaining

class QuME_BSP_Brush_Exporter : public wxThread
{
public:
    QuME_BSP_Brush_Exporter(QuME_Frame* frame,
							QuME_BSP_Data* BSPData,
							const std::wstring& BaseFileName,
							const std::wstring& FullPath);
    ~QuME_BSP_Brush_Exporter();

    // thread execution starts here
    virtual void* Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

    QuME_Frame* Frame;
    std::wstring StrippedFileName;
    std::wstring FullPathNoExt;
    bool FileError;
    QuME_BSP_Data* Data;
};

#endif // QUME_BSP_BRUSH_EXPORTER_H
