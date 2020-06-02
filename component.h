#pragma once
#include<iostream>
#include<string>
#include<vector>

using namespace std;

//This is the base class of all components.
//coz they have same attributes.two terminals and one value

class Component
{
public:

	float value;	//primary value of the component
	int pin_pos;	//+ side node of the component.//for polar components
	int pin_neg;	//same
	string name;
	bool log_on;	//true if you need to print component properties in run time


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
		log_on = false;
		name = "hello";
	}

	Component(string nam,float v, int p1, int p2)
	{
		value = v;
		pin_pos = p1;
		pin_neg = p2;
		voltage_accross = 0;
		current_through = 0;
		name = nam;
		log_on = false;
	}

	void Toggle_log(bool k)
	{
		log_on = k;
	}

	//checks if the given node attached to positive or negative side

	virtual float pin_polarity(int pin)
	{
		if (pin == pin_pos)return 1.00;
		else if (pin == pin_neg)return -1.00;
		else
		{
			return 0.00;
		}
	}
	
	//returns the other pin attatched to this component

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

	Resistor(string n, float v1, int a1, int a2) :Component(n, v1, a1, a2) {}
};

//Ideal independent Voltage source

class Voltage : public Component
{
public:

	Voltage(string n, float v1, int a1, int a2) :Component(n, v1, a1, a2){}
};

class Current : public Component
{
public:

	Current(string n, float v1, int a1, int a2) :Component(n, v1, a1, a2){}
};

class Vdvs : public Component
{

public:

	int cpin_pos;
	int cpin_neg;
	float gain;

	Vdvs(string n, float v, int p1, int p2, int cp1, int cp2)
	{
		gain = v;
		value = 0;
		pin_pos = p1;
		pin_neg = p2;
		voltage_accross = 0;
		current_through = 0;
		cpin_pos = cp1;
		cpin_neg = cp2;
		log_on = false;
	}

	float pin_polarity(int pin)
	{
		float polarity = 0;
		polarity = pin == pin_pos ? 1.0 : pin == pin_neg ? -1.0 : 0.0;
		if (pin == cpin_neg)polarity += gain;
		else if (pin == cpin_pos)polarity -= gain;
		return polarity;
	}

	float pin_polarity(int pin, bool k)
	{
		if (pin == pin_pos)return 1.00;
		else if (pin == pin_neg)return -1.00;
		else
		{
			return 0.00;
		}
	}

};

class Nodes
{
public:

	vector<Resistor> rs;
	vector<Voltage> vs;
	vector<Current> is;
	vector<Vdvs> vdvss;
	float node_voltage;
	bool log_on;

	Nodes()
	{
		//cout << "Creating node\n";
		log_on = false;
		node_voltage = 0;
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

	void add_element(Vdvs v)
	{
		vdvss.push_back(v);
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
			i -= is.at(j).pin_polarity(pin) * is.at(j).value;
		}
		return i;
	}

};
