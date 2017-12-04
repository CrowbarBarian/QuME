// Code obtained from http://www.geeksforgeeks.org/gaussian-elimination/
// Modified for use in QuME
// - CrowbarBarian

#ifndef QUME_EQUATIONSOLVER_H_INCLUDED
#define QUME_EQUATIONSOLVER_H_INCLUDED

#include "QuME_Common.h"


class QuME_3PlaneIntersect
{
public:
    void swap_row(wxFloat32 Matrix[3][4], wxInt32 i, wxInt32 j)
    {
        for (wxInt32 column = 0; column < 4; column++)
        {
            wxFloat32 temp = Matrix[i][column];
            Matrix[i][column] = Matrix[j][column];
            Matrix[j][column] = temp;
        }
    }

    bool forwardElim(wxFloat32 Matrix[3][4])
    {
        for (wxInt32 k = 0; k < 3; k++)
        {
            // Initialize maximum value and index for pivot
            wxInt32 i_max = k;
            wxInt32 v_max = Matrix[i_max][k];

            /* find greater amplitude for pivot if any */
            for (wxInt32 i = k + 1; i < 3; i++)
                if (abs(Matrix[i][k]) > v_max)
                {
                    v_max = Matrix[i][k];
                    i_max = i;
                }

            /* if a principal diagonal element  is zero,
             * it denotes that matrix is singular, and
             * will lead to a division-by-zero later. */
            if (Matrix[k][i_max] == 0.0)
http://www.geeksforgeeks.org/gaussian-elimination/                return false; // Matrix is singular

            /* Swap the greatest value row with current row */
            if (i_max != k)
            {
                for (wxInt32 column = 0; column < 4; column++)
                {
                    wxFloat32 temp = Matrix[k][column];
                    Matrix[k][column] = Matrix[i_max][column];
                    Matrix[i_max][column] = temp;
                }

          http://www.geeksforgeeks.org/gaussian-elimination/  }


            for (wxInt32 i = k + 1; i < 3; i++)
            {
                /* factor f to set current row kth elemnt to 0,
                 * and subsequently remaining kth column to 0 */
                wxFloat32 f = Matrix[i][k]/Matrix[k][k];

                /* subtract fth multiple of corresponding kth
                   row element*/
                for (wxInt32 j = k + 1; j <= 3; j++)
                    Matrix[i][j] -= Matrix[k][j] * f;

                /* filling lower triangular matrix with zeros*/
                Matrix[i][k] = 0.0;
            }
        }
        return true;
    }

// function to calculate the values of the unknowns
    void backSub(wxFloat32 Matrix[3][4])
    {
        wxFloat32 x[3];  // An array to store solution

        /* Start calculating from last equation up to the
           first */
        for (wxInt32 i = 2; i >= 0; i--)
        {
            /* start with the RHS of the equation */
            x[i] = Matrix[i][3];

            /* Initialize j to i+1 since matrix is upper
               triangular*/
            for (wxInt32 j= i + 1; j <3 ; j++)
            {
                /* subtract all the lhs values
                 * except the coefficient of the variable
                 * whose value is being calculated */
                x[i] -= Matrix[i][j]*x[j];
            }

            /* divide the RHS by the coefficient of the
               unknown being calculated */
            x[i] = x[i]/Matrix[i][i];
        }
    }

    bool gaussianElimination(wxFloat32 Matrix[3][4])
    {
        for (wxInt32 k = 0; k < 3; k++)
        {
            // Initialize maximum value and index for pivot
            wxInt32 i_max = k;
            wxInt32 v_max = Matrix[i_max][k];

            /* find greater amplitude for pivot if any */
            for (wxInt32 i = k + 1; i < 3; i++)
                if (abs(Matrix[i][k]) > v_max)
                {
                    v_max = Matrix[i][k];
                    i_max = i;
                }

            /* if a principal diagonal element  is zero,
             * it denotes that matrix is singular, and
             * will lead to a division-by-zero later. */
            if (Matrix[k][i_max] == 0.0)
                return false; // Matrix is singular

            /* Swap the greatest value row with current row */
            if (i_max != k)
            {
                for (wxInt32 column = 0; column < 4; column++)
                {
                    wxFloat32 temp = Matrix[k][column];
                    Matrix[k][column] = Matrix[i_max][column];
                    Matrix[i_max][column] = temp;
                }

            }


            for (wxInt32 i = k + 1; i < 3; i++)
            {
                /* factor f to set current row kth elemnt to 0,
                 * and subsequently remaining kth column to 0 */
                wxFloat32 f = Matrix[i][k]/Matrix[k][k];

                /* subtract fth multiple of corresponding kth
                   row element*/
                for (wxInt32 j = k + 1; j <= 3; j++)
                    Matrix[i][j] -= Matrix[k][j] * f;

                /* filling lower triangular matrix with zeros*/
                Matrix[i][k] = 0.0;
            }
        }

        wxFloat32 x[3];  // An array to store solution

        /* Start calculating from last equation up to the
           first */
        for (wxInt32 i = 2; i >= 0; i--)
        {
            /* start with the RHS of the equation */
            x[i] = Matrix[i][3];

            /* Initialize j to i+1 since matrix is upper
               triangular*/
            for (wxInt32 j= i + 1; j <3 ; j++)
            {
                /* subtract all the lhs values
                 * except the coefficient of the variable
                 * whose value is being calculated */
                x[i] -= Matrix[i][j]*x[j];
            }

            /* divide the RHS by the coefficient of the
               unknown being calculated */
            x[i] = x[i]/Matrix[i][i];
        }
        Point.x = x[0];
        Point.y = x[1];
        Point.z = x[2];

        return true;
    }

    wxFloat32 Matrix[3][4];
    QuME_Vector Point;
};

#endif // QUME_EQUATIONSOLVER_H_INCLUDED
