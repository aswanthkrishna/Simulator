#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <regex>
#include <fstream>
#include <utils.h>

using namespace std;

int extractNumQubits(const string& qasm) {
    regex qregPattern("qreg\\s+q\\[(\\d+)\\];");
    smatch match;
    
    int maxQubit = 0;

    auto it = qasm.cbegin();
    auto end = qasm.cend();

    while (regex_search(it, end, match, qregPattern)) {
        int qubit = stoi(match[1]);
        maxQubit = max(maxQubit, qubit);
        it = match.suffix().first;
    }

    return maxQubit;
}


void applyGate(vector<complex<double>>& state, const vector<vector<complex<double>>>& gate, int target) {
    const int numQubits = static_cast<int>(log2(state.size()));
    const int gateSize = static_cast<int>(sqrt(gate.size()));

    std::vector<std::vector<std::complex<double>>> G = SingleQubitGateLayer(gate, target, numQubits);
    vector<complex<double>> newState(state.size());

    newState = Multiply(G, state);
    state = newState;
}


void applyControlledXGate(vector<complex<double>>& state, int control, int target) {
    const int numQubits = static_cast<int>(log2(state.size()));
    vector<complex<double>> newState(state.size());

    std::vector<std::vector<std::complex<double>>> CX = CNOT(control, target, numQubits);
    newState = Multiply(CX, state);
    state = newState;
}

void measure(vector<complex<double>>& state) {

}

void simulateQuantumCircuit(const string& qasm) {
    const int numQubits = extractNumQubits(qasm);
    cout << "Number of qubits: " << numQubits << endl;

    vector<complex<double>> state(pow(2, numQubits));
    state[0] = 1.0; 

    //sequentially match the Gates and targets and apply corresponding Gates
    regex hPattern("(h)\\s+q\\[(\\d+)\\];");
    regex xPattern("(x)\\s+q\\[(\\d+)\\];");
    regex zPattern("(z)\\s+q\\[(\\d+)\\];");
    regex cxPattern("cx\\s+q\\[(\\d+)\\],\\s*q\\[(\\d+)\\];");
    regex measurePattern("measure\\s+q\\s+->\\s+c");

    stringstream qasmStream(qasm);
    string line;

    while (getline(qasmStream, line)) {
        smatch match;

        if (regex_search(line, match, hPattern)) {

            string gateName = match[1];
            int target = stoi(match[2]);
            cout << "[H] :" << target << endl;

            applyGate(state, H, target);
        }

        if (regex_search(line, match, xPattern)) {
            
            string gateName = match[1];
            int target = stoi(match[2]);
            cout << "[X] :" << target << endl;

            applyGate(state, X, target);
        }

        if (regex_search(line, match, zPattern)) {
        
            string gateName = match[1];
            int target = stoi(match[2]);
            cout << "[Z] :" << target << endl;
            applyGate(state, Z, target);
        }

        else if (regex_search(line, match, cxPattern)) {
            int control = stoi(match[1]);
            int target = stoi(match[2]);
            cout << "[CX] :" << control << "->" << target << endl;
            applyControlledXGate(state, control, target);

        }

        else if (regex_search(line, match, measurePattern)) {
            cout << "[Measure]" << endl;
            Measure(state);

        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <qasm_file>" << endl;
        return 1;
    }

    string qasmFilePath = argv[1];

    // Read the QASM file contents into a string
    ifstream qasmFile(qasmFilePath);
    if (!qasmFile) {
        cout << "Failed to open QASM file: " << qasmFilePath << endl;
        return 1;
    }

    string qasm((istreambuf_iterator<char>(qasmFile)), istreambuf_iterator<char>());

    // Simulate the quantum circuit specified in the QASM file
    simulateQuantumCircuit(qasm);

    return 0;
}