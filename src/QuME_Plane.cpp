/***************************************************************
 * Name:
 * Purpose:
 * Author:    J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * Copyright: J M Thomas (Crowbarbarian) (crowbar.barbarian@gmail.com)
 * License:   GPL v3
 **************************************************************/

#include "QuME_Plane.h"

QuME_Plane::QuME_Plane():Normal(1.0,0.0,0.0)
{
    this->Distance = 0.0;
}

QuME_Plane::QuME_Plane(wxFloat64 a, wxFloat64 b, wxFloat64 c, wxFloat64 d):Normal(a, b, c)
{
    this->Distance = d;
}


QuME_Plane::~QuME_Plane()
{
}

wxFloat64 QuME_Plane::TestSide(const QuME_Vector& v)
{
    return this->Normal.dot(v) - this->Distance;
}

//not my code
//taken from https://en.wikipedia.org/wiki/LU_decomposition

/* INPUT: A - array of pointers to rows of a square matrix having dimension N
 *        Tol - small tolerance number to detect failure when the matrix is near degenerate
 * OUTPUT: Matrix A is changed, it contains both matrices L-E and U as A=(L-E)+U such that P*A=L*U.
 *        The permutation matrix is not stored as a matrix, but in an integer vector P of size N+1
 *        containing column indexes where the permutation matrix has "1". The last element P[N]=S+N,
 *        where S is the number of row exchanges needed for determinant computation, det(P)=(-1)^S
 */
bool LUPDecompose(wxFloat64 A[3][3], wxFloat64 Tol, wxInt32 *P)
{
    wxInt32 i, j, k, imax;
    wxFloat64 maxA, ptr, absA;

    for (i = 0; i <= 3; i++)
        P[i] = i; //Unit permutation matrix, P[N] initialized with N

    for (i = 0; i < 3; i++)
    {
        maxA = 0.0;
        imax = i;

        for (k = i; k < 3; k++)
            if ((absA = fabs(A[k][i])) > maxA)
            {
                maxA = absA;
                imax = k;
            }

        if (maxA < Tol) return false; //failure, matrix is degenerate

        if (imax != i)
        {
            //pivoting P
            j = P[i];
            P[i] = P[imax];
            P[imax] = j;

            //pivoting rows of A
            for(wxUint32 q = 0; q < 3; q++)
			{
				ptr = A[i][q];
				A[i][q] = A[imax][q];
				A[imax][q] = ptr;
			}

            //counting pivots starting from N (for determinant)
            P[3]++;
        }

        for (j = i + 1; j < 3; j++)
        {
            A[j][i] /= A[i][i];

            for (k = i + 1; k < 3; k++)
                A[j][k] -= A[j][i] * A[i][k];
        }
    }

    return true;  //decomposition done
}


/* INPUT: A,P filled in LUPDecompose; b - rhs vector
 * OUTPUT: x - solution vector of A*x=b
 */
void LUPSolve(wxFloat64 A[3][3], wxInt32 *P, wxFloat64 *b, wxFloat64 *x)
{

    for (wxInt32 i = 0; i < 3; i++)
    {
        x[i] = b[P[i]];

        for (wxInt32 k = 0; k < i; k++)
            x[i] -= A[i][k] * x[k];
    }

    for (wxInt32 i = 3 - 1; i >= 0; i--)
    {
        for (wxInt32 k = i + 1; k < 3; k++)
            x[i] -= A[i][k] * x[k];

        x[i] = x[i] / A[i][i];
    }
}

bool QuME_Plane::Intersect(const QuME_Plane& other1, const QuME_Plane& other2, QuME_Vector& v)
{
	//set up initial matrix for solver
    wxFloat64 Matrix[3][3];
    Matrix[0][0] = this->Normal.x;
    Matrix[0][1] = this->Normal.y;
    Matrix[0][2] = this->Normal.z;
    Matrix[1][0] = other1.Normal.x;
    Matrix[1][1] = other1.Normal.y;
    Matrix[1][2] = other1.Normal.z;
    Matrix[2][0] = other2.Normal.x;
    Matrix[2][1] = other2.Normal.y;
    Matrix[2][2] = other2.Normal.z;

    wxFloat64 Vect[3];
    Vect[0] = this->Distance;
    Vect[1] = other1.Distance;
    Vect[2] = other2.Distance;

    wxInt32 P[4];
    if(!LUPDecompose(Matrix, 0.0000001, P))
		return false;

    wxFloat64 vec[3];
    LUPSolve(Matrix, P, Vect, vec);

    v.x = vec[0];
    v.y = vec[1];
    v.z = vec[2];

	return true;
}
