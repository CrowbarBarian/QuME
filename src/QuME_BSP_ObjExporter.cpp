/***************************************************************
 * Name:		QuME_BSP_ObjExporter.cpp
 * Purpose:		Class to export processed BSP geometry
 * Author:    J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright: J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:   GPL v3
 **************************************************************/

#include "QuME_BSP_ObjExporter.h"

QuME_BSP_ObjExporter::QuME_BSP_ObjExporter(QuME_Frame* frame,
                                           QuME_BSP_Data* BSPData,
                                           const std::wstring& BaseFileName,
                                           const std::wstring& FullPath)
                                           : wxThread(), StrippedFileName(BaseFileName), FullPathNoExt(FullPath)
{
    Frame = frame;
    bsp = BSPData;
    FileError = false;
}

QuME_BSP_ObjExporter::~QuME_BSP_ObjExporter()
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

void QuME_BSP_ObjExporter::OnExit()
{
}

//this function exports BSP data into Wavefront's .obj format
wxThread::ExitCode QuME_BSP_ObjExporter::Entry()
{
    //flag used to bypass loops if an error happens
    FileError = false;

    wxThreadEvent event(wxEVT_THREAD, OBJEXPORT_EVENT);
    wxThreadEvent logevent(wxEVT_THREAD, CONSOLELOG_EVENT);

    wxCriticalSectionLocker lock(wxGetApp().CritSecBSP);


    QuME_UniqueStrings UniqueMaterialNames;

    //Count how many total items we will export
    wxInt32 ItemCount = bsp->BSP_Textures.TextureInfo.Count;
    ItemCount += bsp->BSP_Vertices.Vertices.Count;
    ItemCount += bsp->BSP_TextureUVs.UVs.Count;
    for(wxUint32 i = 0; i < bsp->BSP_BrushModels.BrushModels.Count; i++)
    {
        ItemCount += bsp->BSP_BrushModels.BrushModels[i].NumFaces;
    }
    wxInt32 CurrentItemCount = 0;

    //Write .mtl file first
    wxFileOutputStream* outmtlfile = new wxFileOutputStream(this->FullPathNoExt + L".mtl");
    if (!outmtlfile->IsOk())
    {
        if(outmtlfile != nullptr)
        {
            outmtlfile->Close();
            delete outmtlfile;
        }
        FileError = true;
    }
    wxTextOutputStream* outmtl = new wxTextOutputStream(*outmtlfile);

    logevent.SetString(L"Exporting Materials to " + this->FullPathNoExt + L".mtl\n");
    wxQueueEvent(Frame, logevent.Clone());

    wxInt32 updateOld = 0;

    for(wxUint32 i = 0; ((!FileError) && (!TestDestroy()) && (i < bsp->BSP_Textures.TextureInfo.Count)); i++)
    {
        CurrentItemCount++;

        //Message Main thread about progress
        wxInt32 currentProgress = CurrentItemCount * 100 / ItemCount;
        if(currentProgress != updateOld)
		{
			event.SetInt(currentProgress);
			wxQueueEvent(Frame, event.Clone());
			updateOld = currentProgress;
		}

        std::wstring MatName = bsp->BSP_Textures.TextureInfo[i].MaterialName;

        if(UniqueMaterialNames.IsUnique(MatName)) //only write out materials that haven't been written out before
        {
            //remove any slashes, replace with dollar sign
            ReplaceAll(&MatName, L"/", L"$");

            //Write material entry
            *outmtl << L"newmtl " << MatName << L"\n";
            *outmtl << L"Ka 1.000 1.000 1.000\n";
            *outmtl << L"Kd 1.000 1.000 1.000\n";
            *outmtl << L"Ks 0.000 0.000 0.000\n";
            *outmtl << L"Ni 1\n";
            *outmtl << L"Ns 100\n";
            *outmtl << L"Tf 1 1 1\n";
            *outmtl << L"d 1.0\n";
            *outmtl << L"map_Ka " << bsp->baseDir << L"textures/" << bsp->BSP_Textures.TextureInfo[i].MaterialName << L".jpg\n";
            *outmtl << L"map_Kd " << bsp->baseDir << L"textures/" << bsp->BSP_Textures.TextureInfo[i].MaterialName << L".jpg\n\n";
        }
    }

    //output .obj file
    wxFileOutputStream* outfile = new wxFileOutputStream(this->FullPathNoExt + ".obj");

    if (!outfile->IsOk())
    {
        if(outfile != nullptr)
        {
            outfile->Close();
            delete outfile;
        }
        FileError = true;
    }
    wxTextOutputStream* outstr = new wxTextOutputStream(*outfile);

    logevent.SetString(L"Exporting Vertices to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //reference material library
    *outstr << L"mtllib " << this->StrippedFileName << L".mtl\n";

    //dump all vertices to file first
    for(wxUint32 i = 0; ((!FileError) && (!TestDestroy()) && (i < bsp->BSP_Vertices.Vertices.Count)); i++)
    {
        CurrentItemCount++;

        //Message Main thread about progress
        wxInt32 currentProgress = CurrentItemCount * 100 / ItemCount;
        if(currentProgress != updateOld)
		{
			event.SetInt(currentProgress);
			wxQueueEvent(Frame, event.Clone());
			updateOld = currentProgress;
		}

        *outstr << L"v " << bsp->BSP_Vertices.Vertices[i].x * -QUME_EXPORTSCALEFACTOR;
        *outstr << L" " << bsp->BSP_Vertices.Vertices[i].z * QUME_EXPORTSCALEFACTOR;
        *outstr << L" " << bsp->BSP_Vertices.Vertices[i].y * QUME_EXPORTSCALEFACTOR << L"\n";
    }

    *outstr << L"\n";

    logevent.SetString(L"Exporting UV Coordinates to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //now dump all of the UV coordinates
    for(wxUint32 i = 0; ((!FileError) && (!TestDestroy()) && (i < bsp->BSP_TextureUVs.UVs.Count)); i++)
    {
        CurrentItemCount++;

        //Message Main thread about progress
        wxInt32 currentProgress = CurrentItemCount * 100 / ItemCount;
        if(currentProgress != updateOld)
		{
			event.SetInt(currentProgress);
			wxQueueEvent(Frame, event.Clone());
			updateOld = currentProgress;
		}

        *outstr << L"vt " << bsp->BSP_TextureUVs.UVs[i].U << " " << bsp->BSP_TextureUVs.UVs[i].V << "\n";
    }

    *outstr << L"\n";

    logevent.SetString(L"Exporting Objects to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //loop through all the map's brush models and export one by one
    for(wxUint32 i = 0; ((!FileError) && (!TestDestroy()) && (i < bsp->BSP_BrushModels.BrushModels.Count)); i++)
    {
        QuME_BSP_BrushModel* BrushModel = &bsp->BSP_BrushModels.BrushModels[i];
        wxInt32 FirstFace = BrushModel->FirstFace;

        logevent.SetString(L"Exporting Object #" + std::to_string(i) + L"\n");
        wxQueueEvent(Frame, logevent.Clone());


        if(i > 0)
        {
            *outstr << L"o " << L"BrushModel" << L"$" << std::to_wstring(i) << L"\n";
        }

        //loop though all of the current brush model's faces
        for(wxInt32 j = 0; j < BrushModel->NumFaces; j++)
        {
            CurrentItemCount++;

            //Message Main thread about progress
			wxInt32 currentProgress = CurrentItemCount * 100 / ItemCount;
			if(currentProgress != updateOld)
			{
				event.SetInt(currentProgress);
				wxQueueEvent(Frame, event.Clone());
				updateOld = currentProgress;
			}

            QuME_BSP_Face* CurrentFace = &bsp->BSP_Faces.Faces[FirstFace + j];
            wxUint32 TexIndex = CurrentFace->TextureInfo;

            std::wstring MatName = bsp->BSP_Textures.TextureInfo[TexIndex].MaterialName;
            ReplaceAll(&MatName, L"/", L"$");

            if(i == 0)
            {
                *outstr << L"o " << L"WorldGeometry" << L"$" << std::to_wstring(TexIndex) << L"\n";
            }

            *outstr << L"usemtl " << MatName << L"\n";

            for(wxUint32 k = 0; k < CurrentFace->Triangles.Count; k++)
            {
                QuME_Triangle tri = CurrentFace->Triangles[k];
                *outstr << L"f ";
                for(wxUint32 l = 0; l < 3; l++)
                {
                	*outstr << tri.Vertex[2 - l].VertexIndex + 1 << "/" << tri.Vertex[2 - l].UVIndex + 1 << L" ";
                }
                *outstr << L"\n";
            }
        }
        *outstr << L"\n";
    }

    logevent.SetString(L"Export Done!\n\n");
    wxQueueEvent(Frame, logevent.Clone());

    event.SetInt(FileError ? -2 : -1);
    wxQueueEvent(Frame, event.Clone());

    //clean up .mtl file handles
    outmtl->Flush();
    delete outmtl;
    outmtlfile->Close();
    delete outmtlfile;

    //clean up .obj file handles
    outstr->Flush();
    delete outstr;
    outfile->Close();
    delete outfile;

    return nullptr;
}
