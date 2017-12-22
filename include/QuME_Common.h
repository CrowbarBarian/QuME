#ifndef QUME_COMMON_H_INCLUDED
#define QUME_COMMON_H_INCLUDED

#define QUME_VERSION L"0.2.1"
#define QUME_EPSILON 0.015625
#define QUME_VERTEX_WELD_FACTOR 1.0 //need to experiment with this factor a little
#define QUME_EXPORTSCALEFACTOR 0.015625
#define QUME_GRIDSNAP 1.0 //remove any slop from vertices
#define QUME_EXPORTGRIDSNAP false //do we snap to grid on export?
#define QUME_HASHMASK 0xf0
#define QUME_HASHMASKBITCOUNT 4 //how many bits total the mask covers
#define QUME_HASHMASKSHIFT 4 //how many zero bits to shift off to right
#define QUME_HASHMASKRANGE 4096 //how many indices the hash will generate max

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
wxUint32 copys2ws(wxUint8* in, std::wstring& out);

#endif // QUME_COMMON_INCLUDES_H_INCLUDED

