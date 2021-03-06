#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

static int counter;
static const int num_threads = 10;

class Counter {
private:
	int counter = 0;
	std::mutex mutex;
public:
	void increment() {
		mutex.lock();
		counter += 1;
		mutex.unlock();
	}
};

void incrementCounter() {
	counter += 1;
}

void thread_function() {
	std::cout << "Hello World from another thread";
}

// long operation to time
int fib(int n) {
	if (n < 2) {
		return n;
	}
	else {
		return fib(n - 1) + fib(n - 2);
	}
}


int main() {
	//start a thread
	std::thread t1(thread_function);

	//join the thread with the main thread
	t1.join();


	auto start_time = std::chrono::high_resolution_clock::now();

	fib(30);

	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;

	std::cout << "fib(30) took " <<
		std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " ms to run.\n";


	return 0;
}                  

	if (input.is_open()) {

		char buffer[100];
		//preiau numarul de cuvinte
		input.getline(buffer, 100);
		int numarCuvinte = atoi(buffer);
		int nrCuvinteProcesate = 0;
#pragma omp parallel for num_threads(4) reduction(+: nrCuvinteProcesate)
		for (int j = 0; j < numarCuvinte; j++) {
			{
				char cuvant[100];
#pragma omp critical
				input.getline(cuvant, 100);
				nrCuvinteProcesate += 1;
				//int tId = omp_get_thread_num();
				//cout << endl << tId << " - " << cuvant;

				for (int i = 0; i < strlen(cuvant); i++) {
#pragma omp critical
					simboluri[cuvant[i] - 'a'] += 1;
				}
				//verificam palindrom
				//bool estePalindrom = true;
				//for (int i = 0; i < strlen(cuvant); i++)
				//	if (cuvant[i] != cuvant[strlen(cuvant) - 1 - i])
				//		estePalindrom = false;
				//if (estePalindrom)
				//	cout << endl << cuvant;
			}
		}

		cout << endl << "Total cuvinte procesate " << nrCuvinteProcesate;
	}
	input.close();
	return simboluri;
}


int* contorParalel2(string fisier) {

	int* simboluri = new int[26];
	for (int i = 0; i < 26; i++)
		simboluri[i] = 0;

	FILE *pf = fopen(fisier.data(), "r");

	if (pf != NULL)
	{

		char buffer[100];
		//preiau numarul de cuvinte
		//fread(buffer, 1, 100, pf);
		fgets(buffer, 100, pf);

		int numarCuvinte = atoi(buffer);
		int nrCuvinteProcesate = 0;
#pragma omp parallel for num_threads(4) reduction(+: nrCuvinteProcesate)
		for (int j = 0; j < numarCuvinte; j++)
		{
			char cuvant[100];
#pragma omp critical
			fgets(cuvant, 100, pf);
			nrCuvinteProcesate += 1;
			//int tId = omp_get_thread_num();
			//cout << endl << tId << " - " << cuvant;
			int n = strlen(cuvant);

			for (int i = 0; i < strlen(cuvant)-1; i++) {
#pragma omp critical
				simboluri[cuvant[i] - 'a'] += 1;
			}
			//verificam palindrom
			//bool estePalindrom = true;
			//for (int i = 0; i < strlen(cuvant); i++)
			//	if (cuvant[i] != cuvant[strlen(cuvant) - 1 - i])
			//		estePalindrom = false;
			//if (estePalindrom)
			//	cout << endl << cuvant;
		}


		cout << endl << "Total cuvinte procesate " << nrCuvinteProcesate;

		fclose(pf);
		return simboluri;
	}
}

void main() {
	double tStart = omp_get_wtime();
	int* counterLitere = contorSecvential("wordlist.txt");
	double tFinal = omp_get_wtime();

	cout << endl << "Timp procesare: " << tFinal - tStart;
	cout << endl << "Frecvente: ";
	for (int i = 0; i < 26; i++)
		cout << " | " << (char)('a' + i) << " - " << counterLitere[i];

	//implementare paralela

	tStart = omp_get_wtime();
	counterLitere = contorParalel("wordlist.txt");
	tFinal = omp_get_wtime();

	cout << endl << "Timp procesare: " << tFinal - tStart;
	cout << endl << "Frecvente: ";
	for (int i = 0; i < 26; i++)
		cout << " | " << (char)('a' + i) << " - " << counterLitere[i];

	//implementare paralela

	tStart = omp_get_wtime();
	counterLitere = contorParalel2("wordlist.txt");
	tFinal = omp_get_wtime();

	cout << endl << "Timp procesare: " << tFinal - tStart;
	cout << endl << "Frecvente: ";
	for (int i = 0; i < 26; i++)
		cout << " | " << (char)('a' + i) << " - " << counterLitere[i];

}