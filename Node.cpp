#include <iostream>
#include "component.h"
#include "netlist.h"
#include "helper.h"
#include "solver.h"
#include <vector>
#include <fstream>
#include <math.h>
#include <time.h>
#include <limits>

//Global variable for storing components
std::vector<double> nodes;
std::vector<Resistor> res;
std::vector<Voltage> vs;
std::vector<Current> is;
std::vector<Vdvs> vdvs;
std::vector<Vdcs> vdcs;
std::vector<Capacitor> caps;

void print_solution(double* sol);

int main()
{
    double* sol;
    int sim_type;
    ofstream out;
    out.open("output.txt");

    Helper stream(&nodes, &res, &vs, &is, &vdvs, &vdcs, &caps);
    clock_t start, stop;

    while (true) 
    {
        string input;
        getline(cin, input);
        if (input[0] == '/')break;
        stream.Update(input);
    }

    while (true)
    {
        cout << "Simulation type:\n1.Steady state\n2.Time domain\n";
        cin >> sim_type;
        if (sim_type != 1 && sim_type != 2)cout << "Error: Invalid input!!\n";
        else break;
    }
    stream.update_index();

    //Solution part:
    Matrix mat;

    mat.Toggle_log(false);
    double time = 0, del_time = 0.0001, end_time = 0.5;
    
    if (sim_type == 2)
    {
        cout << "start time    End time    time step\n";
        cin >> time >> end_time>> del_time;
        start = clock();
        while (time < end_time)
        {
            out << time << "  " << caps.at(0).voltage_accross << endl;
            sol = mat.transient(stream, del_time);
            time += del_time;
            delete[] sol;
        }
    }
    else if (sim_type == 1)
    {
        stream.list_components();
        start = clock();
        sol = mat.Solution_generator(stream);
        print_solution(sol);
        delete[] sol;
    }

    stop = clock();

    cout << "\n\nSimulation time: " << stop - start <<" mili seconds"<<endl;
    return 0;
}

void print_solution(double* sol)
{
    int len = nodes.size();
    int shift = 0;
    cout << "\n..............................................." << endl;
    for (int i = 1; i < len; i++)
    {
        nodes.at(i) = sol[i - 1];
        cout << "Node " << i << " voltage: " << sol[i - 1] << "v" << endl;
    }

    shift = len;
    len = vs.size();

    for (int i = 0; i < len; i++)
    {
        vs.at(i).current_through = -sol[i + shift - 1];
        cout << "Source " << vs.at(i).name << " current: " << -sol[i + shift - 1] << "A" << endl;
    }
}
