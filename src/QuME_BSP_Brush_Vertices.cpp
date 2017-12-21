#include "QuME_BSP_Brush_Vertices.h"

QuME_BSP_Brush_Vertices::QuME_BSP_Brush_Vertices()
{
	Vertices = nullptr;
	Count = 0;
}

QuME_BSP_Brush_Vertices::~QuME_BSP_Brush_Vertices()
{
	Count = 0;
	SAFE_ARRAY_DELETE(Vertices);
}
