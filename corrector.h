#ifndef __CORRECTOR_H__
#define __CORRECTOR_H__

#include <iostream>
#include <string>
#include <map>
#include "dictionary.h"
#include "candidate.h"
#include <vector>

using namespace std;

class Corrector
{	
	public:
		Corrector();
        vector<Candidate*> get_deviation(char* a, char* b);
		void k_difference(char* a, char* b, int k);
	private:
		Dictionary* dictionary;
		int** get_matrix(int m, int n);
		void free_matrix(int** d, int m, int n);
		int minim(int a, int b, int c);
};
#endif // __CORRECTOR_H__
