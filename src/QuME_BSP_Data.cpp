/***************************************************************
 * Name:      QuME_Main.cpp
 * Purpose:   Code for BSP Data Type Handler
 * Author:    J Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:   2017-06-13
 * Copyright: J Thomas (Crowbarbarian) ()
 * License:   GPL v3
 **************************************************************/
#include "QuME_BSP_Data.h"
#include "QuME_Main.h"

QuME_BSP_Data::QuME_BSP_Data()
{
    this->IBSPMagic = 0;
    this->BSPVersion = 0;
}

QuME_BSP_Data::~QuME_BSP_Data()
{
}

bool QuME_BSP_Data::LoadHeader(wxFileInputStream* infile)
{
    wxDataInputStream binData( *infile );
    if (infile->SeekI(0, wxFromStart) == wxInvalidOffset)
    {
        return false;
    }
    this->IBSPMagic = binData.Read32();

    if(!binData.IsOk())
    {
        return false;
    }

    //"Magic" number 1347633737...Really 'IBSP' in little endian
    if (this->IBSPMagic != QUME_QUAKE_2_BSP_MAGIC)
    {
        return false;
    }
    this->BSPVersion = binData.Read32();
    if(!binData.IsOk())
    {
        return false;
    }
    return true;
}

wxUint32 QuME_BSP_Data::GetLumpTableOffset(wxUint32 version)
{
    wxUint32 i = 0;
    while(i < BSP_LUMP_VERSION_TABLE_MAX)
    {
        if(QuME_BSP_VersionTable[i].ver == version)
        {
            return QuME_BSP_VersionTable[i].lumpTableOffset;
        }
        i++;
    }
    return UNKNOWN_LUMP_TABLE;
}

//this function caches the vertex indexes for each face and calculates UVs
bool QuME_BSP_Data::SetupBSPData(QuME_Frame* Frame)
{
    wxThreadEvent event(wxEVT_THREAD, BSPPROCESSING_EVENT);
    event.SetInt(0);
    wxQueueEvent(Frame, event.Clone());

    wxUint32 UVIndex = 0;
    wxInt32* SEdges = this->SurfaceEdges.SurfaceEdge;
    QuME_BSP_Edge* BSPEdges = this->Edges.EdgeArray;
    QuME_BSP_Texture* Tex = this->Textures.Texture;
    QuME_Vector* Vertexes = this->Vertices.VertexArray;

    for(wxUint32 i = 0; i < this->Faces.Count; i++)
    {
        //A simple progress update message
        if((i % (this->Faces.Count / 100)) == 0)
        {
            event.SetInt(i * 99 / this->Faces.Count);
            wxQueueEvent(Frame, event.Clone());
        }

        QuME_BSP_Face* Face = &this->Faces.Face[i];
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
            wxFloat64 U = Vertexes[VIndex].dot(Tex[TexIndex].uAxis) + Tex[TexIndex].uOffset;
            U /= static_cast<wxFloat64>(Tex[TexIndex].xRes); //normalize U coordinate
            wxFloat64 V = Vertexes[VIndex].dot(Tex[TexIndex].vAxis) + Tex[TexIndex].vOffset;
            V /= -(static_cast<wxFloat64>(Tex[TexIndex].yRes)); //Normalize V coordinate
            this->TextureUVs.AddUVs(U, V); //Add these coordinates to list
            Face->TextureUV[j] = UVIndex++; //set to current index, then increment
        }
        Face->Triangulate();
    }

    this->TextureUVs.ListToArray(); //turn the UV linked list into an array once all UVs have been calculated

    //signal main thread we're done
    event.SetInt(-1);
    wxQueueEvent(Frame, event.Clone());

    return true;
}

LinkedList<wxUint32>* QuME_BSP_Data::QuickHull(wxUint32 Start, wxUint32 End, LinkedList<wxUint32>* Right, QuME_Vector& Normal)
{
    QuME_Vector* va = this->BrushVertices.VertexArray;
    QuME_Line L(va[Start], va[End]);
    LinkedList<wxUint32>* r = new LinkedList<wxUint32>(); //the return set of vertices
    if(Right->Count == 0)
    {
        return r; //empty list
    }

    LinkedList<wxUint32>* f = new LinkedList<wxUint32>();
    LinkedList<wxUint32>* s = new LinkedList<wxUint32>();

    wxUint32 furthest = Right->Head->Item;
    for(ListItem<wxUint32>* i = Right->Head; i != nullptr; i = i->next)
    {
        if(L.distanceto(va[furthest]) < L.distanceto(va[i->Item]))
        {
            furthest = i->Item;
        }
    }
    QuME_Vector BaseLineA = va[furthest] - va[Start];
    QuME_Vector BaseLineB = va[End] - va[furthest];
    for(ListItem<wxUint32>* i = Right->Head; i != nullptr; i = i->next)
    {
        if(i->Item == furthest) continue;
        QuME_Vector Test = va[i->Item] - va[Start];
        if(Test.cross(BaseLineA).dot(Normal) > 0.0)
        {
            f->Append(i->Item);
        }
        Test = va[i->Item] - va[furthest];
        if(Test.cross(BaseLineB).dot(Normal) > 0.0)
        {
            s->Append(i->Item);
        }
    }
    LinkedList<wxUint32>* a = QuickHull(Start, furthest, f, Normal);
    LinkedList<wxUint32>* b = QuickHull(furthest, End, s, Normal);
    r->Append(*a);
    r->Append(furthest);
    r->Append(*b);
    delete a;
    delete b;
    delete f;
    delete s;
    return r;
}

//call this only once all the BSP data is loaded
bool QuME_BSP_Data::ProcessBrushes(QuME_Frame* Frame)
{
    QuME_BSP_Plane* BSPPlane = this->Planes.BSPPlaneArray.Item;
    QuME_BSP_Brush* Brush = this->Brushes.Brush;
    QuME_BSP_BrushSide* BrushSide = this->BrushSides.BrushSideArray.Item;
//    QuME_BSP_Texture* Texture = this->Textures.Texture;

    wxThreadEvent event(wxEVT_THREAD, CONSOLELOG_EVENT);
    wxThreadEvent progress(wxEVT_THREAD, BSPPROCESSING_EVENT);

    event.SetString(L"Processing Brushes...\nNumber of brushes in map:" + std::to_wstring(this->Brushes.Count) + L"\n");
    wxQueueEvent(Frame, event.Clone());

    //iterate through all the brushes in the BSP file
    for(wxUint32 BrushIndex = 0; BrushIndex < this->Brushes.Count; BrushIndex++)
    //for(wxUint32 BrushIndex = 0; BrushIndex < 1; BrushIndex++)
    {
        if((BrushIndex % (this->Brushes.Count / 100)) == 0) //update progress every 100 brushes
        {
            progress.SetInt(BrushIndex * 100 / this->Brushes.Count);
            wxQueueEvent(Frame, progress.Clone());
        }


        QuME_BSP_Brush* CurrentBrush = &Brush[BrushIndex];

        wxUint32 BrushSideCount = CurrentBrush->SideCount;
        wxUint32 FirstBrushSide = CurrentBrush->FirstBrushSide;
        if(BrushSideCount < 4) continue; //skip invalid brushes

        LinkedList<wxUint32> tempPlanes;
        LinkedList<wxUint32> tempSides;

        for(wxUint32 i = FirstBrushSide; i < BrushSideCount + FirstBrushSide; i++)
        {
            if(tempPlanes.AddIfUnique(BrushSide[i].PlaneIndex))
            {
                tempSides.Append(i);
            }

        }

        if(tempSides.Count < 4) continue; //skip brush, it's invalid (too many repeated planes)

        for(ListItem<wxUint32>* i = tempSides.Head; i->next->next != nullptr; i = i->next)
        {
            QuME_Plane* A = &BSPPlane[BrushSide[i->Item].PlaneIndex].Plane;
            for(ListItem<wxUint32>* j = i->next; j->next != nullptr; j = j->next)
            {
                QuME_Plane* B = &BSPPlane[BrushSide[j->Item].PlaneIndex].Plane;
                for(ListItem<wxUint32>* k = j->next; k != nullptr; k = k->next)
                {
                    QuME_Plane* C = &BSPPlane[BrushSide[k->Item].PlaneIndex].Plane;

                    QuME_Vector CurrentVertex;
                    if(A->Intersect(*B, *C, CurrentVertex))
                    {
                        bool VertInside = true;
                        for(ListItem<wxUint32>* l = tempSides.Head; l != nullptr; l = l->next)
                        {
                            wxFloat64 OnSide = BSPPlane[l->Item].Plane.TestSide(CurrentVertex);
                            if(OnSide > 0.0)
                            {
                                VertInside = false;
                                break;
                            }
                        }

                        if(VertInside)
                        {
                            wxUint32 AddedIndex = this->BrushVertices.VertexList.Append(CurrentVertex);
                            CurrentBrush->Indices.AddIfUnique(AddedIndex);
                            BrushSide[i->Item].VertexIndexList.AddIfUnique(AddedIndex);
                            BrushSide[j->Item].VertexIndexList.AddIfUnique(AddedIndex);
                            BrushSide[k->Item].VertexIndexList.AddIfUnique(AddedIndex);
                        }
                    }
                }
            }
        }
/*
        //permute through all of this brush's sides to find intersections
        for(wxUint32 i = FirstBrushSide; i < BrushSideCount + FirstBrushSide - 2; i++) //select the first plane
        {
            QuME_Plane* A = &BSPPlane[BrushSide[i].PlaneIndex].Plane;
            for(wxUint32 j = i + 1; j < BrushSideCount + FirstBrushSide - 1; j++) //select the second plane
            {
                QuME_Plane* B = &BSPPlane[BrushSide[j].PlaneIndex].Plane;

                //LinkedList<wxUint32> VertsAB;
                for(wxUint32 k = j + 1; k < BrushSideCount + FirstBrushSide; k++)
                {

                    QuME_Plane* C = &BSPPlane[BrushSide[k].PlaneIndex].Plane;

                    QuME_Vector CurrentVertex;

                    if(A->Intersect(*B, *C, CurrentVertex))
                    {
                        //check to see if found vertex is on the inside the cluster of planes
                        bool VertInside = true;
                        for(wxUint32 p = 0; p < BrushSideCount; p++)
                        {
                            wxFloat64 OnSide = BSPPlane[BrushSide[FirstBrushSide + p].PlaneIndex].Plane.TestSide(CurrentVertex);
                            if(OnSide > 0.0)
                            {
                                VertInside = false;
                                break;
                            }
                        }

                        if(VertInside)
                        {
                            wxUint32 AddedIndex = this->BrushVertices.VertexList.Append(CurrentVertex);
                            //VertsAB.Append(AddedIndex);
                            CurrentBrush->Indices.Append(AddedIndex);
                            BrushSide[i].VertexIndexList.Append(AddedIndex);
                            BrushSide[j].VertexIndexList.Append(AddedIndex);
                            BrushSide[k].VertexIndexList.Append(AddedIndex);
                        }
                    }
                }
            }
        }*/
    }

    //convert linked list to array once we're done adding vertices
    SAFE_ARRAY_DELETE(this->BrushVertices.VertexArray);
    this->BrushVertices.VertexArray = this->BrushVertices.VertexList.ToArray(this->BrushVertices.ArrayCount);
    for(wxUint32 VertIdx = 0; VertIdx < this->BrushVertices.ArrayCount; VertIdx++)
    {
        //std::cout << "v " << this->BrushVertices.VertexArray[VertIdx] << "\n";
    }

    for(wxInt32 i = 0; i < static_cast<wxInt32>(this->BrushSides.BrushSideArray.Count); i++)
    {
        SAFE_ARRAY_DELETE(BrushSide[i].VertexIndexArray);
        BrushSide[i].VertexIndexArray = BrushSide[i].VertexIndexList.ToArray(BrushSide[i].VertexIndexArrayCount);
        BrushSide[i].Normal = Planes.BSPPlaneArray.Item[BrushSide[i].PlaneIndex].Plane.Normal;
    }

    for(wxUint32 BrushIndex = 0; BrushIndex < this->Brushes.Count; BrushIndex++)
    {
        //std::cout << L"o Brush_" << BrushIndex << L"\n";
        QuME_BSP_Brush* CurrentBrush = &this->Brushes.Brush[BrushIndex];
        wxUint32 BrushSideCount = CurrentBrush->SideCount;
        wxUint32 FirstBrushSide = CurrentBrush->FirstBrushSide;
        for(wxUint32 i = FirstBrushSide; i < BrushSideCount + FirstBrushSide; i++)
        {
            if(BrushSide[i].VertexIndexArrayCount > 2)
            {
                LinkedList<wxUint32> LeftPoints;
                LinkedList<wxUint32> RightPoints;

                //QuME_Vector Start = BrushVertices.VertexArray[BrushSide[i].VertexIndexArray[0]];
                wxUint32 StartIndex = BrushSide[i].VertexIndexArray[0];
                //QuME_Vector End = BrushVertices.VertexArray[BrushSide[i].VertexIndexArray[1]];
                wxUint32 EndIndex = BrushSide[i].VertexIndexArray[1];
                for(wxUint32 j = 2; j < BrushSide[i].VertexIndexArrayCount; j++)
                {
                    QuME_Vector t = BrushVertices.VertexArray[EndIndex] - BrushVertices.VertexArray[StartIndex];
                    QuME_Vector t2 = BrushVertices.VertexArray[BrushSide[i].VertexIndexArray[j]] - BrushVertices.VertexArray[StartIndex];
                    //find farthest point from origin
                    if(t2.length2() > t.length2())
                    {
                        //End = BrushVertices.VertexArray[BrushSide[i].VertexIndexArray[j]];
                        EndIndex = BrushSide[i].VertexIndexArray[j];
                    }
                }
                QuME_Vector Base = BrushVertices.VertexArray[EndIndex] - BrushVertices.VertexArray[StartIndex];
                for(wxUint32 j = 0; j < BrushSide[i].VertexIndexArrayCount; j++)
                {
                    if((BrushSide[i].VertexIndexArray[j] != StartIndex) && (BrushSide[i].VertexIndexArray[j] != EndIndex))
                    {
                        QuME_Vector Test = BrushVertices.VertexArray[BrushSide[i].VertexIndexArray[j]] - BrushVertices.VertexArray[StartIndex];
                        if(Base.cross(Test).dot(BrushSide[i].Normal) > 0.0)
                        {
                            LeftPoints.Append(BrushSide[i].VertexIndexArray[j]);
                        }
                        else
                        {
                            RightPoints.Append(BrushSide[i].VertexIndexArray[j]);
                        }
                    }
                }
/*
                std::cout << "Brush side " << i << "\n";
                std::cout << "Start vertex index:\n" << StartIndex << ", " << this->BrushVertices.VertexArray[StartIndex] << "\n";
                std::cout << "End vertex index:\n" << EndIndex << ", " << this->BrushVertices.VertexArray[EndIndex] << "\n";
                std::cout << "Total vertices: " << BrushSide[i].VertexIndexArrayCount << "\n";
                std::cout << "Left points:\n";
                for(ListItem<wxUint32>* k = LeftPoints.Head; k != nullptr; k =  k->next)
                {
                    std::cout << k->Item << ", ";
                    std::cout << this->BrushVertices.VertexArray[k->Item] << "\n";
                }
                std::cout << "Right points:\n";
                for(ListItem<wxUint32>* k = RightPoints.Head; k != nullptr; k =  k->next)
                {
                    std::cout << k->Item << ", ";
                    std::cout << this->BrushVertices.VertexArray[k->Item] << "\n";
                }
                std::cout << "\nSorted List:\n";
*/
                //std::cout << StartIndex << ", " << EndIndex << "\n";
                LinkedList<wxUint32>* a = QuickHull(StartIndex, EndIndex, &RightPoints, BrushSide[i].Normal);
                LinkedList<wxUint32>* b = QuickHull(EndIndex, StartIndex, &LeftPoints, BrushSide[i].Normal);
                LinkedList<wxUint32> result;
                result.Append(StartIndex);
                result.Append(*a);
                result.Append(EndIndex);
                result.Append(*b);
                //std::cout << L"f ";
                for(ListItem<wxUint32>* r = result.Head; r != nullptr; r = r->next)
                {
                    //std::cout << r->Item + 1 << L" ";
                }
                //std::cout << L"\n";
            }
        }
    }

    //dismiss the progress box
    progress.SetInt(-1);
    wxQueueEvent(Frame, progress.Clone());

    return true;
}

//this dumps all the available info into a HUGE text file
void QuME_BSP_Data::DebugDump(wxTextOutputStream& out)
{
    out << L"BSP version: " << this->BSPVersion << L"\n";
    out << L"\n-----------------------------------\n";
    this->LumpTable.DebugDump(out);
    this->Entities.DebugDump(out);
    this->Vertices.DebugDump(out);
    this->SurfaceEdges.DebugDump(out);
    this->Edges.DebugDump(out);
    this->Faces.DebugDump(out);
    this->Textures.DebugDump(out);
    this->BrushModels.DebugDump(out);
}

