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

WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

class QuME_App : public wxApp
{
public:
    QuME_App();

    virtual bool OnInit();

    wxCriticalSection CritSectThreads;
    wxArrayThread Threads;
    wxSemaphore AllDone;

    // indicates that we're shutting down and all threads should exit
    bool ShuttingDown;

};

#endif // QuME_BSP_TESTAPP_H
