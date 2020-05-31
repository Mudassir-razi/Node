#pragma once
#include<iostream>
#include "helper.h"
#include "component.h"


void Gauss_elem(float** a, int n)
{
    //n is the number of unknown variables
    int ppoint = 0;
    for (int k = 0; k < n - 1; k++)
    {
        if (a[k][ppoint] == 0)
        {
            for (int t = 0; t < n; t++)
            {
                float temp = a[k][t];
                a[k][t] = a[k + 1][t];
                a[k + 1][t] = temp;
            }
        }
        for (int row = k + 1; row < n; row++)
        {
            float factor = (float)a[row][ppoint] / a[k][ppoint];
            for (int l = 0; l < n; l++)
            {
                a[row][l] -= a[k][l] * factor;
            }
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
                cout << a[i][j] << "  ";
            }
            cout << endl;
        }
    }

}