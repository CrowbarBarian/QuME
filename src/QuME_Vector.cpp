#include "QuME_Vector.h"

QuME_Vector::QuME_Vector()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

QuME_Vector::QuME_Vector(wxFloat64 x, wxFloat64 y, wxFloat64 z)
{
    this->x = x;
    this->y = y;
    this->z = z;
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
    return ((x == o.x) && (y == o.y)&& (z == o.z));
}

//test for equality by using a slop factor
bool QuME_Vector::EqualsEpsilon(const QuME_Vector& o, wxFloat64 epsilon)
{
    QuME_Vector t(*this - o);
    if(t.length2() > (epsilon*epsilon)) return false;
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
    return x * o.x + y * o.y + z * o.z;
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

void QuME_Vector::DebugDump(wxTextOutputStream& out)
{
    out << L"X: " << this->x << L", Y: " << this->y << L", Z: " << this->z << L"\n";
}


std::ostream& operator<<(std::ostream& os, const QuME_Vector& o)
{
    // write obj to stream
    os << o.x << L" " << o.y << L" " << o.z;
    return os;
}
