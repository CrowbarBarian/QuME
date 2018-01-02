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

    for(wxUint32 i = 0; i < this->BSP_Faces.Faces.Count; i++)
    {
        //A simple progress update message
        if((i % (this->BSP_Faces.Faces.Count / 100)) == 0) //once every 100 faces
        {
            event.SetInt(i * 50 / this->BSP_Faces.Faces.Count); //fill up first half of progress bar only
            wxQueueEvent(Frame, event.Clone());
        }

        QuME_BSP_Face* Face = &this->BSP_Faces.Faces[i];
        wxUint32 TexIndex = Face->TextureInfo;
        wxInt32 FirstEdge = Face->FirstEdge;
        Face->VArray.Allocate(Face->EdgeCount);
        for(wxInt32 j = 0; j < Face->EdgeCount; j++)
        {
            wxInt32 FIndex = this->BSP_SurfaceEdges.SurfaceEdgeIndices[FirstEdge + j];
            wxUint32 VIndex = ((FIndex > 0) ? this->BSP_Edges.Edges[FIndex].a : this->BSP_Edges.Edges[-FIndex].b);
            Face->VArray[j].VertexIndex = VIndex;

            //Calculate UV indexes
            wxFloat64 U = this->BSP_Vertices.Vertices[VIndex].dot(this->BSP_Textures.TextureInfo[TexIndex].uAxis);
            U += this->BSP_Textures.TextureInfo[TexIndex].uOffset;
            U /= this->BSP_Textures.TextureInfo[TexIndex].xRes; //normalize U coordinate
            wxFloat64 V = this->BSP_Vertices.Vertices[VIndex].dot(this->BSP_Textures.TextureInfo[TexIndex].vAxis);
            V += this->BSP_Textures.TextureInfo[TexIndex].vOffset;
            V /= this->BSP_Textures.TextureInfo[TexIndex].yRes; //Normalize V coordinate
            QuME_UV p(U, -V);
            Face->VArray[j].UVIndex = this->BSP_TextureUVs.UVList.Append(p);
        }
        Face->Triangulate();
    }

    this->BSP_TextureUVs.UVList.ToArray(this->BSP_TextureUVs.UVs); //convert linked list to array

    return true;
}

QuME_LinkedList<wxUint32>* QuME_BSP_Data::QuickHull2D(wxUint32 Start, wxUint32 End, QuME_LinkedList<wxUint32>* Right, QuME_Vector& Normal, wxUint32 BrushID)
{
    QuME_Line L(this->BSP_BrushVertices.Vertices[Start], this->BSP_BrushVertices.Vertices[End]);
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
        if(L.distanceto(this->BSP_BrushVertices.Vertices[furthest]) < L.distanceto(this->BSP_BrushVertices.Vertices[i->Data]))
        {
            furthest = i->Data;
        }
    }
    QuME_Vector BaseLineA = this->BSP_BrushVertices.Vertices[furthest] - this->BSP_BrushVertices.Vertices[Start];
    QuME_Vector BaseLineB = this->BSP_BrushVertices.Vertices[End] - this->BSP_BrushVertices.Vertices[furthest];
    for(ListItem<wxUint32>* i = Right->Head; i != nullptr; i = i->next)
    {
        if(i->Data == furthest) continue;
        QuME_Vector Test = this->BSP_BrushVertices.Vertices[i->Data] - this->BSP_BrushVertices.Vertices[Start];
        QuME_Vector Xed = Test.cross(BaseLineA);
        if(Xed.length2() > 0.0)
		{
			if(Xed.dot(Normal) > 0.0)
			{
				wxUint32 t = i->Data;
				f->Append(t);
			}

		}
        Test = this->BSP_BrushVertices.Vertices[i->Data] - this->BSP_BrushVertices.Vertices[furthest];
       Xed = Test.cross(BaseLineB);
		if(Xed.length2() > 0.0)
		{
			if(Xed.dot(Normal) > 0.0)
			{
				wxUint32 t = i->Data;
				s->Append(t);
			}
		}
    }
    QuME_LinkedList<wxUint32>* a = QuickHull2D(Start, furthest, f, Normal, BrushID);
    QuME_LinkedList<wxUint32>* b = QuickHull2D(furthest, End, s, Normal, BrushID);
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
    wxThreadEvent event(wxEVT_THREAD, CONSOLELOG_EVENT);
    wxThreadEvent progress(wxEVT_THREAD, BSPPROCESSING_EVENT);

    event.SetString(L"Number of brushes in map:" + std::to_wstring(this->BSP_Brushes.Brushes.Count) + L"\n");
    wxQueueEvent(Frame, event.Clone());

    for(wxUint32 i = 0; i < this->BSP_BrushSides.BrushSideArray.Count; i++)
    {
        this->BSP_BrushSides.BrushSideArray[i].Normal = this->BSP_Planes.Planes[this->BSP_BrushSides.BrushSideArray[i].PlaneIndex].Plane.Normal;
    }

    //iterate through all the brushes in the BSP file
    for(wxUint32 BrushIndex = 0; BrushIndex < this->BSP_Brushes.Brushes.Count; BrushIndex++)
    {
        if((BrushIndex % (this->BSP_Brushes.Brushes.Count / 100)) == 0) //update progress every 100 brushes
        {
            progress.SetInt(50 + BrushIndex * 50 / this->BSP_Brushes.Brushes.Count); //fill up other half of progress bar
            wxQueueEvent(Frame, progress.Clone());
        }

        QuME_BSP_Brush* CurrentBrush = &this->BSP_Brushes.Brushes[BrushIndex];

        wxUint32 BrushSideCount = CurrentBrush->SideCount;
        wxUint32 FirstBrushSide = CurrentBrush->FirstBrushSide;

        //now find all possible combinations of 3 brush sides in current brush, and find intersections
        for(wxUint32 i = FirstBrushSide; i < (BrushSideCount + FirstBrushSide) - 2; i++)
        {
            QuME_Plane* A = &this->BSP_Planes.Planes[this->BSP_BrushSides.BrushSideArray[i].PlaneIndex].Plane;
            for(wxUint32 j = i + 1; j < (BrushSideCount + FirstBrushSide) - 1; j++)
            {
                QuME_Plane* B = &this->BSP_Planes.Planes[this->BSP_BrushSides.BrushSideArray[j].PlaneIndex].Plane;
                for(wxUint32 k = j + 1; k < BrushSideCount + FirstBrushSide; k++)
                {
                    QuME_Plane* C = &this->BSP_Planes.Planes[this->BSP_BrushSides.BrushSideArray[k].PlaneIndex].Plane;
                    QuME_Vector CurrentVertex;
                    if(A->Intersect(*B, *C, CurrentVertex))
                    {
                        bool VertInside = true;
                        //check to see if found vertex is inside brush
                        for(wxUint32 l = FirstBrushSide; l < BrushSideCount + FirstBrushSide; l++)
                        {
                            wxFloat64 OnSide = this->BSP_Planes.Planes[this->BSP_BrushSides.BrushSideArray[l].PlaneIndex].Plane.TestSide(CurrentVertex);
                            if(OnSide > QUME_EPSILON)
                            {
                                VertInside = false; //not a valid vertex, ignore
                                break;
                            }
                        }

                        if(VertInside)
                        {
                        	//vertex is inside brush, add to brush vertices
                        	wxUint32 AddedIndex = CurrentBrush->BrushVertexList.AppendIfUnique(CurrentVertex);
                        	//add index of added vertex to brush sides for later
							this->BSP_BrushSides.BrushSideArray[i].BrushVertexList.AppendIfUnique(AddedIndex); //these indices are relative to CurrentBrush->VertexArray
							this->BSP_BrushSides.BrushSideArray[j].BrushVertexList.AppendIfUnique(AddedIndex);
							this->BSP_BrushSides.BrushSideArray[k].BrushVertexList.AppendIfUnique(AddedIndex);
                        }
                    }
                }
            }
        }
    }

    //transfer brush vertices to global list, and convert brush-local vertex indices to global indices
	for(wxUint32 BrushIndex = 0; BrushIndex < this->BSP_Brushes.Brushes.Count; BrushIndex++)
	{
		QuME_BSP_Brush* CurrentBrush = &this->BSP_Brushes.Brushes[BrushIndex];
		CurrentBrush->BrushVertexList.ToArray(CurrentBrush->BrushVertices);

		for(wxUint32 VertexIndex = 0; VertexIndex < CurrentBrush->BrushVertices.Count; VertexIndex++)
		{
			CurrentBrush->Bounds.UpdateBounds(CurrentBrush->BrushVertices[VertexIndex]);
			wxUint32 AddedIndex = this->BSP_BrushVertices.VertexList.Append(CurrentBrush->BrushVertices[VertexIndex]);

			for(wxUint32 SideIndex = CurrentBrush->FirstBrushSide; SideIndex < CurrentBrush->SideCount + CurrentBrush->FirstBrushSide; SideIndex++)
			{
				QuME_BSP_BrushSide* CurrentSide = &this->BSP_BrushSides.BrushSideArray[SideIndex];
				CurrentSide->BrushVertexList.ToArray(CurrentSide->BrushSideVertices);
				for(wxUint32 i = 0; i < CurrentSide->BrushSideVertices.Count; i++)
				{
					if(CurrentSide->BrushSideVertices[i] == VertexIndex)
					{
						CurrentSide->BrushSideVertexIndexList.AppendIfUnique(AddedIndex);
					}
				}
				CurrentSide->BrushSideVertexIndexList.ToArray(CurrentSide->BrushSideVertexIndices);
			}
		}
	}

	//convert linked list to array once we're done adding vertices
	this->BSP_BrushVertices.VertexList.ToArray(this->BSP_BrushVertices.Vertices);
    //calculate bounding box of map
    for(wxUint32 i = 0; i < this->BSP_BrushVertices.Vertices.Count; i++)
	{
		this->BSP_BrushBoundingBox.UpdateBounds(this->BSP_BrushVertices.Vertices[i]);
	}

	//sort Brush faces in counterclockwise order
    for(wxUint32 BrushIndex = 0; BrushIndex < this->BSP_Brushes.Brushes.Count; BrushIndex++)
    {

        QuME_BSP_Brush* CurrentBrush = &this->BSP_Brushes.Brushes[BrushIndex];
        wxUint32 BrushSideCount = CurrentBrush->SideCount;
        wxUint32 FirstBrushSide = CurrentBrush->FirstBrushSide;

        for(wxUint32 BrushSideIndex = FirstBrushSide; BrushSideIndex < BrushSideCount + FirstBrushSide; BrushSideIndex++)
        {
			QuME_BSP_BrushSide* CurrentSide = &this->BSP_BrushSides.BrushSideArray[BrushSideIndex];

            if(CurrentSide->BrushSideVertexIndices.Count > 2)
            {
                QuME_LinkedList<wxUint32> LeftPoints;
                QuME_LinkedList<wxUint32> RightPoints;

                wxUint32 StartIndex = CurrentSide->BrushSideVertexIndices[0];
                wxUint32 EndIndex = CurrentSide->BrushSideVertexIndices[1];
                for(wxUint32 i = 2; i < CurrentSide->BrushSideVertexIndices.Count; i++)
                {
                    QuME_Vector t = this->BSP_BrushVertices.Vertices[EndIndex] - this->BSP_BrushVertices.Vertices[StartIndex];
                    QuME_Vector t2 = this->BSP_BrushVertices.Vertices[CurrentSide->BrushSideVertexIndices[i]] - this->BSP_BrushVertices.Vertices[StartIndex];
                    //find farthest point from origin
                    if(t2.length2() > t.length2())
                    {
                        EndIndex = CurrentSide->BrushSideVertexIndices[i];
                    }
                }
                QuME_Vector Base = this->BSP_BrushVertices.Vertices[EndIndex] - this->BSP_BrushVertices.Vertices[StartIndex];
                QuME_Line BaseLine(this->BSP_BrushVertices.Vertices[StartIndex], this->BSP_BrushVertices.Vertices[EndIndex]);

                for(wxUint32 i = 1; i < CurrentSide->BrushSideVertexIndices.Count; i++)
                {
                    if(CurrentSide->BrushSideVertexIndices[i] != EndIndex)
                    {

                        QuME_Vector Test = this->BSP_BrushVertices.Vertices[CurrentSide->BrushSideVertexIndices[i]] - this->BSP_BrushVertices.Vertices[StartIndex];
                        if(Base.cross(Test).length2() > 0.0)
						{
							if(Base.cross(Test).dot(CurrentSide->Normal) > 0.0)
							{
								LeftPoints.Append(CurrentSide->BrushSideVertexIndices[i]);
							}
							else
							{
								RightPoints.Append(CurrentSide->BrushSideVertexIndices[i]);
							}
						}
                    }
                }
                QuME_LinkedList<wxUint32>* a = QuickHull2D(StartIndex, EndIndex, &RightPoints, CurrentSide->Normal, BrushIndex);
                QuME_LinkedList<wxUint32>* b = QuickHull2D(EndIndex, StartIndex, &LeftPoints, CurrentSide->Normal, BrushIndex);
                QuME_LinkedList<wxUint32> result;
                result.Append(StartIndex);
                result.Append(*a);
                result.Append(EndIndex);
                result.Append(*b);
                SAFE_DELETE(a); //squashed memory leak here...
                SAFE_DELETE(b); //...and here...

                result.ToArray(CurrentSide->BrushSideVertexIndices);

				wxInt32 TexIndex = CurrentSide->TextureIndex;

				if((CurrentSide->BrushSideVertexIndices.Count > 2) && (CurrentSide->TextureIndex >= 0))
				{
					for(wxUint32 j = 0; j < CurrentSide->BrushSideVertexIndices.Count; j++)
					{
						CurrentBrush->Bounds.UpdateBounds(this->BSP_BrushVertices.Vertices[CurrentSide->BrushSideVertexIndices[j]]);

						//Calculate UV indexes by projecting texture onto surface
						wxFloat64 U = this->BSP_BrushVertices.Vertices[CurrentSide->BrushSideVertexIndices[j]].dot(this->BSP_Textures.TextureInfo[TexIndex].uAxis);
						U += this->BSP_Textures.TextureInfo[TexIndex].uOffset;
						U /= this->BSP_Textures.TextureInfo[TexIndex].xRes; //normalize U coordinate
						wxFloat64 V = this->BSP_BrushVertices.Vertices[CurrentSide->BrushSideVertexIndices[j]].dot(this->BSP_Textures.TextureInfo[TexIndex].vAxis);
						V += this->BSP_Textures.TextureInfo[TexIndex].vOffset;
						V /= this->BSP_Textures.TextureInfo[TexIndex].yRes; //Normalize V coordinate
						QuME_UV p(U, -V); //remember to negate V coord here, to flip textures the right way
						wxUint32 q = this->BSP_BrushSideUVs.UVList.Append(p);

						CurrentSide->BrushSideUVIndices.Append(q); //Add these coordinates to list
					}
					CurrentSide->BrushSideUVIndices.ToArray(CurrentSide->BrushSideUVs);
				}
            }
        }
    }
    this->BSP_BrushSideUVs.UVList.ToArray(this->BSP_BrushSideUVs.UVs);

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
    this->BSP_LumpTable.DebugDump(out);
    this->BSP_Entities.DebugDump(out);
    this->BSP_Vertices.DebugDump(out);
    this->BSP_SurfaceEdges.DebugDump(out);
    this->BSP_Edges.DebugDump(out);
    this->BSP_Faces.DebugDump(out);
    this->BSP_Textures.DebugDump(out);
    this->BSP_BrushModels.DebugDump(out);
}

