#pragma once
#include "component.h"
#include "solver.h"
#include<iostream>
#include<vector>
#include<iomanip>

using namespace std;

//...........................................................
//Generating the 'A' matrix requires 4 different matrix.
//They are G,B,C & D matrix
//we generate them one after other.
//Finally we put togather all of them in a single 'A' martix
//...........................................................

//produces the G matrix
//which is a part of 'A' matrix.
//Returns the pointer of the matrix

class Matrix
{
	bool log_on;
	int a_mat_len;

public:

	Matrix()
	{
		a_mat_len = 0;
		log_on = false;
	}


	void Toggle_log(bool k)
	{
		//prints all info if true
		log_on = k;
	}

private:

	void log(string s)
	{
		if (log_on)cout << s << endl;
	}

	//G matrix...duh
	float** G_matrix(vector<Nodes> &n)
	{
		float** gm;
		int len = (int)n.size();
		
		log("Generating G matrix");

		gm = new float* [len - 1];

		for (int i = 1; i < len; i++)
		{
			gm[i - 1] = new float[len - 1];

			for (int j = 1; j < len; j++)
			{
				gm[i - 1][j - 1] = 0;
			}

			gm[i - 1][i - 1] = n.at(i).r_inverse();

			Nodes temp_n = n.at(i);
			int r_count = temp_n.rs.size();
			for (int j = 0; j < r_count; j++)
			{
				int index = temp_n.rs.at(j).getOther(i) - 1;
				if (index >= 0 && index != i-1)gm[i - 1][index] -= 1 / temp_n.rs.at(j).value;
			}
		}

		//If log on is true,prints the values of this matrix
		//usefull for debugging

		if (log_on) {
			cout << "\nG matrix:" << endl;
			for (int i = 0; i < len - 1; i++)
			{
				for (int j = 0; j < len - 1; j++)
				{
					cout << setw(5) <<gm[i][j] << "  ";
				}
				cout << endl;
			}
		}
		return gm;
	}


	//rest of the code works the same.


	float** B_matrix(vector<Nodes> &n, vector<Voltage> &v)
	{
		float** bm;
		int row = (int)n.size();
		int col = (int)v.size();

		log("Generating B matrix");

		bm = new float* [row - 1];
		for (int i = 1; i < row; i++)
		{
			bm[i - 1] = new float[col];

			for (int j = 0; j < col; j++)
			{
				bm[i - 1][j] = v.at(j).pin_polarity(i);
			}
		}

		if (log_on)
		{
			cout << "\nB matrix:" << endl;
			for (int i = 0; i < row - 1; i++)
			{
				for (int j = 0; j < col; j++)
				{
					cout << bm[i][j] << "  ";
				}
				cout << endl;
			}
		}

		return bm;
	}

	float** C_matrix(vector<Nodes> &n, vector<Voltage> &v)
	{
		float** cm;
		int row = (int)v.size();
		int col = (int)n.size();

		log("Generating C matrix");
		cm = new float* [row - 1];
		for (int i = 0; i < row; i++)
		{
			cm[i] = new float[col];

			for (int j = 1; j < col; j++)
			{
				cm[i][j - 1] = v.at(i).pin_polarity(j);
			}
		}

		if (log_on)
		{
			cout << "\nC matrix:" << endl;
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col - 1; j++)
				{
					cout << cm[i][j] << "  ";
				}
				cout << endl;
			}
		}

		return cm;
	}

	float** D_matrix(vector<Voltage> &v)
	{
		int len = v.size();
		float** dm;
		dm = new float* [len];
		log("Generating D matrix");
		for (int i = 0; i < len; i++)
		{
			dm[i] = new float[len];
			for (int j = 0; j < len; j++)
			{
				dm[i][j] = 0;

			}
		}

		if (log_on)
		{
			cout << "\nD matrix:" << endl;
			for (int i = 0; i < len; i++)
			{
				for (int j = 0; j < len; j++)
				{
					cout << dm[i][j] << "  ";
				}
				cout << endl;
			}
		}

		return dm;
	}

	//Here we actually put togather all matrices to form A

public:

	float** A_matrix(vector<Nodes> &n, vector<Voltage> &v)
	{
		float** am;
		float** gm, ** bm, ** cm, ** dm;
		
		log("Generating A matrix");

		int nlen = n.size() - 1;
		int vlen = v.size();
		am = new float* [nlen + vlen];

		for (int i = 0; i < nlen + vlen; i++)
		{
			am[i] = new float[nlen + vlen];
		}

		gm = G_matrix(n);
		bm = B_matrix(n, v);
		cm = C_matrix(n, v);
		dm = D_matrix(v);

		for (int i = 0; i < nlen; i++)
		{
			for (int j = 0; j < nlen; j++)
			{
				am[i][j] = gm[i][j];
			}
			delete[] gm[i];
		}

		for (int i = 0; i < nlen; i++)
		{
			for (int j = 0; j < vlen; j++)
			{
				am[i][j + nlen] = bm[i][j];
			}
			delete[] bm[i];
		}

		for (int i = 0; i < vlen; i++)
		{
			for (int j = 0; j < nlen; j++)
			{
				am[i + nlen][j] = cm[i][j];
			}
			delete[] cm[i];
		}

		for (int i = 0; i < vlen; i++)
		{
			for (int j = 0; j < vlen; j++)
			{
				am[i + nlen][j + nlen] = dm[i][j];
			}
			delete[] dm[i];
		}

		if (log_on) 
		{
			cout << "A matrix:\n";
			for (int i = 0; i < vlen + nlen; i++)
			{
				for (int j = 0; j < vlen + nlen; j++)
				{
					cout << setw(4) << setprecision(3) << am[i][j] << "   ";
				}
				cout << endl;
			}
		}
		return am;
	}

	//Generating 'z' matrix

	float* Z_matrix(vector<Nodes> &n, vector<Voltage> &v)
	{
		log("Generating Z matrix");

		int nlen = (int)n.size();
		int vlen = (int)v.size();
		float* zm = new float[nlen + vlen - 1];
		for (int i = 1; i < nlen; i++)
		{
			zm[i - 1] = n.at(i).total_current(i);
		}
		for (int i = 0; i < vlen; i++)
		{
			zm[i + nlen - 1] = v.at(i).value;
		}

		if (log_on) 
		{
			cout << "\nZ matrix" << endl;
			for (int i = 0; i < nlen + vlen - 1; i++)
			{
				cout << zm[i] << endl;
			}
		}
		return zm;
	}
};
