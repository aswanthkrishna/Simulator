OPENQASM 2.0;
include "qelib1.inc";

qreg q[4];
creg c[1];

x q[0];
h q[0];
cx q[0], q[1];

measure q -> c;