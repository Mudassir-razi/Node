#pragma once
#include<iostream>
#include<iomanip>
#include "helper.h"
#include "component.h"

class Solver
{

private:
    int len;
    bool log_on;

public:

    Solver()
    {
        len = 0;
        log_on = false;
    }

    void Toggle_log(bool k)
    {
        log_on = k;
    }

private:

    void Gauss_elem(float** a, float* z, int n)
    {
        //n is the number of unknown variables
        int ppoint = 0;
        for (int k = 0; k < n - 1; k++)
        {
            //altering rows if pivot point is zero
            if (a[k][ppoint] == 0)
            {
                float temp;

                for (int t = 0; t < n; t++)
                {
                    temp = a[k][t];
                    a[k][t] = a[k + 1][t];
                    a[k + 1][t] = temp;
                }

                temp = z[k];
                z[k] = z[k + 1];
                z[k + 1] = temp;
            }
            for (int row = k + 1; row < n; row++)
            {
                float factor = (float)a[row][ppoint] / a[k][ppoint];
                for (int l = 0; l < n; l++)
                {
                    a[row][l] -= a[k][l] * factor;
                }
                z[row] -= z[k] * factor;
            }
            ppoint++;
            if (ppoint > n - 2)break;
        }

        if (log_on)
        {
            cout << "\nAfter gauss elimination\n" << endl;

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    cout << setw(5) << a[i][j] << "  ";
                }
                cout <<setw(5) <<"|"<< z[i];
                cout << endl;
            }
        }

    }

public:

    float* solution(float** a, float* z, int n)
    {
        len = n;
        int ppoint = len - 1;
        float* sol = new float[len];

        Gauss_elem(a, z, len);

        for (int i = 0; i < len; i++)
        {
            sol[i] = 0;
        }

        float summation;
        for (int i = len - 1; i >= 0; i--)
        {
            summation = 0;
            for (int j = 0; j < len; j++)
            {
                if (j == ppoint)continue;
                summation += sol[j] * a[i][j];
            }
            sol[ppoint] = (z[i] - summation) / a[i][ppoint];
            ppoint--;
        }

        if (log_on)
        {
            for (int i = 0; i < len; i++)
            {
                cout << "solution: " << sol[i] << endl;
            }
        }
        return sol;
    }
};
