#pragma once
#include "..\..\..\mmio.h"
#include "..\..\..\matrix.h"
#include <unordered_set>
#include <set>
#include <map>
#include <unordered_map>
#include <iterator>
#include <vector>
#include <fstream>

std::unordered_set<int> adj(const int& x, std::unordered_map<int, std::unordered_set<int>>& S, std::unordered_map<int, std::unordered_set<int>>& X_S) {
	using namespace std;
	unordered_set<int> adj_;
	for (auto i : X_S[x]) {
		if (S.find(i) != S.end()) {
			for (auto j : S[i]) {
				if (j != x) {
					adj_.insert(j);
				}
			}
		}
		else {
			adj_.insert(i);
		}
	}
	return adj_;
}

void transform_(const int& x, std::unordered_map<int, std::unordered_set<int>>& S, std::unordered_map<int, std::unordered_set<int>>& X_S) {
	using namespace std;
	unordered_set<int> T;
	S[x] = adj(x, S, X_S);
	for (auto elem: X_S[x]) {
		if (S.find(elem) != S.end()) {
			T.insert(elem);
			S.erase(elem);
		}
	}
	X_S.erase(x);
	for (auto y: S[x]) {
		X_S[y].insert(x);
		for (auto elem : T) {
			X_S[y].erase(elem);
		}
	}
}

int degree(const int& x, std::unordered_map<int, std::unordered_set<int>>& S, std::unordered_map<int, std::unordered_set<int>>& X_S, const int& size) {
	int deg = 0;
	std::vector<bool> was(size);
	was[x] = true;
	for (auto i : X_S[x]) {
		if (S.find(i) != S.end()) {
			for (auto j : S[i]) {
				if (!was[j]) {
					++deg;
					was[j] = true;
				}
			}
		}
		else {
			if (!was[i]) {
				++deg;
				was[i] = true;
			}
		}
	}
	return deg;
}

template<typename T>
spMtx<T> MinDegree(const spMtx<T>& mtx) {
	using namespace std;
	vector<int> numeration1, numeration2(mtx.m);
	vector<int> degrees(mtx.m);
	unordered_map<int, unordered_set<int>> S, X_S;
	for (int i = 0; i < mtx.m; ++i) {
		unordered_set<int> node;
		for (int j = mtx.Rst[i]; j < mtx.Rst[i + 1]; ++j) {
			node.insert(mtx.Col[j]);
		}
		X_S[i]= node;
		degrees[i] = node.size();
	}
	int num = 0;
	while (!X_S.empty()) {
		//cout << "Step: " << num << '\n';
		int x, min_deg = mtx.m + 1;
		for (auto& elem : X_S) {
			if (degrees[elem.first] < min_deg) {
				x = elem.first;
				min_deg = degrees[elem.first];
			}
		}
		numeration1.push_back(x);
		numeration2[x] = num++;
		transform_(x, S, X_S);
		for (auto elem : S[x]) {
			degrees[elem] = degree(elem, S, X_S, mtx.m);
		}
	}
	spMtx<T> new_order(mtx.m, mtx.m, mtx.nz);
	int k = 0;
	for (int i = 0; i < mtx.m; ++i) {
		new_order.Rst[i + 1] = new_order.Rst[i] + mtx.Rst[numeration1[i] + 1] - mtx.Rst[numeration1[i]];
		for (int j = mtx.Rst[numeration1[i]]; j < mtx.Rst[numeration1[i] + 1]; ++j) {
			new_order.Col[k] = numeration2[mtx.Col[j]];
			new_order.Val[k] = mtx.Val[j];
			++k;
		}
	}
	ofstream out("out.txt");
	for (int i = 0; i < mtx.m; ++i) {
		out << numeration2[i] << ' ';
	}
	out.close();
	return new_order;
}

void MinDegree(const int& n, int* Rst, int* Col, int* perm) {
	using namespace std;
	vector<int> degrees(n);
	unordered_map<int, unordered_set<int>> S, X_S;
	for (int i = 0; i < n; ++i) {
		for (int j = Rst[i]; j < Rst[i + 1]; ++j) {
			X_S[i].insert(Col[j]);
		}
		degrees[i] = X_S[i].size();
	}
	int num = 0;
	while (!X_S.empty()) {
		//cout << "Step: " << num << '\n';
		int x = 0, min_deg = n + 1;
		for (auto& elem : X_S) {
			if (degrees[elem.first] < min_deg) {
				x = elem.first;
				min_deg = degrees[elem.first];
			}
		}
		perm[x] = num++;
		transform_(x, S, X_S);
		for (auto elem : S[x]) {
			degrees[elem] = degree(elem, S, X_S, n);
		}
	}
}