/***************************************************************
 * Name:      QuME_App.h
 * Purpose:   Defines Application Class
 * Author:    J Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:   2017-06-13
 * Copyright: J Thomas (Crowbarbarian) ()
 * License:   GPL v3
 **************************************************************/

#ifndef QUME_APP_H
#define QUME_APP_H

#include <wx/app.h>
#include "QuME_Common.h"
#include "QuME_BSP_Data.h"

WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

class QuME_App : public wxApp
{
public:
    QuME_App();
    virtual ~QuME_App();

    virtual bool OnInit();

    wxCriticalSection CritSectThreads;
    wxArrayThread Threads;
    wxSemaphore AllDone;

    // indicates that we're shutting down and all threads should exit
    bool ShuttingDown;

    QuME_BSP_Data* bsp; //our main database
    wxCriticalSection CritSecBSP; //multi-thread protection for the BSP database
};

#endif // QuME_BSP_TESTAPP_H
