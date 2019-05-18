/*
 * Partioning.cpp
 */

#include "Partitioning.h"
#include <string.h>


int s_recursive(int n,int k)
{
	if (k > n) return 0; // ERROR
	if (k == 1 || k == n) return 1;
	return s_recursive(n-1, k-1) + k * s_recursive(n-1, k);
}

int s_dynamic(int n,int k)
{
	int pre_calc[n+1][k+1];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= k; j++) {
			if (j > i) continue;

			if (j == 1 || j == i)
				pre_calc[i][j] = 1;
			else
				pre_calc[i][j] = pre_calc[i-1][j-1] + j * pre_calc[i-1][j];
		}
	}

	return pre_calc[n][k];
}


int b_recursive(int n)
{
	int b = 0;
	for (int k = 1; k <= n; k++) b += s_recursive(n, k);
	return b;
}

int b_dynamic(int n)
{
	int pre_calc[n+1][n+1];
	memset(pre_calc, 0, sizeof(pre_calc));

	for (int k = 1; k <= n; k++) {
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= k; j++) {
				if (j > i || pre_calc[i][j] != 0) continue;

				if (j == 1 || j == i)
					pre_calc[i][j] = 1;
				else
					pre_calc[i][j] = pre_calc[i-1][j-1] + j * pre_calc[i-1][j];
			}
		}
	}

	int sum = 0;
	for (int k = 1; k <= n; k++) sum += pre_calc[n][k];
	return sum;
}


