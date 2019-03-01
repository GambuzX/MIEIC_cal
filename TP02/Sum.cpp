/*
 * Sum.cpp
 */

#include "Sum.h"
#include <chrono>
#include <string.h>

string calcSum(int* sequence, int size)
{
	int s[size+1];
	int i[size+1];
	memset(s, 99, sizeof(int) * (size+1));
	memset(i, -1, sizeof(int) * (size+1));

	for (int n = 0; n <= size; n++) {
		for (int m = 0; m <= size; m++) {
			if (m > n) continue;

			int new_sum = 0;
			int last_index = n;
			for (int count = m; count > 0; count--) {
				new_sum += sequence[last_index-1];
				last_index--;
			}
			if (s[m] > new_sum) {
				s[m] = new_sum;
				i[m] = n - m;
			}
		}
	}

	string ret_string = "";
	for (int iter = 1; iter < size+1; iter++)
		ret_string += to_string(s[iter]) + "," + to_string(i[iter]) + ";";

	cout << ret_string << endl;

	return ret_string;
}

