/***************************************************************
 * Name:		QuME_Main.h
 * Purpose:		Defines Application Frame
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:		2017-06-13
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#ifndef QUME_MAIN_H
#define QUME_MAIN_H

#define QUME_VERSION L"0.2.3"

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
    static const long ID_QUME;

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

    struct
    {
        unsigned char	 pixel_data[48 * 48 * 3 + 1];
    } icon_image =
    {
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\1\1\1\2\2\2\2\2\3\2\2\2\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\3\3\3\1\1\1\3\3\3\1\1\1\2\2\2\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\1\0\0\1\2\2\2\5\4\5\23\23\22\13\13\12\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2\2\1\11\14\11\13\14\12\3\3\3"
        "\1\0\1\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\3\3\3"
        "\1\1\1\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\1\1\1\33\37\32V`SOWM\17\21\17\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\6\7\6.4*JVB\40%\36\5\4\3"
        "\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\1\10\11\7/5)EM>\26\30\24\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\14\21\14<K60<+\7\7\6"
        "\0\0\1\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\11\12"
        "\11EQ@AK;\7\7\7\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\5\6\5)0&,3'\11\11\10\1"
        "\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\11\12\10""6A2;H:\14"
        "\15\14\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\34!\31%,!\11"
        "\12\11\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\3\2\2.5)>H8\10\12\10\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\4\5\4\"*\35!(\35"
        "\5\6\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\34!\32>L8\31\36\26\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\14\16\12.:("
        "\34$\31\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\15\17\13@N83@/\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0$,!<K5\20\25"
        "\17\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\1\0\0$-\40QgI\34\"\32\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\10\10\11J`A/<+\2\2"
        "\3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\13\14\12CT=D[>\5\4\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0CZ<UqO\23\30\23\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0"
        "\0\0\0\33\40\31d|]9M5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0""4B0h\215^(2$\2\1\3\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\4\4\4""2"
        "</p\216h=K:\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\1""491k\223^5F.\6\6\7\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\16\17\16RbLj\213a"
        "0;-\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\1\1\1\0\0\0#&!c\210XPoH\22\25\21\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\21\23\20]uUo\226g.<,\0\0\0"
        "\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\1\1\1\0\0\0\31\32\30f\206Ze\214Y\26\33\24\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2\2\2\6\5\5\27\34\26q\223lu\236m9K6"
        "\0\0\0\5\5\5\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\1\1\1\0\0\0(+'j\222`m\230b\31\37\27\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\2\0\1\25\33\25k\207dj\206b08/"
        "\0\0\1\4\3\5\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\1\0\0\0\0:C8a\203Tc\213X\27\36\26\2\0\1\1\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\1\3\1\3\22\25\20YsQt\230kK^G\0\0"
        "\0\6\4\5\3\3\3\3\3\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\1\1"
        "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\1\1\1\0\0\0GY@u\245io\235h\33!\31\2\0\2\1\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2\1\2\25\26\23SfNr\240l[~V\20\20\17"
        "\6\4\5\7\6\7\4\4\4\4\4\3\6\5\6\10\7\7\2\2\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\5\4\4NgE{\254o\\sU\26\26\24\2\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\2\1\2\22\22\21<G8a\210Zn\235h8H5\0\0\0"
        "\5\4\4\3\2\3\6\6\6\13\10\12\6\4\5\5\2\3\4\3\4\1\1\1\5\4\4\1\0\1\1\0\0\1\1"
        "\1\0\0\0\0\0\0\1\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\0\0"
        "\0\0\0\0&($azVy\251mRjM\24\24\22\2\1\2\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\1\1\1\3\3\3\11\11\11""08,Y|P|\266t_\202\\\11\3\7\4\2"
        "\4\2\1\2\2\1\2\11\6\10\4\3\4\7\5\6\4\1\4\1\0\2\16\12\14\20\22\20\10\7\7\0"
        "\0\0\0\0\0\0\0\0\1\0\1\0\0\0\1\3\2\6\7\6\3\4\3\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0?P<}\253q\217\306\203BT<\14\14\15\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2\2\2\4\4\5\32\40\30ZsQm\223bz\241n"
        "3A.\0\0\0\3\3\3\1\1\1\6\5\5\11\7\10\3\0\2MSKdvae\217]k\222c]\177We\203We"
        "|_UqMQkHSkI]fT5<2\2\2\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0*-)_uW}\260t\202\264"
        "x(1%\2\0\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\1\1\1\5\5\6/7,f\200_e\214]RnK\36\36\34\11\4\7\7\7\6\5\3\4\11\7"
        "\7\14\12\13\24\20\22$(!i\222cw\263rh\233[p\207iQiL}\261oe\205Vk\226c%'$\5"
        "\5\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\35\"\34_}Y\177\261y\223\313\213c\177Z"
        "\32\34\31\4\3\4\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\37&\35b}V^\201Ng\216[PhJ\21\24\21\0\0\1\7\4\5"
        "\14\12\13\13\11\11\20\17\17""7>5UlOd\223Vk\243\\AT<)-%OjDe\204Y_\202T\15"
        "\17\14\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\30\30\25auY\203\260{\200\263t\213\272"
        "\2013?0\10\7\10\6\6\6\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\7\10\6""6@/d\204Ym\225ah\220]\\vU%(#\10"
        "\7\7\15\12\12\5\3\4\13\11\12\20\16\17VlK\\\201Os\247c9E602,d\202Yb\200Sh"
        "\216]\12\10\11\1\0\0\1\1\1\0\0\0\0\0\0\27\30\24WgNy\244p\215\301\210\247"
        "\324\240p\203j\32\32\32\2\2\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\15\16\15""6@0YxMb\207"
        "Yi\216ah\205`(-%\5\3\4\10\7\7\12\10\12\2\0\1CT9c\202Vi\234XCR@((%c\200Wg"
        "\207\\PhE\23\21\20\11\11\11\5\5\5\15\16\14;?7\230\257\222\250\333\240x\267"
        "q\210\304\204\205\253\20216-\10\5\5\4\3\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\26"
        "\27\25K[Ej\215ap\213dj\212]_xT6=0\17\22\17\14\11\14\5\0\5\\mTXrJc\221V4C"
        "036.i\216\\f\210[\\yT\36\36\32\32\34\26).%`r]\241\323\234\237\335\236\214"
        "\276\205\205\263{z\237t<C9\10\5\6\5\4\5\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1"
        "\1\0\0\1%'&UaRbsXYhMczRy\233i\200\256vQeI\26\25\22\\nQg\206Xr\236c6A/<D8"
        "x\251j\200\251ni\216`=F7k|d\222\277\210\205\273x\220\325\207\202\300yz\246"
        "r~\223yFPE\2\0\0\4\3\3\3\2\3\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\4\3\3\1"
        "\0\0\16\15\16@H:cyV_xQi\217[p\233`c\204Rr\217i|\236lr\241dn\234^l\217\\c"
        "\200Zv\241gx\250e\200\267s\205\267zw\256p\214\320\204x\265n~\300v\200\277"
        "yl\221e&)#\10\7\10\7\6\7\2\2\2\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\1\1\1\0\0\0\20\17\20RdKf\204Zo\220dq\223eu\237g~\270mj\227^g\212["
        "g\214Vp\221]dxYbzSg\205Vw\232kd\207Xs\232j\205\300~\206\277\177\201\265{"
        "s\227o6=/\12\7\6\6\5\6\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\1\1\1\1\1\1\2\2\2\2\1\3\0\0\0)3&[oTTfKz\255r\233\337\221x\237nYlOe~"
        "T^xR[fPYnO`wRc\205X\216\304\205p\233j|\251ss\220l@F:\25\24\22\11\11\11\7"
        "\6\6\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\2\2\3\3"
        "\3\3\2\2\2\1\1\1\5\4\4\13\11\12\3\0\2\3\0\2I]Dz\236tp\215cS^I\\nMVfM[fRg"
        "{^buXn\226h{\250tYqTCL=\14\12\11\2\0\2\4\4\4\6\6\6\2\2\2\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2\2\3\5\4\5\3\2\2\2\1\1\4\3"
        "\4\3\3\3\6\6\5\12\10\10\22\16\17\27\25\24e\177\\f\177Z[xPTeOGTBn\220`[oP"
        "d\200]\25\21\23\0\0\0\0\0\0\4\3\3\2\3\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\3\3\3\4\3\3\1\1\1\6\6"
        "\6\6\5\6\2\1\2\15\13\13\33\31\30\26\25\23_|Xf\205Wc\203V8C5&*#m\225`_xSg"
        "\211^\10\4\6\3\3\3\4\4\4\3\3\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\2\1\0\0\0\5\5\5"
        "\4\4\4\5\5\5\7\5\7\17\14\17\4\0\3_xW^{Qa\200U/8.\31\25\27p\224fVjK`~Y\6\3"
        "\6\0\0\0\1\1\1\3\3\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\2\2\1\7\7"
        "\10\11\10\11\14\11\14\3\0\4`uYYqN_~R*.(%%#v\230j[nQa}X\0\0\0\0\0\0\0\0\0"
        "\0\0\0\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\1\1\1\5\5\5\6\6\6\7\6\7"
        "\0\0\0av]q\222kk\220]?H=593j\207`k\212bZsT\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1"
        "\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\3\3\2\3\3\3\0\0\0WkUn\225"
        "hk\212^18/\37\37\36y\226n`\202STkQ\0\0\0\3\3\3\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\2\2\2\2\2\3\0\0\0DPBJcCj\207`\24"
        "\25\23\15\14\15u\225lWwKI\\E\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\2\2\3\2\0\1""7<6i\226`p\213l\7\6"
        "\6\11\11\12f\204bc\216\\AM@\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\1\1\1\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\7\6\6$#\"d\211\\k\205j\7\5\6"
        "\12\10\11SfP]\200V%(%\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\5\4\4\15\10\13a\177YXpV\12\11\12\13"
        "\12\12AN;e\203_\14\13\13\4\4\4\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\5\5\5\5\1\4z\241v?N>\5\4\5\12"
        "\6\10;C8f\203a\6\4\6\6\7\7\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\3\3\3\1\1\1\0\0\0\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\2\2\2\0\0\0\7\6\7\3\1\3r\222p'/&\16\14\14\5\2"
        "\4(.'r\215k\2\0\2\4\4\4\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1"
        "\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\3\0\3]s]#%!\6\5\7\10\6\6\21\24\20"
        "^s\\\3\2\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1"
        "\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\1\0\1\1\1\0\0\0<D;\25\25\25\14\15\14\25\22\22\10"
        "\10\10/4-\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\4\4\4\2\2\2\7\7\7\7\7\7\3\3"
        "\3\2\2\2\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\1\1\1\1\1\1\0\0\1\1"
        "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    };
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
