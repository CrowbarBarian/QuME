#ifndef QUME_BOUNDS_H
#define QUME_BOUNDS_H

#include "QuME_Common.h"
#include "QuME_Vector.h"
class QuME_Bounds
{
public:
    QuME_Bounds();

    QuME_Vector Min;
    QuME_Vector Max;

    void UpdateBounds(const QuME_Vector& inVector);
    QuME_Bounds forExport();
    bool BoundsSet;
};

std::ostream& operator<<(std::ostream& os, const QuME_Bounds& o);
wxTextOutputStream& operator<<(wxTextOutputStream& tos, const QuME_Bounds& o);

#endif // QUME_BOUNDS_H
