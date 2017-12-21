#include "QuME_Bounds.h"

QuME_Bounds::QuME_Bounds()
:Min(100000.0, 100000.0, 100000.0), Max(-100000.0, -100000.0, -100000.0)
{
}

void QuME_Bounds::UpdateBounds(const QuME_Vector& inVector)
{
	if(inVector.x < this->Min.x) this->Min.x = inVector.x;
	if(inVector.y < this->Min.y) this->Min.y = inVector.y;
	if(inVector.z < this->Min.z) this->Min.z = inVector.z;
	if(inVector.x > this->Max.x) this->Max.x = inVector.x;
	if(inVector.y > this->Max.y) this->Max.y = inVector.y;
	if(inVector.z > this->Max.z) this->Max.z = inVector.z;
}

QuME_Bounds QuME_Bounds::forExport()
{
	QuME_Bounds t;

	t.Min.x = -this->Max.x * QUME_EXPORTSCALEFACTOR;
	t.Min.y = this->Min.z * QUME_EXPORTSCALEFACTOR;
	t.Min.z = this->Min.y * QUME_EXPORTSCALEFACTOR;
	t.Max.x = -this->Min.x * QUME_EXPORTSCALEFACTOR;
	t.Max.y = this->Max.z * QUME_EXPORTSCALEFACTOR;
	t.Max.z = this->Max.y * QUME_EXPORTSCALEFACTOR;

	return t;
}

std::ostream& operator<<(std::ostream& os, const QuME_Bounds& o)
{
    // write obj to stream
    os << L"Min: " << o.Min.x << " " << o.Min.y << " " << o.Min.z;
    os << L" Max: " << o.Max.x << " " << o.Max.y << " " << o.Max.z;

    return os;
}

wxTextOutputStream& operator<<(wxTextOutputStream& tos, const QuME_Bounds& o)
{
    // write obj to stream
    tos << L"Min: " << o.Min.x << " " << o.Min.y << " " << o.Min.z;
    tos << L" Max: " << o.Max.x << " " << o.Max.y << " " << o.Max.z;

    return tos;
}
