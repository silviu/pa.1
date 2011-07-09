#include "corrector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.h"
#include <vector>
#include <limits>
#include <sstream>
#include <string>

Corrector::Corrector()
{
	dictionary = new Dictionary();
}

int Corrector::minim(int a, int b, int c)
{
	int min = a;
	if (a < b)
		if (a < c) min = a;
		else min = c;
	else if (b < c)	min = b;
		else min = c;
	return min;
}

char* Corrector::remove_spaces(char* pword)
{
	string word = string(pword);
	stringstream word_splitter (stringstream::in | stringstream::out);
	word_splitter << word;
	string new_word;
	while(word_splitter.good())
	{
		string tmp;
		word_splitter >> tmp;
		new_word.append(tmp);
	}
	return strdup(new_word.c_str());
}

Candidate** Corrector::get_deviation(char* a, char* b)
{
	size_t len_a = strlen(a);
	size_t len_b = strlen(b);
	int m = len_a + 1;
	int n = len_b + 1;
	int d[m][n];
	
	for (int i = 0; i < m; i++)
		d[i][0] = i;
		
	for (int i = 0; i < n; i++)
		d[0][i] = i;
	
	int cand_size = len_a;
	Candidate** candidates = (Candidate**)malloc(cand_size * sizeof(Candidate*));;
	for (int i = 0; i < cand_size; i++)
			candidates[i] = new Candidate((char*)"NULL", numeric_limits<int>::max());
	
	for (unsigned int j = 1; j < len_b + 1; j++)
		for (unsigned int i = 1; i < len_a + 1; i++)
			if (a[i-1] == b[j-1])
				d[i][j] = d[i-1][j-1];
			else
				d[i][j] = minim(d[i-1][j], d[i][j-1], d[i-1][j-1]) + 1;
				
	for (unsigned int i = 1; i < len_a+1; i++) {
		Candidate* cand = candidates[i-1];
		cand->set_dist(d[i][len_b]);
		cand->set_word(b);
		cand->set_nr_words(1);
		cand->set_freq(dictionary->get_frequency(b));
	}
	
	return candidates;
}

Candidate* update_candidate(Candidate* c1, Candidate* c2)
{
	Candidate* ret = new Candidate();
	
	char* c1_word = c1->get_word();
	char* c2_word = c2->get_word();
	
	int c1_word_len = strlen(c1_word);
	int c2_word_len = strlen(c2_word);
	
	char* comp_word = (char*) malloc(c1_word_len + c2_word_len + 2); // a space and '\0'
	memcpy(comp_word, c1_word, c1_word_len);
	memcpy(comp_word + c1_word_len, " ", 1);
	memcpy(comp_word + c1_word_len + 1, c2_word, c2_word_len);
	comp_word[c1_word_len + c2_word_len + 1] = '\0';
	
	ret->set_dist(c1->get_dist() + c2->get_dist());
	ret->set_word(comp_word);
	ret->set_freq(c1->get_freq() + c2->get_freq());
	ret->set_nr_words(c1->get_nr_words() + c2->get_nr_words());
	return ret;
}


char* Corrector::dinamic(Candidate*** d, int n)
{	
	for (int k = 2; k < n + 1; k++) {
		for (int i = 1; i < (n - k); i++) {
			int j = i + k;
			Candidate* result = d[i][j];
			for (int l = i + 1; l < j; l++) {
				Candidate* cand = update_candidate(d[i][l], d[l][j]);
				
				int curr_c_dist = cand->get_dist();
				int curr_res_dist = result->get_dist();
				
				if (curr_c_dist < curr_res_dist)
					result = cand;
				else if (curr_c_dist == curr_res_dist) {
					if (cand->get_nr_words() < result->get_nr_words())
						result = cand;
					else if (cand->get_nr_words() == result->get_nr_words()) {
						if (cand->get_freq() > result->get_freq())
							result = cand;
						else if (cand->get_freq() == result->get_freq()) {
							string cond_string = string(cand->get_word());
							string result_string = string(result->get_word());
							int lexico = cond_string.compare(result_string);
							if (lexico < 0)
								result = cand;
						}
					}
				}
			}
			d[i][j] = result;
		}
	}
	return d[1][n-1]->get_word();
}


char* Corrector::correct(char* search_terms)
{
	search_terms = remove_spaces(search_terms);
	int len = strlen(search_terms);
	int n = len + 1;
	
	Candidate*** din_mat = (Candidate***) malloc(n * sizeof(Candidate**));
	for (int i = 0; i < n; i++)
		din_mat[i] = (Candidate**) malloc(n * sizeof(Candidate*));
	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			din_mat[i][j] = new Candidate();

	for (int i = 1; i < n; i++) {
		char* curr_search = (char*) malloc(n-i+1);
		memcpy(curr_search, search_terms + i - 1, n - i);
		curr_search[n-i] = '\0';
		
		int vmin_size = strlen(curr_search);
		Candidate* vmin[vmin_size];
		for (int j = 0; j < vmin_size; j++)
			vmin[j] = new Candidate((char*)"NULL", numeric_limits<int>::max());
		
		dictionary->reset_iterator();
		
		while (dictionary->has_more_words())
		{
			char* dic_word = dictionary->get_next_word();
			Candidate** candidates = get_deviation(curr_search, dic_word);
			for (int j = 0; j < vmin_size; j++)
				if (candidates[j]->get_dist() < vmin[j]->get_dist()) {
					Candidate* curr_min = vmin[j];
					Candidate* curr_cand = candidates[j];
					curr_min->set_dist(curr_cand->get_dist());
					curr_min->set_word(curr_cand->get_word());
					curr_min->set_freq(curr_cand->get_freq());
					curr_min->set_nr_words(curr_cand->get_nr_words());
				}
		}

		for (int j = 0; j < vmin_size; j++) {
			Candidate* curr_din = din_mat[i][j+i];
			curr_din->set_word(vmin[j]->get_word());
			curr_din->set_dist(vmin[j]->get_dist());
			curr_din->set_freq(vmin[j]->get_freq());
			curr_din->set_nr_words(vmin[j]->get_nr_words());
		}
	}
	
	char* result = dinamic(din_mat, n);
	
	return (char*)result;
}


