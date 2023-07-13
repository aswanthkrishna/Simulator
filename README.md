# Quantum Simulator

A Very Simple State Vector Quantum Simulator to parse openQASM programs and execute

### Prerequisites

- C++ compiler supporting C++11 or later
- CMake build system
- OpenBLAS library

can also be run in Docker using the included dockerfile


### Instructions

1. build the docker image `docker build . -t simulator`
2. run bell state example `docker run`
3. run other examples interactively `docker run -it -v $(pwd):/simulator simulator:latest sh ` , `.simulator ../examples/ghz.qasm`


### Enhancement 

- Parse openQASM to Intermediate Representation of the quantum circuit before simulating
- Use n-d tensors for storing Wavefunction instead of flattened vectors, (realized this too late to change)
- Partial Measurements by Partial Trace , Density Matrices 
- Mid circuit Measurements
- More gates and classically conditioned gates


### Sample Output

`./simulator ../examples/ghz.qasm`

`Number of qubits: 4
[H] :0
[CX] :0->1
[CX] :0->2
[CX] :0->3
[Measure]
State Vector : 
0.707107 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0 + 0i
0.707107 + 0i

Probabilities : 
Prob(| 0 >) = 0.5
Prob(| 1 >) = 0
Prob(| 2 >) = 0
Prob(| 3 >) = 0
Prob(| 4 >) = 0
Prob(| 5 >) = 0
Prob(| 6 >) = 0
Prob(| 7 >) = 0
Prob(| 8 >) = 0
Prob(| 9 >) = 0
Prob(| 10 >) = 0
Prob(| 11 >) = 0
Prob(| 12 >) = 0
Prob(| 13 >) = 0
Prob(| 14 >) = 0
Prob(| 15 >) = 0.5`