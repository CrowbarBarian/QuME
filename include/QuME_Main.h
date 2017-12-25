/***************************************************************
 * Name:      QuME_Main.h
 * Purpose:   Defines Application Frame
 * Author:    J Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:   2017-06-13
 * Copyright: J Thomas (Crowbarbarian) ()
 * License:   GPL v3
 **************************************************************/

#ifndef QUME_MAIN_H
#define QUME_MAIN_H

#define QUME_VERSION L"0.2.2"

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
#include <wx/dir.h>

#include "QuME_App.h"
wxDECLARE_APP(QuME_App);

#include "QuME_BSP_ObjExporter.h"
#include "QuME_BSP_Brush_Exporter.h"
#include "QuME_BSP_Load.h"
#include "QuME_BSP_Data.h"
#include "QuME_Lists.h"
#include "QuME_PAK_File.h"

class QuME_Frame : public wxFrame
{
    public:

        QuME_Frame(wxWindow* parent,wxWindowID id = -1);
        virtual ~QuME_Frame();
        bool ObjExportCanceled();
        bool BSPImportCanceled();

        //our event handlers
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);

        void OnSpinFaceExamineChange(wxSpinEvent& event);
        void OnSpinEntityExamineChange(wxSpinEvent& event);
        void OnSpinTextureExamineChange(wxSpinEvent& event);

        void UpdateGLWindow(wxTimerEvent& event);

        void OnOpenBSP(wxCommandEvent& event);
        void OnSetBasedir(wxCommandEvent& event);

        void OnWavefrontExport(wxCommandEvent& event);
        void OnUpdateObjExport(wxUpdateUIEvent& event);
        void OnObjExportEvent(wxThreadEvent& event);

        void OnBrushExport(wxCommandEvent& event);
        void OnUpdateBrushExport(wxUpdateUIEvent& event);
        void OnBrushExportEvent(wxThreadEvent& event);

        void OnUpdateBSPProcessing(wxUpdateUIEvent& event);
        void OnBSPImportEvent(wxThreadEvent& event);
        void OnBSPProcessingEvent(wxThreadEvent& event);

        void OnConsoleLogEvent(wxThreadEvent& event);

        //the ids of all of our GUI elements

        //the main console window
        static const long ID_INFOCONSOLE;

        //labels for stuff
        static const long ID_STATICTEXT_FACEINSPECTOR;
        static const long ID_STATICTEXT_ENTITYINSPECTOR;
        static const long ID_STATICTEXT_TEXTUREINSPECTOR;
        static const long ID_STATICTEXT_INFOCONSOLE;
        static const long ID_STATICTEXT_PREVIEWWINDOW;
        static const long ID_STATICTEXT_XROTATION;
        static const long ID_STATICTEXT_YROTATION;
        static const long ID_STATICTEXT_ZROTATION;

        //data examiners
        static const long ID_SPIN_FACE_EXAMINE;
        static const long ID_SPIN_ENTITY_EXAMINE;
        static const long ID_SPIN_TEXTURE_EXAMINE;

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

        static const long ID_STATUSBAR;

        static const long ID_REFRESHTIMER;
        static const long ID_MAINGLCANVAS; //the id of the GLCanvas object

        //pointer to our console object
        wxTextCtrl* infoConsole;

        wxGLCanvas* mainGLCanvas; //pointer to our OpenGL drawing surface
        wxGLContext* glContext; //context pointer

        //our label pointers
        wxStaticText* staticTextFaceInspector;
        wxStaticText* staticTextEntityInspector;
        wxStaticText* staticTextTextureInspector;
        wxStaticText* staticTextInfoConsole;
        wxStaticText* staticTextPreviewWindow;
        wxStaticText* staticTextXRotation;
        wxStaticText* staticTextYRotation;
        wxStaticText* StaticTextZRotation;

        //our data examiner pointers
        wxSpinCtrl* spinFaceExamine;
        wxSpinCtrl* spinTextureExamine;
        wxSpinCtrl* spinEntityToExamine;

        //pointers to our slider controls
        wxSlider* SliderXRotation;
        wxSlider* SliderYRotation;
        wxSlider* SliderZRotation;

        //menu item pointers
        wxMenu* menuOptions;
        wxMenuItem* menuOpen;
        wxMenuItem* menuExport;
        wxMenuItem* menuBaseDirectory;
        wxMenuItem* menuBrushes;

        //status bar pointer
        wxStatusBar* statusBar;

        //timer
        wxTimer refreshTimer;

        //rotation angles for drawing on Canvas1
        wxFloat64 xRot;
        wxFloat64 yRot;
        wxFloat64 zRot;

        QuME_BSP_Data* bsp; //our main database
        wxCriticalSection CritSecBSP; //multi-thread protection for the BSP database

        QuME_LinkedList<QuME_PAK_File> pakFiles;

        wxProgressDialog* BSPProcessingProgressDialog; //our file loading progress dialog
        bool importCanceled;
        wxCriticalSection CritSecImportCanceled;

        wxProgressDialog *ExportProgressDialog; //our file export progress dialog
        bool exportCanceled;
        wxCriticalSection CritSecExportCanceled;

        wxFileConfig* configuration; //our persistent configuration settings, stored in QuME.cfg in the app directory
        std::wstring gameDir; //the one setting we need saved

        DECLARE_EVENT_TABLE()
};

enum
{
    OBJEXPORT_EVENT=wxID_HIGHEST+1,
    BRUSHEXPORT_EVENT,
    BSPIMPORT_EVENT,
    BSPPROCESSING_EVENT,
    CONSOLELOG_EVENT
};


#endif // QuME_BSP_TESTMAIN_H
