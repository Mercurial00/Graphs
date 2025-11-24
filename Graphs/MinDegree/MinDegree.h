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

//std::unordered_set<int> adj(const int& x, std::unordered_map<int, std::unordered_set<int>>& S, std::unordered_map<int, std::unordered_set<int>>& X_S) {
//	using namespace std;
//	unordered_set<int> adj_;
//	for (auto i : X_S[x]) {
//		if (S.find(i) != S.end()) {
//			for (auto j : S[i]) {
//				if (j != x) {
//					adj_.insert(j);
//				}
//			}
//		}
//		else {
//			adj_.insert(i);
//		}
//	}
//	return adj_;
//}
//
//void transform_(const int& x, std::unordered_map<int, std::unordered_set<int>>& S, std::unordered_map<int, std::unordered_set<int>>& X_S) {
//	using namespace std;
//	unordered_set<int> T;
//	S[x] = adj(x, S, X_S);
//	for (auto elem: X_S[x]) {
//		if (S.find(elem) != S.end()) {
//			T.insert(elem);
//			S.erase(elem);
//		}
//	}
//	X_S.erase(x);
//	for (auto y: S[x]) {
//		X_S[y].insert(x);
//		for (auto elem : T) {
//			X_S[y].erase(elem);
//		}
//	}
//}
//
//int degree(const int& x, std::unordered_map<int, std::unordered_set<int>>& S, std::unordered_map<int, std::unordered_set<int>>& X_S, const int& size) {
//	int deg = 0;
//	std::vector<bool> was(size);
//	was[x] = true;
//	for (auto i : X_S[x]) {
//		if (S.find(i) != S.end()) {
//			for (auto j : S[i]) {
//				if (!was[j]) {
//					++deg;
//					was[j] = true;
//				}
//			}
//		}
//		else {
//			if (!was[i]) {
//				++deg;
//				was[i] = true;
//			}
//		}
//	}
//	return deg;
//}


//void MinDegree(const int& n, int* Rst, int* Col, int* perm) {
//	using namespace std;
//	vector<int> degrees(n);
//	unordered_map<int, unordered_set<int>> S, X_S;
//	for (int i = 0; i < n; ++i) {
//		for (int j = Rst[i]; j < Rst[i + 1]; ++j) {
//			X_S[i].insert(Col[j]);
//		}
//		degrees[i] = X_S[i].size();
//	}
//	int num = 0;
//	while (!X_S.empty()) {
//		//cout << "Step: " << num << '\n';
//		int x = 0, min_deg = n + 1;
//		for (auto& elem : X_S) {
//			if (degrees[elem.first] < min_deg) {
//				x = elem.first;
//				min_deg = degrees[elem.first];
//			}
//		}
//		perm[x] = num++;
//		transform_(x, S, X_S);
//		for (auto elem : S[x]) {
//			degrees[elem] = degree(elem, S, X_S, n);
//		}
//	}
//}

std::vector<int> adj(const int& x, std::vector<std::vector<int>>& NODES, std::vector<int> mask) {
	using namespace std;
	vector<int> adj_;
	adj_.reserve(NODES.size());
	for (auto i : NODES[x]) {
		if (mask[i] == -1) {
			for (auto j : NODES[i]) {
				if (mask[j] == 0 && j != x) {
					mask[j] = 1;
					adj_.push_back(j);
				}
			}
		}
		else if (mask[i] == 0) {
			mask[i] = 1;
			adj_.push_back(i);
		}
	}
	return adj_;
}

void transform_(const int& x, std::vector<std::vector<int>>& NODES, std::vector<int>& mask) {
	using namespace std;
	vector<int> adj_ = adj(x, NODES, mask);
	for (auto elem : NODES[x]) {
		if (mask[elem] == -1) {
			mask[elem] = 1;
		}
	}
	NODES[x] = adj_;
	mask[x] = -1;
	for (auto y : NODES[x]) {
		bool flag = false;
		for (int i = 0; i < NODES[y].size(); ++i) {
			if (NODES[y][i] == x) flag = true;
			if (mask[NODES[y][i]] == 1) {
				NODES[y].erase(NODES[y].begin() + i);
			}
		}
		if (!flag) {
			NODES[y].push_back(x);
		}

	}
}

int degree(const int& x, std::vector<std::vector<int>>& NODES, std::vector<int> mask) {
	int deg = 0;
	//std::vector<bool> was(NODES.size());
	//was[x] = true;
	mask[x] = 2;
	for (auto i : NODES[x]) {
		if (mask[i] == -1) {
			for (auto j : NODES[i]) {
				if (mask[j] == 0) {
					++deg;
					mask[j] = 2;
				}
			}
		}
		else if (mask[i] == 0) {
			++deg;
			mask[i] = 2;
		}
	}
	return deg;
}


void MinDegree(const int& n, int* Rst, int* Col, int* perm) {
	using namespace std;
	vector<int> degrees(n);
	vector<int> mask(n);
	vector<vector<int>> NODES(n);
	for (int i = 0; i < n; ++i) {
		for (int j = Rst[i]; j < Rst[i + 1]; ++j) {
			NODES[i].push_back(Col[j]);
		}
		mask[i] = 0;
		degrees[i] = NODES[i].size();
	}
	int num = 0;
	while (num < n) {
		//cout << "Step: " << num << '\n';
		int x = 0, min_deg = n + 1;
		for (int i = 0; i < n; ++i) {
			if (mask[i] == 0 && degrees[i] < min_deg) {
				x = i;
				min_deg = degrees[i];
			}
		}
		perm[x] = num++;
		transform_(x, NODES, mask);
		for (int i = 0; i < NODES[x].size(); ++i) {
			degrees[NODES[x][i]] = degree(NODES[x][i], NODES, mask);
		}
	}
}