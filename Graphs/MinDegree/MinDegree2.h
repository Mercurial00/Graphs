#pragma once
#include <vector>
#include <queue>
#include <algorithm>

std::vector<int> reach(const int& x, const std::vector<std::vector<int>>& NODES, std::vector<char> mask);

void transform_(std::queue<int>& x, std::vector<std::vector<int>>& NODES, std::vector<char>& mask, int* perm, int& num);

int degree(const int& x, const std::vector<std::vector<int>>& NODES, std::vector<char> mask);

void MinDegree(const int& n, int* Rst, int* Col, int* perm);