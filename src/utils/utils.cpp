#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <regex>
#include <fstream>
#include <utils.h>

std::vector<std::vector<std::complex<double>>> I = { 
        {1.0, 0 }, 
        {0, 1.0 } 
    };

std::vector<std::vector<std::complex<double>>> Z = { 
        {1.0, 0 }, 
        {0, -1.0 } 
    };

std::vector<std::vector<std::complex<double>>> X = { 
        {0, 1.0 }, 
        {1.0, 0 } 
    };

std::vector<std::vector<std::complex<double>>> H = { 
        {1.0 / sqrt(2), 1.0 / sqrt(2)}, 
        {1.0 / sqrt(2), -1.0 / sqrt(2)} 
    };

std::vector<std::vector<std::complex<double>>> proj0 = {
        {1.0, 0.0}, 
        {0.0, 0.0}
    };

std::vector<std::vector<std::complex<double>>> proj1 = {
        {0.0, 0.0}, 
        {0.0, 1.0}
    };

//construct matrix representaion of CNOT gate |0><0| x I x I x I + |1><1| x I x I x X where control is 0 and target 3
std::vector<std::vector<std::complex<double>>> CNOT(int control, int target, int n) {
    
    if (control >= n) {
        throw std::invalid_argument("[CNOTGate] -> control cannot be beyond numQubits");
    }

    if (target >= n) {
        throw std::invalid_argument("[CNOTGate] -> target cannot be beyond numQubits");
    }

    std::vector<std::vector<std::complex<double>>> term1, term2;

    if (control == 0) {
        term1 = proj0;
        term2 = proj1;
    } else if (target == 0) {
        term1 = I;
        term2 = X;
    } else {
        term1 = I;
        term2 = I;
    }

    for (int i = 1; i < n; ++i) {
        if (i == control) {
            term1 = Kroneckerproduct(term1, proj0);
            term2 = Kroneckerproduct(term2, proj1);
        }
        if (i == target) {
            term1 = Kroneckerproduct(term1, I);
            term2 = Kroneckerproduct(term2, X);
        } else {
            term1 = Kroneckerproduct(term1, I);
            term2 = Kroneckerproduct(term2, I);
        }
    }

    std::vector<std::vector<std::complex<double>>> cnot_gate_matrix(pow(2, n), std::vector<std::complex<double>>(pow(2, n)));

    for (int i = 0; i < term1.size(); ++i) {
        for (int j = 0; j < term1.size(); ++j) {
             cnot_gate_matrix[i][j] = term1[i][j] + term2[i][j];
        }
    }

    return cnot_gate_matrix;
}

std::vector<double> Measure(const std::vector<std::complex<double>>& state) {
    std:vector<double> probs(state.size());
    
    cout << "State Vector : " << endl;
    for (const auto& amplitude : state) {
        cout << amplitude.real() << " + " << amplitude.imag() << "i" << endl;
    }
    cout << endl;
    cout << "Probabilities : " << endl;
    for (int i=0; i<state.size(); ++i) {
        probs[i] = pow(state[i].real(), 2) + pow( state[i].imag(), 2);
        cout << "Prob(| " << i << " >) = " << probs[i] << endl;
    }
    return probs;
}


// Function to compute the Kronecker Product of two matrices
std::vector<std::vector<std::complex<double>>> Kroneckerproduct(const std::vector<std::vector<std::complex<double>>>& A, const std::vector<std::vector<std::complex<double>>>& B) {
    const int rowa = A.size();
    const int cola = A[0].size();
    const int rowb = B.size();
    const int colb = B[0].size();

    const int resultRows = rowa * rowb;
    const int resultCols = cola * colb;

    std::vector<std::vector<std::complex<double>>> C(resultRows, std::vector<std::complex<double>>(resultCols));

    for (int i = 0; i < rowa; i++) {
        for (int k = 0; k < rowb; k++) {
            for (int j = 0; j < cola; j++) {
                for (int l = 0; l < colb; l++) {
                    C[i * rowb + k][j * colb + l] = A[i][j] * B[k][l];
                }
            }
        }
    }

    return C;
}

//calculate IxIxGxI 
std::vector<std::vector<std::complex<double>>> SingleQubitGateLayer(const std::vector<std::vector<std::complex<double>>>& G, int pos, int numQubits) {

    if (pos >= numQubits) {
        throw std::invalid_argument("[SingleQubitGateLayer] -> pos cannot be beyond numQubits");
    }

    std::vector<std::vector<std::complex<double>>> B;

    if (pos == 0) { B = G; } else { B = I; }
    
    for (int qubit = 1; qubit < numQubits; ++qubit) {
        if (qubit != pos) {
            B = Kroneckerproduct(B, I);
        } else if (qubit == pos) {
            B = Kroneckerproduct(B, G);
        }
    }
    return B;

}


std::vector<std::vector<std::complex<double>>> operator*(const std::vector<std::vector<std::complex<double>>>& matrix1, const std::vector<std::vector<std::complex<double>>>& matrix2) {
    const int numRows1 = matrix1.size();
    const int numCols1 = matrix1[0].size();
    const int numRows2 = matrix2.size();
    const int numCols2 = matrix2[0].size();

    if (numCols1 != numRows2) {
        throw std::invalid_argument("Column dimensions of matrix1 must match row dimensions of matrix2 for matrix multiplication.");
    }

    // Result matrix
    std::vector<std::vector<std::complex<double>>> result(numRows1, std::vector<std::complex<double>>(numCols2));

    // Perform matrix multiplication
    for (int i = 0; i < numRows1; ++i) {
        for (int j = 0; j < numCols2; ++j) {
            std::complex<double> sum = 0.0;
            for (int k = 0; k < numCols1; ++k) {
                sum += matrix1[i][k] * matrix2[k][j];
            }
            result[i][j] = sum;
        }
    }

    return result;
}

std::vector<std::complex<double>> Multiply(const std::vector<std::vector<std::complex<double>>>& matrix, const std::vector<std::complex<double>>& vector) {
    const int vectorSize = vector.size();
    const int numRows = matrix.size();
    const int numCols = matrix[0].size();

    if (vectorSize != numRows) {
        throw std::invalid_argument("Dimension of vector must match the number of rows in the matrix for vector-matrix multiplication.");
    }

    // Result vector
    std::vector<std::complex<double>> result(numCols);

    // Perform vector-matrix multiplication
    for (int j = 0; j < numCols; ++j) {
        std::complex<double> sum = 0.0;
        for (int i = 0; i < numRows; ++i) {
            sum += vector[i] * matrix[i][j];
        }
        result[j] = sum;
    }

    return result;
}

void print(const std::vector<std::vector<std::complex<double>>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& amplitude : row) {
            cout << amplitude.real() << " + " << amplitude.imag() << "i" << "  ";
        }
        cout << endl;
    }
}

void print(std::vector<std::complex<double>>& vector) {
    for (const auto& amplitude : vector) {
        cout << amplitude.real() << " + " << amplitude.imag() << "i" << endl;
    }
}


// int main() {

//     std::vector<std::vector<std::complex<double>>> A = { {1.0 / sqrt(2), 1.0 / sqrt(2)}, {1.0 / sqrt(2), -1.0 / sqrt(2)} };
//     std::vector<std::vector<std::complex<double>>> B = { {1.0, 0 }, {0, 1.0} };

//     try {
//         std::vector<std::vector<std::complex<double>>> C = Kroneckerproduct(B, A);

//         // Print the resulting matrix C
//         std::cout << "Resulting matrix C:" << std::endl;
//         for (const auto& row : C) {
//             for (const auto& element : row) {
//                 std::cout << element << " ";
//             }
//             std::cout << std::endl;
//         }

//         C = SingleQubitGateLayer(H, 0, 2);
//         cout << C.size() << C[0].size() <<endl;
//         std::cout << "gate layer:" << std::endl;
//         for (const auto& row : C) {
//             for (const auto& element : row) {
//                 std::cout << element << " ";
//             }
//             std::cout << std::endl;
//         }

//         // Perform matrix multiplication
//         std::vector<std::vector<std::complex<double>>> result = X*X;

//         // Print the result
//         for (const auto& row : result) {
//             for (const auto& element : row) {
//                 std::cout << element << " ";
//             }
//             std::cout << std::endl;
//         }

//         // Perform matrix vector mutliply
//         std::vector<std::complex<double>> state(pow(2, 1));
//         state[1] = 1/sqrt(2); // Set the initial state to |0...0>
//         state[0] = 1/sqrt(2);

//         // std::vector<std::complex<double>> result_vector = Multiply(Z,state);

//         // // Print the result
//         // for (const auto& element : result_vector) {
//         //     std::cout << element << std::endl;
//         // }

//         std::vector<std::vector<std::complex<double>>> cnot = CNOT(0, 2, 3);
//         print(cnot);

//         Measure(state);

//     } catch (const std::invalid_argument& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
