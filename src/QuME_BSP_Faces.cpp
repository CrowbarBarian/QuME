#include "QuME_BSP_Faces.h"

QuME_BSP_Face::QuME_BSP_Face()
{
    Head = nullptr;
    Tail = nullptr;
    Triangle = nullptr;
    VertexListCount = 0;
    TriangleCount = 0;
    VertexArray = nullptr;
    TextureUV = nullptr;
}

QuME_BSP_Face::~QuME_BSP_Face()
{
    SAFE_ARRAY_DELETE(this->VertexArray);
    SAFE_ARRAY_DELETE(this->TextureUV);
    SAFE_ARRAY_DELETE(this->Triangle);
    while(Head != nullptr)
	{
		VertexList* t = Head->next;
		delete Head;
		Head = t;
	}
	Tail = nullptr;
    VertexListCount = 0;
    TriangleCount = 0;
}

//private Triangulate() helper method
VertexList* QuME_BSP_Face::AddVertexToList(wxUint32 v, wxUint32 UV)
{
    if(Head == nullptr)
    {
        Head = new VertexList;
        Head->VertexIndex = v;
        Head->UVIndex = UV;
        VertexListCount++;
        Head->next = nullptr;
        Head->prev = nullptr;
        Tail = Head;
        return Head;
    }
    else if (Tail == Head)
	{
		Head->next = new VertexList;
		Tail = Head->next;
		Tail->prev = Head;
		Tail->next = nullptr;
		Tail->VertexIndex = v;
		Tail->UVIndex = UV;
		VertexListCount++;
		return Tail;
	}
    else
    {
    	assert(Tail->next == nullptr);
		Tail->next = new VertexList;
        Tail->next->prev = Tail;
        Tail = Tail->next;
        Tail->next = nullptr;
        Tail->VertexIndex = v;
        Tail->UVIndex = UV;
        VertexListCount++;
        return Tail;
    }
}

//private Triangulate() helper method
VertexList* QuME_BSP_Face::GetVertex(wxUint32 v)
{
    wxUint32 counter = 0;
    for(VertexList* i = Head; i != nullptr; i = i->next)
    {
        if(i->VertexIndex == v) return i;
        counter++;
    }
    return nullptr;
}

//private Triangulate() helper method
VertexList* QuME_BSP_Face::RemoveVertexFromList(wxUint32 v)
{
    VertexList* VCurrent = GetVertex(v);
    if(VCurrent != nullptr)
    {
        if((VCurrent == Head) && (VCurrent == Tail)) //only one in list
        {
            VertexListCount--;
            delete VCurrent;
            Head = nullptr;
            Tail = nullptr;
            return nullptr;
        }
        if(VCurrent == Head) //at start of list
        {
            VertexListCount--;
            Head = Head->next;
            Head->prev = nullptr;
            delete VCurrent;
            if(Head->next == nullptr)
            {
                return Head; //only thing left
            }
            else return Head->next;
        }
        else if(VCurrent == Tail) //at end of list
        {
            VertexListCount--;
            Tail = Tail->prev;
            Tail->next = nullptr;
            delete VCurrent;
            return Head;
        }
        else //in the middle of the list
        {
            VertexListCount--;
            VertexList* t = VCurrent->next;
            VCurrent->next->prev = VCurrent->prev;
            VCurrent->prev->next = VCurrent->next;
            delete VCurrent;
            return t;
        }
    }
    return nullptr;
}

//naive triangulation routine
//this only works on convex polygons (no checks for concavity!)
wxInt32 QuME_BSP_Face::Triangulate()
{

    for(wxInt32 i = 0; i < EdgeCount; i++)
    {
        AddVertexToList(VertexArray[i], TextureUV[i]);
    }

    Triangle = new QuME_BSP_Triangle[EdgeCount - 2];

    VertexList* l = Head;
    for(wxInt32 i = 0; i < EdgeCount-2; i++)
    {
        Triangle[i].v[0] = l->VertexIndex;
        Triangle[i].uv[0] = l->UVIndex;
        if((l = l->next) == nullptr) l = Head; //at end of list, loop around
        Triangle[i].v[1] = l->VertexIndex;
        Triangle[i].uv[1] = l->UVIndex;
        if((l = l->next) == nullptr) l = Head; //ditto
        Triangle[i].v[2] = l->VertexIndex;
        Triangle[i].uv[2] = l->UVIndex;
        RemoveVertexFromList(Triangle[i].v[1]); //eliminate vertex of triangle we just snipped off
        TriangleCount++;
    }
    return TriangleCount;
}

void QuME_BSP_Face::DebugDump(wxTextOutputStream& out)
{
    out << L"Plane #" << this->Plane << L"\n";
    out << L"Plane Side: " << this->PlaneSide << L"\n";
    out << L"First Edge: " << this->FirstEdge << L"\n";
    out << L"Number of Edges: " << this->EdgeCount << L"\n";
    out << L"Texture Info: " << this->TextureInfo << L"\n";
    out << L"Light-map Style #0: " << this->LightmapStyles[0];
    out << L", Light-map Style #1: " << this->LightmapStyles[1];
    out << L", Light-map Style #2: " << this->LightmapStyles[2];
    out << L", Light-map Style #3: " << this->LightmapStyles[3]<< L"\n";
    out << L"Light-map Offset: " << this->LightmapOffset << L"\n";
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

QuME_BSP_Faces::QuME_BSP_Faces()
{
    //ctor
    this->Count = 0;
    this->Face = nullptr;
}

QuME_BSP_Faces::~QuME_BSP_Faces()
{
    //dtor
    this->Count = 0;
    SAFE_ARRAY_DELETE(this->Face);
    this->Face = nullptr;
}

bool QuME_BSP_Faces::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    this->Count = length / Q2_BSP_FACE_DATA_SIZE;

    wxDataInputStream* binData = new wxDataInputStream( *infile );

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    if (infile->SeekI(offset, wxFromStart) == wxInvalidOffset)
    {
        delete binData;
        return false;
    }

    this->Face = new QuME_BSP_Face[this->Count];
    if(this->Face == nullptr)
    {
        delete binData;
        return false;
    }

    for(wxUint32 i = 0; i < this->Count; i++)
    {
        this->Face[i].Plane = binData->Read16();
        this->Face[i].PlaneSide = static_cast<wxInt16>(binData->Read16()); //arg...stoopid wxWidgets don't have signed bin read
        this->Face[i].FirstEdge = static_cast<wxInt32>(binData->Read32());
        this->Face[i].EdgeCount = static_cast<wxInt16>(binData->Read16());
        this->Face[i].TextureInfo = static_cast<wxInt16>(binData->Read16());
        for(wxUint32 j = 0; j < 4; j++) this->Face[i].LightmapStyles[j] = binData->Read8();
        this->Face[i].LightmapOffset= static_cast<wxInt32>(binData->Read32());
    }

    if(!binData->IsOk())
    {
        delete binData;
        return false;
    }

    delete binData;
    return true;
}
void QuME_BSP_Faces::DebugDump(wxTextOutputStream& out)
{
    out << L"Faces:\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << L"Face #" << i << L"\n";
        this->Face[i].DebugDump(out);
    }
    out << L"\n---------------------------------------\n";
}
