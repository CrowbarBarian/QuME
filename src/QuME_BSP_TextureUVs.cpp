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
