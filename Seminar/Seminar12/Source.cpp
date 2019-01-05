#include <iostream>
#include <string>
#include <omp.h>

using namespace std;

void multiplicareFisier(const char* numeFisier, int factorMultiplicare) {
	FILE *pf = fopen(numeFisier, "r");
	fseek(pf, 0, SEEK_END);
	unsigned int dimensiune = ftell(pf);
	char* continut = new char[dimensiune];
	fseek(pf, 0, SEEK_SET);
	fread(continut, dimensiune, 1, pf);

	FILE *copie = fopen("Date.txt", "w");
	for(int i=0;i<factorMultiplicare;i++)
		fwrite(continut, dimensiune, 1, copie);

	fclose(pf);
	fclose(copie);
}

char* getContinutText(const char* numeFisier) {
	FILE *pf = fopen(numeFisier, "r");
	fseek(pf, 0, 2);
	unsigned int dimensiune = ftell(pf);
	fseek(pf, 0, SEEK_SET);
	char* continut = new char[dimensiune+1];
	fread(continut, dimensiune, 1, pf);
	continut[dimensiune] = '\0';
	fclose(pf);
	return continut;
}

//cautare naiva de subsiruri
int cautare_naiva_paralela(const char* text, const char* pattern) {
	int contor = 0;
	int n = strlen(text);
	int m = strlen(pattern);

#pragma omp parallel for reduction(+:contor)
	for (int i = 0; i < n; i++) {
		bool gasit = true;
		for (int j = 0; j < m; j++)
			if (text[i+j] != pattern[j]) {
				gasit = false;
				break;
			}
		if (gasit)
			contor += 1;
	}

	return contor;
}

//boyer-moore
//https://www.geeksforgeeks.org/boyer-moore-algorithm-for-pattern-searching/

# define NO_OF_CHARS 256 

// The preprocessing function for Boyer Moore's 
// bad character heuristic 
void badCharHeuristic(char *str, int size,int badchar[NO_OF_CHARS])
{
	int i;
	for (i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;
	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}

/* A pattern searching function that uses Bad
   Character Heuristic of Boyer Moore Algorithm */
int boyer_moore_search(char *txt, char *pat)
{
	int contor = 0;
	int m = strlen(pat);
	int n = strlen(txt);

	int badchar[NO_OF_CHARS];
	badCharHeuristic(pat, m, badchar);

	int s = 0;  // s is shift of the pattern with  
				// respect to text 
	while (s <= (n - m))
	{
		int j = m - 1;
		while (j >= 0 && pat[j] == txt[s + j])
			j--;
		if (j < 0)
		{
			contor += 1;
			s += (s + m < n) ? m - badchar[txt[s + m]] : 1;
		}

		else {
			int a = 1;
			int b = j - badchar[txt[s + j]];
			s += ((a > b) ? a : b);
		}
	}
	return contor;
}


int boyer_moore_parallel_search(char *txt, char *pat) {
	int NUM_THREADS = 4;
	int contor = 0;
	int chunk_size = strlen(txt) / NUM_THREADS;
	int chunk_reminder = strlen(txt) % NUM_THREADS;
	int m = strlen(pat);
	char* chunk = NULL;

#pragma omp parallel for num_threads(NUM_THREADS) reduction(+:contor)
	for (int i = 0; i < NUM_THREADS; i++) {
		if (i == 0) {
			chunk = new char[chunk_size + 1];
			memcpy(chunk, (char*)txt, chunk_size);
			chunk[chunk_size] = '\0';
		}
		else {
			chunk = new char[chunk_size + 1 + m];
			memcpy(chunk, (char*)txt + i * chunk_size - m, chunk_size + m);
			chunk[chunk_size + m] = '\0';
		}	
		contor += boyer_moore_search(chunk, pat);
	}
	//TO DO
	//mai raman cativa bytes la final care nu sunt verificati

	return contor;
}

int main() {
	//multiplicareFisier("dickens-david-626.txt", 100);
	char* text = getContinutText("Date.txt");
	int n = strlen(text);
	cout << endl << "Dimensiune text " << n;

	char pattern[100];
	cout << endl << "Cuvantul cautat este: ";
	cin >> pattern;

	//test cautare naiva
	double tStart = omp_get_wtime();
	int rezultat = cautare_naiva_paralela(text, pattern);
	double tFinal = omp_get_wtime();
	cout << endl << "Rezultate: " << rezultat << " in " << tFinal - tStart << " secunde";


	//test cautare boyer-moore
	tStart = omp_get_wtime();
	rezultat = boyer_moore_search(text, pattern);
	tFinal = omp_get_wtime();
	cout << endl << "Rezultate: " << rezultat << " in " << tFinal - tStart << " secunde";


	//test cautare boyer-moore
	tStart = omp_get_wtime();
	rezultat = boyer_moore_parallel_search(text, pattern);
	tFinal = omp_get_wtime();
	cout << endl << "Rezultate: " << rezultat << " in " << tFinal - tStart << " secunde";


}