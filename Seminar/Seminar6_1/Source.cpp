#include <iostream>
#include <omp.h>
using namespace std;

void main() {


	double tStart = omp_get_wtime();

	int variabilaPrivata = 5;
	int variabilaGlobala = 100;
	int variabilaLocala = 50;
	variabilaLocala += 30;

#pragma omp parallel firstprivate(variabilaLocala) private(variabilaPrivata) \
	shared(variabilaGlobala) num_threads(4)
	{
		cout << endl << "Valoare initiala locala: " << variabilaLocala;
		variabilaPrivata = 0;
		variabilaPrivata += 1;
		variabilaLocala += 1;
		variabilaGlobala += 1;
		int nrThreaduri = omp_get_num_threads();
		int id = omp_get_thread_num();
		std::cout << endl << "Hello World OMP from "
			<< id << " - " << nrThreaduri;
	}


	double tFinal = omp_get_wtime();

	std::cout << endl << "Variabila globala: " << variabilaGlobala;
	std::cout << endl << "Variabila privata: " << variabilaPrivata;

	std::cout << endl << "Durata (ms): " << tFinal - tStart;

}