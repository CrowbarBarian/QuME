#ifndef QUME_BSP_POSTLOADPROCESSOR_H
#define QUME_BSP_POSTLOADPROCESSOR_H

#include "QuME_App.h"
wxDECLARE_APP(QuME_App);

#include "QuME_BSP_Data.h"
#include "QuME_Main.h"
#include <wx/thread.h>

class QuME_Frame;

class QuME_BSP_PostLoadProcessor : public wxThread
{
public:
    QuME_BSP_PostLoadProcessor(QuME_Frame* frame, QuME_BSP_Data* BSPData);
    ~QuME_BSP_PostLoadProcessor();

    virtual void* Entry() wxOVERRIDE;
    virtual void OnExit() wxOVERRIDE;

    QuME_Frame* Frame;
    QuME_BSP_Data* Data;
};

#endif // QUME_BSP_POSTLOADPROCESSOR_H
