#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include <limits>

using namespace std;

//This is the base class of all components.
//coz they have same attributes.two terminals and one value
//Solution sequence is:
// 1.Node_voltages -> 2.current_through_independent_v_sources -> 3.current_through_dependent_v_sources -> 4.dv/dt_of_capacitors

class Component
{
public:

	string name;
	double value;	//primary value of the component
	int pin_pos;	//+ side node of the component.//for polar components
	int pin_neg;	//same
	int index;		//index on it's vector
	bool log_on;	//true if you need to print component properties in run time

	//index shift
	int gm_shift, bm_shift, cm_shift, dm_shift, zm_shift, xm_shift;

	//to be calculated value
	double voltage_accross;
	double current_through;

	Component()
	{
		value = 100;
		pin_pos = 0;
		pin_neg = 0;
		voltage_accross = 0;
		current_through = 0;
		log_on = false;
		name = "hello";
		gm_shift = bm_shift = cm_shift = dm_shift = zm_shift = xm_shift = 0;
	}

	Component(string nam, double v, int p1, int p2)
	{
		value = v;
		pin_pos = p1;
		pin_neg = p2;
		voltage_accross = 0;
		current_through = 0;
		name = nam;
		log_on = false;
		gm_shift = bm_shift = cm_shift = dm_shift = zm_shift = xm_shift = 0;
	}

	virtual void update_matrix_G(double** g) { return; }
	virtual void update_matrix_B(double** b) { return; }
	virtual void update_matrix_C(double** c) { return; }
	virtual void update_matrix_D(double** d) { return; }
	virtual void update_matrix_Z(double* z) { return; }

	void Log()
	{
		if (log_on)
		{
			cout << name <<" value: " << value << " |  pins: " << pin_pos << " and " << pin_neg << endl;
		} 
	}
};

class Resistor : public Component
{

public:

	Resistor(string n, double v1, int a1, int a2) :Component(n, v1, a1, a2) {}

	void update_matrix_G(double** g)
	{
		int i1 = pin_pos - 1;
		int i2 = pin_neg - 1;

		if(i1 >= 0)g[i1][i1] += 1 / value;
		if (i2 >= 0)g[i2][i2] += 1 / value;

		if (i1 >= 0 && i2 >= 0)
		{
			g[i1][i2] -= 1 / value;
			g[i2][i1] -= 1 / value;
		}
	}
};

//Ideal independent Voltage source

class Voltage : public Component
{
public:

	Voltage(string n, double v1, int a1, int a2) :Component(n, v1, a1, a2){}

	void update_matrix_B(double** b)
	{
		if (pin_pos > 0)b[pin_pos - 1][index] += 1;
		if (pin_neg > 0)b[pin_neg - 1][index] += -1;
	}

	void update_matrix_C(double** c)
	{
		if (pin_pos > 0)c[index][pin_pos - 1] += 1;
		if (pin_neg > 0)c[index][pin_neg - 1] += -1;
	}

	void update_matrix_Z(double* z)
	{
		z[index+zm_shift] += value;
	}

};

class Current : public Component
{
public:

	Current(string n, double v1, int a1, int a2) :Component(n, v1, a1, a2){}

	void update_matrix_Z(double* z)
	{
		if (pin_pos > 0)z[pin_pos - 1] -= value;
		if (pin_neg > 0)z[pin_neg - 1]+= value;
	}
};


class Vdvs : public Component
{

public:

	int cpin_pos;
	int cpin_neg;

	Vdvs(string n, double v, int p1, int p2, int cp1, int cp2):Component(n,v,p1,p2)
	{
		cpin_pos = cp1;
		cpin_neg = cp2;
	}

	void update_matrix_C(double** c)
	{
		if (cpin_pos > 0)c[cm_shift + index][cpin_pos - 1] -= value;
		if(cpin_neg > 0)c[cm_shift + index][cpin_neg - 1] += value;
	}
};

class Vdcs : public Component
{
public:

	int cpin_pos;
	int cpin_neg;

	Vdcs(string n, double v, int p1, int p2, int cp1, int cp2):Component(n,v,p1,p2)
	{
		cpin_pos = cp1;
		cpin_neg = cp2;
	}

	void update_matrix_G(double** g)
	{
		if (pin_pos > 0)
		{
			if (cpin_pos > 0)g[pin_pos - 1][cpin_pos - 1] += value;
			if (cpin_neg > 0)g[pin_pos - 1][cpin_neg - 1] -= value;
		}
		if (pin_neg > 0)
		{
			if (cpin_pos > 0)g[pin_neg - 1][cpin_pos - 1] -= value;
			if (cpin_neg > 0)g[pin_neg - 1][cpin_neg - 1] += value;
		}
	}
};

class Capacitor : public Component
{

public:

	double ic;		//initial condition.For capacitor, it's initial voltage
	Capacitor(string n, double v1, int a1, int a2, double icond) :Component(n, v1, a1, a2)
	{
		ic = icond;
		voltage_accross = ic;
	}

	void update_matrix_B(double** b)
	{
		if (pin_pos > 0)b[pin_pos - 1][index+bm_shift] += 1;
		if (pin_neg > 0)b[pin_neg - 1][index+bm_shift] += -1;
	}

	void update_matrix_C(double** c)
	{
		if (pin_pos > 0)c[index+cm_shift][pin_pos - 1] += 1;
		if (pin_neg > 0)c[index+cm_shift][pin_neg - 1] += -1;
	}

	void update_matrix_Z(double* z)
	{
		z[index + zm_shift] = voltage_accross;
	}

	// val =  i * delta_T
	void update_values(double val)
	{
		double dv = (val / value);
		voltage_accross += dv;
		//cout <<" + " << dv <<" = "<<voltage_accross<<endl;
	}
};
