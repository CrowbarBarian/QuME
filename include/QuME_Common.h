#ifndef QUME_COMMON_H_INCLUDED
#define QUME_COMMON_H_INCLUDED

#define QUME_VERSION "0.1"
#define QUME_EPSILON 0.000001
#define QUME_VERTEX_WELD_FACTOR 0.015625 //need to experiment with this factor a little
#define QUME_EXPORTSCALEFACTOR 0.015625
#define QUME_GRIDSNAP 1.0 //remove any slop from vertices
#define QUME_EXPORTGRIDSNAP false //do we snap to grid on export?

//often used header files
#include <math.h>
#include <iostream>
#include <string>
#include <wx/defs.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <wx/txtstrm.h>

//some delete nullptr dereference guards
#define SAFE_DELETE(x) if(x != nullptr) delete x
#define SAFE_ARRAY_DELETE(x) if(x != nullptr) delete[] x

#endif // QUME_COMMON_INCLUDES_H_INCLUDED
