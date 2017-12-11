#ifndef QUME_COMMON_H_INCLUDED
#define QUME_COMMON_H_INCLUDED

#define QUME_VERSION L"0.1"
#define QUME_EPSILON 0.000001
#define QUME_VERTEX_WELD_FACTOR 0.015625 //need to experiment with this factor a little
#define QUME_EXPORTSCALEFACTOR 0.015625
#define QUME_GRIDSNAP 1.0 //remove any slop from vertices
#define QUME_EXPORTGRIDSNAP false //do we snap to grid on export?

//often used header files
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
wxUint32 copys2ws(wxUint8* in, std::wstring *out);

#endif // QUME_COMMON_INCLUDES_H_INCLUDED

