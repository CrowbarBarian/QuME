/***************************************************************
 * Name:      QuME_Main.h
 * Purpose:   Defines Application Frame
 * Author:    Joseph Thomas (Crowbarbarian) (crowbarbarian@outlook.com)
 * Created:   2017-06-13
 * Copyright: Joseph Thomas (Crowbarbarian) ()
 * License:
 **************************************************************/

#ifndef QUME_MAIN_H
#define QUME_MAIN_H

#include "QuME_Common.h"
#include <wx/spinctrl.h>
#include <wx/menu.h>
#include <wx/slider.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/fileconf.h> //used for ini file functionality
#include <wx/thread.h>
#include <wx/glcanvas.h>
#include <wx/progdlg.h>
#include <wx/filename.h> //for wxFileName::GetCwd()

#include "QuME_App.h"
wxDECLARE_APP(QuME_App);

#include "QuME_BSP_ObjExporter.h"
#include "QuME_BSP_Load.h"
#include "QuME_BSP_Data.h"

class QuME_Frame : public wxFrame
{
    public:

        QuME_Frame(wxWindow* parent,wxWindowID id = -1);
        virtual ~QuME_Frame();
        bool ObjExportCanceled();
        bool BSPImportCanceled();

        wxCriticalSection* GetBSPProcessingCritSec() { return &CritSecBSP; }
        wxCriticalSection* GetImportCritSec() { return &CritSecImportCanceled; }
        wxCriticalSection* GetExportCritSec() { return &CritSecExportCanceled; }


        //our event handlers
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnSpinCtrl1Change(wxSpinEvent& event);
        void OnSpinCtrl2Change(wxSpinEvent& event);
        void OnSpinCtrl3Change(wxSpinEvent& event);

        void UpdateGLWindow(wxTimerEvent& event);

        void OnOpenBSP(wxCommandEvent& event);
        void OnSetBasedir(wxCommandEvent& event);

        void OnWavefrontExport(wxCommandEvent& event);
        void OnUpdateObjExport(wxUpdateUIEvent& event);
        void OnObjExportEvent(wxThreadEvent& event);

        void OnUpdateBSPProcessing(wxUpdateUIEvent& event);
        void OnBSPImportEvent(wxThreadEvent& event);
        void OnBSPProcessingEvent(wxThreadEvent& event);

        void OnConsoleLogEvent(wxThreadEvent& event);

        //the ids of all of our GUI elements

        //the main console window
        static const long ID_TEXTCTRL1;

        //labels for stuff
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_STATICTEXT8;

        //data examiners
        static const long ID_SPIN_FACE_EXAMINE;
        static const long ID_SPINENTITYEXAMINE;
        static const long ID_SPINTEXTUREEXAMINE;

        //OpenGL window rotation
        static const long ID_SLIDER_XROTATION;
        static const long ID_SLIDER_YROTATION;
        static const long ID_SLIDER_ZROTATION;

        //menu items
        static const long ID_MENU_OPEN;
        static const long ID_MENU_EXPORT;
        static const long ID_MENU_BRUSHES;
        static const long ID_MENU_QUIT;
        static const long ID_MENU_BASEDIR;
        static const long ID_MENU_ABOUT;

        static const long ID_STATUSBAR1;

        static const long ID_TIMER1;
        static const long ID_GLCANVAS1; //the id of the GLCanvas object

        //pointer to our console object
        wxTextCtrl* TextCtrl1;

        wxGLCanvas* GLCanvas1; //pointer to our OpenGL drawing surface
        wxGLContext* Context1; //context pointer

        //our label pointers
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;

        //our data examiner pointers
        wxSpinCtrl* SpinCtrl1;
        wxSpinCtrl* SpinCtrl3;
        wxSpinCtrl* SpinCtrl2;

        //pointers to our slider controls
        wxSlider* Slider1;
        wxSlider* Slider2;
        wxSlider* Slider3;

        //menu item pointers
        wxMenu* MenuOptions;
        wxMenuItem* MenuOpen;
        wxMenuItem* MenuExport;
        wxMenuItem* MenuBasedir;
        wxMenuItem* MenuBrushes;

        //status bar pointer
        wxStatusBar* StatusBar1;

        //timer
        wxTimer Timer1;

        //rotation angles for drawing on Canvas1
        wxFloat64 xRot;
        wxFloat64 yRot;
        wxFloat64 zRot;

        QuME_BSP_Data* BSP; //our main database
        wxCriticalSection CritSecBSP; //multi-thread protection for the BSP database

        wxProgressDialog* BSPProcessingProgressDialog; //our file loading progress dialog
        bool ImportCanceled;
        wxCriticalSection CritSecImportCanceled;

        wxProgressDialog *ExportProgressDialog; //our file export progress dialog
        bool ExportCanceled;
        wxCriticalSection CritSecExportCanceled;

        wxFileConfig* Configuration; //our persistent configuration settings, stored in QuME.cfg in the app directory
        wxString GameDir; //the one setting we need saved

        DECLARE_EVENT_TABLE()
};

enum
{
    OBJEXPORT_EVENT=wxID_HIGHEST+1,
    BSPIMPORT_EVENT,
    BSPPROCESSING_EVENT,
    CONSOLELOG_EVENT
};


#endif // QuME_BSP_TESTMAIN_H
