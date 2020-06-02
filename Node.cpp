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
vector<Vdvs> vdvs;

int main()
{
    float** a_mat, * z_mat, * sol;

    int vol_len, node_len, vdvs_len;

    Helper stream(&nodes, &res, &vs, &is, &vdvs);

    while (true) 
    {
        string input;
        getline(cin, input);
        if (input[0] == '/')break;
        stream.Update(input);
    }

    stream.list_components();

    vol_len = vs.size();
    node_len = nodes.size();
    vdvs_len = vdvs.size();
    int len = vol_len + node_len + vdvs_len - 1;
    
    //Solution part:
    Matrix mat;
    Solver solve;

    mat.Toggle_log(true);
    solve.Toggle_log(true);
    a_mat= mat.A_matrix(nodes, vs, vdvs);
    z_mat = mat.Z_matrix(nodes, vs, vdvs);

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
        vs.at(i).current_through = -sol[i + node_len-1];
        cout << "Source " << vs.at(i).name << " current: " << -sol[i +node_len-1] <<"A"<< endl;
    }

    return 0;
}
