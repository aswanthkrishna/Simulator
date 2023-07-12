// utils.h
#ifndef UTILS_H
#define UTILS_H

#include<math.h>

using namespace std;

extern std::vector<std::vector<std::complex<double>>> I;

extern std::vector<std::vector<std::complex<double>>> Z;

extern std::vector<std::vector<std::complex<double>>> X;

extern std::vector<std::vector<std::complex<double>>> H;

extern std::vector<std::vector<std::complex<double>>> Proj0;

extern std::vector<std::vector<std::complex<double>>> Proj1;

std::vector<std::vector<std::complex<double>>> CNOT(int control, int target, int n);

std::vector<std::vector<std::complex<double>>> Kroneckerproduct(const std::vector<std::vector<std::complex<double>>>& A, const std::vector<std::vector<std::complex<double>>>& B);

std::vector<std::vector<std::complex<double>>> SingleQubitGateLayer(const std::vector<std::vector<std::complex<double>>>& G, int pos, int numQubits);

std::vector<std::vector<std::complex<double>>> operator*(const std::vector<std::vector<std::complex<double>>>& matrix1, const std::vector<std::vector<std::complex<double>>>& matrix2);

std::vector<std::complex<double>> Multiply(const std::vector<std::vector<std::complex<double>>>& matrix, const std::vector<std::complex<double>>& vector);

void print(const std::vector<std::vector<std::complex<double>>>& matrix);

void print(std::vector<std::complex<double>>& vector);

std::vector<double> Measure(const std::vector<std::complex<double>>& vector);

#endif // UTILS_H
