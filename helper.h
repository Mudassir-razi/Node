#pragma once
#include<string>
#include<vector>
#include<sstream>
#include "component.h"

using namespace std;

//splitting string into segments

class Helper {

public:

	vector<Resistor> *res;
	vector<Voltage> *ves;
	vector<Current> *ies;
	vector<double>* nos;
	vector<Vdvs>* vdvses;
	vector<Vdcs>* vdcses;
	vector<Capacitor>* capses;

public:

	Helper()
	{
		nos = nullptr;
		res = nullptr;
		ves = nullptr;
		ies = nullptr;
		vdvses = nullptr;
		vdcses = nullptr;
		capses = nullptr;
	}

	//storing pointers 
	Helper(vector<double> *nos_var, vector<Resistor> *res_var, vector<Voltage> *ves_var,
		   vector<Current> *ies_var, vector<Vdvs> *vdvs_var, vector<Vdcs> *vdcs_var,
		   vector<Capacitor>* capses_var)
	{
		nos = nos_var;
		res = res_var;
		ves = ves_var;
		ies = ies_var;
		vdvses = vdvs_var;
		vdcses = vdcs_var;
		capses = capses_var;
	}

	void Update(string s)
	{
		string name;
		string vals;

		vector<double> values;

		for (int i = 0; i < (int)s.size(); i++)
		{
			if (s[i] == ' ')
			{
				name = s.substr(0, i);
				vals = s.substr(i+1, (int)s.size() - 1);
				break;
			}
		}

		values = split_info(vals);

		int p1 = (int)values.at(0);
		int p2 = (int)values.at(1);

		//Now generating appropiate object;
		//Add new components here
		//....................................................

		if (name[0] == 'R' || name[0] == 'r')
		{
			if (values.size() != 3)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Resistor r(name, values.at(2), p1, p2);
			r.index = res->size();
			res->push_back(r);
		}

		else if (name[0] == 'V' || name[0] == 'v')
		{
			if (values.size() != 3)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Voltage r(name, values.at(2), p1, p2);
			r.index = ves->size();
			ves->push_back(r);
		}

		else if (name[0] == 'I' || name[0] == 'i')
		{
			if (values.size() != 3)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Current r(name, values.at(2), p1, p2);
			r.index = ies->size();
			ies->push_back(r);
		}

		else if (name[0] == 'E' || name[0] == 'e')
		{
			if (values.size() != 5)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Vdvs r(name, values.at(2), p1, p2, (int)values.at(2),(int)values.at(3));
			r.index = vdvses->size();
			vdvses->push_back(r);
		}

		else if (name[0] == 'G' || name[0] == 'g')
		{
			if (values.size() != 5)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Vdcs r(name, values.at(2), p1, p2, (int)values.at(2), (int)values.at(3));
			r.index = vdcses->size();
			vdcses->push_back(r);
		}

		else if (name[0] == 'C' || name[0] == 'c')
		{
			if (values.size() != 4)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Capacitor r(name, values.at(2), p1, p2, values.at(3));
			r.index = vdcses->size();
			capses->push_back(r);
		}

		else
		{
			//No matching component found
			//Exception handle
			cout << "Error: No matching component found!!" << endl;
		}
		
		update_node(p1, p2);
	}
	
	//For printing matrix:

	void log_matrix(double** m, int size)
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				cout << std::setw(5) << m[i][j] << "  ";
			}
			cout << endl;
		}
	}

	void log_matrix(double** m, int sizex, int sizey)
	{
		for (int i = 0; i < sizex; i++)
		{
			for (int j = 0; j < sizey; j++)
			{
				cout << std::setw(5) << m[i][j] << "  ";
			}
			cout << endl;
		}
	}

	void log_matrix(double* m, int size)
	{
		for (int i = 0; i < size; i++)
		{
			cout << m[i] << endl;
		}
	}

	void list_components()
	{
		print_element(res);
		print_element(ves);
		print_element(ies);
		print_element(vdvses);
		print_element(vdcses);
		print_element(capses);
	}

	//Corrects the index of components
	//Edit this according to README.md
	//*********...............................................***********
	void update_index()
	{
		shift_index<Voltage>(ves, 0, 0, 0, 0, (int)nos->size()-1,0);
		shift_index<Vdvs>(vdvses, 0, 0, (int)ves->size(), 0, 0, 0);

		int shift = (int)vdvses->size() + (int)ves->size();
		shift_index<Capacitor>(capses, 0, shift, shift, 0, (int)nos->size() + shift - 1, (int)nos->size() + shift - 1);
	}

private:

	template <typename T>
	void shift_index(vector<T>* obj,int gshift, int bshift, int cshift, int dshift, int zshift, int xshift)
	{
		int len = obj->size();
		for (int i = 0; i < len; i++)
		{
			obj->at(i).gm_shift += gshift;
			obj->at(i).bm_shift += bshift;
			obj->at(i).cm_shift += cshift;
			obj->at(i).dm_shift += dshift;
			obj->at(i).zm_shift += zshift;
			obj->at(i).xm_shift += xshift;
		}
	}

	vector<double> split_info(string s)
	{
		int len = (int)s.size();
		int last_step = 0;
		double temp_val;
		vector<double> values;
		string sub;
		for (int i = 0; i < len; i++)
		{
			if (s[i] == ' ')
			{
				sub = s.substr(last_step, i - last_step);
				stringstream stream(sub);
				stream >> temp_val;
				values.push_back(temp_val);
				last_step = i;
			}
		}
		sub = s.substr(last_step, s.size() - 1);
		stringstream stream(sub);
		stream >> temp_val;
		values.push_back(temp_val);

		return values;
	}

	void update_node(int p1, int p2)
	{
		int greater = p1 > p2 ? p1 : p2;
		if ((int)nos->size() - 1 < greater)
		{
			nos->resize(greater + 1);
		}
	}

	template <typename T>
	void print_element(vector<T>* v)
	{
		for (int i = 0; i < (int)v->size(); i++)
		{
			auto temp_r = v->at(i);
			cout <<temp_r.name << " pins:" << temp_r.pin_pos << " & " << temp_r.pin_neg << " | value:" << temp_r.value << endl;
		}
	}
};
