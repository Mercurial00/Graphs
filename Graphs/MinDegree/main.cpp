#define _CRT_SECURE_NO_WARNINGS
#include "..\..\..\mmio.cpp"
#include "..\..\..\matrix.h"
#include "MinDegree.h"
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;
int main() {
	char filename[] = "..\\..\\..\\matrixes\\nasa4704.mtx";
	string format = "mtx";
	spMtx<double> graph(filename, format);
	////graph.print_dense();
	//for (int i = 0; i < graph.m; ++i) {
	//	for (int j = graph.Rst[i]; j < graph.Rst[i + 1]; ++j) {
	//		cout << graph.Col[j] << ' ';
	//	}
	//	cout << " | ";
	//}
	//cout << '\n';
	//for(int i = 0; i < graph.m + 1; ++i){
	//	cout << graph.Rst[i] << ' ';
	//}
	//cout << '\n';
	clock_t t = clock();
	int* perm = new int[graph.m];
	//spMtx<double> new_order(MinDegree(graph));
	MinDegree(graph.m, graph.Rst, graph.Col, perm);
	cout << (double)(clock() - t) / CLOCKS_PER_SEC << '\n';
	//for (int i = 0; i < graph.m; ++i) {
	//	cout << perm[i] << ' ';
	//}
	delete[] perm;
	////for (int i = 0; i < new_order.m; ++i) {
	////	sort(new_order.Col + new_order.Rst[i], new_order.Col + new_order.Rst[i + 1]);
	////}
	////new_order.print_dense();
	//for (int i = 0; i < new_order.m; ++i) {
	//	for (int j = new_order.Rst[i]; j < new_order.Rst[i + 1]; ++j) {
	//		cout << new_order.Col[j] << ' ';
	//	}
	//	cout << " | ";
	//}
	//cout << '\n';
	////for (int i = 0; i < new_order.m; ++i) {
	////	for (int j = new_order.Rst[i]; j < new_order.Rst[i + 1]; ++j) {
	////		cout << new_order.Val[j] << ", ";
	////	}
	////	cout << "\b\b" << " | ";
	////}
	////cout << '\n';
	//for (int i = 0; i < new_order.m + 1; ++i) {
	//	cout << new_order.Rst[i] << ' ';
	//}
	//cout << '\n';
}