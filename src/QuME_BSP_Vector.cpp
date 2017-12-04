#include "QuME_BSP_Vector.h"


QuME_BSP_Vector::QuME_BSP_Vector(const QuME_BSP_Vector& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

QuME_BSP_Vector& QuME_BSP_Vector::operator+=(const wxFloat32 other)
{
    x += other;
    y += other;
    z += other;

    return *this;
}

QuME_BSP_Vector& QuME_BSP_Vector::operator+=(const QuME_BSP_Vector& other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

QuME_BSP_Vector& QuME_BSP_Vector::operator*=(const wxFloat32 other)
{
    x *= other;
    y *= other;
    z *= other;

    return *this;
}

QuME_BSP_Vector& QuME_BSP_Vector::operator*=(const QuME_BSP_Vector& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;

    return *this;
}

QuME_BSP_Vector QuME_BSP_Vector::operator+(const wxFloat32 o)
{
    QuME_BSP_Vector t(*this);
    t.x = x + o;
    t.y = y + o;
    t.z = z + o;
    return t;
}

QuME_BSP_Vector QuME_BSP_Vector::operator+(const QuME_BSP_Vector& o)
{
    QuME_BSP_Vector t(*this);
    t.x = x + o.x;
    t.y = y + o.y;
    t.z = z + o.z;
    return t;
}

QuME_BSP_Vector QuME_BSP_Vector::operator*(const wxFloat32 o)
{
    QuME_BSP_Vector t(*this);
    t.x = x * o;
    t.y = y * o;
    t.z = z * o;
    return t;
}

QuME_BSP_Vector QuME_BSP_Vector::operator*(const QuME_BSP_Vector& o)
{
    QuME_BSP_Vector t(*this);
    t.x = x * o.x;
    t.y = y * o.y;
    t.z = z * o.z;
    return t;
}

QuME_BSP_Vector QuME_BSP_Vector::cross(const QuME_BSP_Vector& o)
{
    QuME_BSP_Vector t;
    t.x = y * o.z - z * o.y;
    t.y = z * o.x - x * o.z;
    t.z = x * o.y - y * o.z;
    return t;
}

wxFloat32 QuME_BSP_Vector::dot(const QuME_BSP_Vector& o)
{
    return x * o.x + y * o.y + z * o.z;
}
