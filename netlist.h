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
	int g_mat_len;
	int b_mat_lenx;
	int b_mat_leny;
	int c_mat_lenx;
	int c_mat_leny;
	int d_mat_len;
	int z_mat_len;

	double** am;
	double** gm, ** bm, ** cm, ** dm;
	double* zm, * sol;
	double del_time;

	bool initialized;

	Solver solve;
	Helper logger;

public:

	Matrix()
	{
		a_mat_len = 0;
		g_mat_len = 0;
		b_mat_lenx = 0;
		b_mat_leny = 0;
		c_mat_lenx = 0;
		c_mat_leny = 0;
		d_mat_len = 0;
		z_mat_len = 0;
		log_on = false;

		am = nullptr;
		gm = nullptr;
		bm = nullptr;
		cm = nullptr;
		dm = nullptr;
		zm = nullptr;
		sol = nullptr;
		del_time = 0;
		initialized = false;
		solve.Toggle_log(false);
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

	double** Gen_matrix(int size)
	{
		double** mat = new double* [size];
		for (int i = 0; i < size; i++)
		{
			mat[i] = new double [size];
			for (int j = 0; j < size; j++)
			{
				mat[i][j] = 0;
			}
		}
		return mat;
	}

	double** Gen_matrix(int sizex, int sizey)
	{
		double** mat = new double* [sizex];
		for (int i = 0; i < sizex; i++)
		{
			mat[i] = new double[sizey];
			for (int j = 0; j < sizey; j++)
			{
				mat[i][j] = 0;
			}
		}
		return mat;
	}

	double* Gen_matrix1D(int size)
	{
		double* mat = new double[size];
		for (int i = 0; i < size; i++)
		{
			mat[i] = 0;
		}
		return mat;
	}

	void copy_matrix(double** to, double** from, int offsetx, int offsety, int sizex, int sizey)
	{
		for (int i = 0; i < sizex; i++)
		{
			for (int j = 0; j < sizey; j++)
			{
				to[i + offsetx][j + offsety] = from[i][j];
			}
			delete[] from[i];
		}
	}

	double** duplicate_matrix(double** from, int sizex, int sizey)
	{
		double** mat;
		mat = new double* [sizex];
		for (int i = 0; i < sizex; i++)
		{
			mat[i] = new double[sizey];
			for (int j = 0; j < sizey; j++)
			{
				mat[i][j] = from[i][j];
			}
		}
		return mat;
	}

	double* duplicate_matrix(double* from, int size)
	{
		double* mat;
		mat = new double[size];
		for (int i = 0; i < size; i++)
		{
			mat[i] = from[i];
		}
		return mat;
	}

	void free_matrix(double** m, int sizex)
	{
		for(int i = 0; i < sizex; i++)
		{
			delete[] m[i];
		}
	}

	template <typename T>
	void update_matrix(vector<T>* comp)
	{
		int len = comp->size();
		for (int i = 0; i < len; i++)
		{
			comp->at(i).update_matrix_G(gm);
			comp->at(i).update_matrix_B(bm);
			comp->at(i).update_matrix_C(cm);
			comp->at(i).update_matrix_D(dm);
			comp->at(i).update_matrix_Z(zm);
		}
	}

	template <typename T>
	void update_transient(vector<T>* comp)
	{
		int len = (int)comp->size();
		for (int i = 0; i < len; i++)
		{
			int shift = comp->at(i).xm_shift;
			comp->at(i).update_values(sol[shift] * del_time);
			comp->at(i).update_matrix_Z(zm);
		}
	}

	//G matrix...duh
	//Here we actually put togather all matrices to form A

public:

	double* Solution_generator(Helper myhelper)
	{
		//calculating matrix sizes
		//Edit this part if you add new components
		g_mat_len = myhelper.nos->size() - 1;
		b_mat_lenx = g_mat_len;
		b_mat_leny = myhelper.ves->size() + myhelper.vdvses->size() + myhelper.capses->size();
		c_mat_lenx = b_mat_leny;
		c_mat_leny = b_mat_lenx;
		d_mat_len = b_mat_leny;
		a_mat_len = g_mat_len + c_mat_lenx;
		z_mat_len = g_mat_len + c_mat_lenx;

		am = Gen_matrix(a_mat_len);
		gm = Gen_matrix(g_mat_len);
		bm = Gen_matrix(b_mat_lenx, b_mat_leny);
		cm = Gen_matrix(c_mat_lenx, c_mat_leny);
		dm = Gen_matrix(d_mat_len);
		zm = Gen_matrix1D(z_mat_len);
		
		//Updating all matrices
		update_matrix<Resistor>(myhelper.res);
		update_matrix<Voltage>(myhelper.ves);
		update_matrix<Current>(myhelper.ies);
		update_matrix<Vdvs>(myhelper.vdvses);
		update_matrix<Vdcs>(myhelper.vdcses);
		update_matrix<Capacitor>(myhelper.capses);

		copy_matrix(am, gm, 0, 0, g_mat_len, g_mat_len);
		copy_matrix(am, bm, 0, g_mat_len, b_mat_lenx, b_mat_leny);
		copy_matrix(am, cm, g_mat_len, 0, c_mat_lenx, c_mat_leny);
		copy_matrix(am, dm, g_mat_len, g_mat_len, d_mat_len, d_mat_len);

		log("A matrix:");
		if(log_on)myhelper.log_matrix(am, a_mat_len);
		log("Z matrix:");
		if(log_on)myhelper.log_matrix(zm, z_mat_len);

		double** temp_am;
		double* temp_zm;
		temp_am = duplicate_matrix(am, a_mat_len, a_mat_len);
		temp_zm = duplicate_matrix(zm, z_mat_len);

		sol = solve.solution(temp_am, temp_zm, a_mat_len);
		log("Solution: ");
		if (log_on)myhelper.log_matrix(sol, z_mat_len);
		update_transient(myhelper.capses);
		initialized = true;
		return sol;
	}

	double* transient(Helper myhelper, double dt)
	{
		del_time = dt;
		if (!initialized)
		{
			Solution_generator(myhelper);
			update_transient(myhelper.capses);
			return sol;
		}
		else
		{
			Solver solve;
			double** temp_am;
			double* temp_zm;
			temp_am = duplicate_matrix(am, a_mat_len, a_mat_len);
			temp_zm = duplicate_matrix(zm, z_mat_len);

			log("A matrix: ");
			if(log_on)myhelper.log_matrix(temp_am, a_mat_len);
			log("Z matrix: ");
			if (log_on)myhelper.log_matrix(temp_zm, z_mat_len);

			sol = solve.solution(temp_am, temp_zm, a_mat_len);
			log("Solution: ");
			if (log_on)myhelper.log_matrix(sol, z_mat_len);
			update_transient(myhelper.capses);
			
			return sol;
		}
	}
};
