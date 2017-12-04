#include "QuME_BSP_Load.h"

QuME_BSP_Load::QuME_BSP_Load(QuME_Frame* frame, QuME_BSP_Data* BSPData, const wxString& FileName)
{
    Frame = frame;
    Data = BSPData;
    BSPFileName = FileName;
    FileError = false;
}

QuME_BSP_Load::~QuME_BSP_Load()
{
    wxCriticalSectionLocker locker(wxGetApp().CritSect);

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
    event.SetString("Loading \"" + BSPFileName + "\"\n\n");
    wxQueueEvent(Frame, event.Clone());
    FileError = false;

    wxFileInputStream input_stream(BSPFileName);

    FileError = !input_stream.IsOk();

    wxCriticalSectionLocker lock(*Frame->GetBSPProcessingCritSec());
    wxCriticalSectionLocker lock2(*Frame->GetImportCritSec());

    //ternary operator "?:" used to select correct string response
    event.SetString(FileError ? "File Open Error!\n" : "Loading Header...\n");
    //now post that response in thread-safe manner to our main thread
    wxQueueEvent(Frame, event.Clone());

    //this if(!FileError) structure is used to chain loading sections based on the previous result of last file operation
    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadHeader(&input_stream);
        event.SetString(FileError ? "Header Load Error!\n" : "Loading Lump Table...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadLumpTable(&input_stream);
        event.SetString(FileError ? "Lump Table Load Error!\n" : "Loading Entities...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadEntities(&input_stream);
        event.SetString(FileError ? "Entities Load Error!\n" : "Loading Vertices...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadVertices(&input_stream);
        event.SetString(FileError ? "Vertices Load Error!\n" : "Loading Faces...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadFaces(&input_stream);
        event.SetString(FileError ? "Faces Load Error!\n" : "Loading Surface Edges...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadSurfaceEdges(&input_stream);
        event.SetString(FileError ? "Surface Edges Load Error!\n" : "Loading Edges...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadEdges(&input_stream);
        event.SetString(FileError ? "Edges Load Error!\n" : "Loading Texture Info...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadTextures(&input_stream);
        event.SetString(FileError ?
                        "Texture Info Load Error!\nError at file: " + Data->Textures.FaultTexture + "\n\n"
                        : "Loading BSP Models...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadBrushModels(&input_stream);
        event.SetString(FileError ? "BSP Models Load Error!\n" : "Loading Plane Data...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadPlanes(&input_stream);
        event.SetString(FileError ? "BSP Planes Load Error!\n" : "Loading Brushes...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadBrushes(&input_stream);
        event.SetString(FileError ? "Brush Load Error!\n" : "Loading Brush Sides...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        FileError = !Data->LoadBrushSides(&input_stream);
        event.SetString(FileError ? "Brush Side Load Error!\n" : "Setting up BSP data...\n");
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        Data->SetupBSPData(Frame);
    }
    else
    {
        //if error, close progress dialog
        wxThreadEvent event(wxEVT_THREAD, BSPPROCESSING_EVENT);
        event.SetInt(-1);
        wxQueueEvent(Frame, event.Clone());
    }

    if((!FileError) && (!Frame->BSPImportCanceled()) && (!TestDestroy()))
    {
        wxThreadEvent event(wxEVT_THREAD, BSPPROCESSING_EVENT);
        event.SetInt(-1);
        event.SetString("Processing Brushes...\n");
        wxQueueEvent(Frame, event.Clone());
        Data->ProcessBrushes(Frame);
    }

    //this message is used to flag success/failure in the event listener in the main thread
    if(FileError)
    {
        event.SetString("Load Error!\n\n");
    }
    else if(Frame->BSPImportCanceled() || TestDestroy())
    {
        event.SetString("Load Canceled!\n\n");
    }
    else
    {
        event.SetString("File Loaded!\n\n");
    }
    wxQueueEvent(Frame, event.Clone());

    return nullptr;
}
