#define _CRT_SECURE_NO_WARNINGS
#include "..\..\..\mmio.cpp"
#include "..\..\..\matrix.h"

using namespace std;

int main() {
	char filename[] = "..\\..\\mycielskian4.mtx";
	string format = "mtx";
	spMtx<int> graph (filename, format);
	cout << "Rst: ";
	for (int i = 0; i < graph.m + 1; ++i) {
		cout << graph.Rst[i] << ' ';
	}
	cout << "\nCol: ";
	for (int i = 0; i < graph.nz; ++i) {
		cout << graph.Col[i] << ' ';
	}
	cout << "\nVal: ";
	for (int i = 0; i < graph.nz; ++i) {
		cout << graph.Val[i] << ' ';
	}
}