/***************************************************************
 * Name:		QuME_Common.h
 * Purpose:		Defines common utility procedures as well as common includes
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#ifndef QUME_COMMON_H_INCLUDED
#define QUME_COMMON_H_INCLUDED

//some basic constants
#define QUME_EPSILON 0.015625
#define QUME_UV_EPSILON 0.000025 //tweak this...
#define QUME_VERTEX_WELD_FACTOR 1.0 //need to experiment with this factor a little
#define QUME_EXPORTSCALEFACTOR 0.015625
#define QUME_GRIDSNAP 1.0 //remove any slop from vertices
#define QUME_EXPORTGRIDSNAP false //do we snap to grid on export?

//often used header files
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <string>
#include <locale>
#include <algorithm>
#include <wx/defs.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <wx/txtstrm.h>

//some delete nullptr dereference guards
#define SAFE_DELETE(x) if(x != nullptr) delete x
#define SAFE_ARRAY_DELETE(x) if(x != nullptr) delete[] x

void ReplaceAll(std::wstring *str, const std::wstring& from, const std::wstring& to);
wxUint32 copys2ws(wxUint8* in, std::wstring& out);

#endif // QUME_COMMON_INCLUDES_H_INCLUDED

