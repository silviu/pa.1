#include "corrector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.h"
#include <vector>

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
	
	for (int i = 0; i < m; i++)
		d[i][0] = i;
		
	for (int i = 0; i < n; i++)
		d[0][i] = i;
	
	return d;
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

vector<Candidate*> Corrector::get_deviation(char* a, char* b)
{
	size_t len_a = strlen(a);
	size_t len_b = strlen(b);
	int m = len_a + 1;
	int n = len_b + 1;
	int** d = get_matrix(m , n);
	vector<Candidate*> candidates;
	
	for (unsigned int j = 1; j < len_b+1; j++)
		for (unsigned int i = 1; i < len_a+1; i++)
			if (a[i-1] == b[j-1])
				d[i][j] = d[i-1][j-1];
			else
				d[i][j] = minim(d[i-1][j], d[i][j-1], d[i-1][j-1]) + 1;
	
	for (unsigned int j = 1; j < len_a+1; j++) {
		for (unsigned int i = 1; i < len_b+1; i++)
			cout << d[j][i] << " ";
		cout << endl;
	}
	
	for (unsigned int j = 1; j < len_b+1; j++) {
		for (unsigned int i = 1; i < len_a+1; i++)
			cout << d[i][j] << " ";
		cout << endl;
	}
	
	
	for (unsigned int j = 1; j < len_a+1; j++) {
		int start_dist = d[j][1];
		cout << "START_DIST = " << start_dist << endl;
		unsigned int how_much;
		if (len_a+1-j < len_b) {
			how_much = abs(j - len_a) + 2;
		}
		else 
			how_much = len_b+1;
		for (unsigned int i = 1; i < how_much; i++) {
			cout << "ADD: j = " << j-1 << "; i = " << i+j-2 << "; DIST = " << d[j + i - 1][len_b] + start_dist << endl;
			candidates.push_back(new Candidate(j-1, i+j-2, d[j + i - 1][len_b] + start_dist));
		}
	}
	
	free_matrix(d, m, n);
	return candidates;
}

