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


###Enhancement 

- Parse openQASM to Intermediate Representation of the quantum circuit before simulating
- Use n-d tensors for storing Wavefunction instead of flattened vectors, (realized this too late to change)
- Partial Measurements by Partial Trace , Density Matrices 
- Mid circuit Measurements
- More gates and classically conditioned gates


