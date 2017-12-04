#include "QuME_BSP_PostLoadProcessor.h"

QuME_BSP_PostLoadProcessor::QuME_BSP_PostLoadProcessor(QuME_Frame* frame, QuME_BSP_Data* BSPData) : wxThread()
{
    this->Frame = frame;
    this->Data = BSPData;
}

QuME_BSP_PostLoadProcessor::~QuME_BSP_PostLoadProcessor()
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

void QuME_BSP_PostLoadProcessor::OnExit()
{
}
wxThread::ExitCode QuME_BSP_PostLoadProcessor::Entry()
{
    wxThreadEvent event(wxEVT_THREAD, BSPPROCESSING_EVENT);
    event.SetInt(0);
    wxQueueEvent(this->Frame, event.Clone());

    wxUint32 UVIndex = 0;
    wxInt32* SEdges = Data->SurfaceEdges.SurfaceEdge;
    QuME_BSP_Edge* BSPEdges = Data->Edges.Edge;
    QuME_BSP_Texture* Tex = Data->Textures.Texture;
    QuME_Vector* Vertexes = Data->Vertices.Vertex;

    for(wxUint32 i = 0; i < Data->Faces.Count; i++)
    {
        //A simple progress update message
        if((i % (Data->Faces.Count / 100)) == 0)
        {
            event.SetInt(i * 99 / Data->Faces.Count);
            wxQueueEvent(this->Frame, event.Clone());
        }

        QuME_BSP_Face* Face = &Data->Faces.Face[i];
        wxUint32 TexIndex = Face->TextureInfo;
        wxInt32 FirstEdge = Face->FirstEdge;
        Face->VertexArray = new wxUint32[Face->EdgeCount];
        Face->TextureUV =  new wxUint32[Face->EdgeCount];
        for(wxInt32 j = 0; j < Face->EdgeCount; j++)
        {
            wxInt32 FIndex = SEdges[FirstEdge + j];
            wxUint16 VIndex = ((FIndex > 0) ? BSPEdges[FIndex].a : BSPEdges[-FIndex].b);
            Face->VertexArray[j] = VIndex;

            //Calculate UV indexes
            wxFloat32 U = Vertexes[VIndex].dot(Tex[TexIndex].uAxis) + Tex[TexIndex].uOffset;
            U /= static_cast<wxFloat32>(Tex[TexIndex].xRes); //normalize U coordinate
            wxFloat32 V = Vertexes[VIndex].dot(Tex[TexIndex].vAxis) + Tex[TexIndex].vOffset;
            V /= -(static_cast<wxFloat32>(Tex[TexIndex].yRes)); //Normalize V coordinate
            Data->TextureUVs.AddUVs(U, V); //Add these coordinates to list
            Face->TextureUV[j] = UVIndex++; //set to current index, then increment
        }
        Face->Triangulate();
    }

    Data->TextureUVs.ListToArray(); //turn the UV linked list into an array once all UVs have been calculated

    //signal main thread we're done
    event.SetInt(-1);
    wxQueueEvent(this->Frame, event.Clone());

    return nullptr;
}
