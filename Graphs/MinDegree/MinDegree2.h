#pragma once
#include <vector>
#include <queue>
#include <algorithm>

struct Active_nodes;

void reach(const int& x, int pfree, int newElem, int* ws, int* pe, int* len,
	int* elen, int* parent, char* mask, int* degrees, const int deg);

bool reach_cmp(const int& x, int* ws, int* pe, int* len, int* elen, int* parent,
	char* mask, int* degrees, const int& degree, int was[]);

void transform(int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, int* degrees, char* mask, int* perm, int& pfree,
	int oldVar, int newElem, int& num, const int wsSize);

int degree(const int& x, int* ws, int* pe, int* len, int* elen, int* parent,
	int* spn_sz, char* mask, int* degrees, int was[]);

void MinDegree_(const int n, const int wsSize, int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, char* mask, int* degrees, Active_nodes& act, int* perm);
void MinDegree(const int n, const int* Rst, const int* Col, int* perm);