#include <iostream>
#include <omp.h>
using namespace std;

void main() {

	//secvential : 18 secunde

	const int N = 100000;
	long long contor = 0;

	double tStart = omp_get_wtime();

	long long rezultate[4][1024];
	for (int i = 0; i < 4; i++)
		rezultate[i][0] = 0;

#pragma omp parallel num_threads(4)
	{
		int nrThreaduri = omp_get_num_threads();
		int id = omp_get_thread_num();
		for (int j = id* N / nrThreaduri; j < (id+1)*N/nrThreaduri; j++)
			for (int i = 0; i < N; i++) {
				if ((i + 1) % 2 == 0)
					rezultate[id][0] += 1;
			}
	}

	double tFinal = omp_get_wtime();

	for (int i = 0; i < 4; i++)
		contor += rezultate[i][0];


	std::cout << endl << "Numar elemente pare: " << contor 
		<< " - in " << tFinal - tStart;

}
