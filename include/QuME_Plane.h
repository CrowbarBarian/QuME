#ifndef QUME_PLANE_H_INCLUDED
#define QUME_PLANE_H_INCLUDED

#include "QuME_Common.h"
#include "QuME_Vector.h"

class QuME_Plane
{
public:
	QuME_Plane();
	QuME_Plane(wxFloat64 a, wxFloat64 b, wxFloat64 c, wxFloat64 d);
	~QuME_Plane();
    //intersect this plane with two others to find a point. Returns false if no valid intersection found.
    bool Intersect(const QuME_Plane& other1, const QuME_Plane& other2, QuME_Vector& v);

    wxFloat64 dot(const QuME_Vector& v);

    //this returns + if above plane, 0 if on plane, and - if below
    wxFloat64 TestSide(const QuME_Vector& v);

    QuME_Vector Normal;
    wxFloat64 Distance;
};

#endif // QUME_PLANE_H_INCLUDED
