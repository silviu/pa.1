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
		candidates[i-1]->set_nr_words(1);
		candidates[i-1]->set_freq(dictionary->get_frequency(b));
	}
	//cout << endl;
	free_matrix(d, m, n);
	return candidates;
}

Candidate* compose(Candidate* c1, Candidate* c2)
{
	Candidate* ret = new Candidate();
	int c1_word_len = strlen(c1->get_word());
	int c2_word_len = strlen(c2->get_word());
	
	char* comp_word = (char*) malloc(c1_word_len + c2_word_len + 2); // a space and '\0'
	memcpy(comp_word, c1->get_word(), c1_word_len);
	memcpy(comp_word + c1_word_len, " ", 1);
	memcpy(comp_word + c1_word_len + 1, c2->get_word(), c2_word_len);
	comp_word[c1_word_len + c2_word_len + 1] = '\0';
	
	ret->set_dist(c1->get_dist() + c2->get_dist());
	ret->set_word(comp_word);
	ret->set_freq(c1->get_freq() + c2->get_freq());
	ret->set_nr_words(c1->get_nr_words() + c2->get_nr_words());
	cout << "SETTING NR WORDS: " << c1->get_nr_words() + c2->get_nr_words() << endl;
	return ret;
}


char* Corrector::dinamic(Candidate*** d, int n)
{	
	int k = 2; 
	while (k <= n) {
		for (int i = 1; i < (n - k ); i++) {
			cout << "I: " << i << endl;
			int j = i + k;
			Candidate* result = d[i][j];
			for (int q = i + 1; q < j; q++) {
				Candidate* c = compose(d[i][q], d[q][j]);
				if (c->get_dist() < result->get_dist()) {
					result = c;
				}
				if (c->get_dist() == result->get_dist()) {
					if (c->get_nr_words() < result->get_nr_words()) {
						result = c;
					}
					if (c->get_nr_words() == result->get_nr_words()) {
						if (c->get_freq() > result->get_freq()) {
							result = c;
						}
						if (c->get_freq() == result->get_freq()) {
							string c_string = string(c->get_word());
							string result_string = string(result->get_word());
							int com = c_string.compare(result_string);
							if (com < 0) {
								result = c;
							}
						}
					}
				}
			}
			d[i][j] = result;
		}
		k++;
	}
	return d[1][n-1]->get_word();
}


char* Corrector::correct(char* search_terms)
{
	int len = strlen(search_terms);
	int n = len + 1;

	Candidate*** din_mat = (Candidate***) malloc(n * sizeof(Candidate**));
	for (int i = 0; i < n; i++)
		din_mat[i] = (Candidate**) malloc(n * sizeof(Candidate*));
	
	for (int i = 0; i < n; i++)
		for (int j= 0; j < n; j++) {
			if (i == 0) {
				din_mat[i][j] = new Candidate();;
				continue;
			}
			if (j == 0) {
				din_mat[i][j] = new Candidate();
				continue;
			}
			din_mat[i][j] = new Candidate();
		}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			if ((i > 9) && (j == 0))
				cout << " " << din_mat[i][j]->get_dist();
			else
				cout << "  " << din_mat[i][j]->get_dist();
		cout << endl;
	}

	for (int i = 1; i < n; i++) {
		char* curr_search = (char*) malloc(n-i+1);
		memcpy(curr_search, search_terms+i-1, n-i);
		curr_search[n-i] = '\0';
		cout << curr_search << endl;
		
		int vmin_size = strlen(curr_search);
		Candidate* vmin[vmin_size];
		for (int j = 0; j < vmin_size; j++)
			vmin[j] = new Candidate(strdup("NULL"), numeric_limits<int>::max());
		
		dictionary->reset_iterator();
		while (dictionary->has_more_words())
		{
			char* dic_word = dictionary->get_next_word();
			Candidate** candidates = get_deviation(curr_search, dic_word);
			for (int j = 0; j < vmin_size; j++)
				if (candidates[j]->get_dist() < vmin[j]->get_dist()) {
					vmin[j]->set_dist(candidates[j]->get_dist());
					vmin[j]->set_word(candidates[j]->get_word());
					vmin[j]->set_freq(candidates[j]->get_dist());
					vmin[j]->set_nr_words(candidates[j]->get_nr_words());
				}
		}

		cout << "VMIN FOR: " << curr_search;
		for (int j = 0; j < vmin_size; j++) {
			cout << "[" << vmin[j]->get_word() << ", " << vmin[j]->get_dist() << "] ";
			din_mat[i][j+i]->set_word(vmin[j]->get_word());
			din_mat[i][j+i]->set_dist(vmin[j]->get_dist());
		}
		cout << endl;
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			if ((i > 9) && (j == 0))
				cout << " " << din_mat[i][j]->get_dist();
			else
				cout << "  " << din_mat[i][j]->get_dist();
		cout << endl;
	}
	
	char* result = dinamic(din_mat, n);
	
	return strdup(result);
}


