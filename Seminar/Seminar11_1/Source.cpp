#include <iostream>
#include <omp.h>

using namespace std;

const int N = 2*1e8;

int cautareSecventiala(int* valori, int N, int valoare) {
	for (int i = 0; i < N; i++)
		if (valoare == valori[i])
			return i;
	return -1;
}

int cautareParalela(int* valori, int N, int valoare) {
	int index = -1;
#pragma omp parallel for num_threads(4) shared(index)
	for (int i = 0; i < N; i++)
		if (valoare == valori[i])
			index = i;
	return index;
}


//https://www.geeksforgeeks.org/binary-search/
int binarySearch(int arr[], int l, int r, int x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		if (arr[mid] == x)
			return mid;

		if (arr[mid] > x)
			return binarySearch(arr, l, mid - 1, x);
		return binarySearch(arr, mid + 1, r, x);
	}
	return -1;
}

int cautareBinara(int* valori, int N, int valoare) {
	return binarySearch(valori, 0, N, valoare);
}

//cautare de subsiruri in string-uri
int cautareSecventialaSubsir(const char* sir, const char* subsir) {
	int n = strlen(sir);
	int m = strlen(subsir);

	for (int i = 0; i < n - m; i++) {
		bool gasit = true;
		for (int j = 0; j < m; j++) {
			if (sir[i+j] != subsir[j]) {
				gasit = false;
				break;
			}
		}
		if (gasit == true)
			return i;
	}

	return -1;
}

//cautare de subsiruri in string-uri
int cautareParalelaSubsir(const char* sir, const char* subsir) {
	int n = strlen(sir);
	int m = strlen(subsir);

	int index = -1;

#pragma omp parallel for num_threads(4) shared(index) \
schedule(static,10000)
	for (int i = 0; i < n - m; i++) {
		bool gasit = true;
		for (int j = 0; j < m; j++) {
			if (sir[i + j] != subsir[j]) {
				gasit = false;
				break;
			}
		}
		if (gasit == true)
			index = i;
	}

	return index;
}

void main()
{
	//generare vector sortat crescator
	int* valori = new int[N];
	for (int i = 0; i < N; i++)
		valori[i] = i + 1;

	//test performanta cautare secventiala
	double tStart = omp_get_wtime();
	cautareSecventiala(valori, N, N);
	double tFinal = omp_get_wtime();
	cout << endl << "Cautare secventiala in " << tFinal - tStart << "secunde";

	//test performanta cautare paralela
	tStart = omp_get_wtime();
	cautareParalela(valori, N, N);
	tFinal = omp_get_wtime();
	cout << endl << "Cautare paralela in " << tFinal - tStart << "secunde";

	//test performanta cautare binara
	tStart = omp_get_wtime();
	cautareBinara(valori, N, N);
	tFinal = omp_get_wtime();
	cout << endl << "Cautare binara in " << tFinal - tStart << "secunde";

	delete[] valori;

	//testare cautare subsiruri
	//generare sir de forma aaaaa.......aab
	char * sir = new char[4*N];
	for (int i = 0; i < 4 * N; i++)
		sir[i] = 'a';
	sir[4 * N - 1] = 'b';

	//test performanta cautare naiva subsir
	tStart = omp_get_wtime();
	int index = cautareSecventialaSubsir(sir, "aab");
	tFinal = omp_get_wtime();
	cout << endl << "Cautare subsir in " << tFinal - tStart << "secunde";
	cout << endl << "Valoare gasita pe pozitia " << index;

	//test performanta cautare paralela subsir
	tStart = omp_get_wtime();
	index = cautareParalelaSubsir(sir, "aab");
	tFinal = omp_get_wtime();
	cout << endl << "Cautare subsir in " << tFinal - tStart << "secunde";
	cout << endl << "Valoare gasita pe pozitia " << index;

}