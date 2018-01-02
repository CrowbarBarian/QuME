/***************************************************************
 * Name:      QuME_App.cpp
 * Purpose:   Code for Application Class
 * Author:    J Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:   2017-06-13
 * Copyright: J Thomas (Crowbarbarian) ()
 * License:   GPL v3
 **************************************************************/

#include "QuME_App.h"

#include "QuME_Main.h"
#include <wx/image.h>

wxIMPLEMENT_APP(QuME_App);

QuME_App::QuME_App()
{
	this->bsp = nullptr;
    this->ShuttingDown = false;
}

QuME_App::~QuME_App()
{
	SAFE_DELETE(this->bsp);
	this->bsp = nullptr;
	this->ShuttingDown = true;
}

bool QuME_App::OnInit()
{
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	QuME_Frame* Frame = new QuME_Frame(0, -1);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    return wxsOK;
}
