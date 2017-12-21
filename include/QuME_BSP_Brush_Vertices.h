#ifndef QUME_BSP_BRUSH_VERTICES_H
#define QUME_BSP_BRUSH_VERTICES_H

#include "QuME_Common.h"
#include "QuME_Lists.h"
#include "QuME_Vector.h"

class QuME_BSP_Brush_Vertices
{
public:
	QuME_BSP_Brush_Vertices();
	~QuME_BSP_Brush_Vertices();

	QuME_Vector* Vertices;
	wxUint32 Count;
	QuME_LinkedList<QuME_Vector> VertexList;
};

#endif // QUME_BSP_BRUSH_VERTICES_H
