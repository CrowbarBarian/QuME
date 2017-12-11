/***************************************************************
 * Name:      QuME_BSP_Data.h
 * Purpose:   The main hub for BSP activities.
 * Author:    J Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:   2017-06-13
 * Copyright: J Thomas (Crowbarbarian) ()
 * License:   GPL v3
 **************************************************************/
#ifndef QUME_BSPData_H_INCLUDED
#define QUME_BSPData_H_INCLUDED

#include "QuME_Common.h"
#include <wx/thread.h>

class QuME_Frame;

//include all of the BSP data types we handle in this class
#include "QuME_BSP_Lumps.h"
#include "QuME_BSP_Vertices.h"
#include "QuME_BSP_Faces.h"
#include "QuME_BSP_Entities.h"
#include "QuME_BSP_Edges.h"
#include "QuME_BSP_SurfaceEdges.h"
#include "QuME_BSP_Textures.h"
#include "QuME_BSP_BrushModels.h"
#include "QuME_BSP_TextureUVs.h"
#include "QuME_BSP_Planes.h"
#include "QuME_BSP_Brushes.h"
#include "QuME_Lists.h"
#include "QuME_Line.h"

#define QUME_QUAKE_2_BSP_MAGIC (('I') | ('B' << 8) | ('S' << 16) | ('P' << 24))
#define BSP_LUMP_VERSION_TABLE_MAX 1 //we only support Quake 2 for now

struct QuME_BSP_Versions
{
    wxUint32 ver;
    wxUint32 lumpTableOffset;
};

//A table for the offset of the lump table directory in the BSP file
const QuME_BSP_Versions QuME_BSP_VersionTable[BSP_LUMP_VERSION_TABLE_MAX] = {{ 38, 8}};

class QuME_BSP_Data
{
public:
    QuME_BSP_Data();
    ~QuME_BSP_Data();

    bool LoadHeader(wxFileInputStream* infile);
    bool LoadLumpTable(wxFileInputStream* infile)
    {
        return LumpTable.LoadLumps(infile, this->GetLumpTableOffset(this->BSPVersion));
    }

    bool LoadEntities(wxFileInputStream* infile)
    {
        return this->Entities.LoadLump(infile,
                                   this->LumpTable.Lump[LUMP_ENTITIES].Offset,
                                   this->LumpTable.Lump[LUMP_ENTITIES].Length);
    }

    bool LoadVertices(wxFileInputStream* infile)
    {
        return this->Vertices.LoadLump(infile,
                                   this->LumpTable.Lump[LUMP_VERTICES].Offset,
                                   this->LumpTable.Lump[LUMP_VERTICES].Length);
    }

    bool LoadFaces(wxFileInputStream* infile)
    {
        return this->Faces.LoadLump(infile,
                                this->LumpTable.Lump[LUMP_FACES].Offset,
                                this->LumpTable.Lump[LUMP_FACES].Length);
    }

    bool LoadSurfaceEdges(wxFileInputStream* infile)
    {
        return this->SurfaceEdges.LoadLump(infile,
                                       this->LumpTable.Lump[LUMP_SURFACE_EDGES].Offset,
                                       this->LumpTable.Lump[LUMP_SURFACE_EDGES].Length);
    }

    bool LoadEdges(wxFileInputStream* infile)
    {
        return this->Edges.LoadLump(infile,
                                this->LumpTable.Lump[LUMP_EDGES].Offset,
                                this->LumpTable.Lump[LUMP_EDGES].Length);
    }

    bool LoadTextures(wxFileInputStream* infile)
    {
        return this->Textures.LoadLump(infile,
                                   this->LumpTable.Lump[LUMP_TEXTURE_INFORMATION].Offset,
                                   this->LumpTable.Lump[LUMP_TEXTURE_INFORMATION].Length,
                                   this->baseDir);
    }

    bool LoadBrushModels(wxFileInputStream* infile)
    {
        return this->BrushModels.LoadLump(infile,
                                      this->LumpTable.Lump[LUMP_MODELS].Offset,
                                      this->LumpTable.Lump[LUMP_MODELS].Length);
    }

    bool LoadPlanes(wxFileInputStream* infile)
    {
        return this->Planes.LoadLump(infile,
                                 this->LumpTable.Lump[LUMP_PLANES].Offset,
                                 this->LumpTable.Lump[LUMP_PLANES].Length);
    }

    bool LoadBrushes(wxFileInputStream* infile)
    {
        return this->Brushes.LoadLump(infile,
                                  this->LumpTable.Lump[LUMP_BRUSHES].Offset,
                                  this->LumpTable.Lump[LUMP_BRUSHES].Length);
    }

    bool LoadBrushSides(wxFileInputStream* infile)
    {
        return this->BrushSides.LoadLump(infile,
                                     this->LumpTable.Lump[LUMP_BRUSH_SIDES].Offset,
                                     this->LumpTable.Lump[LUMP_BRUSH_SIDES].Length);
    }

    bool SetupBSPData(QuME_Frame* Frame);
    bool ProcessBrushes(QuME_Frame* Frame);

    wxUint32 GetLumpTableOffset(wxUint32 version);
    void DebugDump(wxTextOutputStream& out);

    LinkedList<wxUint32>* QuickHull(wxUint32 Start, wxUint32 End, LinkedList<wxUint32>* Right, QuME_Vector& Normal);

    std::wstring baseDir; //used to track down resources
    std::wstring MapName; //the name of this map from load
    wxUint32 IBSPMagic; //this map's 'magic' check value
    wxUint32 BSPVersion; //this map's version number
    wxUint32 EntCount; //how many entities we found on load
    QuME_BSP_Lumps LumpTable;
    wxUint32 VertexCount; //how many vertices are in this BSP file
    QuME_BSP_Vertices Vertices; //the shared vertices in this file
    QuME_BSP_Entities Entities; //the file's entities
    QuME_BSP_Faces Faces; //the polygons of this map
    QuME_BSP_Edges Edges;
    QuME_BSP_SurfaceEdges SurfaceEdges;
    QuME_BSP_Textures Textures; //texture info
    QuME_BSP_TextureUVs TextureUVs; //the UV coordinates of our vertices
    QuME_BSP_BrushModels BrushModels; //the brush models in this map - model #0 is the level itself
    QuME_BSP_Planes Planes;
    QuME_BSP_Brushes Brushes;
    QuME_BSP_BrushSides BrushSides;
    QuME_BSP_Vertices BrushVertices;


};

#endif // QuME_BSP_Data_H_INCLUDED
