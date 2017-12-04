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
    delete[] this->VertexArray;
    delete[] this->TextureUV;
    delete[] this->Triangle;
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
    else
    {
        VertexList* t = Tail;
        t->next = new VertexList();
        t = t->next;
        t->prev = Tail;
        Tail = t;
        t->next = nullptr;
        t->VertexIndex = v;
        t->UVIndex = UV;
        VertexListCount++;
        return t;
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
    out << "Plane #" << this->Plane << "\n";
    out << "Plane Side: " << this->PlaneSide << "\n";
    out << "First Edge: " << this->FirstEdge << "\n";
    out << "Number of Edges: " << this->EdgeCount << "\n";
    out << "Texture Info: " << this->TextureInfo << "\n";
    out << "Light-map Style #0: " << this->LightmapStyles[0];
    out << ", Light-map Style #1: " << this->LightmapStyles[1];
    out << ", Light-map Style #2: " << this->LightmapStyles[2];
    out << ", Light-map Style #3: " << this->LightmapStyles[3]<< "\n";
    out << "Light-map Offset: " << this->LightmapOffset << "\n";
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
    if(this->Face != nullptr) delete[] this->Face;
    this->Face = nullptr;
}

bool QuME_BSP_Faces::LoadLump(wxFileInputStream* infile, wxUint32 offset, wxUint32 length)
{
    this->Count = length / 20;

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
    out << "Faces:\n";
    for(wxUint32 i = 0; i < this->Count; i++)
    {
        out << "Face #" << i << "\n";
        this->Face[i].DebugDump(out);
    }
    out << "\n---------------------------------------\n";
}
