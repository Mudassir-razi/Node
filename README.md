# Node
This is a project that i thought of for a very long time
This is a very basic circuit simulator.
For starters, it's gonna simulate only linear, purely resistive, independent sources, steady state, DC circuit.
After this, I will try to add dependent sources and eventually other passive components and other analysis.

changes needed to add new elements:

1.Component.h:
all circuit elements (except node) must be derrived from class component. It contains basic properties. Then a vector of this class must be
added to the Node class so that nodes can keep track of what is being connected to them.

2.Netlist.h:
As netlist generates matrix out of system of equations, you need to change how every type of matrix is generated.Also most importantly, change the size of the output matrices accordingly.G, B, C, D, A, Z all are needed to change their shape i.e. dimension.

3.Solver.h:
Solver just takes matrix system and does matrix operations.It does not depend on type of components. So it doesn'e need changes.

4.Helper.h:
helper takes input string and converts it into appropriate objects.So helper should be modified to check for new elements, how to generate them properly and where to assign them.

5.Node.cpp:
New vector of added element has to be created. the length of the matrix that solver will operate on has to modified too.
