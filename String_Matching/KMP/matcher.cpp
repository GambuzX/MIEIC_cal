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


int editDistance(string pattern, string text) {
	// TODO
	return 0;
}

float numApproximateStringMatching(string filename,string toSearch) {
	// TODO
	return 0;
}
