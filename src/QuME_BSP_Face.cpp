#include "QuME_BSP_Face.h"

QuME_BSP_Face::QuME_BSP_Face()
{
    Head = nullptr;
    Tail = nullptr;
    Triangles = nullptr;
    VertexListCount = 0;
    TriangleCount = 0;
    VertexArray = nullptr;
    TextureUVs = nullptr;
}

QuME_BSP_Face::~QuME_BSP_Face()
{
    delete[] this->VertexArray;
    delete[] this->TextureUVs;
    delete[] this->Triangles;
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

//naive triangulation routine - I think it's called ear clipping
//this only works on convex polygons (no checks for concavity!)
wxInt32 QuME_BSP_Face::Triangulate()
{
    for(wxInt32 i = EdgeCount - 1; i >= 0; i--)
    {
        AddVertexToList(VertexArray[i], TextureUVs[i]);
    }

    Triangles = new QuME_BSP_Triangle[EdgeCount - 2];

    VertexList* l = Head;
    for(wxUint32 i = 0; i < EdgeCount-2; i++)
    {
        l = Head;
        Triangles[i].v[0] = l->VertexIndex;
        Triangles[i].uv[0] = l->UVIndex;
        l = l->next;
        Triangles[i].v[1] = l->VertexIndex;
        Triangles[i].uv[1] = l->UVIndex;
        l = l->next;
        Triangles[i].v[2] = l->VertexIndex;
        Triangles[i].uv[2] = l->UVIndex;
        RemoveVertexFromList(Triangles[i].v[1]);
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
