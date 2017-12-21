#include "QuME_Plane.h"

QuME_Plane::QuME_Plane():Normal(1.0,0.0,0.0)
{
	this->Distance = 0.0;
}

QuME_Plane::~QuME_Plane()
{
}

wxFloat64 QuME_Plane::TestSide(const QuME_Vector& v)
{
    return this->Normal.dot(v) - this->Distance;
}

bool QuME_Plane::Intersect(const QuME_Plane& other1, const QuME_Plane& other2, QuME_Vector& v)
{
    wxInt32 i,j,k;

    wxFloat64 vec[3];

    wxFloat64 Matrix[3][4];
    Matrix[0][0] = this->Normal.x;
    Matrix[0][1] = this->Normal.y;
    Matrix[0][2] = this->Normal.z;
    Matrix[0][3] = this->Distance;
    Matrix[1][0] = other1.Normal.x;
    Matrix[1][1] = other1.Normal.y;
    Matrix[1][2] = other1.Normal.z;
    Matrix[1][3] = other1.Distance;
    Matrix[2][0] = other2.Normal.x;
    Matrix[2][1] = other2.Normal.y;
    Matrix[2][2] = other2.Normal.z;
    Matrix[2][3] = other2.Distance;

    for (i = 0; i < 3; i++)                  //Pivotisation
        for (k = i + 1; k < 3; k++)
            if (fabs(Matrix[i][i]) < fabs(Matrix[k][i]))
                for (j = 0; j < 4; j++) //swap rows
                {
                    wxFloat64 temp = Matrix[i][j];
                    Matrix[i][j] = Matrix[k][j];
                    Matrix[k][j] = temp;
                }


    for(i = 0; i < 3; i++)
    {
        if (Matrix[i][i] == 0.0) return false; //not a valid intersection
        if ((std::isnan(Matrix[i][i])) || (std::isinf(Matrix[i][i])))
        {
            std::cerr << "NaN or Inf found in input matrix!\n";
            return false;
        }
    }

    for (i = 0; i < 2; i++)          //loop to perform the gauss elimination
        for (k = i + 1; k < 3; k++)
        {
            if(Matrix[i][i] == 0.0) return false;
            wxFloat64 t = Matrix[k][i] / Matrix[i][i];
            for (j = 0; j < 4; j++)
                Matrix[k][j] = Matrix[k][j] - t * Matrix[i][j];    //make the elements below the pivot elements equal to zero or eliminate the variables
        }

    for(i = 0; i < 3; i++)
    {
        if (Matrix[i][i] == 0.0) return false; //no intersection
        if ((std::isnan(Matrix[i][i])) || (std::isinf(Matrix[i][i]))) return false;
    }

    for (i = 2; i >= 0; i--)              //back-substitution
    {
        vec[i] = Matrix[i][3];                //make the variable to be calculated equal to the rhs of the last equation
        for (j = i + 1; j < 3; j++)
            if (j != i)            //then subtract all the lhs values except the coefficient of the variable whose value is being calculated
                vec[i] = vec[i] - Matrix[i][j] * vec[j];
        vec[i] = vec[i] / Matrix[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    }
    v.x = vec[0];
    v.y = vec[1];
    v.z = vec[2];

    if((std::isnan(v.x)) || (std::isnan(v.y)) || (std::isnan(v.z)) || (std::isinf(v.x)) || (std::isinf(v.y)) || (std::isinf(v.z)))
    {
        for(i = 0; i < 3; i++)
        {
            std::cerr << Matrix[i][0] << ",\t" << Matrix[i][1] << ",\t" << Matrix[i][2] << ",\t" << Matrix[i][3] << "\n";
        }
        std::cerr << "\n";
        return false; //bad elimination...something screwed up
    }
    return true;
}


