/***************************************************************
 * Name:		QuME_BSP_Load.cpp
 * Purpose:		Class to load and process all the data in a BSP file
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_Load.h"

QuME_BSP_Load::QuME_BSP_Load(QuME_Frame* frame,
							QuME_BSP_Data* BSPData,
							const std::wstring& FileName)
							:BSPFileName(FileName)
{
    Frame = frame;
    bsp = BSPData;
    FileError = false;
}

QuME_BSP_Load::~QuME_BSP_Load()
{
    wxCriticalSectionLocker locker(wxGetApp().CritSectThreads);

    wxArrayThread& threads = wxGetApp().Threads;
    threads.Remove(this);

    if ( threads.IsEmpty() )
    {
        // signal the main thread that there are no more threads left if it is
        // waiting for us
        if ( wxGetApp().ShuttingDown )
        {
            wxGetApp().ShuttingDown = false;

            wxGetApp().AllDone.Post();
        }
    }
}
void QuME_BSP_Load::OnExit()
{
}

//this thread is responsible for loading all of the data we use from a BSP file
wxThread::ExitCode QuME_BSP_Load::Entry()
{
    wxThreadEvent event(wxEVT_THREAD, BSPIMPORT_EVENT);
    event.SetString(L"Loading \"" + BSPFileName + L"\"\n\n");
    wxQueueEvent(Frame, event.Clone());
    FileError = false;

    wxFileInputStream input_stream(BSPFileName);

    FileError = !input_stream.IsOk();

    wxCriticalSectionLocker lock(wxGetApp().CritSecBSP);

    //ternary operator "?:" used to select correct string response
    event.SetString(FileError ? L"File Open Error!\n" : L"Loading Header...\n");
    //now post that response in thread-safe manner to our main thread
    wxQueueEvent(Frame, event.Clone());

    //this if(!FileError) structure is used to chain loading sections based on the previous result of last file operation
    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadHeader(&input_stream);
        event.SetString(FileError ? L"Header Load Error!\n" : L"Loading Lump Table...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadLumpTable(&input_stream);
        event.SetString(FileError ? L"Lump Table Load Error!\n" : L"Loading Entities...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadEntities(&input_stream);
        event.SetString(FileError ? L"Entities Load Error!\n" : L"Loading Vertices...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadVertices(&input_stream);
        event.SetString(FileError ? L"Vertices Load Error!\n" : L"Loading Faces...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadFaces(&input_stream);
        event.SetString(FileError ? L"Faces Load Error!\n" : L"Loading Surface Edges...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadSurfaceEdges(&input_stream);
        event.SetString(FileError ? L"Surface Edges Load Error!\n" : L"Loading Edges...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadEdges(&input_stream);
        event.SetString(FileError ? L"Edges Load Error!\n" : L"Loading Texture Info...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadTextures(&input_stream);
        event.SetString(FileError ?
                        L"Texture Info Load Error!\nError at file: " + bsp->BSP_Textures.FaultTexture + L"\n\n"
                        : L"Loading BSP Models...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadBrushModels(&input_stream);
        event.SetString(FileError ? L"BSP Models Load Error!\n" : L"Loading Plane Data...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadPlanes(&input_stream);
        event.SetString(FileError ? L"BSP Planes Load Error!\n" : L"Loading Brushes...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadBrushes(&input_stream);
        event.SetString(FileError ? L"Brush Load Error!\n" : L"Loading Brush Sides...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        FileError = !bsp->LoadBrushSides(&input_stream);
        event.SetString(FileError ? L"Brush Side Load Error!\n" : L"Setting up BSP data...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!TestDestroy()))
    {
        bsp->SetupBSPData(Frame);
    }

    if((!FileError) && (!TestDestroy()))
    {
        event.SetString(L"Processing Brushes...\n");
        wxQueueEvent(Frame, event.Clone());
        bsp->ProcessBrushes(Frame);
    }

    //this message is used to flag success/failure in the event listener in the main thread
    if(FileError)
    {
        event.SetString(L"Load Error!\n\n");
    }
    else if(TestDestroy())
    {
        event.SetString(L"Load Canceled!\n\n");
    }
    else
    {
        event.SetString(L"File Loaded!\n\n");
    }
    wxQueueEvent(Frame, event.Clone());

    return nullptr;
}
