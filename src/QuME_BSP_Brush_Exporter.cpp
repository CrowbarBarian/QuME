#include "QuME_BSP_Brush_Exporter.h"

QuME_BSP_Brush_Exporter::QuME_BSP_Brush_Exporter(QuME_Frame* frame,
                                           QuME_BSP_Data* BSPData,
                                           const std::wstring& BaseFileName,
                                           const std::wstring& FullPath)
                                           : wxThread(), StrippedFileName(BaseFileName), FullPathNoExt(FullPath)
{
    Frame = frame;
    Data = BSPData;
    FileError = false;
}

QuME_BSP_Brush_Exporter::~QuME_BSP_Brush_Exporter()
{
}

void QuME_BSP_Brush_Exporter::OnExit()
{
}

//this function exports BSP brush data into Wavefront's .obj format
wxThread::ExitCode QuME_BSP_Brush_Exporter::Entry()
{
    //flag used to bypass loops if an error happens
    FileError = false;

    wxThreadEvent event(wxEVT_THREAD, BRUSHEXPORT_EVENT);
    wxThreadEvent logevent(wxEVT_THREAD, CONSOLELOG_EVENT);

    wxCriticalSectionLocker lock(Frame->CritSecBSP);

    //Pointer to BSP Texture array
    QuME_BSP_Texture* tex = Data->Textures.Texture;

    //our list of material names for export
    QuME_UniqueStrings UniqueMaterialNames;

    //Count how many total items we will export
    wxInt32 ItemCount = Data->Textures.Count;
    ItemCount += Data->BrushSideUVs.UVArrayCount;
    ItemCount += Data->BrushVertices.Count;
    for(wxUint32 i = 0; i < Data->Brushes.Count; i++)
    {
        ItemCount += Data->Brushes.Brush[i].SideCount;
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

    *outstr << L"# Bounding box: " << Data->BrushBoundingBox.forExport() << L"\n";

	*outstr << L"# Vertex Count: " << Data->BrushVertices.Count << L"\n";

    //reference material library
    *outstr << L"mtllib " << this->StrippedFileName << L".mtl\n";

    //dump all vertices to file first
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->BrushVertices.Count)); i++)
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

        //*outstr << L"v " << Data->BrushVertices.Vertices[i].x * -QUME_EXPORTSCALEFACTOR;
        //*outstr << L" " << Data->BrushVertices.Vertices[i].z * QUME_EXPORTSCALEFACTOR;
        //*outstr << L" " << Data->BrushVertices.Vertices[i].y * QUME_EXPORTSCALEFACTOR << L"\n";

        *outstr << L"v " << Data->BrushVertices.Vertices[i].forExport() << L"\n";
    }

    *outstr << L"\n";

    logevent.SetString(L"Exporting UV Coordinates to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    *outstr << "# UV Count: " << Data->BrushSideUVs.UVArrayCount << "\n";
    //now dump all of the UV coordinates
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->BrushSideUVs.UVArrayCount)); i++)
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

        *outstr << L"vt " << Data->BrushSideUVs.UVArray[i].U << " " << Data->BrushSideUVs.UVArray[i].V << L"\n";
    }

    *outstr << L"\n";

    logevent.SetString(L"Exporting Objects to " + this->FullPathNoExt + L".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //loop through all the map's brush models and export one by one
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->Brushes.Count)); i++)
    {
        QuME_BSP_Brush* CurrentBrush = &Data->Brushes.Brush[i];
        //wxInt32 FirstSide = CurrentBrush->FirstBrushSide;

        logevent.SetString(L"Exporting Object #" + std::to_string(i) + L"\n");
        wxQueueEvent(Frame, logevent.Clone());


	    *outstr << L"o " << L"Brush" << L"$" << std::to_wstring(i) << L"\n";

	    *outstr << "# Brush Bounding box: " << CurrentBrush->Bounds.forExport() << "\n";

		//loop though all of the current brush model's faces
		for(wxUint32 j = CurrentBrush->FirstBrushSide; j < CurrentBrush->FirstBrushSide + CurrentBrush->SideCount; j++)
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

			QuME_BSP_BrushSide* CurrentSide = &Data->BrushSides.BrushSideArray.Data[j];
			wxInt32 TexIndex = CurrentSide->TextureIndex;

			if(CurrentSide->VertexIndexArrayCount > 2)
			{
				if(TexIndex >=0)
				{
					std::wstring MatName = tex[TexIndex].MaterialName;
					ReplaceAll(&MatName, L"/", L"$");

					*outstr << L"usemtl " << MatName << L"\n";

					*outstr << L"f ";
					for(wxUint32 k = 0; k < CurrentSide->VertexIndexArrayCount; k++)
					{
						*outstr << CurrentSide->VertexIndexArray[k] + 1 << "/";
						*outstr << CurrentSide->UVIndexArray[k] + 1 << " ";
					}
					*outstr << L"\n";
				}
				else
				{
					*outstr << L"usemtl none\nf ";
					for(wxUint32 k = 0; k < CurrentSide->VertexIndexArrayCount; k++)
					{
						*outstr << CurrentSide->VertexIndexArray[k] + 1 << "/1 ";
					}
					*outstr << L"\n";
				}
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
