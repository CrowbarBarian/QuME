/***************************************************************
 * Name:		QuME_Vector.cpp
 * Purpose:		Code for the basic 3d vector class
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_Vector.h"

QuME_Vector::QuME_Vector()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

QuME_Vector::QuME_Vector(wxFloat64 inx, wxFloat64 iny, wxFloat64 inz)
{
    this->x = inx;
    this->y = iny;
    this->z = inz;
}

QuME_Vector::QuME_Vector(const QuME_Vector& o)
{
    x = o.x;
    y = o.y;
    z = o.z;
}

QuME_Vector& QuME_Vector::operator+=(const wxFloat64 o)
{
    x += o;
    y += o;
    z += o;

    return *this;
}

QuME_Vector& QuME_Vector::operator+=(const QuME_Vector& o)
{
    x += o.x;
    y += o.y;
    z += o.z;

    return *this;
}

QuME_Vector& QuME_Vector::operator-=(const wxFloat64 o)
{
    x -= o;
    y -= o;
    z -= o;

    return *this;
}

QuME_Vector& QuME_Vector::operator-=(const QuME_Vector& o)
{
    x -= o.x;
    y -= o.y;
    z -= o.z;

    return *this;
}

QuME_Vector& QuME_Vector::operator*=(const wxFloat64 o)
{
    x *= o;
    y *= o;
    z *= o;

    return *this;
}

QuME_Vector& QuME_Vector::operator*=(const QuME_Vector& o)
{
    x *= o.x;
    y *= o.y;
    z *= o.z;

    return *this;
}

QuME_Vector& QuME_Vector::operator/=(const QuME_Vector& o)
{
    x /= o.x;
    y /= o.y;
    z /= o.z;

    return *this;
}

QuME_Vector& QuME_Vector::operator/=(const wxFloat64& o)
{
    wxFloat64 t = 1.0 / o;

    x *= t;
    y *= t;
    z *= t;

    return *this;
}

QuME_Vector QuME_Vector::operator+(const wxFloat64 o)
{
    QuME_Vector t(*this);
    t.x = x + o;
    t.y = y + o;
    t.z = z + o;
    return t;
}

QuME_Vector QuME_Vector::operator+(const QuME_Vector& o)
{
    QuME_Vector t(*this);
    t.x = x + o.x;
    t.y = y + o.y;
    t.z = z + o.z;
    return t;
}

QuME_Vector QuME_Vector::operator-(const wxFloat64 o)
{
    QuME_Vector t(*this);
    t.x = x - o;
    t.y = y - o;
    t.z = z - o;
    return t;
}

QuME_Vector QuME_Vector::operator-(const QuME_Vector& o)
{
    QuME_Vector t(*this);
    t.x = x - o.x;
    t.y = y - o.y;
    t.z = z - o.z;
    return t;
}

QuME_Vector QuME_Vector::operator*(const wxFloat64 o)
{
    QuME_Vector t(*this);
    t.x = x * o;
    t.y = y * o;
    t.z = z * o;
    return t;
}

QuME_Vector QuME_Vector::operator*(const QuME_Vector& o)
{
    QuME_Vector t(*this);
    t.x = x * o.x;
    t.y = y * o.y;
    t.z = z * o.z;
    return t;
}

QuME_Vector QuME_Vector::operator/(const wxFloat64 o)
{
    QuME_Vector t(*this);
    t.x = x / o;
    t.y = y / o;
    t.z = z / o;
    return t;
}

QuME_Vector QuME_Vector::operator/(const QuME_Vector& o)
{
    QuME_Vector t(*this);
    t.x = x / o.x;
    t.y = y / o.y;
    t.z = z / o.z;
    return t;
}

bool QuME_Vector::operator==(const QuME_Vector& o)
{
    QuME_Vector t(*this - o);
    if(t.length2() > (QUME_VERTEX_WELD_FACTOR*QUME_VERTEX_WELD_FACTOR)) return false;
    return true;
}

bool QuME_Vector::operator!=(const QuME_Vector& o)
{
    return !(*this == o);
}

QuME_Vector QuME_Vector::cross(const QuME_Vector& o)
{
    QuME_Vector t(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
    return t;
}

wxFloat64 QuME_Vector::dot(const QuME_Vector& o)
{
    return this->x * o.x + this->y * o.y + this->z * o.z;
}

wxFloat64 QuME_Vector::length2()
{
    return x * x + y * y + z * z;
}

wxFloat64 QuME_Vector::length()
{
    return sqrt(this->length2());
}

QuME_Vector& QuME_Vector::normalize()
{
    *this /= this->length();

    return *this;
}

QuME_Vector QuME_Vector::forExport()
{
	QuME_Vector t(this->x * -QUME_EXPORTSCALEFACTOR, this->z * QUME_EXPORTSCALEFACTOR, this->y * QUME_EXPORTSCALEFACTOR);
	return t;
}

void QuME_Vector::DebugDump(wxTextOutputStream& out)
{
    out << L"X: " << this->x << L", Y: " << this->y << L", Z: " << this->z << L"\n";
}


std::ostream& operator<<(std::ostream& os, const QuME_Vector& o)
{
    // write obj to stream
    os << o.x << " " << o.y << " " << o.z;
    return os;
}

wxTextOutputStream& operator<<(wxTextOutputStream& tos, const QuME_Vector& o)
{
    // write obj to stream
    tos << o.x << " " << o.y << " " << o.z;
    return tos;
}
