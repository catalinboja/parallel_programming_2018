#include <iostream>
#include <omp.h>

using namespace std;

//functie ce determina secventa Fibonacci secvential
void fibonacci(long long n) {

	long long t1 = 0, t2 = 1, nextTerm = 0;

	cout << endl << "Secventa Fibonacci:";

	nextTerm = t1 + t2;
	cout << endl << nextTerm;

	while (nextTerm <= n)
	{
		t1 = t2;
		t2 = nextTerm;
		nextTerm = t1 + t2;
		cout << endl << nextTerm;
	}
}

int getNumarValoriFibonacci(int n) {
	return (int)(log(sqrt(5)*n) / log((1 + sqrt(5)) / 2));
}

void fibonacci_parallel (long long n) {

	long long t1 = 0, t2 = 1, nextTerm = 0;

	cout << endl << "Secventa Fibonacci:";

	nextTerm = t1 + t2;
	cout << endl << nextTerm;

	int numarValoriFibonacci = getNumarValoriFibonacci(n);

#pragma omp parallel for num_threads(4) firstprivate(t1, t2) shared(nextTerm)
	for(int i = 1; i <= numarValoriFibonacci;i++)
	{
		t1 = t2;
		t2 = nextTerm;
		nextTerm = t1 + t2;
#pragma omp critical
		cout << endl << nextTerm;
	}
}


//functie ce determina secventa cate numere prime sunt in intervalul 1 .. N
int prime(long long n) {
	int contor = 0;
	for (long long i = 2; i < n; i++) {
		bool estePrim = true;
		for (long long j = 2; j < i / 2; j++)
		{
			if (i % j == 0)
			{
				estePrim = false;
				break;
			}
		}
		if (estePrim) {
			//cout << endl << i;
			contor += 1;
		}
	}

	return contor;
}

int prime_paralel(long long n) {
	int contor = 0;
#pragma omp parallel for reduction(+: contor)
	for (long long i = 2; i < n; i++) {
		bool estePrim = true;
		for (long long j = 2; j < i / 2; j++)
		{
			if (i % j == 0)
			{
				estePrim = false;
				break;
			}
		}
		if (estePrim) {
			//cout << endl << i;
			contor += 1;
		}
	}

	return contor;
}


//teste cu diferite valori pentru schedule
// static
// dynamic, dynamic 10 sau dynamic 100
int prime_paralel_schedule(long long n) {
	int contor = 0;
#pragma omp parallel for schedule(runtime) reduction(+: contor)
	for (long long i = 2; i < n; i++) {
		bool estePrim = true;
		for (long long j = 2; j < i / 2; j++)
		{
			if (i % j == 0)
			{
				estePrim = false;
				break;
			}
		}
		if (estePrim) {
			//cout << endl << i;
			contor += 1;
		}
	}

	return contor;
}



//test nowait
void test_nowait_barrier() {
	const int secunde = 1000;
#pragma omp parallel num_threads(4)
	{
		int tId = omp_get_thread_num();
		_sleep(tId * secunde);
		cout << endl << "Thread " << tId << " incepe prelucrarea ";


#pragma omp barrier
		cout << endl << "Thread " << tId << " porneste iar ";


#pragma omp for
		for (int i = 0; i < 10; i++) {
#pragma omp critical
			cout << endl << "#for 1 Thread " << tId << " proceseaza i =  " << i;
		}

#pragma omp critical
		cout << endl << "#1 Thread " << tId << " porneste iar ";

#pragma omp for nowait
		for (int i = 0; i < 10; i++) {
#pragma omp critical
			cout << endl << "#for 2 Thread " << tId << " proceseaza i =  " << i;
		}

#pragma omp critical
		cout << endl << "#2 Thread " << tId << " porneste iar ";

	}
}



//test master
void test_master() {
	const int secunde = 1000;
#pragma omp parallel
	{
		int tId = omp_get_thread_num();
		_sleep((omp_get_num_threads() -  tId) * secunde);
		cout << endl << "Thread " << tId << " incepe prelucrarea ";

#pragma omp master
		{
			cout << endl << "Thread " << tId << " este master ";
			_sleep(3 * secunde);
		}


	}
}

//test single
void test_single() {
	const int secunde = 1000;
	int valoare = 0;
#pragma omp parallel firstprivate(valoare)
	{
		int tId = omp_get_thread_num();
		valoare = 10 * tId;
		_sleep(tId * secunde);
		cout << endl << "Thread " << tId << " incepe prelucrarea ";

#pragma omp single copyprivate(valoare)
		{
			cout << endl << "Thread " << tId << " executa sectiunea single ";
			_sleep(3 * secunde);
			valoare = 100;
		}

		cout << endl << "Am primit valoarea " << valoare;
	}
}


//test sections
void test_sections() {
#pragma omp parallel num_threads(4)
	{
		int tId = omp_get_thread_num();
#pragma omp critical
		cout << endl << tId << "in sectiune paralela generica";

#pragma omp sections
		{

#pragma omp section
			{
				_sleep(2000); 
				cout << endl << tId << "in sectiunea #1" << endl;
			}

#pragma omp section
			{
				cout << endl << tId << "in sectiunea #2" << endl;
			}
		}
	}
}

//test library fyunctions
void test_library_functions() {
	int nThreaduri;
	omp_set_dynamic(0);
	omp_set_num_threads(omp_get_num_procs());
#pragma omp parallel
	{
		int id = omp_get_thread_num();
#pragma omp single
		nThreaduri = omp_get_num_threads();

		//sectiune paralela
	}
}

//test locks
void test_locks() {
	omp_lock_t my_lock;
	omp_init_lock(&my_lock);

#pragma omp parallel num_threads(4)  
	{
		int tid = omp_get_thread_num();
		int i, j;

		for (i = 0; i < 5; ++i) {
			while (!omp_test_lock(&my_lock)) {
				cout << endl << "Thread " << tid << " - face altceva";
			}

			cout << endl << "Thread " << tid << " - start lock";
			cout << endl << "Thread " << tid << " - elibereaza lock";
			omp_unset_lock(&my_lock);

		}
	}

	omp_destroy_lock(&my_lock);


}


//functie ce masoara performanta
void testPerformanta(int(*pf)(long long), long long N) {
	double tStart = omp_get_wtime();

	int rezultat = pf(N);

	double tFinal = omp_get_wtime();

	cout << endl << "Rezultat " << rezultat << " obtinut in " <<
		tFinal - tStart << " ms";
}

void test_carry_dependency(int n) {
	int vector[100];
#pragma omp parallel for
	for (int i = 1; i < n; i++) {
		vector[i] = i * 2;
		int j = vector[i-1];
		//prelucrari care depind de j
	}
#pragma omp parallel for
	for (int i = 1; i < n; i++) {
		vector[i] = i * 2;
	}

#pragma omp parallel for
	for (int i = 1; i < n; i++) {
		int j = vector[i - 1];
		//prelucrari care depind de j
	}

}

static const long long N = 2 * 1e5;

int main() {
	cout << endl << "Procesare ...";
	//test secvential
	//testPerformanta(prime, N);	//18 sec

	//test paralel
	//testPerformanta(prime_paralel, N);

	//test paralel
	//testPerformanta(prime_paralel_schedule, N);

	//test nowait, bariera
	//test_nowait_barrier();

	//test master
	//test_master();

	//test sectiuni
	//test_sections();

	//test single
	//test_single();

	//test locks
	//test_locks();
	int n = 1000000;
	fibonacci(n);
	fibonacci_parallel(n);
}