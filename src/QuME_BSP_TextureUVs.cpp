/***************************************************************
 * Name:		QuME_BSP_TextureUVs.cpp
 * Purpose:		BSP Texture UV class
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_BSP_TextureUVs.h"

QuME_BSP_TextureUVs::QuME_BSP_TextureUVs()
{
    UVArrayCount = 0;
    UVArray = nullptr;
}

QuME_BSP_TextureUVs::~QuME_BSP_TextureUVs()
{
    SAFE_ARRAY_DELETE(UVArray);
    UVArrayCount = 0;
}
