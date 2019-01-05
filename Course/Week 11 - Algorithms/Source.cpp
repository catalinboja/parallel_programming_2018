#include <string>
#include <fstream>
#include <iostream>
#include <omp.h>

using namespace std;


int sequentialSearch(int* array, int n, int searchedValue) {
	int foundat = -1;
	for (int i = 0; i < n; i++)
		if (array[i] == searchedValue)
			foundat = i + 1;
	return foundat;
}


int parallelSearch(int* array, int n, int searchedValue) {
	int foundat = -1;

#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		if (array[i] == searchedValue)
			foundat = i + 1;
	}
	return foundat;
}

int binarySearchRoutine(int* array, int left, int right, int searchedValues)
{
	if (right >= left)
	{
		int middle = left + (right - left) / 2;
		if (array[middle] == searchedValues)
			return middle;

		if (array[middle] > searchedValues)
			return binarySearchRoutine(array, left, middle - 1, searchedValues);

		return binarySearchRoutine(array, middle + 1, right, searchedValues);
	}

	return -1;
}


int binarySearch(int* array, int n, int searchedValue) {
	return binarySearchRoutine(array, 0, n-1, searchedValue);
}


void initArray(int*& array, int N) {
	array = new int[N];
	for (int i = 0; i < N; i++) {
		array[i] = i + 1;
	}
}

void performanceTest(int(*pf)(int*, int, int), int* array, int n, int searchedValue, string message) {

	cout << endl << "Searching by " << message << " the value " << searchedValue;
	double tStart = omp_get_wtime();

	int index = pf(array, n, searchedValue);

	double tFinal = omp_get_wtime();

	cout << endl << searchedValue << " found on " << index << " in " << tFinal - tStart;
}

//strings
int search(const char *given, const char *value)
{
	int n = strlen(given);
	int m = strlen(value);
	int i, j;
	for (i = 0; i <= n - m; i++)
	{
		for (j = 0; j < m; j++)
			if (given[i + j] != value[j])
				break;

		if (j == m)
			return i;
	}
	return -1;
}


int main() {

	static const long long N = 2*1e8;
	int* array;
	initArray(array, N);

	performanceTest(sequentialSearch, array, N, N-1, "sequentialSearch");
	performanceTest(parallelSearch, array, N, N - 1, "parallelSearch");
	performanceTest(binarySearch, array, N, N - 1, "binarySearch");

	cout << endl << search("asjdkjhjkdddgfghh", "ddd");

}


