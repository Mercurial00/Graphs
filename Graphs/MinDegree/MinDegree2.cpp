//#include <iostream>
#include "MinDegree2.h"
#include <iostream>
//#include <iomanip>

#define EMPTY (-1)
#define FLIP(x) (-(x) - 2)
static int COMPRESS_CNT = 0;

static int32_t counter = 1;
//static uint64_t SIMILAR_VERTEXES_VISIT_CNT = 0;
//static uint64_t DIFFERENT_VERTEXES_VISIT_CNT = 0;

//std::vector<int> debug_nodes;

struct Active_nodes {
	std::vector<std::vector<size_t>> active;
	int* degrees;
	int _min_deg;

	Active_nodes(const int& size, int* degrees) : active(size + 1), degrees(degrees), _min_deg(size) {}

	void push(const size_t& node) {
		active[degrees[node]].push_back(node);
		if (degrees[node] < _min_deg) {
			_min_deg = degrees[node];
		}
	}

	size_t min_node() {
		while (true) {
			while (active[_min_deg].empty()) ++_min_deg;
			while (!active[_min_deg].empty() && degrees[active[_min_deg].back()] != _min_deg) {
				active[_min_deg].pop_back();
			}
			if (!active[_min_deg].empty() && degrees[active[_min_deg].back()] == _min_deg) {
				size_t t = active[_min_deg].back();
				active[_min_deg].pop_back();
				return t;
			}
		}
	}

	~Active_nodes() = default;
};

int compress(int* ws, int* pe, const int* elen, const int* len, const int& pfree, const int nodesCnt) {
	int newPfree = 0;

	for (int i = 0; i < nodesCnt; ++i) {
		if (pe[i] <= EMPTY)
			continue;

		int old = pe[i];
		int length = len[i];
		pe[i] = newPfree;

		for (int j = 0; j < length; ++j) {
			ws[newPfree++] = ws[old + j];
		}
	}
	
	return newPfree;
}

inline int compressPath(int x, int* parent) {
	int r = x;
	while (parent[r] != r) {
		r = parent[r];
	}
	while (parent[x] != x) {
		int p = parent[x];
		parent[x] = r;
		x = p;
	}
	return r;
}

void reach(const int& x, int pfree, int newElem, int* ws, int* pe, int* len, int* elen, int* parent, int* mask, int* degrees, const int deg) {
	using namespace std;
	int k = 0;

	int p = pe[x];
	len[newElem] = 0;
	for (; p < pe[x] + elen[x]; ++p) {
		int id = ws[p];
		while (parent[id] != id) {
			id = compressPath(id, parent);
		}
		if (pe[id] <= EMPTY) 
			continue;
		for (int j = pe[id] + elen[id]; j < pe[id] + len[id]; ++j) {
			if (mask[ws[j]] != counter) {
				mask[ws[j]] = counter;
				ws[pfree++] = ws[j];
				k++;
				len[newElem]++;
			//	DIFFERENT_VERTEXES_VISIT_CNT++;
			}
			//else {
			//	SIMILAR_VERTEXES_VISIT_CNT++;
			//}
		}
	}
	for (; p < pe[x] + len[x]; ++p) {
		if (mask[ws[p]] != counter) {
			mask[ws[p]] = counter;
			ws[pfree++] = ws[p];
			k++;
			len[newElem]++;
			//DIFFERENT_VERTEXES_VISIT_CNT++;
			//reach_.push_back(ws[p]);
		}
		//else {
		//	SIMILAR_VERTEXES_VISIT_CNT++;
		//}
	}

	return;
}

bool reach_cmp(const int& x, int* ws, int* pe, int* len, int* elen, int* parent, int* mask, int* degrees
	, const int& degree, int was[]) {
	using namespace std;
	int k = 0;
	mask[x] = 0;
	for (int p = pe[x]; p < pe[x] + elen[x]; ++p) {
		int id = ws[p];
		while (parent[id] != id) {
			id = parent[id];
		}
		if (pe[id] <= EMPTY)
			continue;
		for (int j = pe[id] + elen[id]; j < pe[id] + len[id]; ++j) {
			if (mask[ws[j]] == counter) {
				mask[ws[j]] = 0;
				was[k++] = ws[j];
			//	DIFFERENT_VERTEXES_VISIT_CNT++;
			}
			//else {
			//	SIMILAR_VERTEXES_VISIT_CNT++;
			//}
		}
		if (k == degree) {
			mask[x] = counter;

			for (int i = 0; i < k; ++i) {
				mask[was[i]] = counter;
			}

			return true;
		}
	}
	for (int p = pe[x] + elen[x]; p < pe[x] + len[x]; ++p) {
		if (mask[ws[p]] == counter) {
			mask[ws[p]] = 0;
			was[k++] = ws[p];
		//	DIFFERENT_VERTEXES_VISIT_CNT++;
		}
		//else {
		//	SIMILAR_VERTEXES_VISIT_CNT++;
		//}
		if (k == degree) break;
	}
	mask[x] = counter;

	for (int i = 0; i < k; ++i) {
		mask[was[i]] = counter;
	}

	return k == degree;
}

int degree(const int& x, int* ws, int* pe, int* len, int* elen, int* parent, int* spn_sz, int* mask, int* degrees) {
	int deg = 0;
	mask[x] = counter;
	int k = 0;

	int p = pe[x];
	for (; p < pe[x] + elen[x]; ++p) {
		int id = ws[p];
		while (parent[id] != id) {
			id = parent[id];
		}
		if (pe[id] <= EMPTY)
			continue;
		for (int j = pe[id] + elen[id]; j < pe[id] + len[id]; ++j) {
			if (mask[ws[j]] != counter) {
				mask[ws[j]] = counter;
				deg += spn_sz[ws[j]];
			//	DIFFERENT_VERTEXES_VISIT_CNT++;
			}
			//else {
			//	SIMILAR_VERTEXES_VISIT_CNT++;
			//}
		}
	}
	for (; p < pe[x] + len[x]; ++p) {
		if (mask[ws[p]] != counter) {
			mask[ws[p]] = counter;
			//was[k++] = ws[p];
			deg += spn_sz[ws[p]];
		//	DIFFERENT_VERTEXES_VISIT_CNT++;
		}
		//else {
		//	SIMILAR_VERTEXES_VISIT_CNT++;
		//}
	}

	return deg;
}


void transform(int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, int* degrees, int* mask, int* perm, int& pfree, int oldVar, int newElem, int& num, const int wsSize) {
	
	parent[newElem] = newElem;
	pe[newElem] = pfree;
	spn_sz[newElem] = spn_sz[oldVar];
	elen[newElem] = 0;
	for (int i = pfree; i < pfree + len[newElem]; ++i) {
		if (mask[ws[i]] == counter - 1) {
			pe[ws[i]] = EMPTY;
			parent[ws[i]] = newElem;
			//spn_sz[newElem] += spn_sz[node];
			spn_sz[ws[i]] = 0;
			elen[ws[i]] = 0;
			degrees[ws[i]] = EMPTY;
			perm[ws[i]] = num++;
			mask[ws[i]] = 0;
			std::swap(ws[i], ws[pfree + len[newElem] - 1]);
			--len[newElem];
			--i;
			continue;
		}
		mask[ws[i]] = counter - 1;
	}
	pfree += len[newElem];
	int length = len[newElem];
	for (int i = pe[newElem]; i < pe[newElem] + length; ++i) {
		bool found = false;
		int curr = ws[i];
		for (int j = pe[curr]; j < pe[curr] + elen[curr];) {
			if (pe[ws[j]] <= EMPTY) {
				std::swap(ws[j], ws[pe[curr] + elen[curr] - 1]);
				--elen[curr];
			}
			else {
				++j;
			}
		}
		for (int j = pe[curr] + elen[curr]; j < pe[curr] + len[curr];) {
			
			if (!found && (ws[j] == oldVar || parent[ws[j]] == newElem)) {
				found = true;
				ws[j] = newElem;
				std::swap(ws[pe[curr] + elen[curr]], ws[j]);
				++elen[curr];
				++j;
			}
			else if (found && (ws[j] == oldVar || parent[ws[j]] == newElem)) {
				std::swap(ws[j], ws[pe[curr] + len[curr] - 1]);
				--len[curr];
			}
			else if (pe[ws[j]] <= EMPTY) {
				std::swap(ws[j], ws[pe[curr] + len[curr] - 1]);
				--len[curr];
			}
			else {
				++j;
			}
		}
	}


}

inline void prepareVertex(int x, int* pe, int* ws, int* len, int* elen, int* parent) {
	int length = len[x];
	int p = pe[x];
	for (int p = pe[x]; p < pe[x] + length; ++p) {
		int curr = ws[p];
		for (int j = pe[curr]; j < pe[curr] + elen[curr];) {
			if (pe[ws[j]] <= EMPTY) {
				std::swap(ws[j], ws[pe[curr] + elen[curr] - 1]);
				--elen[curr];
			}
			else {
				++j;
			}
		}
		for (int j = pe[curr] + elen[curr]; j < pe[curr] + len[curr];) {
			if (pe[ws[j]] <= EMPTY) {
				std::swap(ws[j], ws[pe[curr] + len[curr] - 1]);
				--len[curr];
			}
			else {
				++j;
			}
		}
	}
}

// preparation
void MinDegree(const int n, const int* Rst, const int* Col, int* perm) {
	const int wsSize = (int)(2 * Rst[n]);
	//const int wsSize = (int)(Rst[n] + 2 * n);

	int* ws = new int[wsSize];
	int* pe = new int[2 * n + 1];
	int* len = new int[2 * n];
	int* elen = new int[2 * n] {};
	int* spn_sz = new int[2 * n];
	int* parent = new int[2 * n];

	int* degrees = new int[2 * n];
	int* mask = new int[n] {};
	int* was = new int[n];

	Active_nodes act(n, degrees);

	for (int i = 0; i < n; ++i) {
		pe[i] = Rst[i];
		for (int j = Rst[i]; j < Rst[i + 1]; ++j) {
			ws[j] = Col[j];
		}
		len[i] = Rst[i + 1] - Rst[i];
		mask[i] = 0;
		degrees[i] = len[i];
		spn_sz[i] = 1;
		parent[i] = i;
		act.push(i);
	}
	pe[n] = Rst[n];
	MinDegree_(n, wsSize, pe, ws, len, elen, spn_sz, parent, was, mask, degrees, act, perm);

	delete[] ws;
	delete[] pe;
	delete[] len;
	delete[] elen;
	delete[] spn_sz;
	delete[] parent;

	delete[] was;
	delete[] mask;
	delete[] degrees;
}


void MinDegree_(const int n, const int wsSize, int* pe, int* ws, int* len, int* elen, int* spn_sz, int* parent,
	int* was, int* mask, int* degrees, Active_nodes& act, int* perm) {
	using namespace std;

	int pfree = pe[n];
	int vertexCnt = n;
	int num = 0;

	while (num < n) {
		int x = act.min_node(); // выбор узла с минимальной степенью
		perm[x] = num++;
		prepareVertex(x, pe, ws, len, elen, parent); // очистка списков смежности от лишних узлов
		//debug_nodes.push_back(x);

		int newElem = vertexCnt++;
		mask[x] = counter;
		if (pfree + 2 * degrees[x] > wsSize) {
			pfree = compress(ws, pe, elen, len, pfree, vertexCnt);
			if (pfree + degrees[x] > wsSize) {
					throw "ERROR. Not enough space in ws";
			}
			COMPRESS_CNT++;
			std::cout << "compression function was called on step " << num << ". Total: " << COMPRESS_CNT << '\n';
		}

		// нахождение достижимого множества текущей вершины
		reach(x, pfree, newElem, ws, pe, len, elen, parent, mask, degrees, degrees[x]);
		counter += 2;
		//if (counter < 0) counter = 0;
		mask[x] = counter;

		// временная пометка для сравнений
		for (int i = 0; i < len[newElem]; ++i) {
			mask[ws[pfree + i]] = counter;
		}
		int indCnt = 0;

		// поиск неразличимых вершин
		for (int i = 0; i < len[newElem]; ++i) {
			int y = ws[pfree + i];
			if (degrees[x] == degrees[y]) {

				bool indistinguishable = reach_cmp(y, ws, pe, len, elen, parent, mask, degrees, len[newElem], was);

				if (indistinguishable) {
					//debug_nodes.push_back(y);
					ws[pfree + len[newElem] + indCnt] = y;
					++indCnt;
				}
			}
		}
		//std::sort(debug_nodes.begin(), debug_nodes.end());

		degrees[newElem] = degrees[x] = EMPTY;
		//spn_sz[newElem] = spn_sz[x];
		mask[x] = counter - 1;

		// пометка неразличимых с x вершин
		for (int y = pfree + len[newElem]; y < pfree + len[newElem] + indCnt; ++y) {
			mask[ws[y]] = counter - 1;
		}

		// слияние нового элемента с соседними элементами
		for (int i = pe[x]; i < pe[x] + elen[x]; ++i) {
			parent[ws[i]] = newElem;
			pe[ws[i]] = FLIP(newElem);
			elen[ws[i]] = 0;
			spn_sz[ws[i]] = 0;
			degrees[ws[i]] = EMPTY;
		}

		transform(pe, ws, len, elen, spn_sz, parent, was, degrees, mask, perm, pfree, x, newElem, num, wsSize);
		++counter;
		//if (counter < 0) counter = 0;

		// обновление степеней
		int p = pe[newElem];
		for (int i = 0; i < len[newElem]; ++i) {
			int curr = ws[p + i];
			degrees[curr] = degree(curr, ws, pe, len, elen, parent, spn_sz, mask, degrees);
			act.push(curr);
			++counter;
			//if (counter < 0) counter = 0;

		}
	}
	//cout << "Total different vertexes visits count: " << DIFFERENT_VERTEXES_VISIT_CNT << '\n';
	//cout << "Total similar vertexes visits count: " << SIMILAR_VERTEXES_VISIT_CNT << '\n';
	//cout << "Percentage of empty visits: " << fixed << setprecision(2)
	//	<< (double)SIMILAR_VERTEXES_VISIT_CNT / (SIMILAR_VERTEXES_VISIT_CNT + DIFFERENT_VERTEXES_VISIT_CNT) * 100 << "%\n";
}

