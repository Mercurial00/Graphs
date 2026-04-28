#pragma once
#include <vector>
#include <algorithm>

struct Active_nodes;

void reach(const int& x, int pfree, int newElem, int* ws, int* pe, int* len,
	int* elen, int* parent, int* mask, int* degrees, const int deg);

bool reach_cmp(const int& x, int* ws, int* pe, int* len, int* elen, int* parent,
	int* mask, int* degrees, const int& degree, int was[]);

void transform(int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, int* degrees, int* mask, int* perm, int& pfree,
	int oldVar, int newElem, int& num, const int wsSize);

int degree(const int& x, int* ws, int* pe, int* len, int* elen, int* parent,
	int* spn_sz, int* mask, int* degrees);

int compress(int* ws, int* pe, const int* elen, const int* len, const int& pfree, const int nodesCnt);

inline int compressPath(int x, int* parent);

void MinDegree_(const int n, const int wsSize, int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, int* mask, int* degrees, Active_nodes& act, int* perm);
void MinDegree(const int n, const int* Rst, const int* Col, int* perm);