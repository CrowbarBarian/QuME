#ifndef QUME_BSPVECTOR_H
#define QUME_BSPVECTOR_H

#include "QuME_Common.h"

class QuME_Vector
{
public:
    QuME_Vector();
    QuME_Vector(wxFloat64 x, wxFloat64 y, wxFloat64 z);
    QuME_Vector(const QuME_Vector& o);
    QuME_Vector& operator+=(const wxFloat64 o);
    QuME_Vector& operator+=(const QuME_Vector& o);
    QuME_Vector& operator-=(const wxFloat64 o);
    QuME_Vector& operator-=(const QuME_Vector& o);
    QuME_Vector& operator*=(const wxFloat64 o);
    QuME_Vector& operator*=(const QuME_Vector& o);
    QuME_Vector& operator/=(const QuME_Vector& o);
    QuME_Vector& operator/=(const wxFloat64& o);
    QuME_Vector operator+(const wxFloat64 o);
    QuME_Vector operator+(const QuME_Vector& o);
    QuME_Vector operator-(const wxFloat64 o);
    QuME_Vector operator-(const QuME_Vector& o);
    QuME_Vector operator*(const wxFloat64 o);
    QuME_Vector operator*(const QuME_Vector& o);
    QuME_Vector operator/(const wxFloat64 o);
    QuME_Vector operator/(const QuME_Vector& o);
    bool operator==(const QuME_Vector& o);
    //test for equality by using a slop factor
    //bool EqualsEpsilon(const QuME_Vector& o, wxFloat64 epsilon);
    bool operator!=(const QuME_Vector& o);
    //wxUint32 gethash();
    QuME_Vector cross(const QuME_Vector& o);
    wxFloat64 dot(const QuME_Vector& o);
    wxFloat64 length2();
    wxFloat64 length();
    QuME_Vector& normalize();
    QuME_Vector forExport();
    void DebugDump(wxTextOutputStream& out);

	wxFloat64 x;
	wxFloat64 y;
	wxFloat64 z;
};

std::ostream& operator<<(std::ostream& os, const QuME_Vector& o);
wxTextOutputStream& operator<<(wxTextOutputStream& tos, const QuME_Vector& o);

#endif // QUME_BSP_VECTOR_H
