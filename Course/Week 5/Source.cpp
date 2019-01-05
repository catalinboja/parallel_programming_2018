#include <iostream>
#include <omp.h>
using namespace std;

void main() {

	cout << endl << "Inceput program";

	int contor = 0;

	int contorPrivat = 100;

	int flagParalel = 1;

#pragma omp parallel firstprivate(contorPrivat) shared(contor) \
num_threads(16) if(flagParalel)
	{
		cout << endl << "Numar maxim:" << omp_get_max_threads();
		int threadId = omp_get_thread_num();
		for (int i = 0; i < 20; i++) {
			contor += 1;
			contorPrivat += 1;
			cout << endl << "Hello World de la "
				<< threadId;
		}
	}

	cout << endl << "Numar iteratii: " << contor;
	cout << endl << "Numar iteratii contor privat: " 
		<< contorPrivat;
	cout << endl << "SFARSIT program";
}