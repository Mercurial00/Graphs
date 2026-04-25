#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <chrono>
#include "MinDegree2.h"
#include "../../mtx_read/mmio.h" 
#include "../../mtx_read/matrix.h"

using namespace std;
int main(int argc, char* argv[]) {
	const char* filename;
	if (argc < 2)
		filename = "../../../matrixes/audikw_1.mtx";
	else {
		filename = argv[1];
	}
	string format = "mtx";
	spMtx<double> graph(filename, format);
	string out_name = "";
	{
		int end_pos;
		for (end_pos = 0; filename[end_pos] != '\0'; ++end_pos);
		int start_pos = end_pos;
		for (; filename[start_pos] != '/' && filename[start_pos] != '\\'; --start_pos);
		for (++start_pos; start_pos != end_pos; ++start_pos) {
			out_name += filename[start_pos];
		}
	}
	cout << "matrix: " << out_name;
	cout << "\nn: " << graph.n << "\nmatrix nz: " << graph.nz << "\n\n";	
	int* perm = new int[graph.m];
	auto start = std::chrono::steady_clock::now();
	MinDegree(graph.m, graph.Rst, graph.Col, perm);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	cout << "Reordering: " << fixed << setprecision(6) << elapsed.count() << '\n' << '\n' << '\n';
	ofstream fout("../../../output.txt");
	for (int i = 0; i < graph.m; ++i) {
		fout << perm[i] << ' ';
	}
	fout.close();
	delete[] perm;
}