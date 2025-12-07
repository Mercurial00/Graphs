#pragma once
#include <vector>
#include <queue>
#include <algorithm>

std::vector<int> reach(const int& x, const std::vector<std::vector<int>>& NODES, std::vector<int> mask);

void transform_(std::queue<int>& x, std::vector<std::vector<int>>& NODES, std::vector<int>& mask, int* perm, int& num);

int degree(const int& x, const std::vector<std::vector<int>>& NODES, std::vector<int> mask);

void MinDegree(const int& n, int* Rst, int* Col, int* perm);