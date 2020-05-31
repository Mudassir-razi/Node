#pragma once
#include<iostream>
#include<vector>

using namespace std;

bool log_on;


//This is the base class of all components.
//coz they have same attributes.two terminals and one value

class Component
{
public:

	float value;
	int pin_pos;
	int pin_neg;
	char name[5];

	//to be calculated value
	float voltage_accross;
	float current_through;

	Component()
	{
		value = 100;
		pin_pos = 0;
		pin_neg = 0;
		voltage_accross = 0;
		current_through = 0;
	}

	Component(char nam[5],float v, int p1, int p2)
	{
		value = v;
		pin_pos = p1;
		pin_neg = p2;
		voltage_accross = 0;
		current_through = 0;
		strcpy_s(name, nam);
	}

	int getOther(int in_pin)
	{
		int other_pin;
		if (in_pin == pin_pos)other_pin = pin_neg;
		else if (in_pin == pin_neg)other_pin = pin_pos;
		else
		{
			cout << "Pin selection error!\n";
			other_pin = -1;
		}



		return other_pin;
	}

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

	Resistor()
	{
		//cout << "From default constructor" << endl;
	}

	Resistor(char n[5],float v1, int a1, int a2):Component(n,v1,a1,a2)
	{
		//cout << "From perameterized contrucotor" << endl;
	}
};

//Ideal independent Voltage source

class Voltage : public Component
{
public:

	Voltage()
	{
		//cout << "From default constructor" << endl;
	}

	Voltage(char n[5], float v1, int a1, int a2) :Component(n, v1, a1, a2)
	{
		//cout << "From perameterized contrucotor" << endl;
	}

	float pin_polarity(int pin)
	{
		if (pin == pin_pos)return 1.00;
		else if (pin == pin_neg)return -1.00;
		else
		{
			//cout << "Voltage pin error!!" << endl;
			return 0.00;
		}
	}

};

class Current : public Component
{
public:

	Current()
	{
		//cout << "From default constructor" << endl;
	}

	Current(char n[5], float v1, int a1, int a2) :Component(n, v1, a1, a2)
	{
		//cout << "From perameterized contrucotor" << endl;
	}

	int give_polarity(int p)
	{
		if (p == pin_pos)return -1;
		else if (p == pin_neg)return 1;
		else
		{
			cout << "Current pin error!!" << endl;
			return 0;
		}
	}

};

class Nodes
{
public:

	vector<Resistor> rs;
	vector<Voltage> vs;
	vector<Current> is;

	Nodes()
	{
		//cout << "Creating node\n";
	}


	void add_element(Resistor r)
	{
		rs.push_back(r);
	}

	void add_element(Voltage v)
	{
		vs.push_back(v);
	}

	void add_element(Current i)
	{
		is.push_back(i);
	}


	void log()
	{
		if (log_on) {
			cout << ".............\n";
			for (unsigned i = 0; i < rs.size(); i++)
			{
				rs.at(i).Log();
			}

			for (unsigned i = 0; i < vs.size(); i++)
			{
				vs.at(i).Log();
			}

			for (int i = 0; i < (int)is.size(); i++)
			{
				is.at(i).Log();
			}

		}
	}

	float r_inverse()
	{
		float res = 0.0;
		for (int i = 0; i < (int)rs.size(); i++)
		{
			res += 1 / rs.at(i).value;
		}
		return res;
	}

	float total_current(int pin)
	{
		float i = 0;
		for (int j = 0; j < (int)is.size(); j++)
		{
			i += is.at(j).give_polarity(pin) * is.at(j).value;
		}
		return i;
	}

};