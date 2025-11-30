#include "MinDegree2.h"

std::vector<int> adj(const int& x, const std::vector<std::vector<int>>& NODES, std::vector<int> mask) {
	using namespace std;
	vector<int> adj_;
	//adj_.reserve(NODES.size());
	for (auto i : NODES[x]) {
		if (mask[i] == -1 || mask[i] == 1) {
			for (auto j : NODES[i]) {
				if (mask[j] == 0 && j != x) {
					mask[j] = 2;
					adj_.push_back(j);
				}
			}
		}
		else if (mask[i] == 0) {
			mask[i] = 2;
			adj_.push_back(i);
		}
	}
	return adj_;
}

//void transform_(const int& x, std::vector<std::vector<int>>& NODES, std::vector<int>& mask) {
//	using namespace std;
//	vector<int> adj_ = adj(x, NODES, mask);
//	for (auto elem : NODES[x]) {
//		if (mask[elem] == -1) {
//			mask[elem] = 1;
//		}
//	}
//	NODES[x] = adj_;
//	mask[x] = -1;
//	for (auto y : NODES[x]) {
//		bool flag = false;
//		for (int i = 0; i < NODES[y].size(); ++i) {
//			if (NODES[y][i] == x) flag = true;
//			if (mask[NODES[y][i]] == 1) {
//				NODES[y].erase(NODES[y].begin() + i);
//			}
//		}
//		if (!flag) {
//			NODES[y].push_back(x);
//		}
//	}
//}

int degree(const int& x, const std::vector<std::vector<int>>& NODES, std::vector<int> mask) {
	int deg = 0;
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

void transform_(std::queue<int>& x, std::vector<std::vector<int>>& NODES, std::vector<int>& mask, int* perm, int& num) {
	using namespace std;
	for (auto elem : NODES[x.back()]) {
		if (mask[elem] == -1) {
			mask[elem] = 1;
		}
	}
	if (x.size() != 1) {
		int t = NODES[x.front()].size();
		int front = x.front();
		while (x.size() > 1) {
			mask[x.front()] = 1;
			perm[x.front()] = num++;
			x.pop();
		}
		NODES[x.back()].resize(0);
		for (int i = 0; i < t; ++i) {
			if (NODES[front][i] != x.back() && mask[NODES[front][i]] != 1) {
				NODES[x.back()].push_back(NODES[front][i]);
			}
		}
	}
	//vector<int> adj_ = adj(x.back(), NODES, mask);
	//NODES[x.back()] = adj(x.back(), NODES, mask);
	mask[x.front()] = -1;
	perm[x.front()] = num++;
	for (auto y : NODES[x.back()]) {
		bool flag = false;
		vector<int> tmp;
		for (int i = 0; i < NODES[y].size(); ++i) {
			if (NODES[y][i] == x.back()) flag = true;
			if (mask[NODES[y][i]] != 1) {
				tmp.push_back(NODES[y][i]);
			}
		}
		if (!flag) {
			//NODES[y].push_back(x.back());
			tmp.push_back(x.back());
		}
		NODES[y].swap(tmp);
	}
}


void MinDegree(const int& n, int* Rst, int* Col, int* perm) {
	using namespace std;
	vector<int> degrees(n);
	vector<int> mask(n);
	vector<vector<int>> NODES(n);
	vector<int*> QLINK;
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
		queue<int> indis;
		indis.push(x);
		NODES[x] = adj(x, NODES, mask);
		sort(NODES[x].begin(), NODES[x].end());
		for (int y = 0; y < n; ++y) {
			if (y == x || mask[y] != 0) {
				continue;
			}
			else if (degrees[x] == degrees[y]) {
				bool indistinguishable = true;
				vector<int> y_reach = adj(y, NODES, mask);
				sort(y_reach.begin(), y_reach.end());
				int i = 0, j = 0;
				while (i < NODES[x].size() && j < y_reach.size()) {
					if (NODES[x][i] == y) {
						++i;
						continue;
					}
					else if (y_reach[j] == x) {
						++j;
						continue;
					}
					if (NODES[x][i++] != y_reach[j++]) {
						indistinguishable = false;
						break;
					}
				}
				if (indistinguishable) {
					indis.push(y);
				}
			}
		}
		//perm[x] = num++;
		transform_(indis, NODES, mask, perm, num);
		for (int i = 0; i < NODES[indis.back()].size(); ++i) {
			degrees[NODES[indis.back()][i]] = degree(NODES[indis.back()][i], NODES, mask);
		}
	}
}