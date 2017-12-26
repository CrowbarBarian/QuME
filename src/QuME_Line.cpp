/***************************************************************
 * Name:		QuME_Line.cpp
 * Purpose:		Code for 3d lines
 * Author:		J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright:	J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:		GPL v3
 **************************************************************/

#include "QuME_Line.h"

QuME_Line::QuME_Line()
{
    //ctor
}

QuME_Line::~QuME_Line()
{
    //dtor
}

wxFloat64 QuME_Line::distanceto(QuME_Vector& in)
{
    QuME_Vector v = this->Origin - in;
    wxFloat64 t = v.dot(this->Direction);
    QuME_Vector P = this->Origin + (this->Direction * t);
    QuME_Vector Q = in - P;
    return Q.length();
}
