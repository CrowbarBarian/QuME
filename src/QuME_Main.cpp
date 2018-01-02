/***************************************************************
 * Name:		QuME_Main.cpp
 * Purpose:		Code for Application Frame
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:		2017-06-13
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
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

const long QuME_Frame::ID_QUME = wxNewId();
const long QuME_Frame::ID_INFOCONSOLE = wxNewId();

const long QuME_Frame::ID_SPIN_FACE_EXAMINE = wxNewId();
const long QuME_Frame::ID_SPIN_ENTITY_EXAMINE = wxNewId();
const long QuME_Frame::ID_SPIN_TEXTURE_EXAMINE = wxNewId();

const long QuME_Frame::ID_STATICTEXT_FACEINSPECTOR = wxNewId();
const long QuME_Frame::ID_STATICTEXT_ENTITYINSPECTOR = wxNewId();
const long QuME_Frame::ID_STATICTEXT_TEXTUREINSPECTOR = wxNewId();
const long QuME_Frame::ID_STATICTEXT_INFOCONSOLE = wxNewId();

const long QuME_Frame::ID_MENU_OPEN = wxNewId();
const long QuME_Frame::ID_MENU_EXPORT = wxNewId();
const long QuME_Frame::ID_MENU_BRUSHES = wxNewId();
const long QuME_Frame::ID_MENU_QUIT = wxNewId();
const long QuME_Frame::ID_MENU_BASEDIR = wxNewId();
const long QuME_Frame::ID_MENU_ABOUT = wxNewId();

const long QuME_Frame::ID_PROGRESSBAR = wxNewId();

const long QuME_Frame::ID_STATUSBAR = wxNewId();

BEGIN_EVENT_TABLE(QuME_Frame,wxFrame)
    EVT_THREAD(OBJEXPORT_EVENT, QuME_Frame::OnObjExportEvent)
	EVT_THREAD(BRUSHEXPORT_EVENT, QuME_Frame::OnBrushExportEvent)
    EVT_THREAD(BSPIMPORT_EVENT, QuME_Frame::OnBSPImportEvent)
    EVT_THREAD(BSPPROCESSING_EVENT, QuME_Frame::OnBSPProcessingEvent)
    EVT_THREAD(CONSOLELOG_EVENT, QuME_Frame::OnConsoleLogEvent)
END_EVENT_TABLE()

QuME_Frame::QuME_Frame(wxWindow* parent,wxWindowID id)
{
    wxMenuItem* menuQuit = nullptr;
    wxMenuItem* menuAbout = nullptr;
    wxMenu* fileMenu = nullptr;
    wxMenu* menuHelp = nullptr;
    wxMenuBar* menuBar = nullptr;
    wxGridBagSizer* mainGridBagSizer = nullptr;

    this->ProgressBar = nullptr;
    std::wstring title = L"QuME v";
    title += QUME_VERSION;

    //the main dialog
    Create(parent,
		ID_QUME,
		title,
		wxDefaultPosition,
		wxSize(800, 600),
		((wxMINIMIZE_BOX|wxCLOSE_BOX|wxCAPTION|wxSYSTEM_MENU) & ~wxMAXIMIZE_BOX),
		L"ID_QUME");
    {
        wxIcon frameIcon;
        wxImage iconImage(48,48,true);
        iconImage.SetData(this->icon_image.pixel_data, true);
		frameIcon.CopyFromBitmap(wxBitmap(iconImage));
        SetIcon(frameIcon);
    }

    mainGridBagSizer = new wxGridBagSizer(0, 0);

    staticTextInfoConsole = new wxStaticText(this,
											ID_STATICTEXT_INFOCONSOLE,
											L"Info Console",
											wxDefaultPosition,
											wxDefaultSize,
											0,
											L"ID_STATICTEXT_INFOCONSOLE");
    mainGridBagSizer->Add(staticTextInfoConsole, wxGBPosition(0, 0), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    infoConsole = new wxTextCtrl(this, ID_INFOCONSOLE,
                               wxEmptyString,
                               wxDefaultPosition,
                               wxSize(400,200),
                               wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxSUNKEN_BORDER|wxVSCROLL|wxHSCROLL,
                               wxDefaultValidator,
                               L"ID_INFOCONSOLE");
    wxFont infoConsoleFont(10,wxFONTFAMILY_TELETYPE,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,L"Liberation Mono",wxFONTENCODING_DEFAULT);
    infoConsole->SetFont(infoConsoleFont);
    mainGridBagSizer->Add(infoConsole, wxGBPosition(1, 0), wxGBSpan(1, 2), wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 2);

    //Face inspector GUI items
    staticTextFaceInspector = new wxStaticText(this,
											ID_STATICTEXT_FACEINSPECTOR,
											L"Face Inspector:",
											wxDefaultPosition,
											wxDefaultSize,
											0,
											L"ID_STATICTEXT_FACEINSPECTOR");
    mainGridBagSizer->Add(staticTextFaceInspector, wxGBPosition(2, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    spinFaceExamine = new wxSpinCtrl(this, ID_SPIN_FACE_EXAMINE, L"0", wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, L"ID_SPIN_FACE_EXAMINE");
    spinFaceExamine->SetValue(L"0");
    spinFaceExamine->Disable();
    spinFaceExamine->SetToolTip(L"Face to Examine");
    mainGridBagSizer->Add(spinFaceExamine, wxGBPosition(2, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    //entity inspector GUI items
    staticTextEntityInspector = new wxStaticText(this,
												ID_STATICTEXT_ENTITYINSPECTOR,
												L"Entity Inspector:",
												wxDefaultPosition,
												wxDefaultSize,
												0,
												L"ID_STATICTEXT_ENTITYINSPECTOR");
    mainGridBagSizer->Add(staticTextEntityInspector, wxGBPosition(3, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    spinEntityToExamine = new wxSpinCtrl(this,
										ID_SPIN_ENTITY_EXAMINE,
										L"0",
										wxDefaultPosition,
										wxDefaultSize,
										0,
										0,
										100,
										0,
										L"ID_SPIN_ENTITY_EXAMINE");
    spinEntityToExamine->SetValue(L"0");
    spinEntityToExamine->Disable();
    spinEntityToExamine->SetToolTip(L"Entity to Examine");
    mainGridBagSizer->Add(spinEntityToExamine, wxGBPosition(3, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    //texture inspector GUI items
    staticTextTextureInspector = new wxStaticText(this,
												ID_STATICTEXT_TEXTUREINSPECTOR,
												L"Texture Inspector:",
												wxDefaultPosition,
												wxDefaultSize,
												0,
												L"ID_STATICTEXT_TEXTUREINSPECTOR");
    mainGridBagSizer->Add(staticTextTextureInspector, wxGBPosition(4, 0), wxDefaultSpan, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);

    spinTextureExamine = new wxSpinCtrl(this,
										ID_SPIN_TEXTURE_EXAMINE,
										L"0",
										wxDefaultPosition,
										wxDefaultSize,
										0,
										0,
										100,
										0,
										L"ID_SPIN_TEXTURE_EXAMINE");
    spinTextureExamine->SetValue(L"0");
    spinTextureExamine->Disable();
    spinTextureExamine->SetToolTip(L"Texture to Examine");
    mainGridBagSizer->Add(spinTextureExamine, wxGBPosition(4, 1), wxDefaultSpan, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);

    //menus
    menuBar = new wxMenuBar();
    fileMenu = new wxMenu();
    menuOpen = new wxMenuItem(fileMenu, ID_MENU_OPEN, L"&Open...\tAlt-O", L"Open a BSP File", wxITEM_NORMAL);
    fileMenu->Append(menuOpen);
    menuExport = new wxMenuItem(fileMenu, ID_MENU_EXPORT, L"&Export to .obj/.mtl...\tAlt-E", L"Export current BSP data to Wavefront format", wxITEM_NORMAL);
    fileMenu->Append(menuExport);
    menuExport->Enable(false);
    menuBrushes = new wxMenuItem(fileMenu, ID_MENU_BRUSHES, L"Export Brushes to .obj/.mtl...", L"Export BSP Brushes to Wavefront format", wxITEM_NORMAL);
    fileMenu->Append(menuBrushes);
    menuBrushes->Enable(false);
    fileMenu->AppendSeparator();
    menuQuit = new wxMenuItem(fileMenu, ID_MENU_QUIT, L"Quit\tAlt-F4", L"Quit the application", wxITEM_NORMAL);
    fileMenu->Append(menuQuit);
    menuBar->Append(fileMenu, L"&File");
    menuOptions = new wxMenu();
    menuBaseDirectory = new wxMenuItem(menuOptions, ID_MENU_BASEDIR, L"Set Quake 2 Base Directory...", wxEmptyString, wxITEM_NORMAL);
    menuOptions->Append(menuBaseDirectory);
    menuBar->Append(menuOptions, L"Options");
    menuHelp = new wxMenu();
    menuAbout = new wxMenuItem(menuHelp, ID_MENU_ABOUT, L"About\tF1", L"Show info about this application", wxITEM_NORMAL);
    menuHelp->Append(menuAbout);
    menuBar->Append(menuHelp, L"Help");
    SetMenuBar(menuBar);

    //progress bar
    this->ProgressBar = new wxGauge(this,
									ID_PROGRESSBAR,
									100,
									wxDefaultPosition,
									wxSize(400, 16),
									wxHORIZONTAL,
									wxDefaultValidator,
									L"ID_PROGRESSBAR");
    this->ProgressBar->SetValue(0);
	mainGridBagSizer->Add(this->ProgressBar,
						wxGBPosition(5, 0),
						wxGBSpan(1,3),
						wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,
						2);
    this->ProgressBar->Enable(false);

    //status bar
    statusBar = new wxStatusBar(this, ID_STATUSBAR, wxRAISED_BORDER, L"ID_STATUSBAR");
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    statusBar->SetFieldsCount(1,__wxStatusBarWidths_1);
    statusBar->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(statusBar);

    mainGridBagSizer->Fit(this);
    mainGridBagSizer->SetSizeHints(this);
    SetSizer(mainGridBagSizer);

    //spin control event function handler bindings
    Connect(ID_SPIN_FACE_EXAMINE,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&QuME_Frame::OnSpinFaceExamineChange);
    Connect(ID_SPIN_ENTITY_EXAMINE,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&QuME_Frame::OnSpinEntityExamineChange);
    Connect(ID_SPIN_TEXTURE_EXAMINE,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&QuME_Frame::OnSpinTextureExamineChange);

    //quit event handler binding
    Connect(ID_MENU_QUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&QuME_Frame::OnQuit);

    //about dialog box popup event handler binding
    Connect(ID_MENU_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&QuME_Frame::OnAbout);

    Connect(ID_MENU_OPEN, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnOpenBSP);
    //Connect(ID_MENU_OPEN, wxEVT_UPDATE_UI, (wxObjectEventFunction)&QuME_Frame::OnUpdateBSPProcessing);

    Connect(ID_MENU_EXPORT, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnWavefrontExport);
    //Connect(ID_MENU_EXPORT, wxEVT_UPDATE_UI, (wxObjectEventFunction)&QuME_Frame::OnUpdateBrushExport);

    Connect(ID_MENU_BRUSHES, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnBrushExport);


    Connect(ID_MENU_BASEDIR, wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&QuME_Frame::OnSetBasedir);

    //read in our configuration info
    this->configuration = new wxFileConfig(L"QuME", L"", wxFileName::GetCwd() + L"/QuME.cfg", L"", wxCONFIG_USE_LOCAL_FILE);
    if(this->configuration->HasEntry(L"GameBaseDir"))
    {
        this->gameDir = this->configuration->Read(L"GameBaseDir", "");
        infoConsole->AppendText(L"Game directory is: \'" + this->gameDir + L"\'\n\n");

		wxDir gDir(this->gameDir);

		if(gDir.HasFiles(L"*.pak"))
		{
			//find all the pak files, and add to pak list
			for(wxUint64 i = 0; i < MAX_PAKS; i++)
			{
				std::wstring pakname = this->gameDir + L"pak" + std::to_wstring(i) + L".pak";
				if(wxFile::Exists(pakname))
				{
					QuME_PAK_File f(pakname);
					this->pakFiles.Add(f);
				}
			}
			wxString pakname;
			bool cont = gDir.GetFirst(&pakname, L"*.pak", wxDIR_FILES);
			while(cont)
			{
				std::wstring pn = this->gameDir + pakname.ToStdWstring();
				QuME_PAK_File f;
				f.pakFileName = pn;
				f.LoadPAKFileInfo();
				this->pakFiles.AddIfUnique(f);
				cont = gDir.GetNext(&pakname);
			}

			for(ListItem<QuME_PAK_File>* pf = this->pakFiles.Head; pf !=nullptr; pf = pf->next)
			{
				infoConsole->AppendText(pf->Data.pakFileName);
				infoConsole->AppendText("\n");
				pf->Data.dirEntryList.ToArray(pf->Data.dirEntries);
			}
		}
		else
		{
			infoConsole->AppendText(L"Directory has no pak files!\nTry another one!\n");
		}
    }
    else
    {
        infoConsole->AppendText(L"Game directory not set yet\nPlease set your Quake 2 baseq2 directory in \'Options...\'\n\n");
    }
}

QuME_Frame::~QuME_Frame()
{
    {
        wxCriticalSectionLocker locker(wxGetApp().CritSectThreads);

        const wxArrayThread& threads = wxGetApp().Threads;
        size_t count = threads.GetCount();

        if (count == 0)
        {
			SAFE_DELETE(this->configuration);
			this->configuration = nullptr;
            return;
        }

        wxGetApp().ShuttingDown = true;
    }
    wxGetApp().AllDone.Wait();

	SAFE_DELETE(this->configuration);
	this->configuration = nullptr;
}

void QuME_Frame::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void QuME_Frame::OnAbout(wxCommandEvent& event)
{
    std::wstring msg = L"QuME v";
    msg += QUME_VERSION;
    msg += L"\nBy J M Thomas";
    wxMessageBox(msg, L"About...");
}

void QuME_Frame::OnSetBasedir(wxCommandEvent& event)
{
    wxDirDialog dir(this, L"Select the Quake 2 baseq2 folder", this->gameDir, wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST);

    if(dir.ShowModal() != wxID_CANCEL)
    {
        std::wstring DefaultPath(dir.GetPath());
        if(DefaultPath.back() != L'/')
        {
            DefaultPath += L"/";
        }
        if(!this->configuration->Write(L"GameBaseDir", wxString(DefaultPath)))
        {
            infoConsole->AppendText(L"Couldn't set GameBaseDir in QuME.cfg!\n\n");
        }
        this->configuration->Flush(true);
        this->gameDir = DefaultPath;
		wxDir gDir(this->gameDir);

		if(gDir.HasFiles(L"*.pak"))
		{
			//new directory, so clear all .pak files
			this->pakFiles.ClearList();
			//find all the pak files, and add to pak list
			for(wxUint64 i = 0; i < MAX_PAKS; i++)
			{
				std::wstring pakname = this->gameDir + L"pak" + std::to_wstring(i) + L".pak";
				if(wxFile::Exists(pakname))
				{
					QuME_PAK_File f(pakname);
					this->pakFiles.Add(f);
				}
			}
			wxString pakname;
			bool cont = gDir.GetFirst(&pakname, L"*.pak", wxDIR_FILES);
			while(cont)
			{
				std::wstring pn = this->gameDir + pakname.ToStdWstring();
				QuME_PAK_File f;
				f.pakFileName = pn;
				f.LoadPAKFileInfo();
				this->pakFiles.AddIfUnique(f);
				cont = gDir.GetNext(&pakname);
			}

			//list all found .pak files in console
			for(ListItem<QuME_PAK_File>* pf = this->pakFiles.Head; pf !=nullptr; pf = pf->next)
			{
				infoConsole->AppendText(pf->Data.pakFileName);
				infoConsole->AppendText("\n");
				pf->Data.dirEntryList.ToArray(pf->Data.dirEntries);
			}
		}
		else
		{
			infoConsole->AppendText(L"Directory has no pak files!\nTry another one!\n");
		}
        infoConsole->AppendText(L"Base Directory Set to \'" + this->gameDir + L"\'\n\n");
    }
}

//Do all the file parsing, load data into class BSP
void QuME_Frame::OnOpenBSP(wxCommandEvent& event)
{
    menuOpen->Enable(false);

    wxMessageDialog errorbox(this, L"BSP Import thread can't run!", L"Error!", wxOK|wxICON_ERROR|wxCENTER);

    wxFileDialog openFileDialog(this,
								L"Open BSP file",
								this->gameDir + L"maps/",
								L"",
                                L"BSP files (*.bsp)|*.bsp",
                                wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    if(wxGetApp().bsp != nullptr) delete wxGetApp().bsp; //we're opening a new BSP file, dump old one
    wxGetApp().bsp = new QuME_BSP_Data();
    wxGetApp().bsp->baseDir = this->gameDir;

    menuExport->Enable(false);
    menuBrushes->Enable(false);

    infoConsole->SetValue(openFileDialog.GetFilename() + L"\n");

    wxGetApp().bsp->MapName = openFileDialog.GetFilename();

    QuME_BSP_Load* thread = new QuME_BSP_Load(this, wxGetApp().bsp, std::wstring(openFileDialog.GetPath()));

    if(thread->Create() != wxTHREAD_NO_ERROR)
    {
        errorbox.ShowModal();
        return;
    }

    wxCriticalSectionLocker enter(wxGetApp().CritSectThreads);
    wxGetApp().Threads.Add(thread);

    thread->Run();
}

void QuME_Frame::OnWavefrontExport(wxCommandEvent& event)
{
    std::wstring MapNameNoExt = wxGetApp().bsp->MapName;
    ReplaceAll(&MapNameNoExt, L".bsp", L""); //strip off .bsp extension

    wxFileDialog saveFileDialog(this,
								L"Export BSP to OBJ",
								wxGetApp().bsp->baseDir,
								MapNameNoExt + L".obj",
                                L"OBJ files (*.obj)|*.obj",
                                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed their mind...

    wxMessageDialog errorbox(this, L"Can't save to file!", L"Error!", wxOK|wxICON_ERROR|wxCENTER);

    std::wstring mapFileName(saveFileDialog.GetPath());
    std::wstring FullPath = mapFileName;
    ReplaceAll(&FullPath, L".obj", L"");

    QuME_BSP_ObjExporter *thread = new QuME_BSP_ObjExporter(this, wxGetApp().bsp, MapNameNoExt, FullPath);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        errorbox.ShowModal();
        return;
    }

    wxCriticalSectionLocker enter(wxGetApp().CritSectThreads);
    wxGetApp().Threads.Add(thread);

    thread->Run();
}

void QuME_Frame::OnBrushExport(wxCommandEvent& event)
{
    std::wstring MapNameNoExt = wxGetApp().bsp->MapName;
    ReplaceAll(&MapNameNoExt, L".bsp", L""); //strip off .bsp extension

    wxFileDialog saveFileDialog(this,
								L"Export BSP to OBJ",
								wxGetApp().bsp->baseDir,
								MapNameNoExt + L"_brushes.obj",
                                L"OBJ files (*.obj)|*.obj",
                                wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed their mind...

    wxMessageDialog errorbox(this, L"Can't save to file!", L"Error!", wxOK|wxICON_ERROR|wxCENTER);

    std::wstring mapFileName(saveFileDialog.GetPath());
    std::wstring FullPath = mapFileName;
    ReplaceAll(&FullPath, L".obj", L"");

    QuME_BSP_Brush_Exporter *thread = new QuME_BSP_Brush_Exporter(this, wxGetApp().bsp, MapNameNoExt, FullPath);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        errorbox.ShowModal();
        return;
    }

    wxCriticalSectionLocker enter(wxGetApp().CritSectThreads);
    wxGetApp().Threads.Add(thread);

    thread->Run();
}

void QuME_Frame::OnBSPImportEvent(wxThreadEvent& event)
{
    infoConsole->AppendText(event.GetString());
    if(event.GetString().compare(L"File Loaded!\n\n") == 0)
    {
        //file loaded successfully, enable everything
        menuOpen->Enable(true);
        menuExport->Enable(true);
        menuBrushes->Enable(true);
        spinFaceExamine->Enable();
        spinFaceExamine->SetRange(0, wxGetApp().bsp->BSP_Faces.Faces.Count - 1);
        spinEntityToExamine->Enable();
        spinEntityToExamine->SetRange(0, wxGetApp().bsp->BSP_Entities.Entities.Count - 1);
        spinTextureExamine->Enable();
        spinTextureExamine->SetRange(0, wxGetApp().bsp->BSP_Textures.TextureInfo.Count - 1);
    }
    else if((event.GetString().compare(L"Load Failed!\n\n") == 0) || (event.GetString().compare(L"Load Canceled!\n\n") == 0))
    {
    	//file load failed, disable everything
        spinFaceExamine->Disable();
        spinFaceExamine->SetRange(0, 0);
        spinEntityToExamine->Disable();
        spinEntityToExamine->SetRange(0, 0);
        spinTextureExamine->Disable();
        spinTextureExamine->SetRange(0, 0);
        menuOpen->Enable(true);
        menuExport->Enable(false);
        menuBrushes->Enable(false);
    }
}

void QuME_Frame::OnBSPProcessingEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if(n == -1)
    {
    	this->ProgressBar->SetValue(100);

        wxWakeUpIdle();
    }
    else
    {
    	if(n > 100) n = 100;
    	this->ProgressBar->SetValue(n);
    }
}

//respond to progress messages from the .obj export thread
void QuME_Frame::OnObjExportEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if ( n == -1 ) //done exporting, close progress dialog
    {
    	this->ProgressBar->SetValue(100);

        wxWakeUpIdle();
    }
    else if(n == -2) //error, close progress dialog and report
    {
    	this->ProgressBar->SetValue(0);

        wxMessageBox(L"Error exporting .obj file!", L"Export Error!", 0, this);

        wxWakeUpIdle();
    }
    else
    {
    	if(n > 100) n = 100;
    	this->ProgressBar->SetValue(n);
	}
}

void QuME_Frame::OnBrushExportEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if ( n == -1 ) //done exporting, close progress dialog
    {
    	this->ProgressBar->SetValue(100);

        wxWakeUpIdle();
    }
    else if(n == -2) //error, close progress dialog and report
    {
    	this->ProgressBar->SetValue(0);

        wxMessageBox(L"Error exporting brushes!", L"Export Error!", 0, this);

        wxWakeUpIdle();
    }
    else
    {
    	if(n > 100) n = 100;
    	this->ProgressBar->SetValue(n);
	}
}

void QuME_Frame::OnSpinFaceExamineChange(wxSpinEvent& event)
{
    wxInt32 i = spinFaceExamine->GetValue();
    QuME_BSP_Face* f = &wxGetApp().bsp->BSP_Faces.Faces[i];
    QuME_BSP_Texture* tex = &wxGetApp().bsp->BSP_Textures.TextureInfo[f->TextureInfo];
    if(f != nullptr)
    {
        infoConsole->AppendText(L"*-----------------------------*\n");
        infoConsole->AppendText(L"Face #" + std::to_wstring(i) + L"\n");
        infoConsole->AppendText(L"Plane #" + std::to_wstring(f->Plane) + L"\n");
        infoConsole->AppendText(L"Plane side: " + std::to_wstring(f->PlaneSide) + L"\n");
        infoConsole->AppendText(L"Edge count: " + std::to_wstring(f->EdgeCount) + L"\n");
        infoConsole->AppendText(L"First edge index: " + std::to_wstring(f->FirstEdge) + L"\n");
        for(wxInt32 j = 0; j < f->EdgeCount; j++)
        {
            wxUint32 firstEdge = f->FirstEdge;
            wxInt32 FIndex = wxGetApp().bsp->BSP_SurfaceEdges.SurfaceEdgeIndices[firstEdge + j];
            wxUint32 VIndex = ((FIndex > 0) ? wxGetApp().bsp->BSP_Edges.Edges[FIndex].a : wxGetApp().bsp->BSP_Edges.Edges[-FIndex].b);
            infoConsole->AppendText(std::to_wstring(VIndex) + L"\n");
        }
        for(wxUint32 j = 0; j < f->Triangles.Count; j++)
        {
            infoConsole->AppendText(L"Triangle " + std::to_wstring(j) + L"\n");
            for(wxInt32 k = 0; k < 3; k++)
            {
                infoConsole->AppendText(std::to_wstring(f->Triangles[j].Vertex[k].VertexIndex) + L" ");
            }
            infoConsole->AppendText(L"\n");
        }
        infoConsole->AppendText(L"Texture Name: " + tex->MaterialName + L"\n\n");
    }
}

void QuME_Frame::OnSpinEntityExamineChange(wxSpinEvent& event)
{
    wxInt32 i = spinEntityToExamine->GetValue();
    infoConsole->AppendText(L"*-----------------------------*\n");
    infoConsole->AppendText(L"Entity #" + std::to_wstring(i) + L"\n");
    for(wxUint32 j = 0; j < wxGetApp().bsp->BSP_Entities.Entities[i].KeyCount; j++)
    {
        infoConsole->AppendText(L"\"" + wxGetApp().bsp->BSP_Entities.Entities[i].KeyArray[j].KeyName + L"\" ");
        infoConsole->AppendText(L"\"" + wxGetApp().bsp->BSP_Entities.Entities[i].KeyArray[j].KeyValue + L"\"\n");
    }
    infoConsole->AppendText(L"\n");

}

void QuME_Frame::OnSpinTextureExamineChange(wxSpinEvent& event)
{
    wxInt32 i = spinTextureExamine->GetValue();
    QuME_BSP_Texture* tex = &wxGetApp().bsp->BSP_Textures.TextureInfo[i];
    if(tex != nullptr)
    {
        infoConsole->AppendText(L"*-----------------------------*\n");
        infoConsole->AppendText(L"Texture Name: " + tex->MaterialName + L"\n");
        infoConsole->AppendText(L"Resolution: X = " + std::to_wstring(tex->xRes));
        infoConsole->AppendText(L" Y = " + std::to_wstring(tex->yRes) + L"\n");
        infoConsole->AppendText(L"Flags: " + std::to_wstring(tex->Flags) + L"\n");
        infoConsole->AppendText(L"Value: " + std::to_wstring(tex->Value) + L"\n\n");
    }
}

//method to allow threads to write to console window safely
void QuME_Frame::OnConsoleLogEvent(wxThreadEvent& event)
{
    infoConsole->AppendText(event.GetString());
}
