//#include <iostream>
#include "MinDegree2.h"

struct Active_nodes {
	int size;
	std::set<size_t>* active;
	int* degrees;
	int _min_deg;

	Active_nodes(const size_t& size, int* degrees) : size(size), degrees(degrees) {
		active = new std::set<size_t>[size]();
		_min_deg = size;
	}

	void push(const size_t& node) {
		active[degrees[node]].insert(node);
		if (degrees[node] < _min_deg) {
			_min_deg = degrees[node];
		}
	}

	int min_node() {
		return *active[_min_deg].begin();
	}

	void erase(const size_t& node) {
		if (degrees[node] != -1) {
			if (degrees[node] == _min_deg && active[degrees[node]].size() == 1) {
				int i = _min_deg + 1;
				while (active[i].empty() && i != size) ++i;
				_min_deg = i;
			}
			active[degrees[node]].erase(node);
		}
	}

	int min_deg() {
		return _min_deg;
	}

	~Active_nodes() {
		delete[] active;
	}
};


std::vector<int> reach(const int& x, const std::vector<std::vector<int>>& NODES, char* mask, const int& degree) {
	using namespace std;
	vector<int> reach_(degree);
	int k = 0;
	//reach_.reserve(degree);
	mask[x] = 2;
	for (const auto& i : NODES[x]) {
		if (mask[i] == 0) {
			mask[i] = 2;
			reach_[k++] = i;
		}
		else if (mask[i] == -1) {
			for (const auto& j : NODES[i]) {
				if (mask[j] == 0) {
					mask[j] = 2;
					reach_[k++] = j;
				}
			}
		}
		if (k == degree) break;
	}
	mask[x] = 0;
	sort(reach_.begin(), reach_.end());

	for (const int& i : reach_) {
		mask[i] = 0;
	}

	return reach_;
}

int degree(const int& x, const std::vector<std::vector<int>>& NODES, char* mask, int was[]) {
	int deg = 0;
	mask[x] = 2;
	int k = 0;
	was[k++] = x;
	for (const auto& i : NODES[x]) {
		if (mask[i] == 0) {
			++deg;
			mask[i] = 2;
			was[k++] = i;
		}
		else if (mask[i] == -1) {
			for (const auto& j : NODES[i]) {
				if (mask[j] == 0) {
					++deg;
					mask[j] = 2;
					was[k++] = j;
				}
			}
		}
	}

	for (int i = 0; i < k; ++i) {
		mask[was[i]] = 0;
	}

	return deg;
}

void transform_(std::queue<int>& x, std::vector<std::vector<int>>& NODES, char* mask, int* perm, int& num, const int& deg, Active_nodes& act) {
	using namespace std;
	int curr = x.back();
	int merged_cnt = x.size() - 1;
	while (x.size() > 1) {
		vector<int>().swap(NODES[x.front()]);
		mask[x.front()] = 1;
		perm[x.front()] = num++;
		act.erase(x.front());
		act.degrees[x.front()] = -1;
		x.pop();
	}
	act.erase(curr);
	mask[curr] = -1;
	perm[curr] = num++;

	vector<int> tmp(deg - merged_cnt);
	//tmp.reserve(deg - merged_cnt);
	for (int j = 0, k = 0; j < NODES[curr].size(); ++j) {
		const int& y = NODES[curr][j];
		if (mask[y] == 0) {
			int i = 0;
			while (i < NODES[y].size()) {
				if (mask[NODES[y][i]] == 1) {
					NODES[y][i] = curr;
					break;
				}
				++i;
			}
			if (i == NODES[y].size()) {
				NODES[y].push_back(curr);
			}
			tmp[k++] = y;
		}
	}
	act.degrees[curr] = -1;
	NODES[curr].swap(tmp);
}


void MinDegree(const int& n, const int* Rst, const int* Col, int* perm) {
	using namespace std;
	int* degrees = new int[n];
	char* mask = new char[n];
	int* was = new int[n];
	vector<vector<int>> NODES(n);
	Active_nodes act(n, degrees);
	for (int i = 0; i < n; ++i) {
		NODES[i].insert(NODES[i].end(), Col + Rst[i], Col + Rst[i + 1]);
		mask[i] = 0;
		degrees[i] = NODES[i].size();
		act.push(i);
	}

	int num = 0;
	queue<int> indis;

	while (num < n) {
		//cout << "Step: " << num << '\n';

		int min_deg = act.min_deg();
		int x = act.min_node();

		vector<int> x_reach(reach(x, NODES, mask, degrees[x]));
		for (const auto& y : x_reach) {
			if (degrees[x] == degrees[y]) {
				bool indistinguishable = true;
				vector<int> y_reach(reach(y, NODES, mask, degrees[y]));
				int i = 0, j = 0;
				while (i < x_reach.size() && j < y_reach.size()) {
					if (x_reach[i] == y_reach[j]) {
						i++;
						j++;
					}
					else {
						if (x_reach[i] == y) {
							++i;
							continue;
						}
						else if (y_reach[j] == x) {
							++j;
							continue;
						}
						else {
							indistinguishable = false;
							break;
						}
					}

				}
				if (indistinguishable) {
					indis.push(y);
				}
			}
		}

		for (const auto& y : NODES[x]) {
			if (mask[y] == -1) {
				act.erase(y);
				degrees[y] = -1;
				vector<int>().swap(NODES[y]);
				mask[y] = 1;
			}
		}
		NODES[x].swap(x_reach);
		indis.push(x);

		transform_(indis, NODES, mask, perm, num, degrees[x], act);

		indis.pop();

		for (int i = 0; i < NODES[x].size(); ++i) {
			if (mask[NODES[x][i]] == 0) {
				act.erase(NODES[x][i]);
				degrees[NODES[x][i]] = degree(NODES[x][i], NODES, mask, was);
				act.push(NODES[x][i]);
			}
		}
	}
	delete[] was;
	delete[] mask;
	delete[] degrees;
}