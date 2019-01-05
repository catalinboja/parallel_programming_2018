#include <iostream>
#include <omp.h>
using namespace std;


const long N = 1 * 1e6;


void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void odd_even_sort(int*v, int N) {
	for (int it = 0; it < N; it++)
	{
		if (it % 2 == 1) {
			for (int i = 2; i < N; i += 2)
				if (v[i - 1] > v[i])
					swap(&v[i - 1], &v[i]);
		}
		else {
			for (int i = 1; i < N; i += 2)
				if (v[i - 1] > v[i])
					swap(&v[i - 1], &v[i]);
		}
	}
}

void odd_even_sort_parallel(int*v, int N) {


	for (int it = 0; it < N; it++)
	{
		if (it % 2 == 1) {
#pragma omp parallel for
			for (int i = 2; i < N; i += 2)
				if (v[i - 1] > v[i])
					swap(&v[i - 1], &v[i]);
		}
		else {
#pragma omp parallel for
			for (int i = 1; i < N; i += 2)
				if (v[i - 1] > v[i])
					swap(&v[i - 1], &v[i]);
		}
	}
}


//https://www.thecrazyprogrammer.com/2014/03/c-program-for-implementation-of-merge-sort.html


void merge(int* a, int i1, int j1, int i2, int j2)
{
	int* temp = new int[(j1+1-i1)*2+1];    //array used for merging
	int i, j, k;
	i = i1;    //beginning of the first list
	j = i2;    //beginning of the second list
	k = 0;

	while (i <= j1 && j <= j2)    //while elements in both lists
	{
		if (a[i] < a[j])
			temp[k++] = a[i++];
		else
			temp[k++] = a[j++];
	}

	while (i <= j1)    //copy remaining elements of the first list
		temp[k++] = a[i++];

	while (j <= j2)    //copy remaining elements of the second list
		temp[k++] = a[j++];

	//Transfer elements from temp[] back to a[]
	for (i = i1, j = 0; i <= j2; i++, j++)
		a[i] = temp[j];

	delete[] temp;
}

void mergesort(int* a, int i, int j)
{
	int mid;

	if (i < j)
	{
		mid = (i + j) / 2;
		mergesort(a, i, mid);        //left recursion
		mergesort(a, mid + 1, j);    //right recursion
		merge(a, i, mid, mid + 1, j);    //merging of two sorted sub-arrays
	}
}

void mergesort_parallel(int* a, int i, int j)
{
	int mid;
#pragma omp parallel shared(mid)
	if (i < j)
	{
#pragma omp sections
		{
			mid = (i + j) / 2;
#pragma omp section
			mergesort(a, i, mid);        //left recursion
#pragma omp section
			mergesort(a, mid + 1, j);    //right recursion
		}
#pragma omp barrier
		merge(a, i, mid, mid + 1, j);    //merging of two sorted sub-arrays
	}
}



void printVector(int* v, int N) {
	cout << endl << "Vector:";
	for (int i = 0; i < N; i++)
		cout << " " << v[i];
}

void main() {
	
	int valori[N];

	//generare vector initial
	for (int i = 0; i < N; i++)
		valori[i] = N - i;

	//test odd even sort - secvential
	double tStart = omp_get_wtime();
	//odd_even_sort_parallel(valori, N);
	mergesort(valori, 0, N - 1);
	double tFinal = omp_get_wtime();

	cout << endl << "Durata " << tFinal - tStart;
	printVector(valori, 20);

	//test sortare cu succes
	int suma = 0;
	for (int i = 1; i < N; i++) {
		suma += (valori[i] - valori[i - 1]);
	}
	cout << endl << "Suma diferente: " << suma;

	/*
	********************
	********************
	*/
	//test sortare merge - secvential
	//generare vector initial
	for (int i = 0; i < N; i++)
		valori[i] = N - i;

	//test merge - secvential
	tStart = omp_get_wtime();
	mergesort_parallel(valori, 0, N-1);
	tFinal = omp_get_wtime();

	cout << endl << "Durata " << tFinal - tStart;
	printVector(valori, 20);

	//test sortare cu succes
	suma = 0;
	for (int i = 1; i < N; i++) {
		suma += (valori[i] - valori[i - 1]);
	}
	cout << endl << "Suma diferente: " << suma;
}