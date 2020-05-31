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
    char n[5] = {'a','b','c','d','e'};
    int n1, n2;
    float val;
    bool log_on;
    float** a_mat, * z_mat, * sol;

    int vol_len, node_len;

    while (n[0] != '/') 
    {
        cin >> n;
        if (n[0] == '/')break;
        cin >> n1 >> n2 >> val;
        manage_shit(n, val, n1, n2);
    }

    log_on = false;

    //prints component & node values if log on is true;
    if (log_on)
    {

        for (unsigned int i = 0; i < res.size(); i++)
        {
            res.at(i).Toggle_log(true);
            res.at(i).Log();
        }

        for (unsigned int i = 0; i < vs.size(); i++)
        {
            vs.at(i).Toggle_log(true);
            vs.at(i).Log();
        }

        for (unsigned int i = 0; i < is.size(); i++)
        {
            is.at(i).Toggle_log(true);
            is.at(i).Log();
        }

        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            nodes.at(i).log();
        }
    }
    
    vol_len = vs.size();
    node_len = nodes.size();
    int len = vol_len + node_len - 1;
    
    //Solution part:
    Matrix mat;
    Solver solve;

    mat.Toggle_log(false);
    solve.Toggle_log(false);
    a_mat= mat.A_matrix(nodes, vs);
    z_mat = mat.Z_matrix(nodes, vs);

    sol = solve.solution(a_mat, z_mat, len);

    //Now assigning solutions;
    cout << "\n..............................................." << endl;
    for (int i = 1; i < node_len; i++)
    {
        nodes.at(i).node_voltage = sol[i - 1];
        cout << "Node " << i << " voltage: " << sol[i - 1] <<"v"<< endl;
    }

    for (int i = 0; i < vol_len; i++)
    {
        vs.at(i).current_through = sol[i + node_len-1];
        cout << "Source " << vs.at(i).name << " current: " << sol[i +node_len-1] <<"A"<< endl;
    }

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
