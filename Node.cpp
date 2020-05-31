#include <iostream>
#include "component.h"
#include "netlist.h"
#include "helper.h"
#include "solver.h"
#include <vector>

using namespace std;

//Global variable for storing components
vector<Nodes> nodes;
vector<Resistor> res;
vector<Voltage> vs;
vector<Current> is;

void manage_shit(char name[5], float value, int p1, int p2);

int main()
{
    log_on = true;
    char n[5] = {'a','b','c','d','e'};
    int n1, n2;
    float val;


    while (n[0] != '/') 
    {
        cin >> n;
        if (n[0] == '/')break;
        cin >> n1 >> n2 >> val;
        manage_shit(n, val, n1, n2);
    }
    
    

    //prints component & node values
    if (log_on)
    {

        for (unsigned int i = 0; i < res.size(); i++)
        {
            res.at(i).Log();
        }

        for (unsigned int i = 0; i < vs.size(); i++)
        {
            vs.at(i).Log();
        }

        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            nodes.at(i).log();
        }
    }
    Z_matrix(nodes, vs);
    A_matrix(nodes, vs);
    return 0;
}

void manage_shit(char name[5], float value, int p1, int p2)
{
    //Managing space for storing nodes
    int greater = p1 > p2 ? p1 : p2;

    if ((int)nodes.size() - 1 < greater)nodes.resize(greater + 1);

    if (name[0] == 'R' || name[0] == 'r')
    {
        Resistor r(name, value, p1, p2);
        res.push_back(r);
        nodes.at(p1).add_element(r);
        nodes.at(p2).add_element(r);
    }

    if (name[0] == 'V' || name[0] == 'v')
    {
        Voltage r(name, value, p1, p2);
        vs.push_back(r);
        
        nodes.at(p1).add_element(r);
        nodes.at(p2).add_element(r);
    }

    if (name[0] == 'I' || name[0] == 'i')
    {
        Current i(name, value, p1, p2);
        is.push_back(i);

        nodes.at(p1).add_element(i);
        nodes.at(p2).add_element(i);
    }

    
}