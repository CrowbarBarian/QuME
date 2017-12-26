/***************************************************************
 * Name:		QuME_BSP_Brush_Vertices.cpp
 * Purpose:		Class to hold BSP brush vertex data
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

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
