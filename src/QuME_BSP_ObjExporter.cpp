#include "QuME_BSP_ObjExporter.h"

QuME_BSP_ObjExporter::QuME_BSP_ObjExporter(QuME_Frame* frame,
                                           QuME_BSP_Data* BSPData,
                                           const wxString& BaseFileName,
                                           const wxString& FullPath)
                                           : wxThread()
{
    Frame = frame;
    Data = BSPData;
    StrippedFileName = BaseFileName;
    FullPathNoExt = FullPath;
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

    wxCriticalSectionLocker lock(*Frame->GetBSPProcessingCritSec());

    //Pointer to BSP Texture array
    QuME_BSP_Texture* tex = Data->Textures.Texture;

    QuME_Vector* Vertexes = Data->Vertices.VertexArray;

    QuME_UniqueStrings UniqueMaterialNames;

    //Count how many total items we will export
    wxUint32 ItemCount = Data->Textures.Count;
    ItemCount += Data->Vertices.ArrayCount;
    ItemCount += Data->TextureUVs.UVCount;
    for(wxUint32 i = 0; i < Data->BrushModels.Count; i++)
    {
        ItemCount += Data->BrushModels.BrushModel[i].NumFaces;
    }
    wxUint32 CurrentItemCount = 0;

    //Write .mtl file first
    wxFileOutputStream* outmtlfile = new wxFileOutputStream(this->FullPathNoExt + ".mtl");
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

    logevent.SetString("Exporting Materials to " + this->FullPathNoExt + ".mtl\n");
    wxQueueEvent(Frame, logevent.Clone());


    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->Textures.Count)); i++)
    {
        CurrentItemCount++;

        //Message Main thread about progress
        event.SetInt(CurrentItemCount * 99 / ItemCount);
        wxQueueEvent(Frame, event.Clone());

        wxString MatName = tex[i].MaterialName;

        if(UniqueMaterialNames.IsUnique(MatName)) //only write out materials that haven't been written out before
        {
            //remove any slashes, replace with dollar sign
            MatName.Replace("/", "$", true);

            //Write material entry
            *outmtl << "newmtl " << MatName << "\n";
            *outmtl << "Ka 1.000 1.000 1.000\n";
            *outmtl << "Kd 1.000 1.000 1.000\n";
            *outmtl << "Ks 0.000 0.000 0.000\n";
            *outmtl << "Ni 1\n";
            *outmtl << "Ns 100\n";
            *outmtl << "Tf 1 1 1\n";
            *outmtl << "d 1.0\n";
            *outmtl << "map_Ka " << Data->BaseDir << "textures/" << tex[i].MaterialName << ".jpg\n";
            *outmtl << "map_Kd " << Data->BaseDir << "textures/" << tex[i].MaterialName << ".jpg\n\n";
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

    logevent.SetString("Exporting Vertices to " + this->FullPathNoExt + ".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //reference material library
    *outstr << "mtllib " << this->StrippedFileName << ".mtl\n";

    //dump all vertices to file first
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->Vertices.ArrayCount)); i++)
    {
        CurrentItemCount++;

        //Message Main thread about progress
        event.SetInt(CurrentItemCount * 99 / ItemCount);

        wxQueueEvent( Frame, event.Clone() );

        *outstr << "v " << Vertexes[i].x * -QUME_EXPORTSCALEFACTOR;
        *outstr << " " << Vertexes[i].z * QUME_EXPORTSCALEFACTOR;
        *outstr << " " << Vertexes[i].y * QUME_EXPORTSCALEFACTOR << "\n";
    }

    *outstr << "\n";

    logevent.SetString("Exporting UV Coordinates to " + this->FullPathNoExt + ".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //now dump all of the UV coordinates
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->TextureUVs.UVCount)); i++)
    {
        CurrentItemCount++;

        //Message Main thread about progress
        event.SetInt(CurrentItemCount * 99 / ItemCount);
        wxQueueEvent(Frame, event.Clone());
        *outstr << "vt " << Data->TextureUVs.UArray[i] << " " << Data->TextureUVs.VArray[i] << "\n";
    }

    *outstr << "\n";

    logevent.SetString("Exporting Objects to " + this->FullPathNoExt + ".obj\n");
    wxQueueEvent(Frame, logevent.Clone());

    //loop through all the map's brush models and export one by one
    for(wxUint32 i = 0; ((!FileError) && (!Frame->ObjExportCanceled()) && (!TestDestroy()) && (i < Data->BrushModels.Count)); i++)
    {
        QuME_BSP_BrushModel* BrushModel = &Data->BrushModels.BrushModel[i];
        wxInt32 FirstFace = BrushModel->FirstFace;

        logevent.SetString("Exporting Object #" + std::to_string(i) + "\n");
        wxQueueEvent(Frame, logevent.Clone());


        if(i > 0)
        {
            *outstr << "o " << "BrushModel" << "$" << std::to_wstring(i) << "\n";
        }

        //loop though all of the current brush model's faces
        for(wxInt32 j = 0; j < BrushModel->NumFaces; j++)
        {
            CurrentItemCount++;

            //Message Main thread about progress
            event.SetInt( CurrentItemCount * 99 / ItemCount );
            wxQueueEvent( Frame, event.Clone() );

            QuME_BSP_Face* CurrentFace = &Data->Faces.Face[FirstFace + j];
            wxUint32 TexIndex = CurrentFace->TextureInfo;

            wxString MatName = tex[TexIndex].MaterialName;
            MatName.Replace("/", "$", true);

            if(i == 0)
            {
                *outstr << "o " << "WorldGeometry" << "$" << std::to_wstring(TexIndex) << "\n";
            }

            *outstr << "usemtl " << MatName << "\n";

            for(wxUint32 k = 0; k < CurrentFace->TriangleCount; k++)
            {
                QuME_BSP_Triangle* t = CurrentFace->Triangle;
                *outstr << "f ";
                for(wxUint32 l = 0; l < 3; l++)
                {
                    *outstr << t[k].v[2 - l] + 1 << "/" << t[k].uv[2 - l] + 1 << " ";
                }
                *outstr << "\n";
            }
        }
        *outstr << "\n";
    }

    logevent.SetString("Export Done!\n\n");
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
