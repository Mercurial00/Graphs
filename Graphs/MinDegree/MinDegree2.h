#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

struct Active_nodes;

void reach(const int& x, int pfree, int newElem, int* ws, int* pe, int* len,
	int* elen, int* parent, char* mask, int* degrees, const int deg);
std::vector<int> reach(const int& x, const std::vector<std::vector<int>>& NODES,
	char* mask, const int& degree);

bool reach_cmp(const int& x, int* ws, int* pe, int* len, int* elen, int* parent,
	char* mask, int* degrees, const int& degree, int was[]);
bool reach_cmp(const int& x, const std::vector<std::vector<int>>& NODES,
	char* mask, const int& degree, int was[]);


void transform_(std::queue<int>& x, std::vector<std::vector<int>>& NODES,
	char* mask, int* perm, int& num, const int& deg, Active_nodes& act);

int degree(const int& x, int* ws, int* pe, int* len, int* elen, int* parent,
	int* spn_sz, char* mask, int* degrees, int was[]);
int degree(const int& x, const std::vector<std::vector<int>>& NODES, char* mask, int was[]);

void MinDegree_(const int n, const int wsSize, int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, char* mask, int* degrees, Active_nodes& act, int* perm);
void MinDegree(const int n, const int* Rst, const int* Col, int* perm);