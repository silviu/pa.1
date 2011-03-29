#include "corrector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.h"
#include <vector>
#include <limits>

Corrector::Corrector()
{
	dictionary = new Dictionary();
}

int** Corrector::get_matrix(int m, int n)
{
	int** d = (int**)malloc(m * sizeof(int*));
	if (d == NULL) {
		fprintf(stderr, "Error malloc in get_matrix()\n");
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < m; i++) {
		d[i] = (int*) malloc (n * sizeof(int));
		if (d[i] == NULL) {
			fprintf(stderr, "Error malloc in get_matrix()\n");
			exit(EXIT_FAILURE);
		}
	}
	
	return d;
}

void init_leven(int** d, int m, int n)
{
	for (int i = 0; i < m; i++)
		d[i][0] = i;
		
	for (int i = 0; i < n; i++)
		d[0][i] = i;
}

void init_main(int** d, int n)
{
	for (int i = 0; i <n; i++)
		for (int j= 0; j < n; j++) {
			if (i == 0) {
				d[i][j] = j;
				continue;
			}
			if (j == 0) {
				d[i][j] = i;
				continue;
			}
			d[i][j] = 0;
		}
}


void Corrector::free_matrix(int** d, int m, int n)
{
	for (int i = 0; i < m; i++)
		free(d[i]);
	free(d);
}

int Corrector::minim(int a, int b, int c)
{
	int min = a;
	if (a < b)
		if (a < c)
			min = a;
		else
			min = c;
	else
		if (b < c)
			min = b;
		else
			min = c;
	return min;
}

Candidate** Corrector::get_deviation(char* a, char* b)
{
	size_t len_a = strlen(a);
	size_t len_b = strlen(b);
	int m = len_a + 1;
	int n = len_b + 1;
	int** d = get_matrix(m , n);
	init_leven(d, m, n);
	
	int cand_size = len_a;
	Candidate** candidates = (Candidate**)malloc(cand_size*sizeof(Candidate*));;
	for (int i = 0; i < cand_size; i++)
			candidates[i] = new Candidate(strdup("NULL"), numeric_limits<int>::max());
	
	for (unsigned int j = 1; j < len_b+1; j++)
		for (unsigned int i = 1; i < len_a+1; i++)
			if (a[i-1] == b[j-1])
				d[i][j] = d[i-1][j-1];
			else
				d[i][j] = minim(d[i-1][j], d[i][j-1], d[i-1][j-1]) + 1;
	/*
	for (unsigned int j = 1; j < len_b+1; j++) {
		for (unsigned int i = 1; i < len_a+1; i++)
			cout << d[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	*/
	for (unsigned int i = 1; i < len_a+1; i++) {
		//cout << d[i][len_b] << " ";
		candidates[i-1]->set_dist(d[i][len_b]);
		candidates[i-1]->set_word(strdup(b));
	}
	//cout << endl;
	free_matrix(d, m, n);
	return candidates;
}


char* Corrector::correct(char* search_terms)
{
	int len = strlen(search_terms);
	int n = len + 1;
	int** d = get_matrix(n, n);
	init_main(d, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			if ((i > 9) && (j == 0))
				cout << " " << d[i][j];
			else
				cout << "  " << d[i][j];
		cout << endl;
	}

	for (int i = 1; i < len; i++) {
		char* curr_search = (char*) malloc(n-i+1);
		memcpy(curr_search, search_terms+i-1, n-i);
		curr_search[n-i] = '\0';
		cout << curr_search << endl;
		
		int vmin_size = strlen(curr_search);
		Candidate* vmin[vmin_size];
		for (int i = 0; i < vmin_size; i++)
			vmin[i] = new Candidate(strdup("NULL"), numeric_limits<int>::max());
		
		dictionary->reset_iterator();
		while (dictionary->has_more_words())
		{
			char* dic_word = dictionary->get_next_word();
			Candidate** candidates = get_deviation(curr_search, dic_word);
			for (int i = 0; i < vmin_size; i++)
				if (candidates[i]->get_dist() < vmin[i]->get_dist()) {
					vmin[i]->set_dist(candidates[i]->get_dist());
					vmin[i]->set_word(candidates[i]->get_word());
				}
		}
		cout << "VMIN FOR: " << curr_search;
		for (int i = 0; i < vmin_size; i++)
			cout << "[" << vmin[i]->get_word() << ", " << vmin[i]->get_dist() << "] ";
		cout << endl;
	}
	return strdup("music channel");
}


