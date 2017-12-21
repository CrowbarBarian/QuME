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
:Level_BBox_Min(0.0, 0.0, 0.0), Level_BBox_Max(0.0, 0.0, 0.0)
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
            event.SetInt(i * 50 / this->Faces.Count); //fill up first half of progress bar only
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
            QuME_UV p = QuME_UV(U , V);
            this->TextureUVs.UVs.Append(p); //Add these coordinates to list
            Face->TextureUV[j] = UVIndex++; //set to current index, then increment
        }
        Face->Triangulate();
    }

    this->TextureUVs.UVArrayCount = this->TextureUVs.UVs.ToArray(this->TextureUVs.UVArray); //turn the UV linked list into an array once all UVs have been calculated

    return true;
}

QuME_LinkedList<wxUint32>* QuME_BSP_Data::QuickHull2D(wxUint32 Start, wxUint32 End, QuME_LinkedList<wxUint32>* Right, QuME_Vector& Normal)
{
    QuME_Vector* VertexSet = this->BrushVertices.Vertices;
    QuME_Line L(VertexSet[Start], VertexSet[End]);
    QuME_LinkedList<wxUint32>* r = new QuME_LinkedList<wxUint32>(); //the return set of vertices
    if(Right->Count == 0)
    {
        return r; //empty list
    }

    QuME_LinkedList<wxUint32>* f = new QuME_LinkedList<wxUint32>();
    QuME_LinkedList<wxUint32>* s = new QuME_LinkedList<wxUint32>();

    wxUint32 furthest = Right->Head->Data;
    for(ListItem<wxUint32>* i = Right->Head; i != nullptr; i = i->next)
    {
        if(L.distanceto(VertexSet[furthest]) < L.distanceto(VertexSet[i->Data]))
        {
            furthest = i->Data;
        }
    }
    QuME_Vector BaseLineA = VertexSet[furthest] - VertexSet[Start];
    QuME_Vector BaseLineB = VertexSet[End] - VertexSet[furthest];
    for(ListItem<wxUint32>* i = Right->Head; i != nullptr; i = i->next)
    {
        if(i->Data == furthest) continue;
        QuME_Vector Test = VertexSet[i->Data] - VertexSet[Start];
        QuME_Vector Xed = Test.cross(BaseLineA);
        if(Xed.length2() > 0.0)
		{
			if(Xed.dot(Normal) > QUME_EPSILON)
			{
				wxUint32 t = i->Data;
				f->Append(t);
			}

		}
        Test = VertexSet[i->Data] - VertexSet[furthest];
        Xed = Test.cross(BaseLineB);
		if(Xed.length2() > 0.0)
		{
			if(Xed.dot(Normal) > QUME_EPSILON)
			{
				wxUint32 t = i->Data;
				s->Append(t);
			}
		}
    }
    QuME_LinkedList<wxUint32>* a = QuickHull2D(Start, furthest, f, Normal);
    QuME_LinkedList<wxUint32>* b = QuickHull2D(furthest, End, s, Normal);
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
	wxUint32 AddedIndex = 0;
    QuME_BSP_Plane* BSPPlane = this->Planes.BSPPlaneArray.Data;
    QuME_BSP_Brush* Brush = this->Brushes.Brush;
    QuME_BSP_BrushSide* BrushSide = this->BrushSides.BrushSideArray.Data;
    QuME_BSP_Texture* Texture = this->Textures.Texture;

    wxThreadEvent event(wxEVT_THREAD, CONSOLELOG_EVENT);
    wxThreadEvent progress(wxEVT_THREAD, BSPPROCESSING_EVENT);

    event.SetString(L"Processing Brushes...\nNumber of brushes in map:" + std::to_wstring(this->Brushes.Count) + L"\n");
    wxQueueEvent(Frame, event.Clone());

    //iterate through all the brushes in the BSP file
    for(wxUint32 BrushIndex = 0; BrushIndex < this->Brushes.Count; BrushIndex++)
    {
        if((BrushIndex % (this->Brushes.Count / 100)) == 0) //update progress every 100 brushes
        {
            progress.SetInt(50 + BrushIndex * 50 / this->Brushes.Count); //fill up other half of progress bar
            wxQueueEvent(Frame, progress.Clone());
        }

        QuME_BSP_Brush* CurrentBrush = &Brush[BrushIndex];

        wxUint32 BrushSideCount = CurrentBrush->SideCount;
        wxUint32 FirstBrushSide = CurrentBrush->FirstBrushSide;

        for(wxUint32 i = FirstBrushSide; i < (BrushSideCount + FirstBrushSide) - 2; i++)
        {
            QuME_Plane* A = &BSPPlane[BrushSide[i].PlaneIndex].Plane;
            for(wxUint32 j = i + 1; j < (BrushSideCount + FirstBrushSide) - 1; j++)
            {
                QuME_Plane* B = &BSPPlane[BrushSide[j].PlaneIndex].Plane;
                for(wxUint32 k = j + 1; k < BrushSideCount + FirstBrushSide; k++)
                {
                    QuME_Plane* C = &BSPPlane[BrushSide[k].PlaneIndex].Plane;

                    QuME_Vector CurrentVertex;
                    bool isIntersecting = A->Intersect(*B, *C, CurrentVertex);
                    if(isIntersecting)
                    {
                        bool VertInside = true;
                        for(wxUint32 l = FirstBrushSide; l < BrushSideCount + FirstBrushSide; l++)
                        {
                            wxFloat64 OnSide = BSPPlane[BrushSide[l].PlaneIndex].Plane.TestSide(CurrentVertex);
                            if(OnSide > QUME_EPSILON)
                            {
                                VertInside = false;
                                break;
                            }
                        }

                        if(VertInside)
                        {
							AddedIndex = this->BrushVertices.VertexList.Append(CurrentVertex);
							BrushSide[i].VertexIndexList.Append(AddedIndex);
							BrushSide[j].VertexIndexList.Append(AddedIndex);
							BrushSide[k].VertexIndexList.Append(AddedIndex);
                        }
                    }
                }
            }
        }
    }

    //convert linked list to array once we're done adding vertices
    SAFE_ARRAY_DELETE(this->BrushVertices.Vertices);
    this->BrushVertices.Count = this->BrushVertices.VertexList.ToArray(this->BrushVertices.Vertices);

    //calculate bounding box of map
    for(wxUint32 i = 0; i < this->BrushVertices.Count; i++)
	{
		this->BrushBoundingBox.UpdateBounds(this->BrushVertices.Vertices[i]);
	}

    for(wxUint32 i = 0; i < this->BrushSides.BrushSideArray.Count; i++)
    {
        SAFE_ARRAY_DELETE(BrushSide[i].VertexIndexArray);
        BrushSide[i].VertexIndexArrayCount = BrushSide[i].VertexIndexList.ToArray(BrushSide[i].VertexIndexArray);
        BrushSide[i].Normal = Planes.BSPPlaneArray.Data[BrushSide[i].PlaneIndex].Plane.Normal;
    }

	//sort Brush faces in counterclockwise order
    for(wxUint32 BrushIndex = 0; BrushIndex < this->Brushes.Count; BrushIndex++)
    {

        QuME_BSP_Brush* CurrentBrush = &this->Brushes.Brush[BrushIndex];
        wxUint32 BrushSideCount = CurrentBrush->SideCount;
        wxUint32 FirstBrushSide = CurrentBrush->FirstBrushSide;

        for(wxUint32 BrushSideIndex = FirstBrushSide; BrushSideIndex < BrushSideCount + FirstBrushSide; BrushSideIndex++)
        {
			QuME_BSP_BrushSide* CurrentSide = &BrushSide[BrushSideIndex];

            if(CurrentSide->VertexIndexArrayCount > 2)
            {
                QuME_LinkedList<wxUint32> LeftPoints;
                QuME_LinkedList<wxUint32> RightPoints;

                wxUint32 StartIndex = CurrentSide->VertexIndexArray[0];
                wxUint32 EndIndex = CurrentSide->VertexIndexArray[1];
                for(wxUint32 i = 2; i < CurrentSide->VertexIndexArrayCount; i++)
                {
                    QuME_Vector t = BrushVertices.Vertices[EndIndex] - BrushVertices.Vertices[StartIndex];
                    QuME_Vector t2 = BrushVertices.Vertices[CurrentSide->VertexIndexArray[i]] - BrushVertices.Vertices[StartIndex];
                    //find farthest point from origin
                    if(t2.length2() > t.length2())
                    {
                        EndIndex = CurrentSide->VertexIndexArray[i];
                    }
                }
                QuME_Vector Base = BrushVertices.Vertices[EndIndex] - BrushVertices.Vertices[StartIndex];
                for(wxUint32 i = 0; i < CurrentSide->VertexIndexArrayCount; i++)
                {
                    if((CurrentSide->VertexIndexArray[i] != StartIndex) && (CurrentSide->VertexIndexArray[i] != EndIndex))
                    {
                        QuME_Vector Test = BrushVertices.Vertices[CurrentSide->VertexIndexArray[i]] - BrushVertices.Vertices[StartIndex];
                        if(Base.cross(Test).length2() > 0.0)
						{
							if(Base.cross(Test).dot(CurrentSide->Normal) > QUME_EPSILON)
							{
								LeftPoints.Append(CurrentSide->VertexIndexArray[i]);
							}
							else
							{
								RightPoints.Append(CurrentSide->VertexIndexArray[i]);
							}
						}
                    }
                }
                QuME_LinkedList<wxUint32>* a = QuickHull2D(StartIndex, EndIndex, &RightPoints, CurrentSide->Normal);
                QuME_LinkedList<wxUint32>* b = QuickHull2D(EndIndex, StartIndex, &LeftPoints, CurrentSide->Normal);
                QuME_LinkedList<wxUint32> result;
                result.Append(StartIndex);
                result.Append(*a);
                result.Append(EndIndex);
                result.Append(*b);
                SAFE_DELETE(a); //squashed memory leak here...
                SAFE_DELETE(b);

                CurrentSide->VertexIndexArrayCount = result.ToArray(CurrentSide->VertexIndexArray);

				wxInt32 TexIndex = CurrentSide->TextureIndex;

				if(CurrentSide->VertexIndexArrayCount > 2)
				{
					for(wxUint32 j = 0; j < CurrentSide->VertexIndexArrayCount; j++)
					{
						CurrentBrush->Bounds.UpdateBounds(this->BrushVertices.Vertices[CurrentSide->VertexIndexArray[j]]);

						//Calculate UV indexes by projecting texture onto surface
						wxFloat64 U = this->BrushVertices.Vertices[CurrentSide->VertexIndexArray[j]].dot(Texture[TexIndex].uAxis);
						U += Texture[TexIndex].uOffset;
						U /= Texture[TexIndex].xRes; //normalize U coordinate
						wxFloat64 V = this->BrushVertices.Vertices[CurrentSide->VertexIndexArray[j]].dot(Texture[TexIndex].vAxis);
						V += Texture[TexIndex].vOffset;
						V /= Texture[TexIndex].yRes; //Normalize V coordinate
						QuME_UV p = QuME_UV(U, -V); //remember to negate V coord here, to flip textures the right way
						wxUint32 q = this->BrushSideUVs.UVs.Append(p);

						CurrentSide->UVIndices.Append(q); //Add these coordinates to list
					}
					CurrentSide->UVIndexArrayCount = CurrentSide->UVIndices.ToArray(CurrentSide->UVIndexArray);
					//std::cout << CurrentSide->UVIndexArrayCount << " " << CurrentSide->VertexIndexArrayCount << "\n";
				}
            }
        }
    }
	this->BrushSideUVs.UVArrayCount = this->BrushSideUVs.UVs.ToArray(this->BrushSideUVs.UVArray);
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

