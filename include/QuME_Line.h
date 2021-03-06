#ifndef QUME_LINE_H
#define QUME_LINE_H

#include "QuME_Common.h"
#include "QuME_Vector.h"

class QuME_Line
{
public:
    QuME_Line();
    QuME_Line(QuME_Vector& line_start, QuME_Vector& line_end);
    ~QuME_Line();

    wxFloat64 distanceto(QuME_Vector& in);

    QuME_Vector Origin;
    QuME_Vector Direction;
};

#endif // QUME_LINE_H
