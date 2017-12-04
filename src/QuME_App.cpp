/***************************************************************
 * Name:      QuME_App.cpp
 * Purpose:   Code for Application Class
 * Author:    Joseph Thomas (Crowbarbarian) (crowbarbarian@outlook.com)
 * Created:   2017-06-13
 * Copyright: Joseph Thomas (Crowbarbarian) ()
 * License:
 **************************************************************/

#include "QuME_App.h"

#include "QuME_Main.h"
#include <wx/image.h>

wxIMPLEMENT_APP(QuME_App);

QuME_App::QuME_App()
{
    ShuttingDown = false;
}

bool QuME_App::OnInit()
{
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	QuME_Frame* Frame = new QuME_Frame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    return wxsOK;

}
