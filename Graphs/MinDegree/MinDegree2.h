#pragma once
#include <vector>
#include <queue>
#include <algorithm>

static struct Active_nodes;

std::vector<int> reach(const int& x, const std::vector<std::vector<int>>& NODES,
	char* mask, const int& degree);

void transform_(std::queue<int>& x, std::vector<std::vector<int>>& NODES,
	char* mask, int* perm, int& num, const int& deg, Active_nodes& act);

int degree(const int& x, const std::vector<std::vector<int>>& NODES,
	char* mask, int was[]);

void MinDegree(const int& n, const int* Rst, const int* Col, int* perm);