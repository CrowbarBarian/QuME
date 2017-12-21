#include "QuME_BSP_ObjExporter.h"

QuME_BSP_ObjExporter::QuME_BSP_ObjExporter(QuME_Frame* frame,
                                           QuME_BSP_Data* BSPData,
                                           const std::wstring& BaseFileName,
                                           const std::wstring& FullPath)
                                           : wxThread(), StrippedFileName(BaseFileName), FullPathNoExt(FullPath)
{
    Frame = frame;
    Data = BSPData;
    FileError = false;
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

    wxCriticalSectionLocker lock(Frame->CritSecBSP);

    //Pointer to BSP Texture array
    QuME_BSP_Texture* tex = Data->Textures.Texture;

    QuME_Vector* Vertexes = Data->Vertices.VertexArray;

    QuME_UniqueStrings UniqueMaterialNames;

    //Count how many total items we will export
    wxInt32 ItemCount = Data->Textures.Count;
    ItemCount += Data->Vertices.VertexCount;
    ItemCount += Data->TextureUVs.UVArrayCount;
    for(wxUint32 i = 0; i < Data->BrushModels.Count; i++)
    {
        ItemCount += Data->BrushModels.BrushModel[i].NumFaces;
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

    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->Textures.Count)); i++)
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

        std::wstring MatName = tex[i].MaterialName;

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
            *outmtl << L"map_Ka " << Data->baseDir << L"textures/" << tex[i].MaterialName << L".jpg\n";
            *outmtl << L"map_Kd " << Data->baseDir << L"textures/" << tex[i].MaterialName << L".jpg\n\n";
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
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->Vertices.VertexCount)); i++)
    {
        CurrentItemCount++;
        //std::cout << CurrentItemCount << std::endl;

        //Message Main thread about progress
        wxInt32 currentProgress = CurrentItemCount * 100 / ItemCount;
        if(currentProgress != updateOld)
		{
			event.SetInt(currentProgress);
			wxQueueEvent(Frame, event.Clone());
			updateOld = currentProgress;
		}

        *outstr << L"v " << Vertexes[i].x * -QUME_EXPORTSCALEFACTOR;
        *outstr << L" " << Vertexes[i].z * QUME_EXPORTSCALEFACTOR;
        *outstr << L" " << Vertexes[i].y * QUME_EXPORTSCALEFACTOR << L"\n";
    }

    *outstr << L"\n";

    logevent.SetString(L"Exporting UV Coordinates to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //now dump all of the UV coordinates
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->TextureUVs.UVArrayCount)); i++)
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

        *outstr << L"vt " << Data->TextureUVs.UVArray[i].U << " " << Data->TextureUVs.UVArray[i].V << "\n";
    }

    *outstr << L"\n";

    logevent.SetString(L"Exporting Objects to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //loop through all the map's brush models and export one by one
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->BrushModels.Count)); i++)
    {
        QuME_BSP_BrushModel* BrushModel = &Data->BrushModels.BrushModel[i];
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

            QuME_BSP_Face* CurrentFace = &Data->Faces.Face[FirstFace + j];
            wxUint32 TexIndex = CurrentFace->TextureInfo;

            std::wstring MatName = tex[TexIndex].MaterialName;
            ReplaceAll(&MatName, L"/", L"$");
            //MatName.Replace(L"/", L"$", true);

            if(i == 0)
            {
                *outstr << L"o " << L"WorldGeometry" << L"$" << std::to_wstring(TexIndex) << L"\n";
            }

            *outstr << L"usemtl " << MatName << L"\n";

            for(wxUint32 k = 0; k < CurrentFace->TriangleCount; k++)
            {
                QuME_BSP_Triangle* t = CurrentFace->Triangle;
                *outstr << L"f ";
                for(wxUint32 l = 0; l < 3; l++)
                {
                    *outstr << t[k].v[2 - l] + 1 << "/" << t[k].uv[2 - l] + 1 << " ";
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
