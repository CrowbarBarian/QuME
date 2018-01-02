/***************************************************************
 * Name:      QuME_BSP_Data.h
 * Purpose:   The main hub for BSP activities.
 * Author:    J Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Created:   2017-06-13
 * Copyright: J Thomas (Crowbarbarian) ()
 * License:   GPL v3
 **************************************************************/
#ifndef QUME_BSPDATA_H_INCLUDED
#define QUME_BSPDATA_H_INCLUDED

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
#include "QuME_BSP_Vertices.h"
#include "QuME_BSP_Brush_Vertices.h"
#include "QuME_Lists.h"
#include "QuME_Line.h"
#include "QuME_Vector.h"
#include "QuME_Bounds.h"

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
        return BSP_LumpTable.LoadLumps(infile, this->GetLumpTableOffset(this->BSPVersion));
    }

    bool LoadEntities(wxFileInputStream* infile)
    {
        return this->BSP_Entities.LoadLump(infile,
                                   this->BSP_LumpTable.Lump[LUMP_ENTITIES].Offset,
                                   this->BSP_LumpTable.Lump[LUMP_ENTITIES].Length);
    }

    bool LoadVertices(wxFileInputStream* infile)
    {
        return this->BSP_Vertices.LoadLump(infile,
                                   this->BSP_LumpTable.Lump[LUMP_VERTICES].Offset,
                                   this->BSP_LumpTable.Lump[LUMP_VERTICES].Length);
    }

    bool LoadFaces(wxFileInputStream* infile)
    {
        return this->BSP_Faces.LoadLump(infile,
                                this->BSP_LumpTable.Lump[LUMP_FACES].Offset,
                                this->BSP_LumpTable.Lump[LUMP_FACES].Length);
    }

    bool LoadSurfaceEdges(wxFileInputStream* infile)
    {
        return this->BSP_SurfaceEdges.LoadLump(infile,
                                       this->BSP_LumpTable.Lump[LUMP_SURFACE_EDGES].Offset,
                                       this->BSP_LumpTable.Lump[LUMP_SURFACE_EDGES].Length);
    }

    bool LoadEdges(wxFileInputStream* infile)
    {
        return this->BSP_Edges.LoadLump(infile,
                                this->BSP_LumpTable.Lump[LUMP_EDGES].Offset,
                                this->BSP_LumpTable.Lump[LUMP_EDGES].Length);
    }

    bool LoadTextures(wxFileInputStream* infile)
    {
        return this->BSP_Textures.LoadLump(infile,
                                   this->BSP_LumpTable.Lump[LUMP_TEXTURE_INFORMATION].Offset,
                                   this->BSP_LumpTable.Lump[LUMP_TEXTURE_INFORMATION].Length,
                                   this->baseDir);
    }

    bool LoadBrushModels(wxFileInputStream* infile)
    {
        return this->BSP_BrushModels.LoadLump(infile,
                                      this->BSP_LumpTable.Lump[LUMP_MODELS].Offset,
                                      this->BSP_LumpTable.Lump[LUMP_MODELS].Length);
    }

    bool LoadPlanes(wxFileInputStream* infile)
    {
        return this->BSP_Planes.LoadLump(infile,
                                 this->BSP_LumpTable.Lump[LUMP_PLANES].Offset,
                                 this->BSP_LumpTable.Lump[LUMP_PLANES].Length);
    }

    bool LoadBrushes(wxFileInputStream* infile)
    {
        return this->BSP_Brushes.LoadLump(infile,
                                  this->BSP_LumpTable.Lump[LUMP_BRUSHES].Offset,
                                  this->BSP_LumpTable.Lump[LUMP_BRUSHES].Length);
    }

    bool LoadBrushSides(wxFileInputStream* infile)
    {
        return this->BSP_BrushSides.LoadLump(infile,
                                     this->BSP_LumpTable.Lump[LUMP_BRUSH_SIDES].Offset,
                                     this->BSP_LumpTable.Lump[LUMP_BRUSH_SIDES].Length);
    }

    bool SetupBSPData(QuME_Frame* Frame);
    bool ProcessBrushes(QuME_Frame* Frame);

    wxUint32 GetLumpTableOffset(wxUint32 version);
    void DebugDump(wxTextOutputStream& out);

    QuME_LinkedList<QuME_Vector>* QuickHullVec(QuME_Vector Start, QuME_Vector End, QuME_LinkedList<QuME_Vector>* Right, QuME_Vector& Normal);

    QuME_LinkedList<wxUint32>* QuickHull2D(wxUint32 Start, wxUint32 End, QuME_LinkedList<wxUint32>* Right, QuME_Vector& Normal, wxUint32 BrushID);

    std::wstring baseDir; //used to track down resources
    std::wstring MapName; //the name of this map from load
    wxUint32 IBSPMagic; //this map's 'magic' check value
    wxUint32 BSPVersion; //this map's version number
    wxUint32 EntCount; //how many entities we found on load
    QuME_BSP_Lumps BSP_LumpTable;
    QuME_BSP_Vertices BSP_Vertices; //the shared vertices in this file
    QuME_BSP_Entities BSP_Entities; //the file's entities
    QuME_BSP_Faces BSP_Faces; //the polygons of this map
    QuME_BSP_Edges BSP_Edges;
    QuME_BSP_SurfaceEdges BSP_SurfaceEdges;
    QuME_BSP_Textures BSP_Textures; //texture info
    QuME_BSP_TextureUVs BSP_TextureUVs; //the UV coordinates of our vertices
    QuME_BSP_BrushModels BSP_BrushModels; //the brush models in this map - model #0 is the level itself

    //the original brush data
    QuME_BSP_Planes BSP_Planes;
    QuME_BSP_Brushes BSP_Brushes;
    QuME_BSP_BrushSides BSP_BrushSides;
    QuME_BSP_TextureUVs BSP_BrushSideUVs;
    QuME_BSP_Brush_Vertices BSP_BrushVertices;
    QuME_Bounds BSP_BrushBoundingBox;
};

#endif // QuME_BSP_Data_H_INCLUDED
