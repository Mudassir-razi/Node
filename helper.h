#pragma once
#include<string>
#include<vector>
#include<sstream>
#include "component.h"

using namespace std;

//splitting string into segments


class Helper {

	vector<Resistor> *res;
	vector<Voltage> *ves;
	vector<Current> *ies;
	vector<Nodes>* nos;
	vector<Vdvs>* vdvses;

public:

	//storing pointers 
	Helper(vector<Nodes> *nos_var,vector<Resistor> *res_var, vector<Voltage> *ves_var, vector<Current> *ies_var, vector<Vdvs> *vdvs_var)
	{
		nos = nos_var;
		res = res_var;
		ves = ves_var;
		ies = ies_var;
		vdvses = vdvs_var;
	}

	void Update(string s)
	{
		string name;
		string vals;

		vector<float> values;

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
			res->push_back(r);
			update_info<Resistor>(r, p1, p2);	
		}

		else if (name[0] == 'V' || name[0] == 'v')
		{
			if (values.size() != 3)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Voltage r(name, values.at(2), p1, p2);
			ves->push_back(r);
			update_info(r, p1, p2);
		}

		else if (name[0] == 'I' || name[0] == 'i')
		{
			if (values.size() != 3)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Current r(name, values.at(2), p1, p2);
			ies->push_back(r);
			update_info(r, p1, p2);
		}

		else if (name[0] == 'E' || name[0] == 'e')
		{
			if (values.size() != 5)
			{
				//Exception handle;
				cout << "Error: Not enough info!!" << endl;
			}

			Vdvs r(name, values.at(2), p1, p2, (int)values.at(2),(int)values.at(3));
			vdvses->push_back(r);
			update_info(r, p1, p2);
		}

		else
		{
			//No matching component found
			//Exception handle
			cout << "Error: No matching component found!!" << endl;
		}

	}
	
	void list_components()
	{
		print_element(res);
		print_element(ves);
		print_element(ies);
		print_element(vdvses);
	}

private:

	vector<float> split_info(string s)
	{
		int len = (int)s.size();
		int last_step = 0;
		float temp_val;
		vector<float> values;
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

	template <typename T>
	void update_info(T comp, int p1, int p2)
	{
		
		int last_node = nos->size()-1;
		int greater = p1 > p2 ? p1 : p2;
		if (greater > last_node)
		{
			nos->resize(greater+1);
		}
		nos->at(p1).add_element(comp);
		nos->at(p2).add_element(comp);
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
