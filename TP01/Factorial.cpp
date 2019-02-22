/*
 * Factorial.cpp
 */

#include "Factorial.h"
#include <vector>

int factorialRecurs(int n)
{
	if (n <= 1) return 1;
	return n * factorialRecurs(n-1);
}

int factorialDinam(int n)
{
	vector<int> pre_calc(2, 1);

	for (int i = 2; i <= n; i++)
		pre_calc.push_back(i * pre_calc.at(i-1));

	return pre_calc.at(n);
}
