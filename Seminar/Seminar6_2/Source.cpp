#include <iostream>
#include <omp.h>

using namespace std;

const long long N = 1e4*5;	//10^4
const long long NMAX = 1e8;	//10^9

long long getNumarNumerePrime(long long N) {
	long long contorPrime = 0;

	for (long long i = 2; i < N; i++) {
		bool estePrim = true;
		for (long long j = 2; j <= i / 2; j++)
			if (i%j == 0) {
				estePrim = false;
				break;
			}
		if (estePrim) {
			contorPrime += 1;
			//cout << endl << i;
		}
	}

	return contorPrime;
}

long long getNumarNumerePrime_Paralel(long long N) {
	long long contorPrime = 0;
	int contorLocal = 0;

#pragma omp parallel for reduction(+:contorPrime)  num_threads(4)
		for (long long i = 2; i < N; i++) {
			bool estePrim = true;
			for (long long j = 2; j <= i / 2; j++)
				if (i%j == 0) {
					estePrim = false;
					break;
				}
			if (estePrim) {
				contorPrime += 1;
				//cout << endl << i;
			}
		}

	return contorPrime;
}

int* sumaVectoriSecventiala(int vector1[], int vector2[], int N) {
	int* rezultat = new int[N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			rezultat[i] = vector1[i] + vector2[i];

	return rezultat;
}

int* sumaVectoriParalela(int vector1[], int vector2[], int N) {
	int* rezultat = new int[N];
#pragma omp parallel for num_threads(4)
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			rezultat[i] = vector1[i] + vector2[i];

	return rezultat;
}

//exemplu sincroniare prin bariera - toate thrread-urile trebuie sa ajunga in acelasi punct
void exempluSyncBariera(long long N) {

#pragma omp parallel num_threads(4)
	{
		int id = omp_get_thread_num();
		for (long long i = 0; i < id*N; i++) {
			//ceva costisitor - operatie care sa ia timp
			long long temp = i % 2;
		}

		cout << endl << "thread " << id << " a terminat procesarea";

#pragma omp barrier
		//se executa dupa ce toate thread-urile au ajuns la bariera
		cout << endl << "thread " << id << " continua procesarea";

	}


}

void testPerformanta(long long(*pf)(long long), long long n) {
	double tStart = omp_get_wtime();

	long long rezultat = pf(n);

	double tFinal = omp_get_wtime();

	cout << endl << "Numar prime: " << rezultat << " determinat in "
		<< tFinal - tStart << " secunde";
}

void main() {

	//test solutie secventiala
	//testPerformanta(getNumarNumerePrime, N);

	//test solutie paralela
	//testPerformanta(getNumarNumerePrime_Paralel, N);

	//test performanta suma elemente vector
	int vector1[N];
	int vector2[N];

	double tStart = omp_get_wtime();

	//int* vectorRezultat = sumaVectoriSecventiala(vector1, vector2, N);
	//int* vectorRezultat = sumaVectoriParalela(vector1, vector2, N);

	double tFinal = omp_get_wtime();

	cout << endl << " Suma determinata in "
		<< tFinal - tStart << " secunde";

	exempluSyncBariera(NMAX);

}