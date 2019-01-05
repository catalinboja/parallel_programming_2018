#include <iostream>
using namespace std;

const long N = 1 * 1e5;

void bitonicMerge(int up, int * sequence, int size);
void bitonicCompare(int up, int * sequence, int size);
void swap(int * x, int * y);

void bitonicSort(int up, int * sequence, int size)
{
	if (size == 1)
		return;
	else
	{
		bitonicSort(1, sequence, size / 2);
		bitonicSort(0, sequence + size / 2, size / 2);
		bitonicMerge(up, sequence, size);
	}
}

void bitonicMerge(int up, int * sequence, int size)
{
	if (size == 1)
		return;
	else
	{
		bitonicCompare(up, sequence, size);
		bitonicMerge(up, sequence, size / 2);
		bitonicMerge(up, sequence + size / 2, size / 2);
	}
}

void bitonicCompare(int up, int * sequence, int size)
{
	int distance = size / 2;
	int * start, *end = sequence + distance;
	for (start = sequence; start < end; start++)
		if ((*start > *(start + distance)) == up)
			swap(start, start + distance);
}

void swap(int * x, int * y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

void printVector(int* v, int N) {
	cout << endl << "Vector:";
	for (int i = 0; i < N; i++)
		cout << " " << v[i];
}


int main(int argc, char ** argv)
{

	int valori[N];

	//generare vector initial - sortat descrescator
	for (int i = 0; i < N; i++)
		valori[i] = N - i;

	bitonicSort(1, valori, N);

	printVector(valori, 20);

	//test sortare cu succes
	int suma = 0;
	for (int i = 1; i < N; i++) {
		suma += (valori[i] - valori[i - 1]);
	}
	cout << endl << "N = : " << N;
	cout << endl << "Suma diferente: " << suma;

	return 0;
}

