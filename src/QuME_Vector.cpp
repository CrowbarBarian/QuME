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

#if 0
bool QuME_Vector::operator==(const QuME_Vector& o)
{
    return ((x == o.x) && (y == o.y)&& (z == o.z));
}
#endif // 0

//test for equality by using a slop factor
bool QuME_Vector::operator==(const QuME_Vector& o)
{
    QuME_Vector t(*this - o);
    if(t.length2() > (QUME_VERTEX_WELD_FACTOR*QUME_VERTEX_WELD_FACTOR)) return false;
    return true;
}

#if 0
wxUint32 QuME_Vector::gethash()
{
    wxUint32 xi = std::fabs(std::trunc(this->x));
    wxUint32 yi = std::fabs(std::trunc(this->y));
	wxUint32 zi = std::fabs(std::trunc(this->z));

	wxUint32 result = xi & QUME_HASHMASK >> QUME_HASHMASKSHIFT;
	result |= (yi & QUME_HASHMASK) << QUME_HASHMASKBITCOUNT;
	result |= (zi & QUME_HASHMASK) << QUME_HASHMASKBITCOUNT;

	return result; //should be between 0 and 2^(QUME_HASHMASKSHIFT * 3) - 1
}
#endif // 0

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
