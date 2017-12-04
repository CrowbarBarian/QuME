/***************************************************************
 * Name:      QuME_Main.cpp
 * Purpose:   Code for Application Frame
 * Author:    Joseph Thomas (Crowbarbarian) (crowbarbarian@outlook.com)
 * Created:   2017-06-13
 * Copyright: Joseph Thomas (Crowbarbarian) ()
 * License:
 **************************************************************/

#include "QuME_Main.h"
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/intl.h>
#include <wx/font.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>

const long QuME_Frame::ID_TEXTCTRL1 = wxNewId();

const long QuME_Frame::ID_SPIN_FACE_EXAMINE = wxNewId();
const long QuME_Frame::ID_SPINENTITYEXAMINE = wxNewId();
const long QuME_Frame::ID_SPINTEXTUREEXAMINE = wxNewId();

const long QuME_Frame::ID_STATICTEXT1 = wxNewId();
const long QuME_Frame::ID_STATICTEXT2 = wxNewId();
const long QuME_Frame::ID_STATICTEXT3 = wxNewId();
const long QuME_Frame::ID_STATICTEXT4 = wxNewId();
const long QuME_Frame::ID_STATICTEXT5 = wxNewId();
const long QuME_Frame::ID_STATICTEXT6 = wxNewId();
const long QuME_Frame::ID_STATICTEXT7 = wxNewId();
const long QuME_Frame::ID_STATICTEXT8 = wxNewId();

const long QuME_Frame::ID_SLIDER_XROTATION = wxNewId();
const long QuME_Frame::ID_SLIDER_YROTATION = wxNewId();
const long QuME_Frame::ID_SLIDER_ZROTATION = wxNewId();

const long QuME_Frame::ID_MENU_OPEN = wxNewId();
const long QuME_Frame::ID_MENU_EXPORT = wxNewId();
const long QuME_Frame::ID_MENU_BRUSHES = wxNewId();
const long QuME_Frame::ID_MENU_QUIT = wxNewId();
const long QuME_Frame::ID_MENU_BASEDIR = wxNewId();
const long QuME_Frame::ID_MENU_ABOUT = wxNewId();

const long QuME_Frame::ID_STATUSBAR1 = wxNewId();
const long QuME_Frame::ID_TIMER1 = wxNewId();
const long QuME_Frame::ID_GLCANVAS1 = wxNewId();


BEGIN_EVENT_TABLE(QuME_Frame,wxFrame)
    EVT_THREAD(OBJEXPORT_EVENT, QuME_Frame::OnObjExportEvent)
    EVT_THREAD(BSPIMPORT_EVENT, QuME_Frame::OnBSPImportEvent)
    EVT_THREAD(BSPPROCESSING_EVENT, QuME_Frame::OnBSPProcessingEvent)
    EVT_THREAD(CONSOLELOG_EVENT, QuME_Frame::OnConsoleLogEvent)
END_EVENT_TABLE()

QuME_Frame::QuME_Frame(wxWindow* parent,wxWindowID id)
{
    wxMenuItem* MenuQuit;
    wxMenuItem* MenuAbout;
    wxMenu* FileMenu;
    wxMenu* MenuHelp;
    wxMenuBar* MenuBar1;
    wxGridBagSizer* GridBagSizer1;

    //the main dialog
    Create(parent, wxID_ANY, _("QuME v") + QUME_VERSION, wxDefaultPosition, wxSize(800, 600), wxMINIMIZE_BOX|wxCLOSE_BOX|wxCAPTION, _T("ID_QUME"));
    {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("QuME_icon.png"))));
        SetIcon(FrameIcon);
    }

    GridBagSizer1 = new wxGridBagSizer(0, 0);

    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Info Console"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    GridBagSizer1->Add(StaticText4, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxSize(400,200),
                               wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxSUNKEN_BORDER|wxVSCROLL|wxHSCROLL,
                               wxDefaultValidator,
                               _T("ID_TEXTCTRL1"));
    wxFont TextCtrl1Font(10,wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Liberation Mono"),wxFONTENCODING_DEFAULT);
    TextCtrl1->SetFont(TextCtrl1Font);
    GridBagSizer1->Add(TextCtrl1, wxGBPosition(1, 0), wxGBSpan(1, 2), wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 5);

    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Preview Window"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    GridBagSizer1->Add(StaticText5, wxGBPosition(0, 3), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    wxGLAttributes dispAttrs;
    dispAttrs.PlatformDefaults().MinRGBA(8, 8, 8, 0).DoubleBuffer().Depth(16).EndList(); //low settings for my crappy video card
    GLCanvas1 = new wxGLCanvas(this,
                               dispAttrs,
                               ID_GLCANVAS1,
                               wxDefaultPosition,
                               wxSize(200,200),
                               0, _T("ID_GLCANVAS1"));

    GridBagSizer1->Add(GLCanvas1, wxGBPosition(1, 2), wxGBSpan(1,2), wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    //Face inspector GUI items
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Face Inspector:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    GridBagSizer1->Add(StaticText1, wxGBPosition(2, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    SpinCtrl1 = new wxSpinCtrl(this, ID_SPIN_FACE_EXAMINE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPIN_FACE_EXAMINE"));
    SpinCtrl1->SetValue(_T("0"));
    SpinCtrl1->Disable();
    SpinCtrl1->SetToolTip(_("Face to Examine"));
    GridBagSizer1->Add(SpinCtrl1, wxGBPosition(2, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


    //entity inspector GUI items
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Entity Inspector:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    GridBagSizer1->Add(StaticText2, wxGBPosition(3, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    SpinCtrl2 = new wxSpinCtrl(this, ID_SPINENTITYEXAMINE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINENTITYEXAMINE"));
    SpinCtrl2->SetValue(_T("0"));
    SpinCtrl2->Disable();
    SpinCtrl2->SetToolTip("Entity to Examine");
    GridBagSizer1->Add(SpinCtrl2, wxGBPosition(3, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);


    //texture inspector GUI items
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Texture Inspector:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridBagSizer1->Add(StaticText3, wxGBPosition(4, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    SpinCtrl3 = new wxSpinCtrl(this, ID_SPINTEXTUREEXAMINE, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINTEXTUREEXAMINE"));
    SpinCtrl3->SetValue(_T("0"));
    SpinCtrl3->Disable();
    SpinCtrl3->SetToolTip("Texture to Examine");
    GridBagSizer1->Add(SpinCtrl3, wxGBPosition(4, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("X Rotation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    GridBagSizer1->Add(StaticText6, wxGBPosition(2, 2), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Y Rotation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    GridBagSizer1->Add(StaticText7, wxGBPosition(3, 2), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Z Rotation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    GridBagSizer1->Add(StaticText8, wxGBPosition(4, 2), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    Slider1 = new wxSlider(this, ID_SLIDER_XROTATION, 0, -180, 180, wxDefaultPosition, wxSize(180,20), 0, wxDefaultValidator, _T("ID_SLIDER_XROTATION"));
    Slider1->SetToolTip(_("X Rotation"));
    Slider1->SetHelpText(_("X Rotation"));
    GridBagSizer1->Add(Slider1, wxGBPosition(2, 3), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    Slider2 = new wxSlider(this, ID_SLIDER_YROTATION, 0, -180, 180, wxDefaultPosition, wxSize(180,20), 0, wxDefaultValidator, _T("ID_SLIDER_YROTATION"));
    Slider2->SetToolTip(_("Y Rotation"));
    Slider2->SetHelpText(_("Y Rotation"));
    GridBagSizer1->Add(Slider2, wxGBPosition(3, 3), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    Slider3 = new wxSlider(this, ID_SLIDER_ZROTATION, 0, -180, 180, wxDefaultPosition, wxSize(180,20), 0, wxDefaultValidator, _T("ID_SLIDER_ZROTATION"));
    Slider3->SetToolTip(_("Z Rotation"));
    Slider3->SetHelpText(_("Z Rotation"));
    GridBagSizer1->Add(Slider3, wxGBPosition(4, 3), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);

    //menus
    MenuBar1 = new wxMenuBar();
    FileMenu = new wxMenu();
    MenuOpen = new wxMenuItem(FileMenu, ID_MENU_OPEN, _("&Open...\tAlt-O"), _("Open a BSP File"), wxITEM_NORMAL);
    FileMenu->Append(MenuOpen);
    MenuExport = new wxMenuItem(FileMenu, ID_MENU_EXPORT, _("&Export to .obj/.mtl...\tAlt-E"), _("Export current BSP data to Wavefront format"), wxITEM_NORMAL);
    FileMenu->Append(MenuExport);
    MenuExport->Enable(false);
    MenuBrushes = new wxMenuItem(FileMenu, ID_MENU_BRUSHES, _("Export Brushes to .obj..."), _("Export BSP Brushes to Wavefront format"), wxITEM_NORMAL);
    FileMenu->Append(MenuBrushes);
    MenuBrushes->Enable(false);
    FileMenu->AppendSeparator();
    MenuQuit = new wxMenuItem(FileMenu, ID_MENU_QUIT, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    FileMenu->Append(MenuQuit);
    MenuBar1->Append(FileMenu, _("&File"));
    MenuOptions = new wxMenu();
    MenuBasedir = new wxMenuItem(MenuOptions, ID_MENU_BASEDIR, _("Set Quake 2 Base Directory..."), wxEmptyString, wxITEM_NORMAL);
    MenuOptions->Append(MenuBasedir);
    MenuBar1->Append(MenuOptions, _("Options"));
    MenuHelp = new wxMenu();
    MenuAbout = new wxMenuItem(MenuHelp, ID_MENU_ABOUT, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    MenuHelp->Append(MenuAbout);
    MenuBar1->Append(MenuHelp, _("Help"));
    SetMenuBar(MenuBar1);

    //status bar
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, wxRAISED_BORDER, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    GridBagSizer1->Fit(this);
    GridBagSizer1->SetSizeHints(this);
    SetSizer(GridBagSizer1);

    //wxGLCanvas1 redraw timer
    Timer1.SetOwner(this, ID_TIMER1);
    Timer1.Start(100, false);

    //spin control event function handler bindings
    Connect(ID_SPIN_FACE_EXAMINE,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&QuME_Frame::OnSpinCtrl1Change);
    Connect(ID_SPINENTITYEXAMINE,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&QuME_Frame::OnSpinCtrl2Change);
    Connect(ID_SPINTEXTUREEXAMINE,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&QuME_Frame::OnSpinCtrl3Change);

    //quit event handler binding
    Connect(ID_MENU_QUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&QuME_Frame::OnQuit);

    //about dialog box popup event handler binding
    Connect(ID_MENU_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&QuME_Frame::OnAbout);

    //timer event function handler binding
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&QuME_Frame::UpdateGLWindow);

    Connect(ID_MENU_OPEN, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnOpenBSP);
    Connect(ID_MENU_OPEN, wxEVT_UPDATE_UI, (wxObjectEventFunction)&QuME_Frame::OnUpdateBSPProcessing);

    Connect(ID_MENU_EXPORT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnWavefrontExport);
    Connect(ID_MENU_EXPORT, wxEVT_UPDATE_UI, (wxObjectEventFunction)&QuME_Frame::OnUpdateObjExport);

    Connect(ID_MENU_BASEDIR, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnSetBasedir);

    Context1 = new wxGLContext(GLCanvas1);

    //read in our configuration info
    this->Configuration = new wxFileConfig("QuME", "", wxFileName::GetCwd() + "/QuME.cfg", "", wxCONFIG_USE_LOCAL_FILE);
    if(this->Configuration->HasEntry("GameBaseDir"))
    {
        this->GameDir = this->Configuration->Read("GameBaseDir", "");
        TextCtrl1->AppendText("Game directory is: \'" + this->GameDir + "\'\n\n");
    }
    else
    {
        TextCtrl1->AppendText("Game directory not set yet\nPlease set your Quake 2 baseq2 directory in \'Options...\'\n\n");
    }
    BSP = nullptr;
}

QuME_Frame::~QuME_Frame()
{
    //(*Destroy(QuME_Frame)
    //*)

    {
        wxCriticalSectionLocker locker(wxGetApp().CritSect);

        const wxArrayThread& threads = wxGetApp().Threads;
        size_t count = threads.GetCount();

        if (count == 0)
        {
            delete BSP;
            return;
        }

        wxGetApp().ShuttingDown = true;
    }
    wxGetApp().AllDone.Wait();

    //Wait until all threads have exited before deleting BSP Data
    delete this->BSP;
    delete this->Configuration;
}

bool QuME_Frame::ObjExportCanceled()
{
    wxCriticalSectionLocker lock(CritSecExportCanceled);

    return ExportCanceled;
}

bool QuME_Frame::BSPImportCanceled()
{
     wxCriticalSectionLocker lock(CritSecImportCanceled);

     return ImportCanceled;
}

void QuME_Frame::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void QuME_Frame::OnAbout(wxCommandEvent& event)
{
    wxString msg = _("QuME v") + QUME_VERSION + "\nBy Joseph Thomas";
    wxMessageBox(msg, _("Welcome to..."));
}

void QuME_Frame::OnSetBasedir(wxCommandEvent& event)
{
    wxDirDialog dir(this, "Select the Quake 2 baseq2 folder", this->GameDir, wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST);

    if(dir.ShowModal() != wxID_CANCEL)
    {
        wxString DefaultPath = dir.GetPath();
        if(!DefaultPath.EndsWith("/"))
        {
            DefaultPath.Append("/");
        }
        if(!this->Configuration->Write("GameBaseDir", DefaultPath))
        {
            TextCtrl1->AppendText("Couldn't set GameBaseDir in QuME.cfg!\n\n");
        }
        this->Configuration->Flush(true);
        this->GameDir = DefaultPath;
        TextCtrl1->AppendText(_("Base Directory Set to \'") + this->GameDir + "\'\n\n");
    }
}

//Do all the file parsing, load data into class BSP
void QuME_Frame::OnOpenBSP(wxCommandEvent& event)
{
    MenuOpen->Enable(false);

    wxMessageDialog errorbox(this, "BSP Import thread can't run!", "Error!", wxOK|wxICON_ERROR|wxCENTER);

    wxFileDialog openFileDialog(this, _("Open BSP file"), this->GameDir + "maps/", "",
                                "BSP files (*.bsp)|*.bsp", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    if(BSP != nullptr) delete BSP; //we're opening a new BSP file, dump old one
    BSP = new QuME_BSP_Data();
    BSP->BaseDir = this->GameDir;

    MenuExport->Enable(false);
    MenuBrushes->Enable(false);

    TextCtrl1->SetValue(openFileDialog.GetFilename() + _("\n"));

    BSP->MapName = openFileDialog.GetFilename();

    QuME_BSP_Load* thread = new QuME_BSP_Load(this, BSP, openFileDialog.GetPath());

    if(thread->Create() != wxTHREAD_NO_ERROR)
    {
        errorbox.ShowModal();
        return;
    }

    ImportCanceled = false;

    wxCriticalSectionLocker enter(wxGetApp().CritSect);
    wxGetApp().Threads.Add(thread);

    thread->Run();
}

void QuME_Frame::OnWavefrontExport(wxCommandEvent& event)
{
    wxString MapNameNoExt;
    BSP->MapName.EndsWith(".bsp", &MapNameNoExt); //strip off .bsp extension

    wxFileDialog saveFileDialog(this, "Export BSP to OBJ", BSP->BaseDir, MapNameNoExt + ".obj",
                                "OBJ files (*.obj)|*.obj", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    wxMessageDialog errorbox(this, "Can't save to file!", "Error!", wxOK|wxICON_ERROR|wxCENTER);

    wxString mapFileName = saveFileDialog.GetPath();
    wxString FullPath;
    mapFileName.EndsWith(".obj", &FullPath);

    QuME_BSP_ObjExporter *thread = new QuME_BSP_ObjExporter(this, BSP, MapNameNoExt, FullPath);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        errorbox.ShowModal();
        return;
    }

    // thread is not running yet, no need for crit sect
    ExportCanceled = false;

    thread->Run();

    //BSP->ExportToWavefront(this);
}

void QuME_Frame::OnSpinCtrl1Change(wxSpinEvent& event)
{
    wxUint32 i = SpinCtrl1->GetValue();
    if(i >= BSP->Faces.Count)
    {
        SpinCtrl1->SetValue(0);
        i = 0;
    }
    QuME_BSP_Face* f = &BSP->Faces.Face[i];
    QuME_BSP_Texture* t = &BSP->Textures.Texture[f->TextureInfo];
    wxInt32* SEdge = BSP->SurfaceEdges.SurfaceEdge;
    QuME_BSP_Edge* BSPEdges = BSP->Edges.EdgeArray;
    if(f != nullptr)
    {
        TextCtrl1->AppendText(_("*-----------------------------*\n"));
        TextCtrl1->AppendText(_("Face #") + std::to_wstring(i) + _("\n"));
        TextCtrl1->AppendText(_("Plane #") + std::to_wstring(f->Plane) + _("\n"));
        TextCtrl1->AppendText(_("Plane side: ") + std::to_wstring(f->PlaneSide) + _("\n"));
        TextCtrl1->AppendText(_("Edge count: ") + std::to_wstring(f->EdgeCount) + _("\n"));
        TextCtrl1->AppendText(_("First edge index: ") + std::to_wstring(f->FirstEdge) + _("\n"));
        for(wxInt32 j = 0; j < f->EdgeCount; j++)
        {
            wxUint32 firstEdge = f->FirstEdge;
            QuME_BSP_Edge* vertindexes = &BSPEdges[SEdge[firstEdge + j]];
            TextCtrl1->AppendText(std::to_wstring(vertindexes->a) + _(" ") + std::to_wstring(vertindexes->b) + _("\n"));
        }
        for(wxUint32 j = 0; j < f->TriangleCount; j++)
        {
            TextCtrl1->AppendText("Triangle " + std::to_wstring(j) + "\n");
            for(wxInt32 k = 0; k < 3; k++)
            {
                TextCtrl1->AppendText(std::to_wstring(f->Triangle[j].v[k]) + " ");
            }
            TextCtrl1->AppendText("\n");
        }
        TextCtrl1->AppendText(_("Texture Name: ") + t->MaterialName + _("\n\n"));
    }
}

void QuME_Frame::OnSpinCtrl2Change(wxSpinEvent& event)
{
    wxUint32 i = SpinCtrl2->GetValue();
    if(i >= BSP->Entities.Count)
    {
        SpinCtrl2->SetValue(0);
        i = 0;
    }

    TextCtrl1->AppendText(_("*-----------------------------*\n"));
    TextCtrl1->AppendText(_("Entity #") + std::to_wstring(i) + _("\n"));
    for(ent_key* k = BSP->Entities.Entity[i].keys; k != nullptr; k = k->next)
    {
        TextCtrl1->AppendText(_("\"") + k->keyName + _("\" \"") + k->keyValue + _("\"\n"));
    }
    TextCtrl1->AppendText(_("\n"));

}

void QuME_Frame::OnSpinCtrl3Change(wxSpinEvent& event)
{
    wxUint32 i = SpinCtrl3->GetValue();
    if(i >= this->BSP->Textures.Count)
    {
        SpinCtrl3->SetValue(0);
        i = 0;
    }
    QuME_BSP_Texture* t = &BSP->Textures.Texture[i];
    if(t != nullptr)
    {
        TextCtrl1->AppendText(_("*-----------------------------*\n"));
        TextCtrl1->AppendText(_("Texture Name: ") + t->MaterialName + "\n");
        TextCtrl1->AppendText(_("Resolution: X = ") + std::to_wstring(t->xRes));
        TextCtrl1->AppendText(_(" Y = ") + std::to_wstring(t->yRes) + "\n\n");
    }
}

//this sets all UI objects to disabled if progress dialog exists, to simulate modal behavior across threads
void QuME_Frame::OnUpdateBSPProcessing(wxUpdateUIEvent& event)
{
    //event.Enable( BSPProcessingProgressDialog == nullptr );
}

void QuME_Frame::OnUpdateObjExport(wxUpdateUIEvent& event)
{
    //event.Enable( ExportProgressDialog == nullptr );
}

//timer based GLCanvas refresh + other things
void QuME_Frame::UpdateGLWindow(wxTimerEvent& event)
{
    if(Context1->SetCurrent(*GLCanvas1))
    {
        glEnable(GL_DEPTH_TEST); /* enable depth buffering */
        glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
        glClearDepth(1.0);       /* pedantic, 1.0 is the default */
        /* frame buffer clears should be to black */
        glClearColor(0.0, 0.0, 0.0, 0.0);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10000.0);

        glMatrixMode(GL_MODELVIEW);

        /* reset modelview matrix to the identity matrix */
        glLoadIdentity();

        /* move the camera back six units */
        glTranslatef(0.0, 0.0, -400.0);

        xRot = Slider1->GetValue();
        yRot = Slider2->GetValue();
        zRot = Slider3->GetValue();

        /* rotate by X, Y, and Z angles */
        glRotatef(xRot, 0.1, 0.0, 0.0);
        glRotatef(yRot, 0.0, 0.1, 0.0);
        glRotatef(zRot, 0.0, 0.0, 1.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(MenuExport->IsEnabled())
        {
            glBegin(GL_TRIANGLES);
            glColor3f(0.5, 1.0, 1.0);
            for(wxUint32 i = 0; i < BSP->BrushModels.Count; i++)
            {

            }
            glEnd();

        }

        GLCanvas1->SwapBuffers();

    }
}

//respond to progress messages from the .obj export thread
void QuME_Frame::OnObjExportEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if ( n == -1 ) //done exporting, close progress dialog
    {
        ExportProgressDialog->Destroy();
        ExportProgressDialog = nullptr;

        wxWakeUpIdle();
    }
    else if(n == -2) //error, close progress dialog and report
    {
        ExportProgressDialog->Destroy();
        ExportProgressDialog = nullptr;

        wxMessageBox("Error exporting .obj file!", "Export Error!", 0, this);

        wxWakeUpIdle();
    }
    else
    {
        if (!ExportProgressDialog->Update(n))
        {
            wxCriticalSectionLocker lock(CritSecExportCanceled);

            ExportCanceled = true;
        }
    }
}

void QuME_Frame::OnBSPProcessingEvent(wxThreadEvent& event)
{
    if(this->BSPProcessingProgressDialog == nullptr)
    {
        BSPProcessingProgressDialog = new wxProgressDialog(wxT("Processing BSP data ..."),
                wxT(""),
                100,
                this,
                wxPD_ELAPSED_TIME |
                wxPD_ESTIMATED_TIME |
                wxPD_REMAINING_TIME);
    }

    int n = event.GetInt();
    if(n == -1)
    {
        BSPProcessingProgressDialog->Destroy();
        BSPProcessingProgressDialog = nullptr;

        wxWakeUpIdle();
    }
    else
    {
        BSPProcessingProgressDialog->Update(n);
    }
}

void QuME_Frame::OnBSPImportEvent(wxThreadEvent& event)
{
    TextCtrl1->AppendText(event.GetString());
    if(event.GetString().compare("File Loaded!\n\n") == 0)
    {
        //file loaded successfully, enable everything
        MenuOpen->Enable(true);

        MenuExport->Enable(true);
        MenuBrushes->Enable(true);
        SpinCtrl1->Enable();
        SpinCtrl1->SetRange(0, this->BSP->Faces.Count);
        SpinCtrl2->Enable();
        SpinCtrl2->SetRange(0, this->BSP->Entities.Count);
        SpinCtrl3->Enable();
        SpinCtrl3->SetRange(0, this->BSP->Textures.Count);
    }
    else if((event.GetString().compare("Load Failed!\n\n") == 0) || (event.GetString().compare("Load Canceled!\n\n") == 0))
    {
        SpinCtrl1->Disable();
        SpinCtrl1->SetRange(0, 0);
        SpinCtrl2->Disable();
        SpinCtrl2->SetRange(0, 0);
        SpinCtrl3->Disable();
        SpinCtrl3->SetRange(0, 0);
        MenuOpen->Enable(true);
        MenuExport->Enable(false);
        MenuBrushes->Enable(false);
    }
}

//method to allow threads to write to console window safely
void QuME_Frame::OnConsoleLogEvent(wxThreadEvent& event)
{
    TextCtrl1->AppendText(event.GetString());
}
