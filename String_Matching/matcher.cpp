/*
 * matcher.cpp
 */
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

vector<int> compute_prefix_function(string pattern) {
	int m = pattern.length();
	vector<int> pi(m);
	pi[0] = 0;
	int k = 0;
	for (int q = 1; q < m; q++) {
		while(k > 0 && pattern[k] != pattern[q])
			k = pi[k-1];

		if (pattern[k] == pattern[q])
			k += 1;

		pi[q] = k;
	}
	return pi;
}

int kmpMatcher(string text, string pattern) {
	int n = text.length();
	int m = pattern.length();
	vector<int> pi = compute_prefix_function(pattern);
	int q = 0;

	int total = 0;
	for (int i = 0; i < n; i++) {
		while(q > 0 && pattern[q] != text[i])
			q = pi[q-1];
		if (pattern[q] == text[i])
			q += 1;
		if (q == m) {
			total++;
			q = pi[q-1];
		}
	}
	return total;
}


int numStringMatching(string filename,string toSearch) {
	ifstream ifs(filename);
	if (!ifs.is_open()) return 0;
	string line;
	int total = 0;
	while(getline(ifs, line)) {
		total += kmpMatcher(line, toSearch);
	}

	ifs.close();
	return total;
}

int min_f(int a, int b) {
	return a > b ? b : a;
}


int editDistance(string pattern, string text) {
	int P = pattern.length();
	int T = text.length();
	vector<vector<int>> D;
	D.resize(P+1);
	for (vector<int> & v : D) v.resize(T+1);

	for (int i = 0 ; i <= P; i++) D[i][0] = i;
	for (int i = 0; i <= T; i++) D[0][i] = i;

	for (int i = 1; i <= P; i++) {
		for (int j = 1; j <= T; j++) {
			if (pattern[i-1] == text[j-1]) D[i][j] = D[i-1][j-1];
			else {
				D[i][j] = 1 + min_f(D[i-1][j-1], min_f(D[i-1][j], D[i][j-1]));
			}
		}
	}

	return D[P][T];
}

float numApproximateStringMatching(string filename,string toSearch) {
	ifstream ifs(filename);
	string word;
	int total = 0, count = 0;
	while(!ifs.eof()) {
		ifs >> word;
		count++;
		total += editDistance(toSearch, word);
	}

	ifs.close();
	return (float)total / (float)count;
}
