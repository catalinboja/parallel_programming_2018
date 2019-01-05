#include <iostream>
#include <omp.h>
using namespace std;

static const int N_ITERATII = 1e9; //10^9

long long getMultipliSecvential(int baza, long long n) {
	long long contor = 0;

	for (long long i = 0; i < n; i++)
		if ((i % baza) == 0)
			contor += 1;
	return contor;
}

//variabila partajata - race conditions
long long getMultipliParalel_Shared(int baza, long long n) {
	long long contor = 0;

#pragma omp parallel shared(contor) num_threads(4)
	{
		int id = omp_get_thread_num();
		int nrThreaduriObtinute = omp_get_num_threads();
		long long limitaSuperioara = (id + 1)*n / nrThreaduriObtinute;
		int limitaInferioara = id * n / nrThreaduriObtinute;

		for (long long i = limitaInferioara; i < limitaSuperioara; i++)
			if ((i % baza) == 0)
				contor += 1;
	}
	return contor;
}

long long getMultipliParalel_Private(int baza, long long n) {
	long long contor = 0;

#pragma omp parallel private(contor) num_threads(4)
	{
		int contor = 0;
		int id = omp_get_thread_num();
		int nrThreaduriObtinute = omp_get_num_threads();
		long long limitaSuperioara = (id + 1)*n / nrThreaduriObtinute;
		int limitaInferioara = id * n / nrThreaduriObtinute;
		for (long long i = limitaInferioara; i < limitaSuperioara; i++)
		{
			if ((i % baza) == 0)
				contor += 1;
		}
	}
	return contor;
}

long long getMultipliParalel_NotShared(int baza, long long n) {

	long long contor = 0;
	const int NR_THREADURI = 4;
	long long rezultat[NR_THREADURI];

	for (int i = 0; i < NR_THREADURI; i++) {
		rezultat[i] = 0;
	}

	int nrThreaduriObtinute = 0;

#pragma omp parallel shared(rezultat, nrThreaduriObtinute) num_threads(NR_THREADURI)
	{
		int id = omp_get_thread_num();
		nrThreaduriObtinute = omp_get_num_threads();
		long long limitaSuperioara = (id + 1)*n / nrThreaduriObtinute;
		int limitaInferioara = id * n / nrThreaduriObtinute;
		for (long long i = limitaInferioara; i < limitaSuperioara; i++)
			if ((i % baza) == 0)
				rezultat[id] += 1;
	}

	for (int i = 0; i < nrThreaduriObtinute; i++)
		contor += rezultat[i];

	return contor;

}

//fara false sharing
long long getMultipliParalel_NotShared_NoFalseSharing(int baza, long long n) {

	long long contor = 0;
	const int NR_THREADURI = 4;
	long long rezultat[NR_THREADURI][1024];

	for (int i = 0; i < NR_THREADURI; i++) {
		rezultat[i][0] = 0;
	}

	int nrThreaduriObtinute = 0;

#pragma omp parallel shared(rezultat, nrThreaduriObtinute) num_threads(NR_THREADURI)
	{
		int id = omp_get_thread_num();
		nrThreaduriObtinute = omp_get_num_threads();
		long long limitaSuperioara = (id + 1)*n / nrThreaduriObtinute;
		int limitaInferioara = id * n / nrThreaduriObtinute;
		for (long long i = limitaInferioara; i < limitaSuperioara; i++)
			if ((i % baza) == 0)
				rezultat[id][0] += 1;
	}

	for (int i = 0; i < nrThreaduriObtinute; i++)
		contor += rezultat[i][0];

	return contor;

}


/*

Utilizare OMP atomic si critical

*/


//lock in bucla
long long getMultipliParalel_Shared_Critical(int baza, long long n) {
	long long contor = 0;

#pragma omp parallel shared(contor) num_threads(4)
	{
		int id = omp_get_thread_num();
		int nrThreaduriObtinute = omp_get_num_threads();
		long long limitaSuperioara = (id + 1)*n / nrThreaduriObtinute;
		int limitaInferioara = id * n / nrThreaduriObtinute;
		for (long long i = limitaInferioara; i < limitaSuperioara; i++)
			if ((i % baza) == 0)
#pragma omp atomic
				contor += 1;
	}
	return contor;
}

//lock in afara buclei
long long getMultipliParalel_Shared_Critical2(int baza, long long n) {
	long long contor = 0;
	long long contorLocal = 0;

#pragma omp parallel shared(contor) firstprivate(contorLocal) num_threads(4)
	{

		int id = omp_get_thread_num();
		int nrThreaduriObtinute = omp_get_num_threads();
		long long limitaSuperioara = (id + 1)*n / nrThreaduriObtinute;
		int limitaInferioara = id * n / nrThreaduriObtinute;
		for (long long i = limitaInferioara; i < limitaSuperioara; i++)
			if ((i % baza) == 0)
				contorLocal += 1;

#pragma omp critical
		contor += contorLocal;
	}
	return contor;
}


//structura for
long long getMultipliParalel_Shared_For(int baza, long long n) {
	long long contor = 0;
	long long contorLocal = 0;

#pragma omp parallel shared(contor) firstprivate(contorLocal) num_threads(4)
	{
#pragma omp for
		for (long long i = 0; i < n; i++)
			if ((i % baza) == 0)
				contorLocal += 1;

#pragma omp critical
		contor += contorLocal;
	}
	return contor;
}

//structura for + reduction
long long getMultipliParalel_Shared_For_Reduction(int baza, long long n) {
	long long contor = 0;

#pragma omp parallel num_threads(4) reduction(+: contor)
	{
#pragma omp for
		for (long long i = 0; i < n; i++)
			if ((i % baza) == 0)
				contor += 1;
	}
	return contor;
}

//structura for + reduction - simplificata
long long getMultipliParalel_Shared_For_Reduction2(int baza, long long n) {
	long long contor = 0;

#pragma omp parallel for num_threads(4) reduction(+: contor)
		for (long long i = 0; i < n; i++)
			if ((i % baza) == 0)
				contor += 1;
	return contor;
}


//metoda apel rutine 

void getPerformanta(long long(*pf)(int baza, long long n), int baza, long long n) {
	double tStart = omp_get_wtime();

	long long result = pf(baza, n);

	double tFinal = omp_get_wtime();

	cout << endl << "Rezultat: " << result << " in " << tFinal - tStart << " secunde ";
}

void main() {
	////secvential
	//cout << endl << "******** secvential";
	//getPerformanta(getMultipliSecvential, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila nepartajata (benchmark viteza)";
	////paralel - variabila nepartajata - privata - doar pentru viteza
	//getPerformanta(getMultipliParalel_Private, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila partajata - race conditions";
	////paralel - variabila partajata - race conditions
	//getPerformanta(getMultipliParalel_Shared, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila nepartajata (solutie vector) - false sharing";
	////paralel - variabila nepartajata (solutie vector) - false sharing
	//getPerformanta(getMultipliParalel_NotShared, 16, N_ITERATII);


	//cout << endl << "******** paralel - variabila nepartajata (padding) - fara false sharing";
	////paralel - variabila nepartajata (solutie vector + padding) - fara false sharing
	//getPerformanta(getMultipliParalel_NotShared_NoFalseSharing, 16, N_ITERATII);


	//cout << endl << "******** paralel - variabila partajata - lock in bucla";
	////paralel - variabila partajata - lock in bucla
	//getPerformanta(getMultipliParalel_Shared_Critical, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila partajata - lock in afara";
	////paralel - variabila partajata - lock in afara
	//getPerformanta(getMultipliParalel_Shared_Critical2, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila partajata - for paralel";
	////paralel - variabila partajata - for paralel
	//getPerformanta(getMultipliParalel_Shared_For, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila partajata - for paralel + reduction";
	////paralel - variabila partajata - for paralel + reduction
	//getPerformanta(getMultipliParalel_Shared_For_Reduction, 16, N_ITERATII);

	//cout << endl << "******** paralel - variabila partajata - for paralel + reduction";
	////paralel - variabila partajata - for paralel + reduction
	//getPerformanta(getMultipliParalel_Shared_For_Reduction2, 16, N_ITERATII);
}
