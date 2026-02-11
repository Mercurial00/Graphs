#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <chrono>
#include "MinDegree2.h"
#include "..\..\mtx_read\mmio.cpp" 
#include "..\..\mtx_read\matrix.h"

using namespace std;
int main() {
	char filename[] = "..\\..\\..\\matrixes\\hood.mtx";
	string format = "mtx";
	spMtx<double> graph(filename, format);
	cout << "matrix: " << filename;
	cout << "\nn: " << graph.n << "\nmatrix nz: " << graph.nz << "\n\n";	
	int* perm = new int[graph.m];
	auto start = std::chrono::steady_clock::now();
	MinDegree(graph.m, graph.Rst, graph.Col, perm);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	cout << "Reordering: " << fixed << setprecision(6) << elapsed.count() << '\n' << '\n' << '\n';
	ofstream fout("..//..//..//output.txt");
	for (int i = 0; i < graph.m; ++i) {
		fout << perm[i] << ' ';
	}
	fout.close();
	delete[] perm;
}